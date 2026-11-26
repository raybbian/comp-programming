"""Polygon API client: signing, transport, errors.

Credentials come from the environment (POLYGON_KEY, POLYGON_SECRET) or from the
.env file next to this module. They are never printed.
"""
import hashlib
import json
import os
import secrets
import sys
import time
import urllib.error
import urllib.request
import uuid

HERE = os.path.dirname(os.path.abspath(__file__))
DEFAULT_URL = "https://polygon.codeforces.com/api/"

# Methods whose effect is not safe to repeat blindly after a lost response.
NO_RETRY = {
    "problem.create", "problem.commitChanges", "problem.buildPackage", "problem.addIssue",
    "problem.updateIssue", "problem.saveStress", "problem.runStresses", "problem.addStressCountertest",
}


class PolygonError(Exception):
    """The API answered FAILED. `result` holds the failure details some methods return."""

    def __init__(self, method, comment, result=None):
        super().__init__(f"{method}: {comment}")
        self.method, self.comment, self.result = method, comment, result


def _load_env():
    values = {}
    path = os.path.join(HERE, ".env")
    if os.path.exists(path):
        with open(path, encoding="utf-8") as f:
            for line in f:
                line = line.strip()
                if "=" in line and not line.startswith("#"):
                    k, v = line.split("=", 1)
                    values[k.strip()] = v.strip().strip('"').strip("'")
    return values


def _to_bytes(value):
    if isinstance(value, bytes):
        return value
    if value is True:
        return b"true"
    if value is False:
        return b"false"
    return str(value).encode("utf-8")


def sign(method, pairs, secret, rand):
    """apiSig for `pairs`, a list of (name, value) byte pairs that already includes apiKey and time."""
    to_sign = (rand + b"/" + method.encode("utf-8") + b"?" + b"&".join(k + b"=" + v for k, v in sorted(pairs))
               + b"#" + secret.encode("utf-8"))
    return rand + hashlib.sha512(to_sign).hexdigest().encode("ascii")


def _api_failure(data):
    """The parsed body if it is exactly an API failure envelope, else None."""
    if data[:1] != b"{" or len(data) > 1 << 20:
        return None
    try:
        parsed = json.loads(data.decode("utf-8"))
    except ValueError:
        return None
    if (isinstance(parsed, dict) and parsed.get("status") == "FAILED"
            and set(parsed) <= {"status", "comment", "result"}):
        return parsed
    return None


class Polygon:
    def __init__(self, key=None, secret=None, url=None, pin=None):
        env = _load_env()
        self.key = key or os.environ.get("POLYGON_KEY") or env.get("POLYGON_KEY")
        self.secret = secret or os.environ.get("POLYGON_SECRET") or env.get("POLYGON_SECRET")
        self.url = url or os.environ.get("POLYGON_URL") or env.get("POLYGON_URL") or DEFAULT_URL
        self.pin = pin or os.environ.get("POLYGON_PIN") or env.get("POLYGON_PIN")
        if not self.key or not self.secret:
            raise SystemExit("POLYGON_KEY / POLYGON_SECRET not found in the environment or in "
                             + os.path.join(HERE, ".env"))

    # -- transport ---------------------------------------------------------

    def _request(self, method, params):
        """Sign and POST. `params` maps name -> value or list of values. Returns (status, bytes)."""
        pairs, binary = [], set()
        for name, value in params.items():
            if value is None:
                continue
            for v in (value if isinstance(value, (list, tuple)) else [value]):
                pairs.append((name.encode("utf-8"), _to_bytes(v)))
                if isinstance(v, bytes):
                    binary.add(name.encode("utf-8"))
        if self.pin and not any(k == b"pin" for k, _ in pairs):
            pairs.append((b"pin", _to_bytes(self.pin)))
        pairs.append((b"apiKey", self.key.encode("utf-8")))
        pairs.append((b"time", str(int(time.time())).encode("ascii")))
        pairs.append((b"apiSig", sign(method, pairs, self.secret, secrets.token_hex(3).encode("ascii"))))

        boundary = uuid.uuid4().hex.encode("ascii")
        body = b"".join(
            b"--" + boundary + b'\r\nContent-Disposition: form-data; name="' + k + b'"'
            # file content goes as a file part, so the server takes the bytes as they are
            + (b'; filename="' + k + b'"\r\nContent-Type: application/octet-stream' if k in binary else b"")
            + b"\r\n\r\n" + v + b"\r\n"
            for k, v in pairs) + b"--" + boundary + b"--\r\n"
        req = urllib.request.Request(self.url + method, data=body, headers={
            "Content-Type": "multipart/form-data; boundary=" + boundary.decode("ascii")})
        try:
            with urllib.request.urlopen(req, timeout=600) as resp:
                return resp.status, resp.read()
        except urllib.error.HTTPError as e:
            return e.code, e.read()

    def raw(self, method, **params):
        """Call a method and return the response body as bytes (for file-returning methods)."""
        attempts = 1 if method in NO_RETRY else 4
        throttled = 0
        attempt = 0
        while True:
            try:
                status, data = self._request(method, params)
            except (urllib.error.URLError, TimeoutError, ConnectionError):
                if attempt == attempts - 1:
                    raise
                attempt += 1
                time.sleep(2 * attempt)
                continue
            if status == 429 and throttled < 8:      # rejected before processing: always safe to repeat
                throttled += 1
                wait = min(60, 5 * 2 ** (throttled - 1))
                print(f"rate limited by Polygon, waiting {wait}s ...", file=sys.stderr)
                time.sleep(wait)
                continue
            if status >= 500 and attempt < attempts - 1:
                attempt += 1
                time.sleep(2 * attempt)
                continue
            break
        if status != 200:
            self._raise(method, data, status)
        failure = _api_failure(data)
        if failure:
            raise PolygonError(method, failure.get("comment"), failure.get("result"))
        return data

    def call(self, method, **params):
        """Call a JSON method and return its `result`."""
        data = self.raw(method, **params)
        try:
            parsed = json.loads(data.decode("utf-8"))
        except ValueError:
            raise PolygonError(method, f"unexpected response: {data[:200]!r}")
        if not isinstance(parsed, dict) or parsed.get("status") != "OK":
            comment = parsed.get("comment") if isinstance(parsed, dict) else repr(parsed)[:200]
            raise PolygonError(method, comment, parsed.get("result") if isinstance(parsed, dict) else None)
        return parsed.get("result")

    @staticmethod
    def _raise(method, data, status):
        try:
            parsed = json.loads(data.decode("utf-8"))
            raise PolygonError(method, parsed.get("comment") or f"HTTP {status}", parsed.get("result"))
        except (ValueError, AttributeError):
            raise PolygonError(method, f"HTTP {status}: {data[:200]!r}")

    # -- helpers -----------------------------------------------------------

    def resolve(self, ref):
        """Problem id from an id or an exact problem name."""
        ref = str(ref)
        if ref.isdigit():
            return int(ref)
        found = [p for p in self.call("problems.list", name=ref) if p["name"] == ref and not p.get("deleted")]
        if not found:
            raise SystemExit(f"no problem named {ref!r}")
        if len(found) > 1:
            owners = ", ".join(f"{p['id']} ({p['owner']})" for p in found)
            raise SystemExit(f"several problems are named {ref!r}: {owners}; use the id")
        return found[0]["id"]
