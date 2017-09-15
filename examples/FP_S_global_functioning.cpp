#include <iostream>

#include "bash_tools/bash_tools.h"

#include "Fixed_Point/Fixed_Point_Static.hpp"

int main()
{
	std::cout << italic(bold_yellow("!!! Welcome inside the Fixed Point API !!!")) << std::endl << std::endl;

/* ***** Set values and display modes
 * The fixed point quantification is set in the template for performance optimizations.
 * The first value is the total number of bits including the sign bit.
 * The second is the fractional part number of bits.
 * The FP value is signed only for now
 */
	std::cout << bold_orange("Set values and display modes") << std::endl;

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
	 * Saturations are since symmetric. So min + max = 0 and a 3 bits signed number runs between (-3) and (+3).
	 * That's why multiplying a and b together does not give a 8 bit FP but only 7 bit one because there is no need anymore of this extra bit.
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



/* ***** Relational Operators
 * Compare two FP together or a FP and another typical scalar (float, int)
 */
	std::cout << bold_orange("Relational Operators") << std::endl;

	FP_S<10,2> k = 15.5f;
	FP_S< 5,1> l = -4.f;
	FP_S< 8,3> m = -4.f;
	FP_S<10,0> n = -4.5f; // <- fractional part precision loss

	std::cout << "k <  l = " << (k <  l) << std::endl; // display: false
	std::cout << "k >  l = " << (k >  l) << std::endl; // display: true
	std::cout << "k <= l = " << (k <= l) << std::endl; // display: false
	std::cout << "k >= l = " << (k >= l) << std::endl; // display: true
	std::cout << "l <=-4 = " << (l <= -4) << std::endl; // display: true

	std::cout << "k == l = " << (k == l) << std::endl; // display: false
	std::cout << "k != l = " << (k != l) << std::endl; // display: true
	std::cout << "m == l = " << (m == l) << std::endl; // display: true
	std::cout << "m == n = " << (m == n) << std::endl; // display: true

	std::cout << "m.same_quantification(n) = " << m.same_quantification(n) << std::endl; // display: false

/* ***** Binary Operators
 * They do not care about the quantification, they act directly on the bits
 * when the operator contains the '=' sign, then the result is stocked in the left operand.
 * Else a new object is created;
 */
	std::cout<< std::endl << bold_orange("Binary Operators") << std::endl;

	FP_S<10,2> o = 15.5f;
	FP_S<15,6> p = -26.85f;
	std::cout << "o = " << o.to_bin() << std::endl; // display: 0000000000111110
	std::cout << "p = " << p.to_bin() << std::endl; // display: 1111100101001010

	// bitwise not operator
	std::cout << "~o= " << (~o).to_bin() << std::endl; // display: 1111111111000001

	// bitwise and operator
	std::cout << "c = o & p = " << FP_numeric::to_bin(o&p)       << std::endl; // display: 0000000000001010
	std::cout << "o &= p; o = " << (FP_S<10,2>(o) &= p).to_bin() << std::endl; // display: 0000000000001010

	// bitwise or operator
	std::cout << "c = o | p = " << FP_numeric::to_bin(o|p)       << std::endl; // display: 1111100101111110
	std::cout << "o |= p; o = " << (FP_S<10,2>(o) |= p).to_bin() << std::endl; // display: 1111100101111110

	// bitwise xor operator
	std::cout << "c = o ^ p = " << FP_numeric::to_bin(o^p)       << std::endl; // display: 1111100101110100
	std::cout << "o ^= p; o = " << (FP_S<10,2>(o) ^= p).to_bin() << std::endl; // display: 1111100101110100

	p = 3.2;
	// left shift operator
	std::cout << "c = o << 3 = " << (o << 3).to_bin()              << std::endl; // display: 0000000111110000
	std::cout << "c = o << p = " << (o << p).to_bin()              << std::endl; // display: 0000000111110000 <- p is converted into a strictly positive integer
	std::cout << "o <<= 3; o = " << (FP_S<10,2>(o) <<= 3).to_bin() << std::endl; // display: 0000000111110000

	// right shift operator
	std::cout << "c = o >> 3 = " << (o >> 3).to_bin()              << std::endl; // display: 0000000000000111
	std::cout << "c = o >> p = " << (o >> p).to_bin()              << std::endl; // display: 0000000000000111 <- p is converted into a strictly positive integer
	std::cout << "o >>= 3; o = " << (FP_S<10,2>(o) >>= 3).to_bin() << std::endl; // display: 0000000000000111



/* ***** Basic math Operators
 * All those following operations can be done between fixed point numbers and any other type of.
 * Result of an operation is always without precision loss as possible. But putting it into a
 * pre-defined Fixed Point will change the result quantification to fit its new container.
 */
	std::cout<< std::endl << bold_orange("Basic math Operators") << std::endl;

	std::cout << "o = " << o << std::endl; // display:    15.5(10, 2, s)
	std::cout << "p = " << p << std::endl; // display: 3.20312(15, 6, s)

	// addition
	{
		auto res = o + p;
		FP_S<10,3> q = res;
		FP_S<10,3> q2= o + p; // q and q2 are same
		std::cout << "res = o + p = " << res                  << std::endl; // display:  18.7031(16, 6, s)
		std::cout << "q   = res   = " << q                    << std::endl; // display:   18.625(10, 3, s)
		std::cout << "q2  = o + p = " << q2                   << std::endl; // display:   18.625(10, 3, s)
		std::cout << "o += p; o   = " << (FP_S<10,2>(o) += p) << std::endl; // display:     18.5(10, 2, s)
		std::cout << "o ++  ; o   = " << (FP_S<10,2>(o) ++)   << std::endl; // display:     16.5(10, 2, s)
	}

	std::cout <<std::endl;
	// subtraction
	std::cout << "c = o - p = " << (o-p)                << std::endl; // display: 12.2969(16, 6, s)
	std::cout << "o -= p; o = " << (FP_S<10,2>(o) -= p) << std::endl; // display:   12.25(10, 2, s)
	std::cout << "o --  ; o = " << (FP_S<10,2>(o) --)   << std::endl; // display:    14.5(10, 2, s)

	std::cout <<std::endl;
	// multiplication
	{
		auto res = o * p;
		FP_S<10,3> q = res;
		FP_S<10,3> q2= o * p; // q and q2 are same
		std::cout << "res = o * p = " << res                  << std::endl; // display:  49.6484(24, 8, s)
		std::cout << "q   = res   = " << q                    << std::endl; // display:   49.625(10, 3, s)
		std::cout << "q2  = o * p = " << q2                   << std::endl; // display:   49.625(10, 3, s)
		std::cout << "o *= p;   o = " << (FP_S<10,2>(o) *= p) << std::endl; // display:     49.5(10, 2, s)
	}

	std::cout <<std::endl;
	// division
	std::cout << "c = o / p = " << (o/p)                << std::endl; // display:  4.83887(26,11, s)
	std::cout << "o /= p; o = " << (FP_S<10,2>(o) /= p) << std::endl; // display:     4.75(10, 2, s)

	std::cout <<std::endl;
	// modulo
	std::cout << "c = o % p = " << (o%p)                << std::endl; // display:   2.6875(16, 6, s)
	std::cout << "o %= p; o = " << (FP_S<10,2>(o) %= p) << std::endl; // display:      2.5(10, 2, s)

/* ***** Basic math functions
 * give information about the sign or the value of the fixed point number.
 */
	std::cout<< std::endl << bold_orange("Basic math functions") << std::endl;

	std::cout << "o         = " << o << std::endl; // display:    15.5(10, 2, s)

	// return min, max values of this fixed point quantification into another Fixed Point
	std::cout << "o.min ()  = " << o.min ()  << std::endl; // display:  -127.75(10, 2, s)
	std::cout << "o.max ()  = " << o.max ()  << std::endl; // display:   127.75(10, 2, s)
	std::cout << "o.one ()  = " << o.one ()  << std::endl; // display:        1(10, 2, s)
	std::cout << "o.zero()  = " << o.zero()  << std::endl; // display:        0(10, 2, s)


	std::cout <<std::endl;
	std::cout << "o.is_pos()  = " << o.is_pos()  << std::endl; // display: true
	std::cout << "o.is_spos() = " << o.is_spos() << std::endl; // display: true
	std::cout << "o.is_neg()  = " << o.is_neg()  << std::endl; // display: false

	// positive, negative, or strictly
	std::cout << "o.is_pos()  = " << o.is_pos()  << std::endl; // display: true
	std::cout << "o.is_spos() = " << o.is_spos() << std::endl; // display: true
	std::cout << "o.is_neg()  = " << o.is_neg()  << std::endl; // display: false
	std::cout << "o.is_sneg() = " << o.is_sneg() << std::endl; // display: false
	std::cout << "o.is_zero() = " << o.is_zero() << std::endl; // display: false

	std::cout <<std::endl;
	FP_S<6,1> q;
	std::cout << "q           = " << q << std::endl; // display:        0( 6, 1, s)

	// positive, negative, or strictly
	std::cout << "q.is_pos()  = " << q.is_pos()  << std::endl; // display: true
	std::cout << "q.is_spos() = " << q.is_spos() << std::endl; // display: false
	std::cout << "q.is_neg()  = " << q.is_neg()  << std::endl; // display: true
	std::cout << "q.is_sneg() = " << q.is_sneg() << std::endl; // display: false
	std::cout << "q.is_zero() = " << q.is_zero() << std::endl; // display: true

	std::cout <<std::endl;
	q = 1;
	std::cout << "q          = " << q          << std::endl; // display:        1( 6, 1, s)
	std::cout << "q.is_one() = " << q.is_one() << std::endl; // display: true

	std::cout <<std::endl;
	q = -1024;
	std::cout << "q          = " << q          << std::endl; // display:    -15.5( 6, 1, s)
	std::cout << "q.is_sat() = " << q.is_sat() << std::endl; // display: true
	std::cout << "q.is_max() = " << q.is_max() << std::endl; // display: false
	std::cout << "q.is_min() = " << q.is_min() << std::endl; // display: true
	std::cout << "q.sign  () = " << q.sign  () << std::endl; // display: -1
	std::cout << "q.abs   () = " << q.abs   () << std::endl; // display:     15.5( 6, 1, s)



/* **** Conversions and type cast
 * Conversions return the value of the FP into another type. This aim to have the "same" interpreted value.
 * There are binary modifications. There might be saturations to keep the FP number fit in its new container
 * Cast return the raw data contained in the FP object into another type to be used as a different type.
 * There is NO binary modification.
 */
	std::cout<< std::endl << bold_orange("Conversions and type cast") << std::endl;

	auto coutf = std::cout.flags();

	// int precision = std::numeric_limits<double>::max_digits10;
	std::cout << std::setprecision(50);

	FP_S<50,40> r = -1000;
	// predefined conversions
	std::cout << "r             = " << r             << std::endl; // display:     -512(50,40, s)
	std::cout << "r.to_raw   () = " << r.to_raw   () << std::endl; // display: -562949953421311
	std::cout << "r.to_int   () = " << r.to_int   () << std::endl; // display: -512
	std::cout << "r.to_uint  () = " << r.to_uint  () << std::endl; // display: 512
	std::cout << "r.to_lint  () = " << r.to_lint  () << std::endl; // display: -512
	std::cout << "r.to_luint () = " << r.to_luint () << std::endl; // display: 512

	std::cout << "r.to_float () = " << r.to_float () << std::endl; // display: -512

	std::cout << "r.to_double() = " << r.to_double() << std::endl; // display: -511.9999999999990905052982270717620849609375

	// user defined conversions
	std::cout << "r.convert< char >() = " << (int)r.convert<char>() << std::endl; // display: -128  <- saturation
	std::cout << "r.convert< short>() = " << r.convert<short>()     << std::endl; // display: -512
	std::cout << "r.convert<ushort>() = " << r.convert<unsigned short>() << std::endl; // display: 512


	// cast types
	std::cout <<std::endl;
	std::cout << "r.cast<int>()  = " << r.cast<int>() << std::endl; // display: 1 <- take the 32 right bits
	std::cout << "(int)r         = " << (int)r        << std::endl; // display: 1 <- same as above
	std::cout << "r.cast<char>() = " << (int)r.cast<char>() << std::endl; // display: 1 <- take the 8 right bits
	std::cout << "(char)r        = " << (int)(char)r        << std::endl; // display: 1 <- same as above

	FP_S<16,5> s = 12.785;
	std::cout << "s              = " << s                   << std::endl; // display: -512(50,40, s)
	std::cout << "s.to_bin()     = " << s.to_bin()          << std::endl; // display: 0000000110011001
	std::cout << "s.cast<int>()  = " << s.cast<int>()       << std::endl; // display: 409 <- take the 32 right bits
	std::cout << "(int)s         = " << (int)s              << std::endl; // display: 409 <- same as above
	std::cout << "s.cast<char>() = " << (int)s.cast<char>() << std::endl; // display: -103 <- take the 8 right bits
	std::cout << "(char)s        = " << (int)(char)s        << std::endl; // display: -103 <- same as above

	std::cout <<std::endl;
	std::cout << "r.to_bin   () = " << r.to_bin () << std::endl; // display: 1111111111111110000000000000000000000000000000000000000000000001
	std::cout << "r.to_hex   () = " << r.to_hex () << std::endl; // display: fffe000000000001

	// return only the "used" bits
	std::cout << "r.to_sbin  () = " << r.to_sbin() << std::endl; // display: 10000000000000000000000000000000000000000000000001
	std::cout << "r.to_shex  () = " << r.to_shex() << std::endl; // display: e000000000001
	std::cout.flags(coutf);

	std::cout << std::endl << italic(bold_yellow("!!! Good bye !!!")) << std::endl;
	return 0;
}
