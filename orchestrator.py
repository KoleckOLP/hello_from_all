import ctypes
import os

import py_lib
import c_lib
import cpp_lib
import cs_lib
import pl_lib
import rb_lib
import java_lib
import js_lib

def main():
	py_lib.hello_from_py()
	c_lib.hello_from_c()
	cpp_lib.hello_from_cpp()
	cs_lib.hello_from_cs()
	pl_lib.hello_from_pl()
	rb_lib.hello_from_rb()
	java_lib.hello_from_java()
	js_lib.hello_from_js()

if __name__ == "__main__":
	main()
