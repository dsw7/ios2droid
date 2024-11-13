from os import EX_OK
from subprocess import run, DEVNULL
from unittest import TestCase


class TestBasic(TestCase):
    command = "build/ios2droid"

    def test_help_short(self):
        process = run([self.command, "-h"], stdout=DEVNULL, stderr=DEVNULL)
        self.assertEqual(process.returncode, EX_OK)

    def test_help(self):
        process = run([self.command, "--help"], stdout=DEVNULL, stderr=DEVNULL)
        self.assertEqual(process.returncode, EX_OK)
