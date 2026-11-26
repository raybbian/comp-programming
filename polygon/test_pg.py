"""Offline tests: python -m unittest test_pg   (no network, no credentials)."""
import copy
import hashlib
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import api  # noqa: E402
import pg  # noqa: E402
import sync  # noqa: E402
from methods import METHODS, parse_spec  # noqa: E402


def sample_state():
    st = sync.empty_state()
    st["polygon"] = {"id": 1, "name": "sample", "owner": "me", "revision": 3}
    st["info"] = {"timeLimit": 1000, "memoryLimit": 256, "inputFile": "stdin", "outputFile": "stdout",
                  "interactive": False}
    st["checker"], st["validator"] = "std::ncmp.cpp", "validator.cpp"
    st["tags"] = ["graphs"]
    st["pointsEnabled"] = True
    st["statements"]["english"] = {"meta": {"name": "Sample", "encoding": "UTF-8"},
                                   "fields": {"legend": "Find $x \\le 5$.", "input": "One integer.", "scoring": ""}}
    st["resources"]["pic.png"] = b"\x89PNG\r\n\x1a\n\x00\xff"
    st["files"]["source"]["validator.cpp"] = {"meta": {"sourceType": "cpp.g++17"}, "data": b"int main() {}\n"}
    st["solutions"]["ok.cpp"] = {"tag": "MA", "sourceType": "cpp.g++17", "groupTags": {}, "testsetTags": {},
                                 "data": b"int main() {}\n"}
    st["solutions"]["slow.cpp"] = {"tag": "TL", "sourceType": "cpp.g++17", "groupTags": {"1": "OK"},
                                   "testsetTags": {}, "data": b"int main() { for (;;); }\n"}
    st["testsets"]["tests"] = {
        "groupsEnabled": True,
        "groups": {"1": {"pointsPolicy": "COMPLETE_GROUP", "feedbackPolicy": "POINTS", "dependencies": []}},
        "script": "gen 1 > 2\ngen 2 > 3",
        "tests": {1: {"manual": True, "input": b"1\n", "group": "1", "points": 0, "useInStatements": True},
                  2: {"manual": False, "group": "1", "points": 0},
                  3: {"manual": False, "group": "1", "points": 10.0, "description": "big"}},
    }
    st["validatorTests"][1] = {"input": "0\n", "expectedVerdict": "INVALID"}
    st["stresses"][1] = {"scriptPattern": "gen [1..9]", "solutionNames": ["ok.cpp", "slow.cpp"], "timeLimit": 1000,
                         "memoryLimit": 256, "totalTimeLimit": 5000, "ignoreValidators": False, "description": ""}
    return st


def descriptions(local, remote, prune=False):
    return [d for d, _ in sync.plan(None, 1, local, remote, prune=prune)]


class SpecTests(unittest.TestCase):
    def test_every_command_parses(self):
        parser = pg.build_parser()          # raises on clashing flags or bad option names
        for path, method, spec, kind, fixed, _ in METHODS:
            argv = path.split()
            for p in parse_spec(spec):
                if p["positional"]:
                    argv.append(os.devnull if p["file"] else "x")
            args = parser.parse_args(argv)
            self.assertEqual(args._method, method, path)
            self.assertIn(kind, ("json", "raw", "none"))

    def test_api_parameters_do_not_shadow_cli_flags(self):
        reserved = {"problem", "json", "out_file", "pin", "func", "_method", "_spec", "_kind", "_fixed"}
        for path, _, spec, _, _, _ in METHODS:
            self.assertFalse({p["name"] for p in parse_spec(spec)} & reserved, path)

    def test_command_paths_are_unique(self):
        paths = [m[0] for m in METHODS]
        self.assertEqual(len(paths), len(set(paths)))

    def test_flags_and_defaults(self):
        parser = pg.build_parser()
        args = parser.parse_args(["commit", "-m", "msg"])
        self.assertEqual((args.message, args.minorChanges), ("msg", True))
        args = parser.parse_args(["statement", "set", "english", "--output", "text", "-o", "file"])
        self.assertEqual((args.output, args.out_file), ("text", "file"))
        with self.assertRaises(SystemExit):   # no prefix guessing
            parser.parse_args(["test", "set", "1", "--test-inp", "x"])


class ApiTests(unittest.TestCase):
    def test_signature_matches_the_documented_formula(self):
        pairs = [(b"time", b"100"), (b"apiKey", b"key"), (b"problemId", b"7")]
        expected = hashlib.sha512(b"abcdef/problem.info?apiKey=key&problemId=7&time=100#secret").hexdigest()
        self.assertEqual(api.sign("problem.info", pairs, "secret", b"abcdef"), b"abcdef" + expected.encode())

    def test_repeated_parameters_sort_by_value(self):
        pairs = [(b"testIndex", b"2"), (b"testIndex", b"10"), (b"apiKey", b"k")]
        expected = hashlib.sha512(b"abcdef/m?apiKey=k&testIndex=10&testIndex=2#s").hexdigest()
        self.assertEqual(api.sign("m", pairs, "s", b"abcdef")[6:], expected.encode())

    def test_only_a_failure_envelope_is_an_error(self):
        self.assertIsNotNone(api._api_failure(b'{"status":"FAILED","comment":"no"}'))
        self.assertIsNone(api._api_failure(b'{"status":"FAILED","comment":"no","mine":1}'))   # a user's JSON file
        self.assertIsNone(api._api_failure(b'{"status":"OK","result":1}'))
        self.assertIsNone(api._api_failure(b"int main() {}"))


class SyncTests(unittest.TestCase):
    def roundtrip(self, st):
        folder = tempfile.mkdtemp()
        sync.write(st, folder)
        return folder, sync.read(folder)

    def test_write_then_read_needs_no_push(self):
        st = sample_state()
        _, local = self.roundtrip(st)
        self.assertEqual(descriptions(local, st), [])
        self.assertEqual(descriptions(local, st, prune=True), [])

    def test_crlf_and_cached_digests_compare_equal(self):
        st = sample_state()
        _, local = self.roundtrip(st)
        remote = copy.deepcopy(st)
        remote["solutions"]["ok.cpp"]["data"] = b"int main() {}\r\n"
        remote["files"]["source"]["validator.cpp"]["data"] = sync.Cached(sync.digest(b"int main() {}\n"))
        self.assertEqual(descriptions(local, remote), [])

    def test_binary_resource_is_not_normalised(self):
        st = sample_state()
        folder, local = self.roundtrip(st)
        self.assertEqual(local["resources"]["pic.png"], st["resources"]["pic.png"])

    def test_changes_are_found(self):
        st = sample_state()
        folder, _ = self.roundtrip(st)
        with open(os.path.join(folder, "solutions", "new.cpp"), "w") as f:
            f.write("int main() {}\n")
        with open(os.path.join(folder, "solutions", "ok.cpp"), "a") as f:
            f.write("// edit\n")
        with open(os.path.join(folder, "tests", "tests", "4.in"), "w") as f:
            f.write("4\n")
        manifest = sync.load_manifest(folder)
        manifest["info"]["timeLimit"] = 2000
        manifest["solutions"]["slow.cpp"]["groupTags"] = {}
        manifest["testsets"]["tests"]["tests"]["2"]["points"] = 5
        sync.save_manifest(folder, manifest)
        got = descriptions(sync.read(folder), st)
        self.assertEqual(got, [
            "info: timeLimit=2000",
            "add solution new.cpp [OK]",
            "update solution ok.cpp",
            "solution slow.cpp: drop extra tag on testGroup 1",
            "tests: manual test 4",
            "tests: test 2: testPoints=5",
        ])

    def test_nothing_is_deleted_without_prune(self):
        st = sample_state()
        folder, _ = self.roundtrip(st)
        os.remove(os.path.join(folder, "solutions", "slow.cpp"))
        os.remove(os.path.join(folder, "statements", "english", "input.tex"))
        os.remove(os.path.join(folder, "tests", "tests", "1.in"))
        local = sync.read(folder)
        self.assertEqual(descriptions(local, st), [])
        self.assertEqual(descriptions(local, st, prune=True), [
            "delete solution slow.cpp", "statement english: input", "tests: delete manual tests [1]"])

    def test_zero_points_survive_a_pull(self):
        st = sample_state()
        folder, local = self.roundtrip(st)
        self.assertEqual(local["testsets"]["tests"]["tests"][2]["points"], 0)

    def test_local_only_files_are_reported(self):
        st = sample_state()
        remote = copy.deepcopy(st)
        del remote["solutions"]["slow.cpp"]
        remote["testsets"]["tests"]["tests"][1] = {"manual": False}
        self.assertEqual(sync.local_only(st, remote), ["solutions/slow.cpp", "tests/tests/1.in"])


if __name__ == "__main__":
    unittest.main()
