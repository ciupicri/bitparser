# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C02
# using the Microsoft compiler
# Note: does not make files that will 
# not compile with this compiler
# Invoke with: make -f Microsoft.makefile

# Note: this requires the service Pack 3 from
# www.Microsoft.com for successful compilation!
# Also, you cannot run the "test" makefiles
# unless you go through and put 'return 0;' at
# the end of every main(), because VC++ does not
# follow the C++ Standard for defaulting return
# values from main().

# de dezactivat depanarea!!!

CPP = cl
CPPFLAGS = -GX -GR # /DNDEBUG
OFLAG = -o
.SUFFIXES : .obj .cpp .c
.cpp.obj :
	$(CPP) $(CPPFLAGS) -c $<
.c.obj :
	$(CPP) $(CPPFLAGS) -c $<

all: test

test:
	BitParserTest.exe

BitParserTest.exe: BitParserTest.obj BitParser.obj
	$(CPP) $(OFLAG)BitParserTest.exe BitParserTest.obj BitParserTest.obj  

BitParserTest.obj: BitParserTest.cpp
BitParser.obj: BitParser.cpp
