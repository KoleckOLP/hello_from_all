import ctypes
from pathlib import Path

_here = Path(__file__).resolve().parent

_lib = ctypes.CDLL(
    str(_here / "rs_lib.dll")
)

_lib.hello_from_rs.restype = None


def hello_from_rs():
    _lib.hello_from_rs()
