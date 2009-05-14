#include "BitParser.h"
#include <algorithm>		// min, max
#include <iostream>
#include <climits>		// CHAR_BIT
#include <cassert>		// assert
#include <cstdlib>		// NULL

using namespace std;

/* initialize the static data members */
const unsigned char BitParser::run[2] = { 0, ~((unsigned char) 0) };
const word BitParser::wrun[2] = { 0, ~((word) 0) };



/* Constructor */
/*
 *	Initialiases the fields of the class like this:
 * lastPos = -1 so that the search will begin with the first bit
 * lastPos = 0  so that if the first bit is 1 it will be a changing element
 */
BitParser::BitParser(unsigned char *line, int bits)
{
	assert(bits >= 0);
	assert(line != NULL);

	this->line = line;
	this->bits = bits;

	lastPos = -1;
	lastBit = 0;

	sentinel = &line[(bits - 1) / CHAR_BIT];
	wsentinel = (word *)
	    ((unsigned long int) &line[(bits - 1) / CHAR_BIT]
	     / sizeof(wsentinel) * sizeof(wsentinel));
}



/* 
 * Returns the position of the first bit with the value "bit" in
 * the byte c, starting with the position pos. If it's not found
 * it returns -1.
 * c has a length of "bits" bits.
 */
int BitParser::findBit(unsigned char c, int bits, int pos,
		       unsigned char bit)
{
	assert(pos < bits);

	c >>= pos;
	for (int i = pos; i < bits; i++) {
		if ((c & ((unsigned char) 1)) == bit)
			return i;
		c >>= 1;
	}

	return -1;		// not found
}



/* Searches for the next changing element */
/*
 *	First we check if we can find a changing element; we check if the last
 * search was succesfull, or if we have where to search.
 *	Then we start the main search. 
 *	We will start from lastPos + 1.
 *	The part of line in wich we search can be split in 3 parts:
 * (last chunk of a char) | (chars, words ... ,chars) | (first chunk of a char)
 *	Some parts can be missing (the last parts).
 * 	The last bit of the first part/char is at index:
 *  min(CHAR_BIT, remaining bits to parse)
 * 	If we don't find anything in the first part, we search at a char 
 * level untill c is "word aligned". Then we search at a word level.
 * 	If we still didn't find anything or we found a word with a changing
 * element (different than run) we search the remaining bits at a char level,
 * with the last char trunctated at a length of:
 *  bits % CHAR_BIT
 *	If we don't find a changing element we return -1, and set lastPos to
 * bits in order to detect that we've searched all string, the next time
 * the method is called.
 */

// the number of bytes already parsed
#define BYTES_PARSED()	(c - line)
int BitParser::nextChangingElement()
{
	unsigned char *c;	// pointer to the "byte" examined
	word *w;		// pointer to the "word" examined
	unsigned char bit;	// the searched bit
	int pos;		// position within the char
	int startPos;		// starting position

	if (lastPos == bits)	// nowhere to search
		return -1;	// not found

	// start the search
	startPos = lastPos + 1;
	if (startPos == bits) {	// nowhere to search
		lastPos = bits;
		return -1;	// not found
	}

	bit = 1 - lastBit;	// invert the last bit
	c = &line[startPos / CHAR_BIT];

	// first part
	pos = findBit(*c,
		      min(CHAR_BIT, bits - BYTES_PARSED()*CHAR_BIT),
		      startPos % CHAR_BIT,
		      bit);
	if (pos >= 0) {		// found
		lastBit = bit;
		lastPos = pos + BYTES_PARSED() * CHAR_BIT;
		return lastPos + 1;
	}
	// parse untill we are aligned
	for (c++;
	     (c < sentinel) && (((unsigned int) c % sizeof(word)) != 0);
	     c++) {
		if (*c != run[lastBit])
			break;
	}
	if (*c != run[lastBit]) {
		pos = findBit(*c,
			      min(CHAR_BIT,
				  bits - BYTES_PARSED() * CHAR_BIT),
			      0, bit);
		if (pos >= 0) {	// found
			lastBit = bit;
			lastPos = pos + BYTES_PARSED() * CHAR_BIT;
			return lastPos + 1;
		}
		// not found
		lastPos = bits;
		return -1;
	}
	
	// middle part
	// skip a possible large run
	for (w = (word *) c; w < wsentinel; w++) {
		if (*w != wrun[lastBit])
			break;
	}

	// search in a word or in the last part
	for (c = (unsigned char *) w; c < sentinel; c++) {
		if (*c != run[lastBit])
			break;
	}
	pos = findBit(*c,
		      min(CHAR_BIT,
			  bits - BYTES_PARSED() * CHAR_BIT), 0, bit);
	if (pos >= 0) {		// found
		lastBit = bit;
		lastPos = pos + BYTES_PARSED() * CHAR_BIT;
		return lastPos + 1;
	}

	// not found
	lastPos = bits;
	return -1;
}

#undef BYTES_PARSED
