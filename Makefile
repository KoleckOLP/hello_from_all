# Ultimate Polyglot Makefile

CC = gcc
CXX = g++
DOTNET = dotnet

CFLAGS = -shared -fPIC
CXXFLAGS = -shared -fPIC -static -static-libstdc++ -static-libgcc

PERL_INC = C:/msys64/mingw64/lib/perl5/core_perl/CORE
PERL_LIB = C:/msys64/mingw64/lib/perl5/core_perl/CORE

RUBY_INC = C:/msys64/mingw64/include/ruby-3.4.0
RUBY_ARCH_INC = C:/msys64/mingw64/include/ruby-3.4.0/x64-mingw32
RUBY_LIB = C:/msys64/mingw64/lib

JAVA_HOME = C:/Program Files/Java/jdk-21.0.12.1
JAVA_INC = $(JAVA_HOME)/include
JAVA_WIN_INC = $(JAVA_INC)/win32
JAVA_SERVER = $(JAVA_HOME)/bin/server
JAVA_IMPORT = java_lib/jvm.dll.a

all: c_lib/c_lib.dll cpp_lib/cpp_lib.dll cs_lib/cs_lib.dll pl_lib/pl_lib.dll rb_lib/rb_lib.dll java_lib/java_lib.dll

c_lib/c_lib.dll: c_lib/c_lib.c
	$(CC) $(CFLAGS) c_lib/c_lib.c -o c_lib/c_lib.dll

cpp_lib/cpp_lib.dll: cpp_lib/cpp_lib.cpp
	$(CXX) $(CXXFLAGS) cpp_lib/cpp_lib.cpp -o cpp_lib/cpp_lib.dll

cs_lib/cs_lib.dll: cs_lib/cs_lib.cs cs_lib/cs_lib.csproj global.json
	$(DOTNET) build cs_lib/cs_lib.csproj -c Release --nologo
	cp cs_lib/bin/Release/net8.0/cs_lib.dll cs_lib/cs_lib.dll

pl_lib/pl_lib.dll: pl_lib/pl_lib.c
	$(CC) $(CFLAGS) -I$(PERL_INC) pl_lib/pl_lib.c -o pl_lib/pl_lib.dll \
		$(PERL_LIB)/libperl538.a

rb_lib/rb_lib.dll: rb_lib/rb_lib.c
	$(CC) $(CFLAGS) \
		-I$(RUBY_INC) \
		-I$(RUBY_ARCH_INC) \
		rb_lib/rb_lib.c \
		-o rb_lib/rb_lib.dll \
		$(RUBY_LIB)/libx64-msvcrt-ruby340.dll.a

java_lib/java_lib.class: java_lib/java_lib.java
	javac -d java_lib java_lib/java_lib.java

java_lib/java_lib.dll: java_lib/java_lib.c java_lib/java_lib.class
	$(CC) $(CFLAGS) \
		-I"$(JAVA_INC)" \
		-I"$(JAVA_WIN_INC)" \
		java_lib/java_lib.c \
		-o java_lib/java_lib.dll \
		"$(JAVA_SERVER)/jvm.dll"

run: all
	py orchestrator.py

clean:
	rm -f c_lib/c_lib.dll
	rm -f cpp_lib/cpp_lib.dll
	rm -f cs_lib/cs_lib.dll
	rm -f pl_lib/pl_lib.dll
	rm -f rb_lib/rb_lib.dll
	rm -f java_lib/java_lib.dll
	rm -f java_lib/*.class
	rm -f java_lib/jvm.dll.a
	rm -f java_lib/jvm.def
	rm -f java_lib/jvm.exports
	rm -rf cs_lib/bin cs_lib/obj
