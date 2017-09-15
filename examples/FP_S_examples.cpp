#include <iostream>

#include "bash_tools/bash_tools.h"

#include "Fixed_Point/Fixed_Point_Static.hpp"

int main()
{
	std::cout << italic(bold_yellow("!!! Welcome inside the Fixed Point API !!!")) << std::endl << std::endl;

/*
 * The fixed point quantification is set in the template for performance optimizations.
 * The first value is the total number of bits including the sign bit.
 * The second is the fractional part number of bits.
 * The FP value is signed only for now
 */

	FP_S< 4,0> a =   3;    // is same as FP_S< 4,0> a(3);
	FP_S< 4,1> b =   3.f;  // is same as FP_S< 4,1> a(3.f);
	FP_S<10,4> c = -25.6f; // is same as FP_S<10,4> a(-25.6f);


	std::cout << "a = " << a << std::endl; // display:       3( 4, 0, s) <- "s" means signed value
	std::cout << "b = " << b << std::endl; // display:       3( 4, 1, s)
	std::cout << "c = " << c << std::endl; // display:-25.5625(10, 4, s) <- this is not exactly the given number, of course its quantified

	std::cout << std::endl;

	// easy operation
	auto res = a*b + c;
	std::cout << "a*b + c = " << res << std::endl; // display: -16.5625(11, 4, s)

	// gives the same result than the following
	auto ab = a*b;
	std::cout << "a*b     = " << ab  << std::endl; // display:        9( 7, 1, s)
	auto abc = ab + c;
	std::cout << "ab + c  = " << abc << std::endl; // display: -16.5625(11, 4, s)

	std::cout << std::endl;
	// but you can also constraint operation results that can give weird results:
	FP_S<5,2> ab2 = a*b; // is same as "FP_S<5,2> ab2 = res;"  the quantification change is made in the ab2 constructor not in the multiplication
	std::cout << "a*b     = " << ab2 << std::endl; // display:     3.75( 5, 2, s)
	auto abc2 = ab2 + c;
	std::cout << "ab2 + c = " << abc2 << std::endl; // display: -21.8125(11, 4, s)

/*
 * As you may noticed above, the FP numbers have min and max values in function of their quantifications.
 * Those values are the limits that can take a value in it. So a 3 bits signed number shall run between (-4) and (+3).
 * It is true. But when developing this API, their were some issues in my computation dynamics because of this asymmetry.
 * Saturations are since symmetric. So min + max = 0 and a 3 bits signed number runs between (-3) and (+3).
 * That's why multiplying a and b together does not give a 8 bit FP but only 7 bit one because there is no need anymore of this extra bit.
 */

	std::cout << std::endl;
	// now display your results !
	std::cout << "ab = " << std::endl << ab.resume();   // will display all these lines:
											// value  = 9.000000( 7, 1, s)    <- the number with its quantification and arithmetic sign
											// sign   = pos, abs = 9.000000   <- the sign of the number and its absolute value
											// bin    = 00010010 (18)         <- the binary value (and its value interpretation)
											// hex    = 12                    <- the hexadecimal value
											// f mask = 00000001 (1)          <- the fractional part mask (number of bits)
											// i mask = 00111110 (5)          <- the integral part mask (number of bits)
											// n mask = 01111111              <- the total number mask with the sign bit
											// one    = 00000010 (1.000000)   <- a one value in this given quantification
											// max    = 00111111 (31.500000)  <- the maximum value in this given quantification
											// min    = 11000001 (-31.500000) <- the minimum value in this given quantification
	// or shorter
	std::cout << "ab2 = " << std::endl << ab2.short_resume();  // will display all these lines:
											// value  = 3.750000( 5, 2, s)
											// bin    = 01111 (15)
											// mask   = SIIFF           <- S is a sign bit, I an integral bit and F a fractional bit
											// hex    = 0f


	std::cout << std::endl << italic(bold_yellow("!!! Good bye !!!")) << std::endl;
	return 0;
}
