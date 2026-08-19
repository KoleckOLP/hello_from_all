import ctypes
from pathlib import Path

_lib = ctypes.CDLL(str(Path(__file__).with_name("cpp_lib.dll")))

hello_from_cpp = _lib.hello_from_cpp
hello_from_cpp.restype = None
