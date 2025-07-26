from unittest import TestCase
from .utils import run_subprocess


class TestInspectFile(TestCase):

    def test_inspect_jpgs(self) -> None:
        test_cases = [
            ("tests/jpg_android.jpg", "LGE"),
            ("tests/jpg_apple.jpg", "Apple"),
        ]

        for filename, exp_make in test_cases:
            with self.subTest(filename=filename, exp_make=exp_make):
                process = run_subprocess([filename])

                self.assertEqual(process.returncode, 0)
                make: str | None = None

                for line in process.stdout.decode().splitlines():
                    if "Camera make" in line:
                        make = line.split(": ")[-1]
                        break
                else:
                    self.fail("Could not find 'Camera make' line in stdout")

                self.assertEqual(make, exp_make)

    def test_missing_jpg(self) -> None:
        filename = "tests/foo.jpg"
        stderr = "File 'tests/foo.jpg' does not exist"
        process = run_subprocess([filename])

        self.assertNotEqual(process.returncode, 0)
        self.assertEqual(process.stderr.decode().strip(), stderr)

    def test_empty_jpg(self) -> None:
        filename = "tests/jpg_empty.jpg"
        stderr = "File is empty"
        process = run_subprocess([filename])

        self.assertNotEqual(process.returncode, 0)
        self.assertEqual(process.stderr.decode().strip(), stderr)

    def test_fake_jpg(self) -> None:
        filename = "tests/jpg_fake.jpg"
        stderr = "No JPEG markers found in buffer. Is this an image file?"
        process = run_subprocess([filename])

        self.assertNotEqual(process.returncode, 0)
        self.assertEqual(process.stderr.decode().strip(), stderr)

    def test_no_exif_header_jpg(self) -> None:
        filename = "tests/jpg_no_exif_header.jpg"
        stderr = "Could not find EXIF header in file"
        process = run_subprocess([filename])

        self.assertNotEqual(process.returncode, 0)
        self.assertEqual(process.stderr.decode().strip(), stderr)

    def test_inspect_dir(self) -> None:
        filename = "/tmp"
        stderr = "File '/tmp' is not a regular file"
        process = run_subprocess([filename])

        self.assertNotEqual(process.returncode, 0)
        self.assertEqual(process.stderr.decode().strip(), stderr)
