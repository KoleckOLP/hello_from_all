import ctypes
import os
from pathlib import Path

_here = Path(__file__).resolve().parent

os.add_dll_directory(r"C:\msys64\mingw64\bin")

_lib = ctypes.CDLL(
    str(_here / "rb_lib.dll")
)

_lib.polyglot_rb_init.restype = ctypes.c_int

_lib.polyglot_rb_load.argtypes = [ctypes.c_char_p]
_lib.polyglot_rb_load.restype = ctypes.c_int

_lib.polyglot_rb_call_hello.restype = ctypes.c_int

_lib.polyglot_rb_destroy.restype = None


result = _lib.polyglot_rb_init()

if result != 0:
    raise RuntimeError(
        f"Failed to initialize Ruby: {result}"
    )


_rb_file = _here / "rb_lib.rb"

result = _lib.polyglot_rb_load(
    str(_rb_file).encode()
)

if result != 0:
    _lib.polyglot_rb_destroy()
    raise RuntimeError(
        f"Failed to load Ruby file: {result}"
    )


def hello_from_rb():
    result = _lib.polyglot_rb_call_hello()

    if result != 0:
        raise RuntimeError(
            f"Ruby function failed: {result}"
        )
