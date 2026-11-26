"""Mirror a Polygon problem as a folder: snapshot (remote), read/write (disk), diff (push plan).

Folder layout:
    problem.json                     everything that is not a file (see README)
    statements/<lang>/<field>.tex    legend, input, output, scoring, interaction, notes, tutorial
    statements/resources/<name>      statement resource files
    files/{source,resource,aux}/<name>
    solutions/<name>
    tests/<testset>/script.txt       generator script
    tests/<testset>/<index>.in       manual test inputs
    tests/<testset>/<index>.statement.{in,out}   input/output shown in the statement, if overridden
"""
import base64
import hashlib
import json
import os

from api import PolygonError

MANIFEST = "problem.json"
CACHE = ".pg-cache.json"      # digests of remote files, so diff/push need not download them again
STATEMENT_FIELDS = ["legend", "input", "output", "scoring", "interaction", "notes", "tutorial"]
STATEMENT_META = ["name", "encoding", "showInReview", "showCautionsAndGrammaticalFixes"]
FILE_TYPES = ["source", "resource", "aux"]
PROTECTED_RESOURCES = {"olymp.sty", "problem.tex", "statements.ftl"}
INTERACTIVE_ONLY = {"useInteractionLogInExampleTests", "interactiveSecondRun"}
TEST_META = {  # manifest key -> problem.saveTest parameter
    "points": "testPoints", "description": "testDescription", "useInStatements": "testUseInStatements",
    "verifyInputOutputForStatements": "verifyInputOutputForStatements",
}
STRESS_KEYS = ["scriptPattern", "solutionNames", "timeLimit", "memoryLimit", "totalTimeLimit", "testset", "group",
               "ignoreValidators", "description"]


def norm(data):
    """Bytes with CRLF folded to LF when the content is text."""
    if isinstance(data, str):
        data = data.encode("utf-8")
    try:
        data.decode("utf-8")
    except UnicodeDecodeError:
        return data
    return data.replace(b"\r\n", b"\n")


def text(data):
    return norm(data or b"").decode("utf-8", "replace").strip()


class Cached:
    """Remote content known only by its digest (unchanged since it was last downloaded)."""

    def __init__(self, sha):
        self.sha = sha


def digest(data):
    data = data or b""
    canon = norm(data)
    if canon is not data or _is_text(data):
        canon = canon.rstrip()
    return hashlib.sha256(canon).hexdigest()


def _is_text(data):
    try:
        data.decode("utf-8")
        return True
    except UnicodeDecodeError:
        return False


def same(a, b):
    if isinstance(a, Cached) or isinstance(b, Cached):
        sha = lambda x: x.sha if isinstance(x, Cached) else digest(x)
        return sha(a) == sha(b)
    return digest(a) == digest(b)


def load_cache(root):
    try:
        with open(os.path.join(root, CACHE), encoding="utf-8") as f:
            return json.load(f)
    except (OSError, ValueError):
        return {}


def load_manifest(root):
    path = os.path.join(root, MANIFEST)
    if not os.path.exists(path):
        return {}
    with open(path, encoding="utf-8") as f:
        return json.load(f)


def save_manifest(root, manifest):
    with open(os.path.join(root, MANIFEST), "w", encoding="utf-8", newline="\n") as f:
        json.dump(manifest, f, indent=2, ensure_ascii=False)
        f.write("\n")


def local_only(local, remote):
    """Paths (relative to the folder) of files that exist locally but not in `remote`."""
    out = []
    out += [f"solutions/{n}" for n in local["solutions"] if n not in remote["solutions"]]
    for t in FILE_TYPES:
        out += [f"files/{t}/{n}" for n in local["files"][t] if n not in remote["files"][t]]
    out += [f"statements/resources/{n}" for n in local["resources"] if n not in remote["resources"]]
    for lang, s in local["statements"].items():
        have = remote["statements"].get(lang, {"fields": {}})["fields"]
        out += [f"statements/{lang}/{f}.tex" for f in s["fields"] if f not in have]
    for ts, entry in local["testsets"].items():
        have = remote["testsets"].get(ts, {"tests": {}})["tests"]
        for i, t in sorted(entry["tests"].items()):
            if t.get("manual") and not have.get(i, {}).get("manual"):
                out.append(f"tests/{ts}/{i}.in")
            for key, suffix in (("inputForStatement", "statement.in"), ("outputForStatement", "statement.out")):
                if t.get(key) and not have.get(i, {}).get(key):
                    out.append(f"tests/{ts}/{i}.{suffix}")
    return out


def save_cache(root, cache):
    with open(os.path.join(root, CACHE), "w", encoding="utf-8", newline="\n") as f:
        json.dump(cache, f, indent=1, sort_keys=True)


def empty_state():
    return {"polygon": {}, "info": {}, "checker": None, "validator": None, "interactor": None, "tags": [],
            "description": "", "generalTutorial": "", "pointsEnabled": False, "statements": {}, "resources": {},
            "files": {t: {} for t in FILE_TYPES}, "solutions": {}, "testsets": {}, "validatorTests": {},
            "checkerTests": {}, "stresses": {}, "materials": {}}


# -- remote ---------------------------------------------------------------

def snapshot(api, pid, testsets=("tests",), log=lambda s: None, cache=None):
    """Current working copy as a state. With `cache` (from load_cache), files whose modification time
    and length are unchanged are not downloaded again. The fresh cache is returned in state["_cache"]."""
    P = dict(problemId=pid)
    fresh = {}

    def fetch(key, meta, method, **kw):
        stamp = [meta.get("modificationTimeSeconds"), meta.get("length")]
        old = (cache or {}).get(key)
        if cache is not None and old and old["stamp"] == stamp:
            fresh[key] = old
            return Cached(old["sha"])
        data = api.raw(method, **P, **kw)
        fresh[key] = {"stamp": stamp, "sha": digest(data)}
        return data

    def soft(method, default, **kw):
        try:
            return api.call(method, **P, **kw)
        except PolygonError:
            return default

    st = empty_state()
    listed = [p for p in api.call("problems.list", id=pid) if p["id"] == pid]
    if listed:
        st["polygon"] = {k: listed[0].get(k) for k in ("id", "name", "owner", "revision")}
    else:
        st["polygon"] = {"id": pid}
    log("info, statements")
    st["info"] = api.call("problem.info", **P)
    st["checker"] = soft("problem.checker", None) or None
    st["validator"] = soft("problem.validator", None) or None
    st["interactor"] = (soft("problem.interactor", None) or None) if st["info"].get("interactive") else None
    st["tags"] = sorted(soft("problem.viewTags", []))
    st["description"] = soft("problem.viewGeneralDescription", "") or ""
    st["generalTutorial"] = soft("problem.viewGeneralTutorial", "") or ""

    for lang, s in api.call("problem.statements", **P).items():
        st["statements"][lang] = {
            "meta": {k: s[k] for k in STATEMENT_META if k in s},
            "fields": {k: s[k] for k in STATEMENT_FIELDS if k in s and (s[k] or k == "scoring")},
        }
    for f in api.call("problem.statementResources", **P):
        st["resources"][f["name"]] = fetch("resource:" + f["name"], f, "problem.viewStatementResource",
                                           name=f["name"])

    log("files, solutions")
    files = api.call("problem.files", **P)
    for t in FILE_TYPES:
        for f in files.get(t + "Files", []):
            meta = {}
            if f.get("sourceType"):
                meta["sourceType"] = f["sourceType"]
            if f.get("resourceAdvancedProperties"):
                meta["advanced"] = f["resourceAdvancedProperties"]
            st["files"][t][f["name"]] = {"meta": meta, "data": fetch(f"file:{t}:{f['name']}", f, "problem.viewFile", type=t,
                                                                       name=f["name"])}
    for s in api.call("problem.solutions", **P):
        st["solutions"][s["name"]] = {"tag": s.get("tag"), "sourceType": s.get("sourceType"), "groupTags": {},
                                      "testsetTags": {},
                                      "data": fetch("solution:" + s["name"], s, "problem.viewSolution",
                                                    name=s["name"])}
    # Extra tags can only be read from a stored invocation, so this is best effort.
    invocations = soft("problem.invocations", [])
    st["extraTagsKnown"] = bool(invocations)
    if invocations:
        detail = soft("problem.invocation", {}, invocationId=invocations[0]["id"])
        for t in detail.get("solutionTags", []):
            if t.get("name") in st["solutions"]:
                st["solutions"][t["name"]]["groupTags"] = t.get("groupTags") or {}
                st["solutions"][t["name"]]["testsetTags"] = t.get("testsetTags") or {}

    for ts in testsets:
        log(f"testset {ts}")
        try:
            tests = api.call("problem.tests", testset=ts, **P)
        except PolygonError:
            continue
        entry = {"groupsEnabled": False, "groups": {}, "script": "", "tests": {}}
        try:
            entry["script"] = text(api.raw("problem.script", testset=ts, **P))
        except PolygonError:
            pass
        for t in tests:
            meta = {"manual": bool(t.get("manual"))}
            if t.get("manual"):
                meta["input"] = (base64.b64decode(t["inputBase64"]) if t.get("inputBase64") is not None
                                 else (t.get("input") or "").encode("utf-8"))
            for k in ("group", "points", "description", "useInStatements", "inputForStatement", "outputForStatement",
                      "interactionLogForStatement", "verifyInputOutputForStatements"):
                if t.get(k) is not None and t[k] != "" and t[k] is not False:
                    meta[k] = t[k]
            if "points" in t:
                st["pointsEnabled"] = True
            entry["tests"][int(t["index"])] = meta
        try:
            for g in api.call("problem.viewTestGroup", testset=ts, **P):
                entry["groups"][g["name"]] = {"pointsPolicy": g.get("pointsPolicy"),
                                              "feedbackPolicy": g.get("feedbackPolicy"),
                                              "dependencies": g.get("dependencies") or []}
            entry["groupsEnabled"] = bool(entry["groups"]) or any("group" in t for t in entry["tests"].values())
        except PolygonError:
            pass
        st["testsets"][ts] = entry

    log("validator/checker tests, stresses, materials")
    for t in soft("problem.validatorTests", []):
        st["validatorTests"][int(t["index"])] = {k: t[k] for k in ("input", "expectedVerdict", "testset", "group")
                                                 if t.get(k) is not None}
    for t in soft("problem.checkerTests", []):
        st["checkerTests"][int(t["index"])] = {k: t[k] for k in ("input", "output", "answer", "expectedVerdict")
                                               if t.get(k) is not None}
    for s in soft("problem.stresses", []):
        st["stresses"][int(s["index"])] = {k: s[k] for k in STRESS_KEYS if k in s}
    for m in soft("problem.materials", []):
        st["materials"][m["name"]] = {"publishStrategy": m.get("publishStrategy"), "items": m.get("items") or []}
    st["_cache"] = fresh
    return st


# -- disk -----------------------------------------------------------------

def _write(path, data):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "wb") as f:
        f.write(norm(data))


def write(st, root):
    """Write a state as a folder. Returns the manifest path."""
    for lang, s in st["statements"].items():
        for field, value in s["fields"].items():
            _write(os.path.join(root, "statements", lang, field + ".tex"), text(value) + "\n" if value else b"")
    for name, data in st["resources"].items():
        _write(os.path.join(root, "statements", "resources", name), data)
    for t in FILE_TYPES:
        for name, f in st["files"][t].items():
            _write(os.path.join(root, "files", t, name), f["data"])
    for name, s in st["solutions"].items():
        _write(os.path.join(root, "solutions", name), s["data"])

    manifest = {
        "polygon": st["polygon"], "info": st["info"], "checker": st["checker"], "validator": st["validator"],
        "interactor": st["interactor"], "tags": st["tags"], "description": st["description"],
        "generalTutorial": st["generalTutorial"], "pointsEnabled": st["pointsEnabled"],
        "statements": {lang: s["meta"] for lang, s in st["statements"].items()},
        "files": {t: {n: f["meta"] for n, f in st["files"][t].items() if f["meta"]} for t in FILE_TYPES},
        "solutions": {n: {k: v for k, v in s.items() if k != "data" and v not in (None, {})}
                      for n, s in st["solutions"].items()},
        "testsets": {},
        "validatorTests": {str(i): t for i, t in sorted(st["validatorTests"].items())},
        "checkerTests": {str(i): t for i, t in sorted(st["checkerTests"].items())},
        "stresses": {str(i): s for i, s in sorted(st["stresses"].items())},
        "materials": st["materials"],
    }
    if not st["info"].get("interactive"):
        manifest.pop("interactor")
    for ts, entry in st["testsets"].items():
        base = os.path.join(root, "tests", ts)
        _write(os.path.join(base, "script.txt"), entry["script"] + "\n" if entry["script"] else b"")
        tests = {}
        for i, t in sorted(entry["tests"].items()):
            meta = {k: v for k, v in t.items() if k not in ("manual", "input", "inputForStatement",
                                                            "outputForStatement")}
            if t.get("manual"):
                _write(os.path.join(base, f"{i}.in"), t.get("input") or b"")
            if t.get("inputForStatement"):
                _write(os.path.join(base, f"{i}.statement.in"), t["inputForStatement"])
            if t.get("outputForStatement"):
                _write(os.path.join(base, f"{i}.statement.out"), t["outputForStatement"])
            tests[str(i)] = meta
        manifest["testsets"][ts] = {"groupsEnabled": entry["groupsEnabled"], "groups": entry["groups"],
                                    "tests": tests}
    save_manifest(root, manifest)
    return os.path.join(root, MANIFEST)


def _ls(path):
    return sorted(n for n in os.listdir(path) if os.path.isfile(os.path.join(path, n))) if os.path.isdir(path) else []


def _read(path):
    with open(path, "rb") as f:
        return f.read()


def read(root):
    """Load a folder as a state. Files on disk decide what exists; problem.json supplies the rest."""
    manifest = load_manifest(root)
    st = empty_state()
    for k in ("polygon", "info", "checker", "validator", "interactor", "tags", "description", "generalTutorial",
              "pointsEnabled", "materials"):
        if k in manifest:
            st[k] = manifest[k]
    st["tags"] = sorted(st["tags"])

    sdir = os.path.join(root, "statements")
    langs = [d for d in (os.listdir(sdir) if os.path.isdir(sdir) else [])
             if d != "resources" and os.path.isdir(os.path.join(sdir, d))]
    for lang in sorted(set(langs) | set(manifest.get("statements", {}))):
        fields = {}
        for field in STATEMENT_FIELDS:
            p = os.path.join(sdir, lang, field + ".tex")
            if os.path.exists(p):
                fields[field] = text(_read(p))
        st["statements"][lang] = {"meta": manifest.get("statements", {}).get(lang, {}), "fields": fields}
    for name in _ls(os.path.join(sdir, "resources")):
        st["resources"][name] = _read(os.path.join(sdir, "resources", name))
    for t in FILE_TYPES:
        for name in _ls(os.path.join(root, "files", t)):
            st["files"][t][name] = {"meta": manifest.get("files", {}).get(t, {}).get(name, {}),
                                    "data": _read(os.path.join(root, "files", t, name))}
    for name in _ls(os.path.join(root, "solutions")):
        meta = manifest.get("solutions", {}).get(name, {})
        st["solutions"][name] = {"tag": meta.get("tag"), "sourceType": meta.get("sourceType"),
                                 "groupTags": meta.get("groupTags", {}), "testsetTags": meta.get("testsetTags", {}),
                                 "data": _read(os.path.join(root, "solutions", name))}

    tdir = os.path.join(root, "tests")
    names = set(manifest.get("testsets", {})) | {d for d in (os.listdir(tdir) if os.path.isdir(tdir) else [])
                                                 if os.path.isdir(os.path.join(tdir, d))}
    for ts in sorted(names):
        m = manifest.get("testsets", {}).get(ts, {})
        base = os.path.join(tdir, ts)
        entry = {"groupsEnabled": m.get("groupsEnabled", False), "groups": m.get("groups", {}), "script": "",
                 "tests": {int(i): dict(t) for i, t in m.get("tests", {}).items()}}
        if os.path.exists(os.path.join(base, "script.txt")):
            entry["script"] = text(_read(os.path.join(base, "script.txt")))
        for name in _ls(base):
            head = name.split(".", 1)[0]
            if not head.isdigit():
                continue
            t = entry["tests"].setdefault(int(head), {})
            data = _read(os.path.join(base, name))
            if name == head + ".in":
                t["manual"], t["input"] = True, data
            elif name == head + ".statement.in":
                t["inputForStatement"] = text(data)
            elif name == head + ".statement.out":
                t["outputForStatement"] = text(data)
        if any("group" in t for t in entry["tests"].values()):
            entry["groupsEnabled"] = True
        if any("points" in t for t in entry["tests"].values()):
            st["pointsEnabled"] = True
        st["testsets"][ts] = entry
    for key in ("validatorTests", "checkerTests", "stresses"):
        st[key] = {int(i): v for i, v in manifest.get(key, {}).items()}
    return st


# -- diff -----------------------------------------------------------------

def plan(api, pid, local, remote, prune=False):
    """List of (description, callable) that turns `remote` into `local`."""
    P = dict(problemId=pid)
    ops = []

    def op(desc, method, **kw):
        ops.append((desc, lambda: api.call(method, **P, **kw)))

    # general info
    changed = {k: v for k, v in local["info"].items() if remote["info"].get(k) != v
               and (k not in INTERACTIVE_ONLY or local["info"].get("interactive"))}
    if changed:
        op("info: " + ", ".join(f"{k}={v}" for k, v in changed.items()), "problem.updateInfo", **changed)

    # files
    for t in FILE_TYPES:
        for name, f in local["files"][t].items():
            r = remote["files"][t].get(name)
            if r and same(r["data"], f["data"]) and (not f["meta"] or f["meta"] == r["meta"]):
                continue
            kw = dict(type=t, name=name, file=norm(f["data"]))
            if f["meta"].get("sourceType"):
                kw["sourceType"] = f["meta"]["sourceType"]
            adv = f["meta"].get("advanced")
            if adv:
                kw.update(forTypes=adv.get("forTypes", ""), stages=";".join(adv.get("stages", [])),
                          assets=";".join(adv.get("assets", [])))
            op(f"{'update' if r else 'add'} {t} file {name}", "problem.saveFile", **kw)
        if prune:
            for name in remote["files"][t]:
                if name not in local["files"][t] and not (t == "resource" and name in PROTECTED_RESOURCES):
                    op(f"delete {t} file {name}", "problem.deleteFile", type=t, name=name)
    for key, method in (("checker", "problem.setChecker"), ("validator", "problem.setValidator"),
                        ("interactor", "problem.setInteractor")):
        if local.get(key) and local[key] != remote.get(key):
            op(f"set {key} {local[key]}", method, **{key: local[key]})

    # solutions
    for name, s in local["solutions"].items():
        r = remote["solutions"].get(name)
        kw = {}
        if not r or not same(r["data"], s["data"]):
            kw["file"] = norm(s["data"])
        tag = s.get("tag") or (None if r else "OK")
        if tag and (not r or r.get("tag") != tag):
            kw["tag"] = tag
        if s.get("sourceType") and (not r or r.get("sourceType") != s["sourceType"]):
            kw["sourceType"] = s["sourceType"]
        if kw:
            what = "add" if not r else "update" if "file" in kw else "retag"
            op(f"{what} solution {name}" + (f" [{kw['tag']}]" if "tag" in kw else ""),
               "problem.saveSolution", name=name, **kw)
        for scope, param in (("groupTags", "testGroup"), ("testsetTags", "testset")):
            have = (r or {}).get(scope) or {}
            want = s.get(scope) or {}
            for target, tag in want.items():
                if have.get(target) != tag:
                    op(f"solution {name}: expect {tag} on {param} {target}", "problem.editSolutionExtraTags",
                       name=name, remove=False, tag=tag, **{param: target})
            for target in have:
                if target not in want:
                    op(f"solution {name}: drop extra tag on {param} {target}", "problem.editSolutionExtraTags",
                       name=name, remove=True, **{param: target})
    if prune:
        for name in remote["solutions"]:
            if name not in local["solutions"]:
                op(f"delete solution {name}", "problem.deleteSolution", name=name)

    # statements
    for lang, s in local["statements"].items():
        r = remote["statements"].get(lang, {"meta": {}, "fields": {}})
        kw = {k: v for k, v in s["meta"].items() if r["meta"].get(k) != v}
        for field, value in s["fields"].items():
            if field not in r["fields"] or text(r["fields"][field]) != text(value):
                kw[field] = text(value)
        if prune:
            for field in r["fields"]:
                if field not in s["fields"] and field != "scoring":
                    kw[field] = ""
        if kw or lang not in remote["statements"]:
            op(f"statement {lang}: " + (", ".join(sorted(kw)) or "create"), "problem.saveStatement", lang=lang, **kw)
    for name, data in local["resources"].items():
        if name not in remote["resources"] or not same(remote["resources"][name], data):
            op(f"statement resource {name}", "problem.saveStatementResource", name=name, file=data)
    if prune:
        for lang in remote["statements"]:
            if lang not in local["statements"]:
                op(f"delete statement {lang}", "problem.deleteStatement", lang=lang)
        for name in remote["resources"]:
            if name not in local["resources"]:
                op(f"delete statement resource {name}", "problem.deleteStatementResource", name=name)

    # tests
    if local["pointsEnabled"] and not remote["pointsEnabled"]:
        op("enable points", "problem.enablePoints", enable=True)
    for ts, entry in local["testsets"].items():
        r = remote["testsets"].get(ts, {"groupsEnabled": False, "groups": {}, "script": "", "tests": {}})
        T = dict(testset=ts)
        if entry["groupsEnabled"] and not r["groupsEnabled"]:
            op(f"{ts}: enable groups", "problem.enableGroups", enable=True, **T)
        for i, t in sorted(entry["tests"].items()):
            if t.get("manual") and not (r["tests"].get(i, {}).get("manual")
                                        and same(r["tests"][i].get("input"), t.get("input"))):
                op(f"{ts}: manual test {i}", "problem.saveTest", testIndex=i, testInput=norm(t.get("input") or b""),
                   **T)
        if prune:
            gone = [i for i, t in sorted(r["tests"].items()) if t.get("manual")
                    and not entry["tests"].get(i, {}).get("manual")]
            if gone:
                op(f"{ts}: delete manual tests {gone}", "problem.deleteTest", testIndices=",".join(map(str, gone)),
                   **T)
        if text(entry["script"]) != text(r["script"]):
            if entry["script"]:
                op(f"{ts}: generator script ({len(entry['script'].splitlines())} lines)", "problem.saveScript",
                   source=entry["script"] + "\n", **T)
            elif prune:
                op(f"{ts}: clear generator script", "problem.clearScript", **T)
        by_group = {}
        for i, t in sorted(entry["tests"].items()):
            have = r["tests"].get(i, {})
            if t.get("group") is not None and str(t["group"]) != str(have.get("group")):
                by_group.setdefault(str(t["group"]), []).append(i)
        for group, idx in by_group.items():
            op(f"{ts}: group {group} <- tests {_ranges(idx)}", "problem.setTestGroup", testGroup=group,
               testIndices=",".join(map(str, idx)), **T)
        for i, t in sorted(entry["tests"].items()):
            have = r["tests"].get(i, {})
            kw = {}
            for key, param in TEST_META.items():
                if key not in t:
                    continue
                default = False if isinstance(t[key], bool) else 0 if isinstance(t[key], (int, float)) else ""
                if t[key] != have.get(key, default):
                    kw[param] = t[key]
            for key, param in (("inputForStatement", "testInputForStatements"),
                               ("outputForStatement", "testOutputForStatements"),
                               ("interactionLogForStatement", "testInteractionLogForStatements")):
                if key in t and text(t[key]) != text(have.get(key)):
                    kw[param] = t[key]
            if kw:
                op(f"{ts}: test {i}: " + ", ".join(f"{k}={_short(v)}" for k, v in kw.items()), "problem.saveTest",
                   testIndex=i, **kw, **T)
        for group, g in entry["groups"].items():
            have = r["groups"].get(group, {})
            kw = {k: g[k] for k in ("pointsPolicy", "feedbackPolicy") if g.get(k) and g[k] != have.get(k)}
            if sorted(g.get("dependencies") or []) != sorted(have.get("dependencies") or []):
                kw["dependencies"] = ",".join(g.get("dependencies") or [])
            if kw:
                op(f"{ts}: group {group}: " + ", ".join(f"{k}={v}" for k, v in kw.items()), "problem.saveTestGroup",
                   group=group, **kw, **T)

    # tags, descriptions
    if sorted(local["tags"]) != sorted(remote["tags"]) and (local["tags"] or prune):
        op("tags: " + ", ".join(local["tags"]), "problem.saveTags", tags=",".join(local["tags"]))
    if text(local["description"]) != text(remote["description"]):
        op("general description", "problem.saveGeneralDescription", description=local["description"])
    if text(local["generalTutorial"]) != text(remote["generalTutorial"]):
        op("general tutorial", "problem.saveGeneralTutorial", tutorial=local["generalTutorial"])

    # validator and checker tests
    for i, t in sorted(local["validatorTests"].items()):
        if _vt(t) != _vt(remote["validatorTests"].get(i)):
            kw = dict(testIndex=i, testInput=t.get("input", ""), testVerdict=t.get("expectedVerdict"))
            kw.update({p: t[k] for k, p in (("testset", "testset"), ("group", "testGroup")) if t.get(k)})
            op(f"validator test {i} [{t.get('expectedVerdict')}]", "problem.saveValidatorTest", **kw)
    for i, t in sorted(local["checkerTests"].items()):
        if _vt(t) != _vt(remote["checkerTests"].get(i)):
            op(f"checker test {i} [{t.get('expectedVerdict')}]", "problem.saveCheckerTest", testIndex=i,
               testInput=t.get("input", ""), testOutput=t.get("output", ""), testAnswer=t.get("answer", ""),
               testVerdict=t.get("expectedVerdict"))
    if prune:
        for i in remote["validatorTests"]:
            if i not in local["validatorTests"]:
                op(f"delete validator test {i}", "problem.deleteValidatorTest", testIndex=i)
        for i in remote["checkerTests"]:
            if i not in local["checkerTests"]:
                op(f"delete checker test {i}", "problem.deleteCheckerTest", testIndex=i)

    # stresses, materials
    for i, s in sorted(local["stresses"].items()):
        have = remote["stresses"].get(i)
        if have != s:
            kw = {k: (",".join(v) if isinstance(v, list) else v) for k, v in s.items() if k in STRESS_KEYS}
            if have:
                op(f"update stress {i}: {s.get('scriptPattern')}", "problem.saveStress", stressIndex=i, **kw)
            else:
                ops.append((f"create stress {i}: {s.get('scriptPattern')}",
                            lambda i=i, kw=kw: _create_stress(api, P, i, kw)))
    for name, m in local["materials"].items():
        if remote["materials"].get(name) != m:
            op(f"material {name}", "problem.setMaterial", name=name, publishStrategy=m.get("publishStrategy"),
               items=json.dumps(m.get("items") or []))
    if prune:
        for i in remote["stresses"]:
            if i not in local["stresses"]:
                op(f"delete stress {i}", "problem.deleteStress", stressIndex=i)
        for name in remote["materials"]:
            if name not in local["materials"]:
                op(f"remove material {name}", "problem.setMaterial", name=name, remove=True)
    return ops


def _create_stress(api, P, wanted, kw):
    created = api.call("problem.saveStress", **P, **kw)
    if created and created.get("index") != wanted:
        raise PolygonError("problem.saveStress", f"created as stress {created.get('index')}, but problem.json "
                           f"calls it {wanted}; renumber it there or the next push creates it again")


def _vt(t):
    if not t:
        return None
    return {k: (text(v) if isinstance(v, str) else v) for k, v in t.items()}


def _short(v):
    s = str(v).replace("\n", "\\n")
    return s if len(s) <= 30 else s[:27] + "..."


def _ranges(idx):
    out, start, prev = [], None, None
    for i in idx:
        if start is None:
            start = prev = i
        elif i == prev + 1:
            prev = i
        else:
            out.append(str(start) if start == prev else f"{start}-{prev}")
            start = prev = i
    if start is not None:
        out.append(str(start) if start == prev else f"{start}-{prev}")
    return ",".join(out)
