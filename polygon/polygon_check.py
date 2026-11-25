#!/usr/bin/env python3
"""Read back what Polygon stored: python polygon_check.py <problemId>"""
import json
import sys

from polygon_upload import call, TESTSET

pid = int(sys.argv[1])
P = dict(problemId=pid)
print("info:", call("problem.info", **P))
print("checker:", call("problem.checker", **P), "| validator:", call("problem.validator", **P))
print("solutions:", [(s["name"], s["tag"]) for s in call("problem.solutions", **P)])
tests = call("problem.tests", testset=TESTSET, noInputs=True, **P)
print("tests:", len(tests))
groups = {}
for t in tests:
    g = groups.setdefault(t.get("group"), [0, 0.0, 0])
    g[0] += 1
    g[1] += t.get("points") or 0
    g[2] += 1 if t.get("useInStatements") else 0
for g in sorted(groups, key=str):
    print("  group", g, "tests", groups[g][0], "points", groups[g][1], "in statements", groups[g][2])
for g in call("problem.viewTestGroup", testset=TESTSET, **P):
    print("  ", g)
r = call("problem.renderStatements", **P)
for kind in ("statements", "tutorials"):
    for s in r[kind]:
        for fmt in ("html", "pdf"):
            x = s[fmt]
            print(kind, s["language"], fmt, x["status"], x.get("sizeBytes"), (x.get("message") or "")[:1500])
c = call("problem.cautions", **P)
for k in ("common", "statement", "structure", "issues"):
    for item in c.get(k, []):
        print("caution", k, json.dumps(item)[:400])
