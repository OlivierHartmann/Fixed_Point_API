//============================================================================
// Name        : Fixed_Point_API.cpp
// Author      : Olivier Hartmann
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "bash_tools/bash_tools.h"

#include "Fixed_Point/Fixed_Point_Static.hpp"
#include "Fixed_Point_Tests.hpp"

int main()
{
	std::cout << italic(bold_yellow("!!! Welcome inside the Fixed Point API test bench !!!")) << std::endl;

	Fixed_Point_Tests fpt;

	FP_S< 8,3> a = 3.0f;
	FP_S<10,5> b = 9.5f;
	FP_S< 9,4> c = 8.3f;



	fpt.test_arithmetics(a,b,c);
	fpt.test_comparisons(a,b,c);


	std::cout << italic(bold_yellow("!!!Good bye!!!")) << std::endl;
	return 0;
}
