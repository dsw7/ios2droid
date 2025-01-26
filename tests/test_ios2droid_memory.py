from os import chdir
from pathlib import Path
from shutil import rmtree, copyfile
from subprocess import run, DEVNULL, PIPE
from unittest import TestCase
from .utils import get_path_to_ios2droid_binary


def run_valgrind(args: list[str]) -> None:
    root = [
        "valgrind",
        "--error-exitcode=2",
        "--leak-check=full",
        get_path_to_ios2droid_binary(),
    ]

    process = run(root + args, stdout=DEVNULL, stderr=PIPE)
    stderr = process.stderr.decode()

    if process.returncode == 2:
        assert False, f"Found a memory leak\n\n{stderr}"


class TestHelpMessagesMemory(TestCase):

    def test_help_short(self) -> None:
        run_valgrind(["-h"])

    def test_help(self) -> None:
        run_valgrind(["--help"])


class TestInspectFileMemory(TestCase):

    def test_inspect_jpgs(self) -> None:
        test_cases = [
            ("tests/jpg_android.jpg"),
            ("tests/jpg_apple.jpg"),
            ("tests/foo.jpg"),
            ("tests/jpg_empty.jpg"),
            ("tests/jpg_fake.jpg"),
        ]

        for filename in test_cases:
            with self.subTest(filename=filename):
                run_valgrind([filename])


class TestRenameMemory(TestCase):

    def setUp(self) -> None:
        self.tmpdir = Path(".tmp")

        if self.tmpdir.exists():
            rmtree(self.tmpdir)

        self.tmpdir.mkdir()
        copyfile("tests/jpg_android.jpg", self.tmpdir / "jpg_android.jpg")
        copyfile("tests/jpg_apple.jpg", self.tmpdir / "jpg_apple.jpg")
        chdir(self.tmpdir)

    def tearDown(self) -> None:
        chdir("..")
        rmtree(self.tmpdir)

    def test_dry_run(self) -> None:
        run_valgrind([])

    def test_rename(self) -> None:
        run_valgrind(["--rename"])
