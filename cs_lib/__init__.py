import clr
from pathlib import Path

dll = Path(__file__).with_name("cs_lib.dll")

clr.AddReference(str(dll))

from System.Reflection import Assembly

_assembly = Assembly.LoadFile(str(dll.resolve()))
_type = _assembly.GetType("CsLib")
_method = _type.GetMethod("HelloFromCs")

def hello_from_cs():
    _method.Invoke(None, None)
