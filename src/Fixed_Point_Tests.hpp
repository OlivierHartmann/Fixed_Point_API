#ifndef FIXED_POINT_TESTS_HPP_
#define FIXED_POINT_TESTS_HPP_

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

#include "Fixed_Point/Fixed_Point_Static.hpp"

#include "bash_tools/bash_tools.h"

class Fixed_Point_Tests
{
private:
	std::ostream& os;
	const int header_prec;

public:
	Fixed_Point_Tests(std::ostream& os = std::cout, int header_prec = 6);
	virtual ~Fixed_Point_Tests();

	/*
	 * Methods that call different tests in function of the tested thematic
	 * as arithmetic or comparison functions
	 */
	template <typename A, typename B>
	void test_arithmetics(A a, B b);
	template <typename A, typename B, typename C>
	void test_arithmetics(A a, B b, C c);
	template <typename A, typename B>
	void test_comparisons(A a, B b);
	template <typename A, typename B, typename C>
	void test_comparisons(A a, B b, C c);

	/*
	 * Methods that print an header with the tested given values
	 */
	template <typename A>
	void test_header(const std::string& method_name, A a,
	                 const std::string& str_a);
	template <typename A, typename B>
	void test_header(const std::string& method_name, A a, B b,
	                 const std::string& str_a,
	                 const std::string& str_b);
	template <typename A, typename B, typename C>
	void test_header(const std::string& method_name, A a, B b, C c,
	                 const std::string& str_a,
	                 const std::string& str_b,
	                 const std::string& str_c);

	/*
	 * Methods that apply the given function to the argument a alone,
	 * or a and b as arguments of f
	 */
	template <typename A, class Functor>
	void test_method(A a, Functor f);
	template <typename A, typename B, class Functor>
	void test_method(A a, B b, Functor f);

	/*
	 * Methods that display results of its different arguments executed by f
	 */
	template <typename A, class Functor>
	void test_bank(A a, Functor f, const std::string& op,
	               const std::string& str_a);
	template <typename A, typename B, class Functor>
	void test_bank(A a, B b, Functor f, const std::string& op,
	               const std::string& str_a,
	               const std::string& str_b);
	template <typename A, typename B, typename C, class Functor>
	void test_bank(A a, B b, C c, Functor f, const std::string& op,
	               const std::string& str_a,
	               const std::string& str_b,
	               const std::string& str_c);

	/*
	 * Methods that test a, b and c by adding them together 2 by 2
	 */
	template <typename A, typename B>
	void addition(A a, B b);
	template <typename A, typename B, typename C>
	void addition(A a, B b, C c);

	/*
	 * Methods that test a, b and c by subtracting them together 2 by 2
	 */
	template <typename A, typename B>
	void subtraction(A a, B b);
	template <typename A, typename B, typename C>
	void subtraction(A a, B b, C c);

	/*
	 * Methods that test a, b and c by multiplying them together 2 by 2
	 */
	template <typename A, typename B>
	void multiplication(A a, B b);
	template <typename A, typename B, typename C>
	void multiplication(A a, B b, C c);

	/*
	 * Methods that test a, b and c by dividing them together 2 by 2
	 */
	template <typename A, typename B>
	void division(A a, B b);
	template <typename A, typename B, typename C>
	void division(A a, B b, C c);

	/*
	 * Methods that test a and b by incrementing or decrementing it
	 */
	template <typename A>
	void increment(A a);
	template <typename A, typename B>
	void increment(A a, B b);
	template <typename A, typename B, typename C>
	void increment(A a, B b, C c);

	template <typename A>
	void decrement(A a);
	template <typename A, typename B>
	void decrement(A a, B b);
	template <typename A, typename B, typename C>
	void decrement(A a, B b, C c);

	/*
	 * Methods that display the maximum between a, b and c 2 by 2
	 */
	template <typename A, typename B>
	void maximum(A a, B b);
	template <typename A, typename B, typename C>
	void maximum(A a, B b, C c);

	/*
	 * Methods that display the minimum between a, b and c 2 by 2
	 */
	template <typename A, typename B>
	void minimum(A a, B b);
	template <typename A, typename B, typename C>
	void minimum(A a, B b, C c);

	/*
	 * Methods that compare a, b and c 2 by 2
	 * such as (a == b)
	 */
	template <typename A, typename B>
	void comp_equal(A a, B b);
	template <typename A, typename B, typename C>
	void comp_equal(A a, B b, C c);

	/*
	 * Methods that compare a, b and c 2 by 2
	 * such as (a > b)
	 */
	template <typename A, typename B>
	void comp_high(A a, B b);
	template <typename A, typename B, typename C>
	void comp_high(A a, B b, C c);

	/*
	 * Methods that compare a, b and c 2 by 2
	 * such as (a >= b)
	 */
	template <typename A, typename B>
	void comp_high_or_equal(A a, B b);
	template <typename A, typename B, typename C>
	void comp_high_or_equal(A a, B b, C c);

	/*
	 * Methods that compare a, b and c 2 by 2
	 * such as (a < b)
	 */
	template <typename A, typename B>
	void comp_low(A a, B b);
	template <typename A, typename B, typename C>
	void comp_low(A a, B b, C c);

	/*
	 * Methods that compare a, b and c 2 by 2
	 * such as (a <= b)
	 */
	template <typename A, typename B>
	void comp_low_or_equal(A a, B b);
	template <typename A, typename B, typename C>
	void comp_low_or_equal(A a, B b, C c);
};

#include "Fixed_Point_Tests.hxx"

#endif /* FIXED_POINT_TESTS_HPP_ */
