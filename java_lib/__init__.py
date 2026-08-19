import ctypes
import os
from pathlib import Path

_here = Path(__file__).resolve().parent

JAVA_HOME = Path(r"C:\Program Files\Java\jdk-21.0.12.1")

os.add_dll_directory(
    str(JAVA_HOME / "bin")
)

os.add_dll_directory(
    str(JAVA_HOME / "bin" / "server")
)

_lib = ctypes.CDLL(
    str(_here / "java_lib.dll")
)

_lib.java_init.restype = ctypes.c_int
_lib.java_load.restype = ctypes.c_int
_lib.java_call_hello.restype = ctypes.c_int
_lib.java_destroy.restype = None

result = _lib.java_init()

if result != 0:
    raise RuntimeError(
        f"Failed to initialize JVM: {result}"
    )

result = _lib.java_load()

if result != 0:
    _lib.java_destroy()
    raise RuntimeError(
        f"Failed to load Java class: {result}"
    )


def hello_from_java():
    result = _lib.java_call_hello()

    if result != 0:
        raise RuntimeError(
            f"Java function failed: {result}"
        )
