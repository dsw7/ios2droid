from os import chdir
from pathlib import Path
from shutil import rmtree, copyfile
from unittest import TestCase
from .utils import run_subprocess


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
        self.assertEqual(process.returncode, 0)
        self.assertTrue(Path("jpg_android.jpg").exists())
        self.assertTrue(Path("jpg_apple.jpg").exists())

    def test_rename(self) -> None:
        process = run_subprocess(["--rename"])
        self.assertEqual(process.returncode, 0)

        # Android originating file should retain name
        self.assertTrue(Path("jpg_android.jpg").exists())

        # Apple originating file should be renamed
        self.assertTrue(Path("20241113_024948.jpg").exists())

    def test_do_not_rename_already_renamed_file(self) -> None:
        process = run_subprocess(["--rename"])
        self.assertEqual(process.returncode, 0)
        self.assertTrue(Path("20241113_024948.jpg").exists())

        # Try a second time...
        process = run_subprocess(["--rename"])
        self.assertEqual(process.returncode, 0)
        self.assertIn(
            "Filename is already in YYYYMMDD_HHMMSS format", process.stderr.decode()
        )
