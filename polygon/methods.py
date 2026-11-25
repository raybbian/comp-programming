"""One command per Polygon API method.

Each entry: (command path, API method, parameter spec, kind, fixed parameters, help).

Parameter spec tokens (API parameter names, shown on the command line in kebab-case):
    name        required positional
    name@       required positional: path of a file whose content is sent ("-" = stdin)
    name?       optional flag --name VALUE   (VALUE of the form @path is read from that file)
    name?=x     optional flag with default x
    name?b      optional boolean flag --name / --no-name
    name?b=true boolean flag with a default
    name?^file  optional flag that defaults to the base name of the positional `file`

kind: "json" (print the result), "raw" (the method returns a file), "none" (no result).
Commands whose method starts with "problem." take the problem from -p / problem.json.
"""

TS = "testset?=tests"

METHODS = [
    # problems
    ("list", "problems.list", "name? owner? id? showDeleted?b", "json", {}, "problems you can access"),
    ("create", "problem.create", "name", "json", {}, "create an empty problem"),
    ("contest problems", "contest.problems", "contestId", "json", {}, "problems of a contest"),

    # access, note, general info
    ("access ls", "problem.accesses", "", "json", {}, "direct access entries"),
    ("access set", "problem.setAccess", "login accessType", "none", {}, "READ, WRITE or NONE for a user"),
    ("info show", "problem.info", "", "json", {}, "limits, input/output files, flags"),
    ("info set", "problem.updateInfo",
     "timeLimit? memoryLimit? inputFile? outputFile? interactive?b wellFormed?b "
     "skipDuplicatedTestsValidation?b runTwice?b interactiveSecondRun?b useInteractionLogInExampleTests?b",
     "none", {}, "change limits (ms, MB), files, flags"),
    ("note show", "problem.note", "", "json", {}, "problem note"),
    ("note set", "problem.saveNote", "note", "none", {}, "set the problem note (empty clears)"),
    ("tags show", "problem.viewTags", "", "json", {}, "problem tags"),
    ("tags set", "problem.saveTags", "tags", "none", {}, "replace tags (comma separated)"),
    ("description show", "problem.viewGeneralDescription", "", "json", {}, "general description"),
    ("description set", "problem.saveGeneralDescription", "description", "none", {}, "set general description"),
    ("general-tutorial show", "problem.viewGeneralTutorial", "", "json", {}, "general tutorial"),
    ("general-tutorial set", "problem.saveGeneralTutorial", "tutorial", "none", {}, "set general tutorial"),

    # working copy
    ("wc update", "problem.updateWorkingCopy", "", "none", {}, "update the working copy"),
    ("wc discard", "problem.discardWorkingCopy", "", "none", {}, "discard uncommitted changes"),
    ("commit", "problem.commitChanges", "message? minorChanges?b=true", "json", {},
     "commit the working copy (--no-minor-changes sends the email notification)"),

    # statements
    ("statement ls", "problem.statements", "", "json", {}, "statements by language"),
    ("statement set", "problem.saveStatement",
     "lang name? legend? input? output? scoring? interaction? notes? tutorial? encoding? "
     "showInReview?b showCautionsAndGrammaticalFixes?b", "none", {},
     "create or edit a statement, e.g. --legend @legend.tex"),
    ("statement rm", "problem.deleteStatement", "lang", "none", {}, "delete a statement language"),
    ("statement render-json", "problem.renderStatements", "includeContent?b", "json", {},
     "render HTML/PDF and print the raw result (see also: render)"),
    ("resource ls", "problem.statementResources", "", "json", {}, "statement resource files"),
    ("resource get", "problem.viewStatementResource", "name", "raw", {}, "download a statement resource"),
    ("resource put", "problem.saveStatementResource", "file@ name?^file checkExisting?b", "none", {},
     "upload a statement resource (image, ...)"),
    ("resource rm", "problem.deleteStatementResource", "name", "none", {}, "delete a statement resource"),

    # checker, validator, interactor
    ("checker show", "problem.checker", "", "json", {}, "current checker"),
    ("checker set", "problem.setChecker", "checker", "none", {}, "set checker (source file or std::ncmp.cpp ...)"),
    ("validator show", "problem.validator", "", "json", {}, "current validator"),
    ("validator set", "problem.setValidator", "validator", "none", {}, "set validator (a source file)"),
    ("validator extra", "problem.extraValidators", "", "json", {}, "extra validators"),
    ("interactor show", "problem.interactor", "", "json", {}, "current interactor"),
    ("interactor set", "problem.setInteractor", "interactor", "none", {}, "set interactor (a source file)"),
    ("vtest ls", "problem.validatorTests", "", "json", {}, "validator tests"),
    ("vtest set", "problem.saveValidatorTest",
     "testIndex testInput? testVerdict? testGroup? testset? checkExisting?b", "none", {},
     "add or edit a validator test (--test-verdict VALID|INVALID)"),
    ("vtest rm", "problem.deleteValidatorTest", "testIndex", "none", {}, "delete a validator test"),
    ("ctest ls", "problem.checkerTests", "", "json", {}, "checker tests"),
    ("ctest set", "problem.saveCheckerTest",
     "testIndex testInput? testOutput? testAnswer? testVerdict? checkExisting?b", "none", {},
     "add or edit a checker test (--test-verdict OK|WRONG_ANSWER|PRESENTATION_ERROR|CRASHED)"),
    ("ctest rm", "problem.deleteCheckerTest", "testIndex", "none", {}, "delete a checker test"),

    # files and solutions
    ("file ls", "problem.files", "", "json", {}, "resource, source and aux files"),
    ("file get", "problem.viewFile", "type name", "raw", {}, "download a file (type: resource|source|aux)"),
    ("file put", "problem.saveFile",
     "type file@ name?^file sourceType? forTypes? stages? assets? checkExisting?b", "none", {},
     "upload a file (type: resource|source|aux)"),
    ("file rm", "problem.deleteFile", "type name", "none", {}, "delete a file"),
    ("solution ls", "problem.solutions", "", "json", {}, "solutions and their tags"),
    ("solution get", "problem.viewSolution", "name", "raw", {}, "download a solution"),
    ("solution put", "problem.saveSolution", "file@ name?^file tag? sourceType? checkExisting?b", "none", {},
     "upload a solution (--tag MA|OK|RJ|TL|TO|TM|WA|PE|ML|NR|RE)"),
    ("solution tag", "problem.saveSolution", "name tag", "none", {}, "change a solution's tag"),
    ("solution extra-tag", "problem.editSolutionExtraTags", "name tag testGroup? testset?", "none",
     {"remove": False}, "expect TAG from a solution on one test group or testset"),
    ("solution rm-extra-tag", "problem.editSolutionExtraTags", "name testGroup? testset?", "none",
     {"remove": True}, "remove a group/testset extra tag"),
    ("solution rm", "problem.deleteSolution", "name", "none", {}, "delete a solution"),

    # tests
    ("script get", "problem.script", TS, "raw", {}, "generator script"),
    ("script set", "problem.saveScript", "source@ " + TS, "none", {}, "replace the generator script"),
    ("script clear", "problem.clearScript", TS, "none", {}, "clear the generator script"),
    ("test ls", "problem.tests", TS, "json", {"noInputs": True}, "tests, without their inputs"),
    ("test dump", "problem.tests", TS, "json", {"noInputs": False}, "tests with the inputs of manual tests"),
    ("test preview", "problem.previewTests", TS, "json", {}, "generated previews of tests"),
    ("test input", "problem.testInput", "testIndex " + TS, "raw", {}, "download a test input"),
    ("test answer", "problem.testAnswer", "testIndex " + TS, "raw", {}, "download a test answer"),
    ("test set", "problem.saveTest",
     "testIndex testInput? testGroup? testPoints? testDescription? testUseInStatements?b "
     "testInputForStatements? testOutputForStatements? testInteractionLogForStatements? "
     "verifyInputOutputForStatements?b checkExisting?b " + TS, "none", {},
     "add or edit a test, e.g. --test-input @01.in"),
    ("test rm", "problem.deleteTest", "testIndices " + TS, "none", {}, "delete tests (comma separated indices)"),
    ("test group", "problem.setTestGroup", "testGroup testIndices " + TS, "none", {},
     "put tests (comma separated indices) into a group"),
    ("group enable", "problem.enableGroups", TS, "none", {"enable": True}, "enable test groups"),
    ("group disable", "problem.enableGroups", TS, "none", {"enable": False}, "disable test groups"),
    ("group ls", "problem.viewTestGroup", "group? " + TS, "json", {}, "test groups"),
    ("group set", "problem.saveTestGroup", "group pointsPolicy? feedbackPolicy? dependencies? " + TS, "none", {},
     "policies (COMPLETE_GROUP|EACH_TEST, NONE|POINTS|ICPC|COMPLETE) and dependencies"),
    ("points enable", "problem.enablePoints", "", "none", {"enable": True}, "enable test points"),
    ("points disable", "problem.enablePoints", "", "none", {"enable": False}, "disable test points"),
    ("points percent", "problem.enableTreatPointsFromCheckerAsPercent", "", "none", {"enable": True},
     "treat checker points as percents"),
    ("points no-percent", "problem.enableTreatPointsFromCheckerAsPercent", "", "none", {"enable": False},
     "stop treating checker points as percents"),

    # materials, packages, invocations, cautions
    ("material ls", "problem.materials", "", "json", {}, "publishable materials"),
    ("material set", "problem.setMaterial", "name publishStrategy items originalName?", "none", {},
     "create/replace/rename a material (items: JSON array, or @file)"),
    ("material rm", "problem.setMaterial", "name", "none", {"remove": True}, "remove a material"),
    ("package ls", "problem.packages", "", "json", {}, "built packages"),
    ("package get", "problem.package", "packageId type?", "raw", {},
     "download a package zip (--type standard|linux|windows)"),
    ("package build", "problem.buildPackage", "full?b=true verify?b=true", "none", {},
     "start a package build (see also: build --wait)"),
    ("invocation ls", "problem.invocations", "packageId?", "json", {}, "invocations of the working copy"),
    ("invocation show", "problem.invocation", "invocationId", "json", {}, "every run of an invocation"),
    ("cautions", "problem.cautions", "", "json", {}, "warnings and package-readiness issues"),

    # issues
    ("issue ls", "problem.issues", "", "json", {}, "issues with comments"),
    ("issue add", "problem.addIssue", "type content assignee?", "json", {},
     "open an issue (type: DISCUSSION|ENHANCEMENT|BUG)"),
    ("issue update", "problem.updateIssue", "issueId comment? status? type? assignee?", "json", {},
     "comment on, close (--status CLOSED), reopen, retype or reassign an issue"),

    # stresses
    ("stress ls", "problem.stresses", "stressIndex?", "json", {}, "stresses and their latest runs"),
    ("stress set", "problem.saveStress",
     "stressIndex? scriptPattern? solutionNames? timeLimit? memoryLimit? totalTimeLimit? testset? group? "
     "ignoreValidators?b description?", "json", {},
     "create (no --stress-index) or update a stress"),
    ("stress rm", "problem.deleteStress", "stressIndex", "none", {}, "delete a stress"),
    ("stress run", "problem.runStresses", "stressIndices?", "none", {}, "start stresses (default: all)"),
    ("stress cancel", "problem.cancelStresses", "stressIndices?", "none", {}, "cancel running stresses"),
    ("stress add-test", "problem.addStressCountertest", "stressIndex " + TS, "json", {},
     "add the countertest found by a stress to the tests"),
]


def parse_spec(spec):
    """Yield dicts: name, positional, file, bool, default, basename_of."""
    for token in spec.split():
        p = dict(name=token, positional=True, file=False, bool=False, default=None, basename_of=None)
        if "?" in token:
            name, rest = token.split("?", 1)
            p.update(name=name, positional=False)
            if rest.startswith("^"):
                p["basename_of"] = rest[1:]
            else:
                if rest.startswith("b"):
                    p["bool"] = True
                    rest = rest[1:]
                if rest.startswith("="):
                    p["default"] = (rest[1:] == "true") if p["bool"] else rest[1:]
        elif token.endswith("@"):
            p.update(name=token[:-1], file=True)
        yield p
