from os import EX_OK, chdir
from pathlib import Path
from shutil import rmtree, copyfile
from subprocess import run, PIPE, CompletedProcess
from unittest import TestCase
from .utils import get_path_to_ios2droid_binary


def run_subprocess(args: list[str]) -> CompletedProcess:
    command = [get_path_to_ios2droid_binary()] + args
    return run(command, stdout=PIPE, stderr=PIPE)


class TestHelpMessages(TestCase):

    def test_help_short(self) -> None:
        process = run_subprocess(["-h"])
        self.assertEqual(process.returncode, EX_OK)

    def test_help(self) -> None:
        process = run_subprocess(["--help"])
        self.assertEqual(process.returncode, EX_OK)


class TestInspectFile(TestCase):

    def test_inspect_jpgs(self) -> None:
        test_cases = [
            ("tests/jpg_android.jpg", "LGE"),
            ("tests/jpg_apple.jpg", "Apple"),
        ]

        for filename, exp_make in test_cases:
            with self.subTest(filename=filename, exp_make=exp_make):
                process = run_subprocess([filename])

                self.assertEqual(process.returncode, EX_OK)
                make: str | None = None

                for line in process.stdout.decode().splitlines():
                    if "Camera make" in line:
                        make = line.split(": ")[-1]
                        break
                else:
                    self.fail("Could not find 'Camera make' line in stdout")

                self.assertEqual(make, exp_make)

    def test_invalid_cases(self) -> None:
        test_cases = [
            ("tests/foo.jpg", "File 'tests/foo.jpg' does not exist"),
            ("tests/jpg_empty.jpg", "File is empty"),
            (
                "tests/jpg_fake.jpg",
                "No JPEG markers found in buffer. Is this an image file?",
            ),
        ]

        for filename, stderr in test_cases:
            with self.subTest(filename=filename, stderr=stderr):
                process = run_subprocess([filename])

                self.assertNotEqual(process.returncode, EX_OK)
                self.assertEqual(process.stderr.decode().strip(), stderr)


class TestRename(TestCase):
    tmpdir = Path(".tmp")

    def setUp(self) -> None:
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
        process = run_subprocess([])
        self.assertEqual(process.returncode, EX_OK)
        self.assertTrue(Path("jpg_android.jpg").exists())
        self.assertTrue(Path("jpg_apple.jpg").exists())

    def test_rename(self) -> None:
        process = run_subprocess(["--rename"])
        self.assertEqual(process.returncode, EX_OK)

        # Android originating file should retain name
        self.assertTrue(Path("jpg_android.jpg").exists())

        # Apple originating file should be renamed
        self.assertTrue(Path("20241113_024948.jpg").exists())

    def test_do_not_rename_already_renamed_file(self) -> None:
        process = run_subprocess(["--rename"])
        self.assertEqual(process.returncode, EX_OK)
        self.assertTrue(Path("20241113_024948.jpg").exists())

        # Try a second time...
        process = run_subprocess(["--rename"])
        self.assertEqual(process.returncode, EX_OK)
        self.assertIn(
            "Filename is already in YYYYMMDD_HHMMSS format", process.stderr.decode()
        )
