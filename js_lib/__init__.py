import ctypes
import os
from pathlib import Path

_here = Path(__file__).resolve().parent

# Node.js installation
NODE_HOME = Path(r"C:\msys64\mingw64\bin")

os.add_dll_directory(str(NODE_HOME))

_lib = ctypes.CDLL(
    str(_here / "js_lib.dll")
)

_lib.js_init.restype = ctypes.c_int

_lib.js_load.argtypes = [ctypes.c_char_p]
_lib.js_load.restype = ctypes.c_int

_lib.js_call_hello.restype = ctypes.c_int

_lib.js_destroy.restype = None


result = _lib.js_init()

if result != 0:
    raise RuntimeError(
        f"Failed to initialize Node.js: {result}"
    )


result = _lib.js_load(
    str(_here / "js_lib.js").encode()
)

if result != 0:
    _lib.js_destroy()
    raise RuntimeError(
        f"Failed to load JavaScript: {result}"
    )


def hello_from_js():
    result = _lib.js_call_hello()

    if result != 0:
        raise RuntimeError(
            f"JavaScript function failed: {result}"
        )
    