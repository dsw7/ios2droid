from subprocess import run, PIPE, CompletedProcess
from functools import cache
from os import environ


@cache
def get_path_to_ios2droid_binary() -> str:
    return environ["PATH_BIN"]


def run_subprocess(args: list[str]) -> CompletedProcess:
    command = [get_path_to_ios2droid_binary()] + args
    return run(command, stdout=PIPE, stderr=PIPE)
