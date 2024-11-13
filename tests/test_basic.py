from os import EX_OK
from subprocess import run, DEVNULL, PIPE
from unittest import TestCase


class TestBasic(TestCase):
    command = "build/ios2droid"

    def test_help_short(self) -> None:
        process = run([self.command, "-h"], stdout=DEVNULL, stderr=DEVNULL)
        self.assertEqual(process.returncode, EX_OK)

    def test_help(self) -> None:
        process = run([self.command, "--help"], stdout=DEVNULL, stderr=DEVNULL)
        self.assertEqual(process.returncode, EX_OK)

    def test_inspect_android(self) -> None:
        process = run([self.command, "tests/jpg_android.jpg"], stdout=PIPE, stderr=PIPE)
        self.assertEqual(process.returncode, EX_OK)

        make: str | None = None
        for line in process.stdout.decode().splitlines():
            if "Camera make" in line:
                make = line.split(": ")[-1]
                break
        else:
            self.fail("Could not find 'Camera make' line in stdout")

        self.assertEqual(make, "LGE")
