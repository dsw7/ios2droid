from functools import cache
from os import environ


@cache
def get_path_to_ios2droid_binary() -> str:
    return environ["PATH_BIN"]
