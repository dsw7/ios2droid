from os import chdir
from pathlib import Path
from shutil import rmtree, copyfile
from unittest import TestCase
from .utils import run_subprocess

Temp = Path(".tmp")


class TestRename(TestCase):
    def setUp(self) -> None:
        if Temp.exists():
            rmtree(Temp)

        Temp.mkdir()
        copyfile("tests/jpg_android.jpg", Temp / "jpg_android.jpg")
        copyfile("tests/jpg_apple.jpg", Temp / "jpg_apple.jpg")
        chdir(Temp)

    def tearDown(self) -> None:
        chdir("..")
        rmtree(Temp)

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


class TestRenameInvalidFile(TestCase):
    def setUp(self) -> None:
        if Temp.exists():
            rmtree(Temp)

        Temp.mkdir()
        copyfile("tests/jpg_fake.jpg", Temp / "jpg_fake.jpg")
        copyfile("tests/jpg_no_exif_header.jpg", Temp / "jpg_no_exif_header.jpg")
        chdir(Temp)

    def tearDown(self) -> None:
        chdir("..")
        rmtree(Temp)

    def test_rename(self) -> None:
        process = run_subprocess(["-r"])
        self.assertEqual(process.returncode, 0)
        stderr = process.stderr.decode()
        self.assertIn("No JPEG markers found in buffer. Is this an image file?", stderr)
        self.assertIn("Could not find EXIF header in file", stderr)
        self.assertTrue(Path("jpg_fake.jpg").exists())
        self.assertTrue(Path("jpg_no_exif_header.jpg").exists())


class TestRenameNoFiles(TestCase):
    def setUp(self) -> None:
        if Temp.exists():
            rmtree(Temp)

        Temp.mkdir()
        chdir(Temp)

    def tearDown(self) -> None:
        chdir("..")
        rmtree(Temp)

    def test_rename(self) -> None:
        process = run_subprocess(["-r"])
        self.assertEqual(process.returncode, 0)
        self.assertIn("Directory is empty!", process.stderr.decode())


class TestRenameDirectory(TestCase):
    def setUp(self) -> None:
        if Temp.exists():
            rmtree(Temp)

        Temp.mkdir()
        (Temp / "foo").mkdir()
        chdir(Temp)

    def tearDown(self) -> None:
        chdir("..")
        rmtree(Temp)

    def test_rename(self) -> None:
        process = run_subprocess(["-r"])
        self.assertEqual(process.returncode, 0)
        self.assertIn("Is a directory", process.stderr.decode())


class TestNoOverwrite(TestCase):
    def setUp(self) -> None:
        if Temp.exists():
            rmtree(Temp)

        Temp.mkdir()
        copyfile("tests/jpg_apple.jpg", Temp / "jpg_apple1.jpg")
        copyfile("tests/jpg_apple.jpg", Temp / "jpg_apple2.jpg")
        chdir(Temp)

    def tearDown(self) -> None:
        chdir("..")
        rmtree(Temp)

    def test_do_not_overwrite_file(self) -> None:
        process = run_subprocess(["--rename"])
        self.assertEqual(process.returncode, 0)

        self.assertTrue(Path("20241113_024948.jpg").exists())
        self.assertTrue(Path("jpg_apple2.jpg").exists())
        self.assertIn(
            "'20241113_024948.jpg' already exists. Therefore cannot rename 'jpg_apple2.jpg' to '20241113_024948.jpg'",
            process.stderr.decode(),
        )
