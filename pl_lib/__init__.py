import ctypes
import os
from pathlib import Path

_here = Path(__file__).resolve().parent

# Make MinGW64 Perl runtime available to Windows DLL loading
os.add_dll_directory(r"C:\msys64\mingw64\bin")

_lib = ctypes.CDLL(
    str(_here / "pl_lib.dll")
)

_lib.pl_init.restype = ctypes.c_int

_lib.pl_load.argtypes = [ctypes.c_char_p]
_lib.pl_load.restype = ctypes.c_int

_lib.pl_call_hello.restype = ctypes.c_int

_lib.pl_destroy.restype = None

result = _lib.pl_init()

if result != 0:
    raise RuntimeError(f"Failed to initialize Perl: {result}")

_pl_file = _here / "pl_lib.pl"

result = _lib.pl_load(
    str(_pl_file).encode()
)

if result != 0:
    _lib.pl_destroy()
    raise RuntimeError(f"Failed to load Perl file: {result}")


def hello_from_pl():
    result = _lib.pl_call_hello()

    if result != 0:
        raise RuntimeError(f"Perl function failed: {result}")
    