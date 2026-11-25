#!/usr/bin/env python3
"""Per-group verdicts of the latest package verification: python polygon_verdicts.py <problemId>"""
import sys

from polygon_upload import call, TESTSET

pid = int(sys.argv[1])
P = dict(problemId=pid)
group_of = {t["index"]: t.get("group") for t in call("problem.tests", testset=TESTSET, noInputs=True, **P)}
groups = sorted(set(group_of.values()), key=str)
invs = [i for i in call("problem.invocations", **P) if i.get("packageId")]
if not invs:
    sys.exit("no package verification invocations")
inv = invs[0]
data = call("problem.invocation", invocationId=inv["id"], **P)
print("invocation", inv["id"], inv["status"], "package", inv["packageId"],
      "violateTag" if inv.get("violateTag") else "tags respected", "runs", len(data["runs"]))
table = {}
for r in data["runs"]:
    cell = table.setdefault((r.get("solution"), group_of.get(r["test"])), [set(), 0, 0])
    cell[0].add(r["verdict"])
    cell[1] = max(cell[1], r.get("timeMillis") or 0)
    cell[2] += 1
print(f"{'solution':24}" + "".join(f"{'g' + str(g):>16}" for g in groups))
for sol in sorted({k[0] for k in table}, key=str):
    row = f"{str(sol):24}"
    for g in groups:
        c = table.get((sol, g))
        row += f"{'-':>16}" if not c else f"{'/'.join(sorted(c[0])) + ' ' + str(c[1]) + 'ms':>16}"
    print(row)
