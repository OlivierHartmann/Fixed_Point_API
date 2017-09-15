//============================================================================
// Name        : Fixed_Point_API.cpp
// Author      : Olivier Hartmann
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "../bash_tools/bash_tools.h"

#include "../Fixed_Point/Fixed_Point_Static.hpp"
#include "Fixed_Point_Tests.hpp"

int main()
{
	std::cout << italic(bold_yellow("!!! Welcome inside the Fixed Point API test bench !!!")) << std::endl << std::endl;

	Fixed_Point_Tests fpt;

	FP_S<31,3> a = 3.0f;
	FP_S<32,5> b = 9.8925f;
	FP_S< 9,4> c = 8.3f;

	// FP_D d(3.0f, 8, 3);
	// FP_D e(9.5f,10, 5);
	// FP_D f(8.3f, 9, 4);

	// BIG WARNING ! To be compared, the == operator of the fixed point (static or dynamic)
	// number is used. So results in floating point are converted
	// in the fixed point quantification of the FP result. As always, once the floating
	// point converted, its value is not exactly the same anymore. It is this last value that
	// is then compared to the FP result to see if they are equal or not.
	// To put it in a nutshell, the fixed point and floating point values that are displayed might
	// be different (especially after a multiplication or a division) but are successfully compared.

	fpt.test_arithmetics(a,b,c);
	fpt.test_comparisons(a,b,c);
//	fpt.test_arithmetics(d,e,f);
//	fpt.test_comparisons(d,e,f);


	std::cout << std::endl << italic(bold_yellow("!!!Good bye!!!")) << std::endl;
	return 0;
}
