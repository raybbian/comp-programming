# pg: Polygon from the command line

A client for the [Polygon API](https://codeforces.github.io/polygon-misc/API). Python 3.9+, standard
library only.

```
pg pull downhill                  # mirror a problem into ./downhill
cd downhill                       # edit solutions, statements, tests, problem.json ...
pg diff                           # what push would change
pg push --commit -m "new tests"   # upload only what differs, then commit
pg build                          # build + verify a package, print the verdict table
```

## Setup

1. Create an API key on Polygon's settings page.
2. Copy `.env.example` to `.env` in this folder and fill in `POLYGON_KEY` and `POLYGON_SECRET`
   (environment variables with the same names also work). `.env` is git-ignored.
3. Put this folder on `PATH` (`pg` for sh, `pg.cmd` for cmd/PowerShell), or run `python pg.py`.

Optional: `POLYGON_PIN` for pin-protected problems (or `--pin`), `POLYGON_URL` for another Polygon.

## Choosing the problem

Commands that act on a problem take it from, in order: `-p ID|NAME`, `POLYGON_PROBLEM`, `problem.json`
in the current folder. So inside a pulled folder you never name the problem.

## Folder sync

`pg pull [PROBLEM] [DIR]` writes:

| Path | Content |
| --- | --- |
| `problem.json` | limits, checker/validator/interactor, tags, descriptions, statement names, source types, solution tags, per-test group/points/description, groups, validator and checker tests, stresses, materials |
| `statements/<lang>/<field>.tex` | `legend`, `input`, `output`, `scoring`, `interaction`, `notes`, `tutorial` |
| `statements/resources/` | statement resource files (images, ...) |
| `files/source`, `files/resource`, `files/aux` | generators, checker, validator, testlib.h, ... |
| `solutions/` | solutions; tags live in `problem.json` |
| `tests/<testset>/script.txt` | generator script |
| `tests/<testset>/<index>.in` | manual tests; `<index>.statement.in/.out` override what the statement shows |

`pg push [DIR]` compares the folder with your working copy on Polygon and uploads only the differences.

- Files on disk decide what exists: drop a file into `solutions/` and it is uploaded (tag `OK` unless
  `problem.json` says otherwise).
- Nothing is deleted on Polygon unless you pass `--prune`.
- `--dry-run` (or `pg diff`) lists the changes. `--commit -m MSG` commits after a fully successful push.
- `--create NAME` creates a new problem from the folder. `--force` uploads everything without comparing.
- `.pg-cache.json` remembers digests of remote files so that diff and push do not download them again.
  It is safe to delete and should not be committed.

Per-group solution tags (`groupTags` in `problem.json`, e.g. "this TL solution must pass group 1") can
only be read from Polygon through a stored invocation. pg remembers the ones it pushed or pulled; tags
changed in the web UI are not noticed until an invocation exists.

## Everyday commands

| Command | What it does |
| --- | --- |
| `pg list [--name X] [--owner X]` | problems you can access |
| `pg status` | revision, uncommitted changes, cautions, package warnings |
| `pg commit -m MSG` | commit the working copy (no email; `--no-minor-changes` sends it) |
| `pg wc update` / `pg wc discard` | update / throw away the working copy |
| `pg build [--no-verify] [--no-full] [--no-wait]` | build a package, wait, print verdicts |
| `pg verdicts [INVOCATION]` | verdict and slowest time of every solution per test group |
| `pg download [--type linux\|windows\|standard] [-o FILE]` | newest READY package |
| `pg render -o DIR` | statement and tutorial as HTML and PDF |
| `pg stress set --script-pattern "gen [1..10]" --solution-names wa.cpp` | create a stress |
| `pg stress watch --run [INDICES]` | run stresses, wait, print countertests |
| `pg stress add-test INDEX` | add the countertest to the tests |

## One command per API method

`pg --help` lists the groups, `pg <group> --help` the verbs, and every verb's help shows the API
method it calls. Flags are the API parameter names in kebab-case. A flag value `@path` is read from
that file (`@-` is stdin).

```
pg info set --time-limit 2000 --memory-limit 512
pg solution put sol.cpp --tag WA            pg solution tag sol.cpp TL
pg solution extra-tag sol.cpp OK --test-group 1
pg file put source gen.cpp                  pg checker set std::ncmp.cpp
pg statement set english --legend @legend.tex --name "My problem"
pg test set 7 --test-input @big.in --test-group 2 --test-points 10
pg test group 3 20,21,22                    pg group set 3 --dependencies 1,2
pg script set script.txt                    pg test input 12 -o 12.in
pg vtest set 1 --test-input @bad.in --test-verdict INVALID
pg access set someone WRITE                 pg issue add BUG "test 5 is invalid"
pg contest problems 12345
```

Groups: `access`, `info`, `note`, `tags`, `description`, `general-tutorial`, `wc`, `statement`,
`resource`, `checker`, `validator`, `interactor`, `vtest`, `ctest`, `file`, `solution`, `script`, `test`,
`group`, `points`, `material`, `package`, `invocation`, `issue`, `stress`, `contest`.

Anything else, including methods added to the API later:

```
pg api problem.info
pg api problem.saveFile type=aux name=notes.txt file=@notes.txt
pg api problem.viewFile type=source name=gen.cpp --raw -o gen.cpp
```

Output is a table for lists and JSON otherwise; `--json` always prints JSON.

## What the API itself cannot do

These need the web UI, because the documented API has no method for them:

- delete or rename a problem; create, edit or delete a contest; add a problem to a contest
- start an invocation on chosen solutions and tests (only package verification runs solutions, via `pg build`)
- create or delete testsets; upload tests as an archive (`pg push` uploads them one by one instead)
- set extra validators (they can only be listed); set a problem's pin code
- give access to a user group; assign reviewer/supervisor/translator roles
- read per-group solution tags directly (see above)

## Notes

- Polygon compiles sources on upload, so a solution or generator that does not compile is rejected.
- Polygon rate-limits the API (HTTP 429). pg waits and retries on its own.
- Requests that fail with a server error are retried, except the ones that are not safe to repeat
  (create, commit, build, issues, stress create/run).

## As a library

```python
from api import Polygon
pg = Polygon()
pid = pg.resolve("downhill")
info = pg.call("problem.info", problemId=pid)                         # JSON methods
source = pg.raw("problem.viewSolution", problemId=pid, name="a.cpp")  # file methods
```

`methods.py` is the table of commands, `sync.py` implements pull/diff/push.
