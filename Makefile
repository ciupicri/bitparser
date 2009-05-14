include gcc.Makefile
include common.Makefile

TARGETS = BitParserTest

.SUFFIXES : .o .cpp .c
.cpp.o:
	$(CPP) $(CPPFLAGS) -c $<
.c.o:
	$(CPP) $(CPPFLAGS) -c $<

default: test

test: BitParserTest

BitParserTest: BitParserTest.o BitParser.o
	$(CPP) $(CPPFLAGS) $(OFLAG)BitParserTest BitParser.o BitParserTest.o

