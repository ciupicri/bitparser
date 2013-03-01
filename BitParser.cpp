#include "BitParser.h"
#include <algorithm>  // min, max
#include <iostream>
#include <climits>    // CHAR_BIT
#include <cassert>    // assert
#include <cstdlib>    // NULL

using namespace std;

/* initialize the static data members */
const unsigned char BitParser::run[2] = { 0, std::numeric_limits<unsigned char>::max() };
const word BitParser::wrun[2] = { 0, std::numeric_limits<word>::max() };



/* Constructor */
/*
 * 	Initializes the fields of the class like this:
 * lastPos = -1 so that the search will begin with the first bit
 * lastBit = 0  so that if the first bit is 1 it will be a changing element
 */
BitParser::BitParser(unsigned char *line, int nbits)
{
	assert(nbits >= 0);
	assert(line != NULL);

	this->line = line;
	this->nbits = nbits;

	lastPos = -1;
	lastBit = 0;

	sentinel = &line[(nbits - 1) / CHAR_BIT];
	wsentinel = (word *)
	    ((unsigned long int) &line[(nbits - 1) / CHAR_BIT]
	     / sizeof(wsentinel) * sizeof(wsentinel));
}



/* 
 * Returns the position of the first bit with the value "bit" in
 * the byte c, starting with the position pos. If it's not found
 * it returns -1.
 * c has a length of "nbits" nbits.
 */
int BitParser::findBit(unsigned char c, int nbits, int pos,
                       unsigned char bit)
{
	assert(pos < nbits);

	c >>= pos;
	for (int i = pos; i < nbits; i++) {
		if ((c & ((unsigned char) 1)) == bit)
			return i;
		c >>= 1;
	}

	return -1; // not found
}



/* Searches for the next changing element */
/*
 * 	First we check if we can find a changing element; we check if the last
 * search was successful, or if we have where to search.
 * 	Then we start the main search. 
 * 	We will start from lastPos + 1.
 * 	The part of line in wich we search can be split in 3 parts:
 * (last chunk of a char) | (chars, words ... ,chars) | (first chunk of a char)
 * 	Some parts can be missing (the last parts).
 * 	The last bit of the first part/char is at index:
 *  min(CHAR_BIT, remaining nbits to parse)
 * 	If we don't find anything in the first part, we search at a char 
 * level until c is "word aligned". Then we search at a word level.
 * 	If we still didn't find anything or we found a word with a changing
 * element (different than run) we search the remaining nbits at a char level,
 * with the last char truncated at a length of:
 *  nbits % CHAR_BIT
 * 	If we don't find a changing element we return -1, and set lastPos to
 * nbits in order to detect that we've searched all string, the next time
 * the method is called.
 */

// the number of bytes already parsed
#define PARSED_CHARS() (c - line)
int BitParser::nextChangingElement()
{
	unsigned char *c;	// pointer to the "byte" examined
	word *w;		// pointer to the "word" examined
	unsigned char bit;	// the searched bit
	int pos;		// position within the char
	int startPos;		// starting position

	if (lastPos == nbits)	// nowhere to search
		return -1;	// not found

	// start the search
	startPos = lastPos + 1;
	if (startPos == nbits) { // nowhere to search
		lastPos = nbits;
		return -1; // not found
	}

	bit = 1 - lastBit; // invert the last bit
	c = &line[startPos / CHAR_BIT];

	// first part
	pos = findBit(*c,
		      min<int>(CHAR_BIT, nbits - PARSED_CHARS() * CHAR_BIT),
		      startPos % CHAR_BIT,
		      bit);
	if (pos >= 0) { // found
		lastBit = bit;
		lastPos = pos + PARSED_CHARS() * CHAR_BIT;
		return lastPos + 1;
	}
	// parse until we are aligned
	for (c++;
	     (c < sentinel) && (((intptr_t) c % sizeof(word)) != 0);
	     c++) {
		if (*c != run[lastBit])
			break;
	}
	if (*c != run[lastBit]) {
		pos = findBit(*c,
		              min<int>(CHAR_BIT,
		                       nbits - PARSED_CHARS() * CHAR_BIT),
		              0,
		              bit);
		if (pos >= 0) { // found
			lastBit = bit;
			lastPos = pos + PARSED_CHARS() * CHAR_BIT;
			return lastPos + 1;
		}
		// not found
		lastPos = nbits;
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
	              min<int>(CHAR_BIT,
	                       nbits - PARSED_CHARS() * CHAR_BIT),
	              0,
	              bit);
	if (pos >= 0) { // found
		lastBit = bit;
		lastPos = pos + PARSED_CHARS() * CHAR_BIT;
		return lastPos + 1;
	}

	// not found
	lastPos = nbits;
	return -1;
}
#undef PARSED_CHARS
