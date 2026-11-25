#!/usr/bin/env python3
"""pg - command line for Polygon (https://polygon.codeforces.com).

    pg pull downhill                 mirror a problem into ./downhill
    cd downhill; edit files ...
    pg diff                          what push would change
    pg push --commit -m "message"    upload the changes and commit them
    pg build                         build + verify a package, print the verdict table
    pg <group> <verb> ...            one command per API method (pg --help lists them)
    pg api METHOD key=value ...      any API method, including ones added later

The problem is taken from -p ID|NAME, else POLYGON_PROBLEM, else problem.json in the current folder.
"""
import argparse
import base64
import json
import os
import re
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import sync  # noqa: E402
from api import Polygon, PolygonError  # noqa: E402
from methods import METHODS, parse_spec  # noqa: E402


def kebab(name):
    return re.sub(r"([A-Z])", lambda m: "-" + m.group(1).lower(), name)


def read_arg(value):
    """@path -> file content (bytes), @- -> stdin, anything else unchanged."""
    if isinstance(value, str) and value.startswith("@"):
        return read_path(value[1:])
    return value


def read_path(path):
    if path == "-":
        return sys.stdin.buffer.read()
    with open(path, "rb") as f:
        return sync.norm(f.read())


# -- output ---------------------------------------------------------------

def emit(result, args):
    if result is None:
        print("ok")
    elif isinstance(result, str):
        print(result)
    elif (not args.json and isinstance(result, list) and result
          and all(isinstance(r, dict) for r in result)):
        table(result)
    else:
        print(json.dumps(result, indent=2, ensure_ascii=False))


def table(rows):
    cols = []
    for r in rows:
        for k in r:
            if k not in cols:
                cols.append(k)

    def cell(v):
        if v is None:
            return ""
        if isinstance(v, bool):
            return "yes" if v else "no"
        s = json.dumps(v, ensure_ascii=False) if isinstance(v, (dict, list)) else str(v)
        s = s.replace("\r", "").replace("\n", "\\n")
        return s if len(s) <= 60 else s[:57] + "..."

    data = [[cell(r.get(c)) for c in cols] for r in rows]
    width = [max(len(c), *(len(d[i]) for d in data)) for i, c in enumerate(cols)]
    print("  ".join(c.ljust(w) for c, w in zip(cols, width)).rstrip())
    for d in data:
        print("  ".join(x.ljust(w) for x, w in zip(d, width)).rstrip())


def write_out(data, args, default_name=None):
    target = args.out_file or default_name
    if target:
        with open(target, "wb") as f:
            f.write(data)
        print(f"wrote {target} ({len(data)} bytes)")
    else:
        sys.stdout.buffer.write(data)


# -- problem selection ----------------------------------------------------

def problem_id(api, args, folder="."):
    ref = getattr(args, "problem", None) or os.environ.get("POLYGON_PROBLEM")
    if not ref:
        path = os.path.join(folder, sync.MANIFEST)
        if os.path.exists(path):
            ref = (json.load(open(path, encoding="utf-8")).get("polygon") or {}).get("id")
    if not ref:
        raise SystemExit("no problem given: use -p ID|NAME, set POLYGON_PROBLEM, or run inside a pulled folder")
    return api.resolve(ref)


# -- table-driven commands ------------------------------------------------

def run_method(args):
    api = Polygon(pin=args.pin)
    method, spec, kind, fixed = args._method, args._spec, args._kind, args._fixed
    params = dict(fixed)
    values = vars(args)
    for p in spec:
        v = values.get(p["name"])
        if p["positional"]:
            v = read_path(v) if p["file"] else v
        elif p["basename_of"]:
            v = v or os.path.basename(values[p["basename_of"]])
        elif not p["bool"]:
            v = read_arg(v)
        if v is not None:
            params[p["name"]] = v
    if method.startswith("problem.") and method != "problem.create":
        params["problemId"] = problem_id(api, args)
    if kind == "raw":
        write_out(api.raw(method, **params), args)
    else:
        emit(api.call(method, **params), args)


# -- hand-written commands ------------------------------------------------

def cmd_api(args):
    api = Polygon(pin=args.pin)
    params = {}
    for item in args.params:
        if "=" not in item:
            raise SystemExit(f"expected key=value, got {item!r}")
        k, v = item.split("=", 1)
        v = read_arg(v)
        params[k] = [params[k], v] if k in params and not isinstance(params[k], list) else \
            params[k] + [v] if k in params else v
    needs_problem = args.method.startswith("problem.") and args.method != "problem.create"
    if needs_problem and "problemId" not in params:
        params["problemId"] = problem_id(api, args)
    if args.raw:
        write_out(api.raw(args.method, **params), args)
    else:
        emit(api.call(args.method, **params), args)


def cmd_pull(args):
    api = Polygon(pin=args.pin)
    pid = api.resolve(args.problem_ref) if args.problem_ref else problem_id(api, args, args.dir or ".")
    st = sync.snapshot(api, pid, args.testset or ["tests"], log=lambda s: print("  reading", s))
    folder = args.dir or st["polygon"].get("name") or str(pid)
    if not st["extraTagsKnown"]:
        # Polygon exposes per-group/testset solution tags only through a stored invocation.
        known = sync.load_cache(folder).get("_extraTags") or extra_tags(sync.read(folder))             if os.path.exists(os.path.join(folder, sync.MANIFEST)) else {}
        for name, tags in known.items():
            if name in st["solutions"]:
                st["solutions"][name].update(tags)
        print("  note: no invocation is stored, so per-group solution tags could not be read from Polygon"
              + ("; kept the ones already in this folder" if known else ""))
    sync.write(st, folder)
    st["_cache"]["_extraTags"] = extra_tags(st)
    sync.save_cache(folder, st["_cache"])
    print(f"pulled problem {pid} ({st['polygon'].get('name')}, revision {st['polygon'].get('revision')}) "
          f"into {os.path.abspath(folder)}")
    print(f"  {len(st['solutions'])} solutions, {sum(len(v) for v in st['files'].values())} files, "
          f"{len(st['statements'])} statements, "
          + ", ".join(f"{len(e['tests'])} tests in {ts}" for ts, e in st["testsets"].items()))


def cmd_push(args):
    api = Polygon(pin=args.pin)
    folder = args.dir or "."
    if not os.path.exists(os.path.join(folder, sync.MANIFEST)):
        raise SystemExit(f"{os.path.join(folder, sync.MANIFEST)} not found")
    local = sync.read(folder)
    if getattr(args, "create", None):
        created = api.call("problem.create", name=args.create)
        pid = created["id"]
        print(f"created problem {pid} ({created['name']})")
        path = os.path.join(folder, sync.MANIFEST)
        manifest = json.load(open(path, encoding="utf-8"))
        manifest["polygon"] = {"id": pid, "name": created["name"], "owner": created.get("owner")}
        with open(path, "w", encoding="utf-8", newline="\n") as f:
            json.dump(manifest, f, indent=2, ensure_ascii=False)
            f.write("\n")
    else:
        pid = problem_id(api, args, folder)
    testsets = list(local["testsets"]) or ["tests"]
    if getattr(args, "force", False):
        remote = sync.empty_state()
    else:
        cache = sync.load_cache(folder)
        remote = sync.snapshot(api, pid, testsets, cache=cache)
        # Last known per-group solution tags: what this folder pushed or pulled, else the newest invocation.
        if "_extraTags" in cache:
            for name, s in remote["solutions"].items():
                s.update(cache["_extraTags"].get(name, {"groupTags": {}, "testsetTags": {}}))
        remote["_cache"]["_extraTags"] = extra_tags(remote)
        sync.save_cache(folder, remote["_cache"])
    for ts in testsets:
        remote["testsets"].setdefault(ts, {"groupsEnabled": False, "groups": {}, "script": "", "tests": {}})
    ops = sync.plan(api, pid, local, remote, prune=args.prune)
    if not ops:
        print(f"problem {pid}: nothing to change")
    dry = args.dry_run
    failed = 0
    for desc, run in ops:
        if dry:
            print("  would", desc)
            continue
        try:
            run()
            print("  ok   ", desc)
        except PolygonError as e:
            failed += 1
            print("  FAIL ", desc, "->", e.comment)
    if dry:
        return
    if failed:
        raise SystemExit(f"{failed} of {len(ops)} changes failed; nothing was committed")
    if not getattr(args, "force", False):
        remote["_cache"]["_extraTags"] = extra_tags(local)
        sync.save_cache(folder, remote["_cache"])
    if ops and args.commit:
        print(" ", api.call("problem.commitChanges", problemId=pid, minorChanges=not args.notify,
                            message=args.message or "Update through the API")["message"])
    elif ops:
        print(f"{len(ops)} changes are in your working copy; run `pg commit -m ...` to commit them")


def extra_tags(state):
    return {name: {"groupTags": s.get("groupTags") or {}, "testsetTags": s.get("testsetTags") or {}}
            for name, s in state["solutions"].items() if s.get("groupTags") or s.get("testsetTags")}


def cmd_diff(args):
    args.dry_run, args.commit, args.create, args.force = True, False, None, False
    cmd_push(args)


def latest_packages(api, pid):
    packages = api.call("problem.packages", problemId=pid)
    if not packages:
        return []
    newest = max(p["id"] for p in packages)
    return [p for p in packages if p["id"] == newest]


def cmd_build(args):
    api = Polygon(pin=args.pin)
    pid = problem_id(api, args)
    before = max([p["id"] for p in api.call("problem.packages", problemId=pid)] or [0])
    api.call("problem.buildPackage", problemId=pid, full=args.full, verify=args.verify)
    print("build started")
    if not args.wait:
        return
    state = wait_package(api, pid, newer_than=before)
    if args.verify:
        verdicts(api, pid)
    if state != "READY":
        raise SystemExit(1)


def wait_package(api, pid, newer_than=0, timeout=3600):
    start = time.time()
    while True:
        packages = [p for p in latest_packages(api, pid) if p["id"] > newer_than]
        states = {p["state"] for p in packages}
        if packages and not states & {"PENDING", "RUNNING"}:
            for p in packages:
                print(f"package {p['id']} revision {p['revision']} {p['type']}: {p['state']} {p.get('comment') or ''}")
            return "READY" if states == {"READY"} else "FAILED"
        if time.time() - start > timeout:
            raise SystemExit("timed out waiting for the package")
        time.sleep(5)


def cmd_wait(args):
    api = Polygon(pin=args.pin)
    if wait_package(api, problem_id(api, args)) != "READY":
        raise SystemExit(1)


def verdicts(api, pid, invocation_id=None, testset=None):
    P = dict(problemId=pid)
    if invocation_id is None:
        invocations = api.call("problem.invocations", **P)
        if not invocations:
            print("no invocations stored for this working copy")
            return
        invocation_id = invocations[0]["id"]
    data = api.call("problem.invocation", invocationId=invocation_id, **P)
    inv = data["invocation"]
    group_of = {t["index"]: t.get("group") for t in
                api.call("problem.tests", testset=testset or inv.get("testset") or "tests", noInputs=True, **P)}
    groups = sorted({g for g in group_of.values()}, key=lambda g: (g is None, str(g)))
    tags = {t["name"]: t.get("tag", "") for t in data.get("solutionTags", [])}
    cells, bad = {}, []
    for r in data["runs"]:
        c = cells.setdefault((r.get("solution"), group_of.get(r["test"])), [set(), 0])
        c[0].add(r["verdict"])
        c[1] = max(c[1], r.get("timeMillis") or 0)
        if r.get("violateTag"):
            bad.append(r)
    print(f"invocation {inv['id']} ({inv.get('testset')}): {inv['status']}, {len(data['runs'])} runs, "
          + (f"{len(bad)} runs violate tags" if bad else "tags respected"))
    head = ["solution", "tag"] + [("all" if g is None else "g" + str(g)) for g in groups]
    rows = []
    for sol in sorted({k[0] for k in cells}, key=str):
        row = [str(sol), tags.get(sol, "")]
        for g in groups:
            c = cells.get((sol, g))
            row.append("-" if not c else "/".join(sorted(c[0])) + f" {c[1]}ms")
        rows.append(row)
    width = [max(len(r[i]) for r in [head] + rows) for i in range(len(head))]
    for r in [head] + rows:
        print("  ".join(x.ljust(w) for x, w in zip(r, width)).rstrip())
    for r in bad[:20]:
        print(f"  violates tag: {r.get('solution')} test {r['test']}: {r['verdict']} {r.get('comment') or ''}"[:200])


def cmd_verdicts(args):
    api = Polygon(pin=args.pin)
    verdicts(api, problem_id(api, args), args.invocation)


def cmd_render(args):
    api = Polygon(pin=args.pin)
    result = api.call("problem.renderStatements", problemId=problem_id(api, args), includeContent=True)
    out = args.out_file or "."
    os.makedirs(out, exist_ok=True)
    failed = False
    for kind, label in (("statements", "statement"), ("tutorials", "tutorial")):
        for s in result.get(kind, []):
            for fmt in ("html", "pdf"):
                r = s[fmt]
                if r["status"] != "OK":
                    failed = True
                    print(f"{label} {s['language']} {fmt}: FAILED\n{r.get('message')}")
                    continue
                path = os.path.join(out, f"{label}-{s['language']}.{fmt}")
                with open(path, "wb") as f:
                    f.write(base64.b64decode(r["contentBase64"]))
                print(f"{label} {s['language']} {fmt}: {path}")
    if failed:
        raise SystemExit(1)


def cmd_download(args):
    api = Polygon(pin=args.pin)
    pid = problem_id(api, args)
    ready = [p for p in api.call("problem.packages", problemId=pid)
             if p["state"] == "READY" and p["type"] == args.type]
    if not ready:
        raise SystemExit(f"no READY {args.type} package; run `pg build`")
    p = max(ready, key=lambda p: p["id"])
    data = api.raw("problem.package", problemId=pid, packageId=p["id"], type=args.type)
    write_out(data, args, f"package-{pid}-r{p['revision']}-{args.type}.zip")


def cmd_stress_watch(args):
    api = Polygon(pin=args.pin)
    pid = problem_id(api, args)
    if args.run:
        api.call("problem.runStresses", problemId=pid, stressIndices=args.indices)
    wanted = {int(i) for i in args.indices.split(",")} if args.indices else None
    while True:
        stresses = [s for s in api.call("problem.stresses", problemId=pid) if wanted is None or s["index"] in wanted]
        if not any((s.get("run") or {}).get("status") in ("PENDING", "RUNNING") for s in stresses):
            break
        time.sleep(5)
    for s in stresses:
        run = s.get("run")
        if not run:
            print(f"stress {s['index']}: not run")
            continue
        ct = run.get("countertest")
        print(f"stress {s['index']} ({s['scriptPattern']}): {run.get('verdict')}, {run.get('judgedTestCount')} tests, "
              f"max {run.get('maxConsumedTime')} ms" + (f", {run['comment']}" if run.get("comment") else ""))
        if ct:
            print(f"  countertest: {ct.get('scriptLine')}")
            for rep in ct.get("reports", []):
                print(f"    {rep['solutionName']}: {rep['verdict']} {rep.get('comment') or ''}")
        else:
            print("  no countertest found")


def cmd_status(args):
    api = Polygon(pin=args.pin)
    pid = problem_id(api, args)
    p = [x for x in api.call("problems.list", id=pid) if x["id"] == pid][0]
    print(f"{p['name']} (id {pid}, owner {p['owner']}, access {p['accessType']}): revision {p.get('revision')}, "
          f"working copy {p.get('workingCopyRevision')}, "
          + ("uncommitted changes" if p.get("modified") else "no uncommitted changes")
          + f", latest package for revision {p.get('latestPackage')}")
    c = api.call("problem.cautions", problemId=pid)
    for key in ("common", "statement", "structure", "issues"):
        for item in c.get(key, []):
            print(f"  {item['severity'].lower()} caution: {item['message']}")
    for item in c.get("packageReadinessIssues", []):
        print(f"  blocks packaging: {item['message']}")
    for w in c.get("latestPackageWarnings", []):
        print(f"  package warning: {w}")


# -- parser ---------------------------------------------------------------

class Tree:
    """Nested argparse sub-commands addressed by a path like 'solution put'."""

    def __init__(self, parser, common):
        self.common = common
        self.nodes = {(): (parser, None)}

    def _subs(self, path):
        parser, subs = self.nodes[path]
        if subs is None:
            subs = parser.add_subparsers(metavar="COMMAND")
            subs.required = True
            self.nodes[path] = (parser, subs)
        return subs

    def add(self, path, help_text):
        parts = tuple(path.split())
        for depth in range(1, len(parts)):
            if parts[:depth] not in self.nodes:
                group = self._subs(parts[:depth - 1]).add_parser(parts[depth - 1], help=f"{parts[depth - 1]} commands")
                self.nodes[parts[:depth]] = (group, None)
        leaf = self._subs(parts[:-1]).add_parser(parts[-1], help=help_text, description=help_text,
                                                 parents=[self.common])
        self.nodes[parts] = (leaf, None)
        return leaf


def build_parser():
    common = argparse.ArgumentParser(add_help=False)
    common.add_argument("-p", "--problem", default=argparse.SUPPRESS, help="problem id or name")
    common.add_argument("--json", action="store_true", default=argparse.SUPPRESS, help="print raw JSON")
    common.add_argument("-o", "--out", dest="out_file", default=argparse.SUPPRESS,
                        help="write the downloaded file (or rendered files) here")
    common.add_argument("--pin", default=argparse.SUPPRESS, help="pin code of the problem or contest")

    parser = argparse.ArgumentParser(prog="pg", description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("-p", "--problem", help="problem id or name")
    parser.add_argument("--json", action="store_true", help="print raw JSON instead of tables")
    parser.add_argument("-o", "--out", dest="out_file", help="write the downloaded file (or rendered files) here")
    parser.add_argument("--pin", help="pin code of the problem or contest")
    tree = Tree(parser, common)

    for path, method, spec, kind, fixed, help_text in METHODS:
        spec = list(parse_spec(spec))
        leaf = tree.add(path, f"{help_text}  [{method}]")
        for p in spec:
            if p["positional"]:
                leaf.add_argument(p["name"], metavar=("FILE" if p["file"] else kebab(p["name"]).upper()))
            elif p["bool"]:
                leaf.add_argument("--" + kebab(p["name"]), dest=p["name"], action=argparse.BooleanOptionalAction,
                                  default=p["default"])
            else:
                leaf.add_argument("--" + kebab(p["name"]), dest=p["name"], default=p["default"],
                                  help="default: base name of FILE" if p["basename_of"] else
                                  (f"default: {p['default']}" if p["default"] else "VALUE or @file"))
        leaf.set_defaults(func=run_method, _method=method, _spec=spec, _kind=kind, _fixed=fixed)

    c = tree.add("api", "call any API method: pg api problem.info, pg api problem.saveFile type=aux name=x file=@x")
    c.add_argument("method")
    c.add_argument("params", nargs="*", metavar="key=value", help="value @path reads a file; repeat a key for lists")
    c.add_argument("--raw", action="store_true", help="the method returns a file, not JSON")
    c.set_defaults(func=cmd_api)

    c = tree.add("pull", "mirror a problem into a folder (default folder: the problem name)")
    c.add_argument("problem_ref", nargs="?", metavar="PROBLEM", help="id or name (default: -p / problem.json)")
    c.add_argument("dir", nargs="?")
    c.add_argument("--testset", action="append", help="testset to mirror (repeatable, default: tests)")
    c.set_defaults(func=cmd_pull)

    for name, func, text in (("push", cmd_push, "upload what differs between a pulled folder and Polygon"),
                             ("diff", cmd_diff, "show what push would change")):
        c = tree.add(name, text)
        c.add_argument("dir", nargs="?", help="folder with problem.json (default: current folder)")
        c.add_argument("--prune", action="store_true", help="also delete what exists on Polygon but not locally")
        if name == "push":
            c.add_argument("-n", "--dry-run", action="store_true", help="only list the changes")
            c.add_argument("--force", action="store_true", help="upload everything without comparing")
            c.add_argument("--create", metavar="NAME", help="create a new problem and push into it")
            c.add_argument("--commit", action="store_true", help="commit after a fully successful push")
            c.add_argument("-m", "--message", help="commit message")
            c.add_argument("--notify", action="store_true", help="send the commit email notification")
        c.set_defaults(func=func)

    c = tree.add("build", "build a package, wait for it and print the verdict table")
    c.add_argument("--full", action=argparse.BooleanOptionalAction, default=True, help="also linux/windows packages")
    c.add_argument("--verify", action=argparse.BooleanOptionalAction, default=True, help="run all solutions")
    c.add_argument("--wait", action=argparse.BooleanOptionalAction, default=True)
    c.set_defaults(func=cmd_build)
    tree.add("wait", "wait for the newest package build to finish").set_defaults(func=cmd_wait)
    c = tree.add("verdicts", "per-group verdict and slowest time of every solution in an invocation")
    c.add_argument("invocation", nargs="?", help="invocation id (default: the newest)")
    c.set_defaults(func=cmd_verdicts)
    tree.add("render", "render statements and tutorials to HTML and PDF files (-o FOLDER)").set_defaults(
        func=cmd_render)
    c = tree.add("download", "download the newest READY package")
    c.add_argument("--type", default="linux", choices=["standard", "linux", "windows"])
    c.set_defaults(func=cmd_download)
    c = tree.add("stress watch", "wait for stresses to finish and print countertests")
    c.add_argument("indices", nargs="?", help="comma separated stress indices (default: all)")
    c.add_argument("--run", action="store_true", help="start them first")
    c.set_defaults(func=cmd_stress_watch)
    tree.add("status", "revision, uncommitted changes and cautions").set_defaults(func=cmd_status)
    return parser


def main(argv=None):
    for stream in (sys.stdout, sys.stderr):
        try:
            stream.reconfigure(encoding="utf-8", errors="replace")
        except (AttributeError, ValueError):
            pass
    args = build_parser().parse_args(argv)
    try:
        args.func(args)
    except PolygonError as e:
        print(f"error: {e}", file=sys.stderr)
        if e.result is not None:
            print(json.dumps(e.result, indent=2, ensure_ascii=False), file=sys.stderr)
        sys.exit(1)
    except BrokenPipeError:
        pass


if __name__ == "__main__":
    main()
