#include <iostream>
#include <cassert>
#include <climits>
#include <ctime>          // clock
#include "BitParser.h"

using namespace std;




/*
 * 	Initialises the line with the pattern required for test 1.
 * 	1,0,1,0,...
 */
void test1_initLine(unsigned char line2Parse[], unsigned int size)
{
	unsigned char c, bit; // pattern and the current bit

	// compute the 1,0,1,0 ... pattern
	bit = 1;
	c = 1;
	for (int i = 0; i < CHAR_BIT; i++) {
		c <<= 1;
		c |= bit;
		bit = 1 - bit;
	}
	if ((CHAR_BIT % 2) == 0)
		c = ~c; // in order to start with 1,0,..

	// initialize the line with the pattern ....
	for (unsigned int i = 0; i < size; i++)
		line2Parse[i] = c;
}



/*
 * "va trebui sa parseze o linie de 1.000.001 (un  milion unu) biti 
 *  care este compusa din sirul de biti 1,0,1,0,...."
 */
bool test1(const int NBITS = 1000001)
{
	unsigned char line2Parse[(NBITS - 1) / CHAR_BIT + 1];
	int pos, correct_pos;
	int changing_elements;
	clock_t ti, tf;
	bool failed = false;

	cout << "Test1 (NBITS = " << NBITS << ")" << endl;

	test1_initLine(line2Parse, sizeof(line2Parse));
	BitParser bp(line2Parse, NBITS);
	cout << "BitParser initialized" << endl;

	// run the test
	changing_elements = 0;
	correct_pos = 1;
	ti = clock();
	while (1) {
		pos = bp.nextChangingElement();
		if (pos == -1)
			break;
		changing_elements++;
		if (pos != correct_pos) {
			failed = true;
			cerr << "Error: pos = " << pos
			     << " correct_pos = " << correct_pos << endl;
		}
		correct_pos += 1;
	}

	tf = clock();
	if (changing_elements != NBITS) {
		failed = true;
		cerr << "Error at the end ..." << endl;
	}

	cout << ((tf - ti) /
	         (double) CLOCKS_PER_SEC) << " seconds" << endl;

	return failed;
}



/*
 * 	Initialises the line with the pattern required for test 2.
 */
void test2_initLine(unsigned char line2Parse[], unsigned int size, 
                    int NBITS, int LRUN)
{
	unsigned char change[CHAR_BIT]; // border chars like 1,..1,0,...0
	// chars with a changing element
	// change = {0,0,..0; 1,0..0; 1,1,..0; ... 1,1,..1} 
	unsigned char c;
	unsigned int limit; // where is the border (changing element)
	unsigned int bit;   // the current bit value

	// initialize change
	change[0] = 0;
	for (int i = 1; i < CHAR_BIT; i++)
		change[i] = (change[i - 1] << 1) | ((unsigned char) 1);

	// initialize line2Parse
	bit = 0;
	c = change[0];
	limit = LRUN;
	for (unsigned int i = 0; i < size; i++) {
		line2Parse[i] = c;
		if (((i + 1) * CHAR_BIT - 1) >= limit) {
			if (bit) { // 1,...1,0,...0
				line2Parse[i] = change[limit % CHAR_BIT];
				c = change[0];
			} else {   // 0,...0,1,...1
				line2Parse[i] = ~change[limit % CHAR_BIT];
				c = ~change[0];
			}
			bit = 1 - bit;
			limit += LRUN;
		}
	}
}



/*
 * "va trebui sa parseze o linie de 1.000.001 (un milion unu) biti care
 *  este compusa din "run"-uri de cate 10.001 de biti,"
 */
bool test2(int NBITS = 1000001, int LRUN = 10001)
{
	unsigned char line2Parse[(NBITS - 1) / CHAR_BIT + 1];
	int pos, correct_pos;
	int changing_elements;
	clock_t ti, tf;
	bool failed = false;

	assert(NBITS > LRUN);
	cout << "Test2 (NBITS = " << NBITS
	    << ", LRUN = " << LRUN << ")" << endl;

	test2_initLine(line2Parse, sizeof(line2Parse), NBITS, LRUN);
	BitParser bp(line2Parse, NBITS);
	cout << "BitParser initialized" << endl;

	// run the test
	correct_pos = LRUN + 1;
	changing_elements = 0;
	ti = clock();
	while (1) {
		pos = bp.nextChangingElement();
		if (pos == -1)
			break;
		changing_elements++;
		if (pos != correct_pos) {
			failed = true;
			cerr << "Error: pos = " << pos <<
			    " correct_pos = " << correct_pos << endl;
		}
		correct_pos += LRUN;
	}

	tf = clock();
	if (changing_elements !=
	    (NBITS / LRUN + ((NBITS % LRUN) ? 1 : 0) - 1)) {
		failed = true;
		cerr << "Error at the end ... "
		     << changing_elements << " changing elements detected"
		     << endl;
	}

	cout << ((tf - ti) /
		 (double) CLOCKS_PER_SEC) << " seconds" << endl;

	return failed;
}



int main(int argc, char *argv[])
{
	bool failed = false;

	cout << " -- main tests" << endl;
	failed = failed || test1();
	cout << endl;
	failed = failed || test2();

	// speed tests
	cout << endl << " -- speed tests" << endl;
	failed = failed || test1(70000000);
	cout << endl;
	failed = failed || test2(70000000, 700000);

	return (int) failed;
}
