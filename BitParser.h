#ifndef _BitParser_H
#define _BitParser_H


typedef unsigned long int word;

/*
 *	Class for searching a bit pattern in a string/buffer.
 */

class BitParser {
      public:
	BitParser(unsigned char *line2Parse, int bits2Parse);

	/* Returns the position of the next changing elements */
	int nextChangingElement();

      private:
	unsigned char *line2Parse;
	int bits2Parse;
	int lastBit;		// previous changing bit
	int lastPos;		// previous result
	unsigned char *sentinel;
	word *wsentinel;
	/* constants; ar trebui sa fie doar in cpp sau sa fie membri? */
	static const unsigned char run[2];	// runs of 0s and 1s
	static const word wrun[2];	// runs of 0s and 1s

	int findBit(unsigned char c, int bits, int pos, unsigned char bit);
};

#endif				/* _BitParser_H */
