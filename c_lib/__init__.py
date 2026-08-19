import ctypes
from pathlib import Path

_lib = ctypes.CDLL(str(Path(__file__).with_name("c_lib.dll")))

hello_from_c = _lib.hello_from_c
hello_from_c.restype = None
