from unittest import TestCase
from .utils import run_subprocess


class TestHelpMessages(TestCase):

    def test_help_short(self) -> None:
        process = run_subprocess(["-h"])
        self.assertEqual(process.returncode, 0)

    def test_help(self) -> None:
        process = run_subprocess(["--help"])
        self.assertEqual(process.returncode, 0)
