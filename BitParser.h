#ifndef _BitParser_H
#define _BitParser_H


typedef unsigned long int word;

/*
 * 	Class for searching a bit pattern in a string/buffer.
 */

class BitParser {
      public:
	BitParser(unsigned char *line, int nbits);

	/* Returns the position of the next changing elements */
	int nextChangingElement();

      private:
	unsigned char *line;
	int nbits;
	int lastBit; // previous changing bit
	int lastPos; // previous result
	unsigned char *sentinel;
	word *wsentinel;
	/* constants (should these be part of the class declaration?) */
	static const unsigned char run[2]; // runs of 0s and 1s
	static const word wrun[2]; // runs of 0s and 1s

	int findBit(unsigned char c, int nbits, int pos, unsigned char bit);
};

#endif /* _BitParser_H */
