from os import EX_OK, chdir, environ
from pathlib import Path
from shutil import rmtree, copyfile
from subprocess import run, DEVNULL, PIPE
from unittest import TestCase, SkipTest


def setUpModule() -> None:
    if "PATH_BIN" not in environ:
        raise SkipTest("Environment variable 'PATH_BIN' is required but not set.")


class TestHelpMessages(TestCase):

    def test_help_short(self) -> None:
        process = run([environ["PATH_BIN"], "-h"], stdout=DEVNULL, stderr=DEVNULL)
        self.assertEqual(process.returncode, EX_OK)

    def test_help(self) -> None:
        process = run([environ["PATH_BIN"], "--help"], stdout=DEVNULL, stderr=DEVNULL)
        self.assertEqual(process.returncode, EX_OK)


class TestInspectFile(TestCase):

    def test_inspect_jpgs(self) -> None:
        test_cases = [
            ("tests/jpg_android.jpg", "LGE"),
            ("tests/jpg_apple.jpg", "Apple"),
        ]

        for filename, exp_make in test_cases:
            with self.subTest(filename=filename, exp_make=exp_make):
                process = run(
                    [environ["PATH_BIN"], filename], stdout=PIPE, stderr=DEVNULL
                )

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
                process = run(
                    [environ["PATH_BIN"], filename], stdout=DEVNULL, stderr=PIPE
                )

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
        process = run([environ["PATH_BIN"]], stdout=DEVNULL, stderr=DEVNULL)
        self.assertEqual(process.returncode, EX_OK)
        self.assertTrue(Path("jpg_android.jpg").exists())
        self.assertTrue(Path("jpg_apple.jpg").exists())
