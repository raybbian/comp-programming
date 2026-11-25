#!/usr/bin/env python3
"""Upload a problem package folder to Polygon through its API.

The package folder (--package, default: current directory) must contain
statement/, files/, solutions/, manual/ and scripts/tests.txt.

    python polygon_upload.py list [name-filter]
    python polygon_upload.py upload --name <polygon-problem-name>     (creates the problem)
    python polygon_upload.py upload --id <problemId>                  (re-uploads into an existing one)
    python polygon_upload.py commit --id <problemId> [-m message]
    python polygon_upload.py build --id <problemId>
    python polygon_upload.py packages --id <problemId>
    python polygon_upload.py cautions --id <problemId>

Credentials: POLYGON_KEY and POLYGON_SECRET from the environment, or from the
.env file next to this script. They are never printed.
"""
import argparse
import hashlib
import json
import os
import random
import string
import sys
import time
import urllib.error
import urllib.parse
import urllib.request

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.getcwd()          # package folder, set by --package
API = "https://polygon.codeforces.com/api/"
TESTSET = "tests"

TIME_LIMIT_MS = 3000
MEMORY_LIMIT_MB = 256

SOLUTIONS = [            # file, tag, groups it is expected to pass (extra tag OK)
    ("full_lct.cpp", "MA", []),
    ("full_sqrt.cpp", "OK", []),
    ("sub4_segtree_walk.cpp", "TL", ["0", "1", "4"]),
    ("sub3_lifting.cpp", "TL", ["0", "1", "2", "3"]),
    ("sub2_walk.cpp", "TL", ["0", "1", "2"]),
    ("brute_bfs.cpp", "TL", ["0", "1"]),
    ("wa_one_chain.cpp", "WA", []),
    ("wa_stuck_check.cpp", "WA", []),
    ("wa_undirected.cpp", "WA", []),
]
GROUP_POINTS = {"0": 0, "1": 5, "2": 10, "3": 20, "4": 25, "5": 40}
GROUP_DEPENDENCIES = {"3": "2", "5": "1,2,3,4"}


def credentials():
    key, secret = os.environ.get("POLYGON_KEY"), os.environ.get("POLYGON_SECRET")
    env_path = os.path.join(HERE, ".env")
    if (not key or not secret) and os.path.exists(env_path):
        with open(env_path, encoding="utf-8") as f:
            for line in f:
                line = line.strip()
                if "=" in line and not line.startswith("#"):
                    k, v = line.split("=", 1)
                    v = v.strip().strip('"').strip("'")
                    if k.strip() == "POLYGON_KEY":
                        key = v
                    elif k.strip() == "POLYGON_SECRET":
                        secret = v
    if not key or not secret:
        sys.exit("POLYGON_KEY / POLYGON_SECRET not found in the environment or .env")
    return key, secret


KEY, SECRET = None, None


def call(method, raw=False, **params):
    """POST one API method. Returns result (JSON) or the raw body; raises on FAILED."""
    global KEY, SECRET
    if KEY is None:
        KEY, SECRET = credentials()
    params = {k: ("true" if v is True else "false" if v is False else str(v)) for k, v in params.items()}
    params["apiKey"] = KEY
    params["time"] = str(int(time.time()))
    rand = "".join(random.choice(string.ascii_lowercase + string.digits) for _ in range(6))
    joined = "&".join(f"{k}={v}" for k, v in sorted(params.items()))
    digest = hashlib.sha512(f"{rand}/{method}?{joined}#{SECRET}".encode("utf-8")).hexdigest()
    params["apiSig"] = rand + digest
    body = urllib.parse.urlencode(params).encode("utf-8")
    req = urllib.request.Request(API + method, data=body)
    for attempt in range(4):
        try:
            with urllib.request.urlopen(req, timeout=300) as resp:
                data = resp.read()
            break
        except urllib.error.HTTPError as e:
            data = e.read()
            if e.code >= 500 and attempt < 3:
                time.sleep(3 * (attempt + 1))
                continue
            break
        except urllib.error.URLError:
            if attempt == 3:
                raise
            time.sleep(3 * (attempt + 1))
    if raw:
        return data
    try:
        parsed = json.loads(data.decode("utf-8"))
    except ValueError:
        raise RuntimeError(f"{method}: unexpected response {data[:200]!r}")
    if parsed.get("status") != "OK":
        raise RuntimeError(f"{method}: {parsed.get('comment')}")
    return parsed.get("result")


def read(rel):
    with open(os.path.join(ROOT, rel), encoding="utf-8") as f:
        return f.read().replace("\r\n", "\n")


def read_spec():
    spec = []
    for line in read("scripts/tests.txt").splitlines():
        line = line.split("#")[0].strip()
        if line:
            group, cmd = [x.strip() for x in line.split("|", 1)]
            spec.append((group, cmd))
    return spec


def step(label, fn):
    try:
        out = fn()
        print(f"  ok    {label}")
        return out
    except RuntimeError as e:
        print(f"  FAIL  {label}: {e}")
        return None


def upload(pid):
    P = dict(problemId=pid)
    print("general info")
    step("limits and stdin/stdout", lambda: call("problem.updateInfo", inputFile="stdin", outputFile="stdout",
                                                 interactive=False, timeLimit=TIME_LIMIT_MS,
                                                 memoryLimit=MEMORY_LIMIT_MB, **P))
    print("statement")
    step("english statement", lambda: call(
        "problem.saveStatement", lang="english", encoding="UTF-8", name=read("statement/name.txt").strip(),
        legend=read("statement/legend.tex"), input=read("statement/input.tex"),
        output=read("statement/output.tex"), scoring=read("statement/scoring.tex"),
        notes=read("statement/notes.tex"), tutorial=read("statement/tutorial.tex"), **P))

    print("files")
    for name in ("gen.cpp", "validator.cpp", "check.cpp"):
        step(name, lambda name=name: call("problem.saveFile", type="source", name=name,
                                          file=read("files/" + name), **P))
    step("set validator", lambda: call("problem.setValidator", validator="validator.cpp", **P))
    step("set checker", lambda: call("problem.setChecker", checker="check.cpp", **P))

    print("solutions")
    for name, tag, _ in SOLUTIONS:
        step(f"{name} [{tag}]", lambda name=name, tag=tag: call(
            "problem.saveSolution", name=name, file=read("solutions/" + name), tag=tag, **P))

    print("tests")
    spec = read_spec()
    step("enable groups", lambda: call("problem.enableGroups", testset=TESTSET, enable=True, **P))
    step("enable points", lambda: call("problem.enablePoints", enable=True, **P))
    script = []
    for i, (group, cmd) in enumerate(spec, start=1):
        if cmd.startswith("manual "):
            step(f"manual test {i}", lambda i=i, group=group, cmd=cmd: call(
                "problem.saveTest", testset=TESTSET, testIndex=i, testInput=read(cmd.split(None, 1)[1]),
                testGroup=group, testPoints=0, testDescription="sample", testUseInStatements=True, **P))
        else:
            script.append(f"gen {cmd} > {i}")
    step(f"generator script ({len(script)} tests)", lambda: call(
        "problem.saveScript", testset=TESTSET, source="\n".join(script) + "\n", **P))

    by_group = {}
    for i, (group, _) in enumerate(spec, start=1):
        by_group.setdefault(group, []).append(i)
    for group, idx in sorted(by_group.items()):
        step(f"group {group}: tests {idx[0]}..{idx[-1]}", lambda group=group, idx=idx: call(
            "problem.setTestGroup", testset=TESTSET, testGroup=group,
            testIndices=",".join(map(str, idx)), **P))
    for group, idx in sorted(by_group.items()):
        for i in idx:                      # all the group's points sit on its last test
            pts = GROUP_POINTS[group] if i == idx[-1] else 0
            if pts or i == idx[-1]:
                step(f"points {pts} on test {i}", lambda i=i, pts=pts: call(
                    "problem.saveTest", testset=TESTSET, testIndex=i, testPoints=pts, **P))
    for group in sorted(by_group):
        kwargs = dict(pointsPolicy="EACH_TEST" if group == "0" else "COMPLETE_GROUP")
        if group in GROUP_DEPENDENCIES:
            kwargs["dependencies"] = GROUP_DEPENDENCIES[group]
        step(f"group {group} policy {kwargs}", lambda group=group, kwargs=kwargs: call(
            "problem.saveTestGroup", testset=TESTSET, group=group, **kwargs, **P))

    print("per-group solution tags")
    for name, _, groups in SOLUTIONS:
        for group in groups:
            step(f"{name}: group {group} OK", lambda name=name, group=group: call(
                "problem.editSolutionExtraTags", remove=False, name=name, testGroup=group, tag="OK", **P))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("command", choices=["list", "upload", "commit", "build", "packages", "cautions", "info",
                                        "invocations"])
    ap.add_argument("arg", nargs="?")
    ap.add_argument("--id", type=int)
    ap.add_argument("--name")
    ap.add_argument("--package", default=".", help="package folder (default: current directory)")
    ap.add_argument("-m", "--message", default="Upload package through the API")
    args = ap.parse_args()
    global ROOT
    ROOT = os.path.abspath(args.package)

    if args.command == "list":
        for p in call("problems.list"):
            if not args.arg or args.arg in p["name"]:
                print(p["id"], p["name"], p["owner"], p["accessType"], "rev", p.get("revision"),
                      "modified" if p.get("modified") else "")
        return
    pid = args.id
    if args.command == "upload" and pid is None:
        if not args.name:
            sys.exit("upload needs --id or --name")
        created = call("problem.create", name=args.name)
        pid = created["id"]
        print("created problem", pid, created["name"])
    if pid is None:
        sys.exit("--id is required")
    if args.command == "upload":
        upload(pid)
        print("problem id:", pid)
    elif args.command == "commit":
        print(call("problem.commitChanges", problemId=pid, minorChanges=True, message=args.message))
    elif args.command == "build":
        print(call("problem.buildPackage", problemId=pid, full=True, verify=True))
    elif args.command == "packages":
        for p in call("problem.packages", problemId=pid):
            print(p["id"], "rev", p["revision"], p["type"], p["state"], (p.get("comment") or "")[:300])
    elif args.command == "cautions":
        print(json.dumps(call("problem.cautions", problemId=pid), indent=1)[:6000])
    elif args.command == "info":
        print(call("problem.info", problemId=pid))
    elif args.command == "invocations":
        for inv in call("problem.invocations", problemId=pid):
            print(inv["id"], inv["testset"], inv["status"], "violateTag" if inv.get("violateTag") else "",
                  inv.get("description", ""))


if __name__ == "__main__":
    main()
