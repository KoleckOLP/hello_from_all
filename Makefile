# Ultimate Polyglot Makefile

CC = gcc
CXX = g++
DOTNET = dotnet
RUSTC = rustc

CFLAGS = -shared -fPIC
CXXFLAGS = -shared -fPIC -std=c++20 -static -static-libstdc++ -static-libgcc

PERL_INC = C:/msys64/mingw64/lib/perl5/core_perl/CORE
PERL_LIB = C:/msys64/mingw64/lib/perl5/core_perl/CORE

RUBY_INC = C:/msys64/mingw64/include/ruby-3.4.0
RUBY_ARCH_INC = C:/msys64/mingw64/include/ruby-3.4.0/x64-mingw32
RUBY_LIB = C:/msys64/mingw64/lib

JAVA_HOME = C:/Program Files/Java/jdk-21.0.12.1
JAVA_INC = $(JAVA_HOME)/include
JAVA_WIN_INC = $(JAVA_INC)/win32
JAVA_SERVER = $(JAVA_HOME)/bin/server

NODE_HOME = C:/msys64/mingw64
NODE_INC = $(NODE_HOME)/include/node
NODE_LIB = $(NODE_HOME)/lib/libnode.dll.a

all: c_lib/c_lib.dll cpp_lib/cpp_lib.dll cs_lib/cs_lib.dll pl_lib/pl_lib.dll rb_lib/rb_lib.dll java_lib/java_lib.dll js_lib/js_lib.dll rs_lib/rs_lib.dll

c_lib/c_lib.dll: c_lib/c_lib.c
	$(info )
	$(info compiling C)
	$(CC) $(CFLAGS) c_lib/c_lib.c -o c_lib/c_lib.dll

cpp_lib/cpp_lib.dll: cpp_lib/cpp_lib.cpp
	$(info )
	$(info compiling C++)
	$(CXX) $(CXXFLAGS) cpp_lib/cpp_lib.cpp -o cpp_lib/cpp_lib.dll

cs_lib/cs_lib.dll: cs_lib/cs_lib.cs cs_lib/cs_lib.csproj global.json
	$(info )
	$(info compiling C#)
	$(DOTNET) build cs_lib/cs_lib.csproj -c Release --nologo
	cp cs_lib/bin/Release/net8.0/cs_lib.dll cs_lib/cs_lib.dll

pl_lib/pl_lib.dll: pl_lib/pl_lib.c
	$(info )
	$(info compiling Perl)
	$(CC) $(CFLAGS) -I$(PERL_INC) pl_lib/pl_lib.c -o pl_lib/pl_lib.dll \
		$(PERL_LIB)/libperl538.a

rb_lib/rb_lib.dll: rb_lib/rb_lib.c
	$(info )
	$(info compiling Ruby)
	$(CC) $(CFLAGS) \
		-I$(RUBY_INC) \
		-I$(RUBY_ARCH_INC) \
		rb_lib/rb_lib.c \
		-o rb_lib/rb_lib.dll \
		$(RUBY_LIB)/libx64-msvcrt-ruby340.dll.a

java_lib/java_lib.class: java_lib/java_lib.java
	$(info )
	$(info compiling Java)
	javac -d java_lib java_lib/java_lib.java

java_lib/java_lib.dll: java_lib/java_lib.c java_lib/java_lib.class
	$(CC) $(CFLAGS) \
		-I"$(JAVA_INC)" \
		-I"$(JAVA_WIN_INC)" \
		java_lib/java_lib.c \
		-o java_lib/java_lib.dll \
		"$(JAVA_SERVER)/jvm.dll"

js_lib/js_lib.dll: js_lib/js_lib.cpp
	$(info )
	$(info compiling JavaScript bridge)
	$(CXX) $(CXXFLAGS) \
		-I"$(NODE_INC)" \
		js_lib/js_lib.cpp \
		-o js_lib/js_lib.dll \
		"$(NODE_LIB)" \
		-lpsapi -liphlpapi -lwinmm -lws2_32 -ldbghelp -lcrypt32 -luserenv

rs_lib/rs_lib.dll: rs_lib/rs_lib.rs
	$(info )
	$(info compiling Rust)
	$(RUSTC) --crate-type cdylib rs_lib/rs_lib.rs -o rs_lib/rs_lib.dll

run: all
	$(info )
	$(info the money shot)
	py orchestrator.py

clean:
	rm -f c_lib/c_lib.dll
	rm -f cpp_lib/cpp_lib.dll
	rm -f cs_lib/cs_lib.dll
	rm -f pl_lib/pl_lib.dll
	rm -f rb_lib/rb_lib.dll
	rm -f java_lib/java_lib.dll
	rm -f java_lib/*.class
	rm -f js_lib/js_lib.dll
	rm -f node_dev/libnode.dll.a
	rm -f java_lib/jvm.dll.a
	rm -f java_lib/jvm.def
	rm -f java_lib/jvm.exports
	rm -rf cs_lib/bin cs_lib/obj
	rm -f rs_lib/rs_lib.dll
