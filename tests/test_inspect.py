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

    def test_invalid_cases(self) -> None:
        test_cases = [
            ("tests/foo.jpg", "File 'tests/foo.jpg' does not exist"),
            ("tests/jpg_empty.jpg", "File is empty"),
            (
                "tests/jpg_fake.jpg",
                "No JPEG markers found in buffer. Is this an image file?",
            ),
            ("/tmp", "File '/tmp' is not a regular file"),
        ]

        for filename, stderr in test_cases:
            with self.subTest(filename=filename, stderr=stderr):
                process = run_subprocess([filename])

                self.assertNotEqual(process.returncode, 0)
                self.assertEqual(process.stderr.decode().strip(), stderr)
