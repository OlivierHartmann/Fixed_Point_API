#include <iostream>

#include "bash_tools/bash_tools.h"

#include "Fixed_Point/Fixed_Point_Static.hpp"

int main()
{
	std::cout << italic(bold_yellow("!!! Welcome inside the Fixed Point API !!!")) << std::endl << std::endl;

/* ***** Set values
 * The fixed point quantification is set in the template for performance optimizations.
 * The first value is the total number of bits including the sign bit.
 * The second is the fractional part number of bits.
 * The FP value is signed only for now
 */
	std::cout << bold_orange("Set values") << std::endl;

	FP_S< 4,0> a =   3;    // is same as FP_S< 4,0> a(3);
	FP_S< 4,1> b =   3.f;  // is same as FP_S< 4,1> a(3.f);
	FP_S<10,4> c = -25.6f; // is same as FP_S<10,4> a(-25.6f);


	/* **** Display
	 * There are several display modes: easy display, a debug resume view, a debug short resume view and a precision display.
	 */

	std::cout << std::endl << bold_blue("Easy display") << std::endl;

	std::cout << "a = " << a << std::endl; // display:       3( 4, 0, s) <- "s" means signed value
	std::cout << "b = " << b << std::endl; // display:       3( 4, 1, s)
	std::cout << "c = " << c << std::endl; // display:-25.5625(10, 4, s) <- this is not exactly the given number, of course its quantified

	// for more details, you can print the debug resume view:
	std::cout << std::endl << bold_blue("Debug resume view") << std::endl;

	std::cout << "a = " << std::endl << a.resume();   // will display all these lines:
											// value  = 3.000000( 4, 0, s)   <- the number with its quantification and arithmetic sign
											// sign   = pos, abs = 3.000000  <- the sign of the number and its absolute value
											// bin    = 00000011 (3)         <- the binary value (and its value interpretation)
											// hex    = 03                   <- the hexadecimal value
											// f mask = 00000000 (0)         <- the fractional part mask (number of bits)
											// i mask = 00000111 (3)         <- the integral part mask (number of bits)
											// n mask = 00001111             <- the total number mask with the sign bit
											// one    = 00000001 (1.000000)  <- a one value in this given quantification
											// max    = 00000111 (7.000000)  <- the maximum value in this given quantification
											// min    = 11111001 (-7.000000) <- the minimum value in this given quantification

	std::cout << "b = " << std::endl << b.resume();   // will display all these lines:
											// value  = 3.000000( 4, 1, s)
											// sign   = pos, abs = 3.000000
											// bin    = 00000110 (6)         <- the binary value as a fractional bit
											// hex    = 06
											// f mask = 00000001 (1)         <- the fractional mask as one bit
											// i mask = 00000110 (2)
											// n mask = 00001111
											// one    = 00000010 (1.000000)
											// max    = 00000111 (3.500000)
											// min    = 11111001 (-3.500000)

	std::cout << "c = " << std::endl << c.resume();   // will display all these lines:
											// value  = -25.562500(10, 4, s)
											// sign   = neg, abs = 25.562500
											// bin    = 1111111001100111 (-409)
											// hex    = fe67
											// f mask = 0000000000001111 (4)
											// i mask = 0000000111110000 (5)
											// n mask = 0000001111111111
											// one    = 0000000000010000 (1.000000)
											// max    = 0000000111111111 (31.937500)
											// min    = 1111111000000001 (-31.937500)

	// or with less details:
	std::cout << std::endl << bold_blue("Debug short resume view") << std::endl;

	std::cout << "a = " << std::endl  << a.short_resume();   // will display all these lines:
											// value  = 3.000000( 4, 0, s)
											// bin    = 0011 (3)
											// mask   = SIII           <- S is a sign bit and I an integral bit
											// hex    = 3


	std::cout << "b = " << std::endl  << b.short_resume();   // will display all these lines:
											// value  = 3.000000( 4, 1, s)
											// bin    = 0110 (6)
											// mask   = SIIF           <- F is a fractional bit
											// hex    = 6

	std::cout << "c = " << std::endl  << c.short_resume();   // will display all these lines:
											// value  = -25.562500(10, 4, s)
											// bin    = 1001100111 (-409)
											// mask   = SIIIIIFFFF
											// hex    = e67

	// display with more or less precision: will play with the std::cout floating point display precision
	std::cout << std::endl << bold_blue("More or less display precision") << std::endl;
	std::cout << "c = " << c.display(4) << " (4 digits)" << std::endl; //display: c = -25.56 (4 digits)
	std::cout << "c = " << c.display(6) << " (6 digits)" << std::endl; //display: c = -25.5625 (6 digits)

	// you can note there is no quantification details. To display it alone, use the following.
	// It is useful to display vectors of same quantification numbers.
	std::cout << "c quantif = " << c.display_quantification() << std::endl; //display: c quantif = (10, 4, s)



	/* **** Saturations
	 * As you may noticed above, the FP numbers have min and max values in function of their quantifications.
	 * Those values are the limits that can take a value in it.
	 * You may wonder why is it a symmetric saturation? A 3 bits signed number shall run between (-4) and (+3).
	 * It is true. But when developing this API, their were some issues in my computation dynamics because of this asymmetry.
	 * Saturations are since symmetric. So min + max = 0!
	 */

	std::cout << std::endl << bold_blue("Saturations") << std::endl;

	FP_S< 4,0> d =   15;
	FP_S< 4,1> e = -4.f;

	std::cout << "d = " << d << std::endl; // display:        7( 4, 0, s)
	std::cout << "e = " << e << std::endl; // display:     -3.5( 4, 1, s)

	d = -8;
	e = 6;

	std::cout << "d = " << d << std::endl; // display:       -7( 4, 0, s)
	std::cout << "e = " << e << std::endl; // display:      3.5( 4, 1, s)



	/* **** From fixed point to another
	 * The rule is easy: a FP can't change its quantification. The main information is carried in the integral part.
	 * So there may have a lost of precision on the fractional part. And of course saturations may appear.
	 */

	std::cout << std::endl << bold_blue("From fixed point to another") << std::endl;

	FP_S< 8,3> f = 15.75;
	FP_S< 6,3> g = f;
	FP_S< 6,1> h = f;

	std::cout << "f = " << f << std::endl; // display:    15.75( 8, 3, s)
	std::cout << "g = " << g << std::endl; // display:    3.875( 6, 3, s) <- saturation
	std::cout << "h = " << h << std::endl; // display:     15.5( 6, 1, s) <- precision loss on the fractional part


	/* **** Force the set of a data
	 * As you seen above, when you use the operator = with an integer, the new value is set on the integral part,
	 * leaving the fractional part at null. But you may want to set this value as the binary value in this FP.
	 * Then use the following function. But you have to be sure of what you are doing. There is no saturation check.
	 * There is in fact a sign extension that takes the given sign bit of the number and extend it on the left part of the number
	 */
	std::cout << std::endl << bold_blue("Force the set of a data") << std::endl;

	FP_S< 6,1> i = 15;
	std::cout << "i = " << std::endl  << i.short_resume();   // will display all these lines:
											// value  = 15.000000( 6, 1, s)
											// bin    = 011110 (30)
											// mask   = SIIIIF
											// hex    = 1e


	i.set_data(15);
	std::cout << "i = " << std::endl  << i.short_resume();   // will display all these lines:
											// value  = 7.500000( 6, 1, s)
											// bin    = 001111 (15)
											// mask   = SIIIIF
											// hex    = 0f

	i.set_data(72);
	std::cout << "i = " << std::endl  << i.short_resume();   // will display all these lines:
											// value  = 4.000000( 6, 1, s)
											// bin    = 001000 (8)       <- the sign extension has erased the most significant bits.
											// mask   = SIIIIF
											// hex    = 08

	FP_S<8,1> j;
	j.set_data(72);
	std::cout << "j = " << std::endl  << j.short_resume();   // will display all these lines:
											// value  = 36.000000( 8, 1, s)
											// bin    = 01001000 (72)
											// mask   = SIIIIIIF
											// hex    = 48



	/* **** Data size
	 * The size of the number on which the data is stocked depends on the given quantification.
	 * It actually use the shortest available type. On linux you may go up to 128 bits!
	 * Above it will assert when compiling.
	 */
	std::cout << std::endl << bold_blue("Data size") << std::endl;

	std::cout << "1 byte = " << std::endl  << (FP_S<6,1>(15)).short_resume();   // will display all these lines:
											// value  = 15.000000( 6, 1, s)
											// bin    = 011110 (30)
											// mask   = SIIIIF
											// hex    = 1e

	std::cout << "2 bytes = " << std::endl  << (FP_S<10,1>(15)).short_resume();   // will display all these lines:
											// value  = 15.000000(10, 1, s)
											// bin    = 0000011110 (30)
											// mask   = SIIIIIIIIF
											// hex    = 01e

	std::cout << "4 bytes = " << std::endl  << (FP_S<30,1>(15)).short_resume();   // will display all these lines:
											// value  = 15.000000(30, 1, s)
											// bin    = 000000000000000000000000011110 (30)
											// mask   = SIIIIIIIIIIIIIIIIIIIIIIIIIIIIF
											// hex    = 0000001e


	std::cout << "8 bytes = " << std::endl  << (FP_S<60,1>(15)).short_resume();   // will display all these lines:
											// value  = 15.000000(60, 1, s)
											// bin    = 000000000000000000000000000000000000000000000000000000011110 (30)
											// mask   = SIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIF
											// hex    = 00000000000001e


#if defined(__GNUC__) && defined(__x86_64__)
	std::cout << "16 bytes = " << std::endl  << (FP_S<70,10>(-15)).short_resume();   // will display all these lines:
											// value  = -15.000000(70,10, s)
											// bin    = 1111111111111111111111111111111111111111111111111111111100010000000000 (-15360)
											// mask   = SIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIFFFFFFFFFF
											// hex    = ffffffffffffffc400
#endif


	std::cout << std::endl << italic(bold_yellow("!!! Good bye !!!")) << std::endl;
	return 0;
}
