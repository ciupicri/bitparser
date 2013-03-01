CPP = g++
CPPFLAGS = -Wall -std=c++11 # -O2 -march=i586 -g # -DNDEBUG # -g
OFLAG = -o

INDENTSTYLE = -kr -i8

TARGETS = BitParserTest

.SUFFIXES: .cpp .o
.cpp.o:
	$(CPP) $(CPPFLAGS) -c $<

all: $(TARGETS)

check: BitParserTest
	./BitParserTest

.PHONY: objclean distclean indent count 
objclean:
	find . -name '*.o' -print0 | \
		xargs -0 rm -f

distclean: 
	find . \( -name '*~' -or -name 'core*' \) -print0 | \
		xargs -0 rm -f

clean: objclean distclean
	-rm -rf $(TARGETS)

indent:
	find . \( -name '*.cpp' -or -name '*.h' \) -print0 | \
		xargs -0 -r -n 1 indent $(INDENTSTYLE)

count:
	find . \( -name '*.cpp' -or -name '*.h' \) -print0 | \
		xargs -0 -r -n 1 wc -l		


BitParserTest: BitParserTest.o BitParser.o
	$(CPP) $(CPPFLAGS) $(OFLAG)BitParserTest BitParser.o BitParserTest.o


# Dependencies
BitParser.o: BitParser.cpp BitParser.hpp
