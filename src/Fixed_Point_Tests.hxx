#ifndef FIXED_POINT_TESTS_HXX_
#define FIXED_POINT_TESTS_HXX_

#include "Fixed_Point_Tests.hpp"

#define os_nline    os << std::endl

Fixed_Point_Tests::Fixed_Point_Tests(std::ostream& os, int header_prec)
: os(os), header_prec(header_prec)
{
}

Fixed_Point_Tests::~Fixed_Point_Tests()
{
}

template <typename A, typename B>
void Fixed_Point_Tests::test_arithmetics(A a, B b)
{
	os << bold_orange("Arithmetic functions tests *******************"); os_nline;

	addition      (a,b);
	subtraction   (a,b);
	multiplication(a,b);
	division      (a,b);
	increment     (a,b);
	decrement     (a,b);

	os << bold_orange("**********************************************"); os_nline;
}

template <typename A, typename B, typename C>
void Fixed_Point_Tests::test_arithmetics(A a, B b, C c)
{
	os << bold_orange("Arithmetic functions tests *******************"); os_nline;

	addition      (a,b,c);
	subtraction   (a,b,c);
	multiplication(a,b,c);
	division      (a,b,c);
	increment     (a,b,c);
	decrement     (a,b,c);

	os << bold_orange("**********************************************"); os_nline;
}

template <typename A, typename B>
void Fixed_Point_Tests::test_comparisons(A a, B b)
{
	os << bold_orange("Comparison functions tests *******************"); os_nline;

	minimum           (a,b);
	maximum           (a,b);
	comp_equal        (a,b);
	comp_high         (a,b);
	comp_high_or_equal(a,b);
	comp_low          (a,b);
	comp_low_or_equal (a,b);

	os << bold_orange("**********************************************"); os_nline;
}

template <typename A, typename B, typename C>
void Fixed_Point_Tests::test_comparisons(A a, B b, C c)
{
	os << bold_orange("Comparison functions tests *******************"); os_nline;

	minimum           (a,b,c);
	maximum           (a,b,c);
	comp_equal        (a,b,c);
	comp_high         (a,b,c);
	comp_high_or_equal(a,b,c);
	comp_low          (a,b,c);
	comp_low_or_equal (a,b,c);

	os << bold_orange("**********************************************"); os_nline;
}


template <typename A>
void Fixed_Point_Tests::test_header(const std::string& method_name, A a,
                                    const std::string& str_a)
{
	std::stringstream sstream;
	sstream << "Test \"" << method_name << "\" with "
	        << str_a << " = " << a.display(header_prec) << a.display_quantification()
	        << ":";

	os << bold_blue(sstream.str()); os_nline;
}

template <typename A, typename B>
void Fixed_Point_Tests::test_header(const std::string& method_name, A a, B b,
                                    const std::string& str_a,
                                    const std::string& str_b)
{
	std::stringstream sstream;
	sstream << "Test \"" << method_name << "\" with "
	        << str_a << " = " << a.display(header_prec) << a.display_quantification() << ", "
	        << str_b << " = " << b.display(header_prec) << b.display_quantification()
	        << ":";

	os << bold_blue(sstream.str()); os_nline;
}

template <typename A, typename B, typename C>
void Fixed_Point_Tests::test_header(const std::string& method_name, A a, B b, C c,
                                    const std::string& str_a,
                                    const std::string& str_b,
                                    const std::string& str_c)
{
	std::stringstream sstream;
	sstream << "Test \"" << method_name << "\" with "
	        << str_a << " = " << a.display(header_prec) << a.display_quantification() << ", "
	        << str_b << " = " << b.display(header_prec) << b.display_quantification() << ", "
	        << str_c << " = " << c.display(header_prec) << c.display_quantification()
	        << ":";

	os << bold_blue(sstream.str()); os_nline;
}


template <typename A, class Functor>
void Fixed_Point_Tests::test_method(A a, Functor f)
{
	try
	{
		auto float_res = f(a.to_float());
		auto fp_res    = f(a);
		auto comp_res  = fp_res.to_float() == float_res;

		std::stringstream sstream;
		sstream << fp_res << ",   float : "  << std::setprecision(6) << std::setw(8) << float_res
				<< ",   comp : " << comp_res;

		if (comp_res)
			os << green(sstream.str());
		else
			os <<   red(sstream.str());
	}
	catch(std::exception& e)
	{
		os << inverted(bold_red(std::string(" thrown exception : ") + e.what()));
	}
}

template <typename A, typename B, class Functor>
void Fixed_Point_Tests::test_method(A a, B b, Functor f)
{
	try
	{
		auto float_res = f(a.to_float(),b.to_float());
		auto fp_res    = f(a,b);
		auto comp_res  = fp_res == float_res;

		std::stringstream sstream;
		sstream << fp_res << ",   float : "  << std::setprecision(6) << std::setw(8) << float_res
				<< ",   comp : " << comp_res;

		if (comp_res)
			os << green(sstream.str());
		else
			os <<   red(sstream.str());
	}
	catch(std::exception& e)
	{
		os << inverted(bold_red(std::string(" thrown exception : ") + e.what()));
	}
}


template <typename A, class Functor>
void Fixed_Point_Tests::test_bank(A a, Functor f, const std::string& op,
                                  const std::string& str_a)
{
	os << "\t" << str_a << " " << op << " = "; test_method(a,f); os_nline;
}

template <typename A, typename B, class Functor>
void Fixed_Point_Tests::test_bank(A a, B b, Functor f, const std::string& op,
                                  const std::string& str_a,
                                  const std::string& str_b)
{
	os << "\t" << str_a << " " << op << " " << str_b << " = "; test_method(a,b,f); os_nline;
	os << "\t" << str_b << " " << op << " " << str_a << " = "; test_method(b,a,f); os_nline;
}

template <typename A, typename B, typename C, class Functor>
void Fixed_Point_Tests::test_bank(A a, B b, C c, Functor f, const std::string& op,
                                  const std::string& str_a,
                                  const std::string& str_b,
                                  const std::string& str_c)
{
	test_bank(a, b, f, op, str_a, str_b);
	test_bank(a, c, f, op, str_a, str_c);
	test_bank(b, c, f, op, str_b, str_c);
}


struct adder_type
{
	template <typename A, typename B>
	auto operator()(const A& a, const B& b) -> decltype(a+b) // decltype needed for C++11 but not anymore from C++14
	{
		return a + b;
	}
};

template <typename A, typename B>
void Fixed_Point_Tests::addition(A a, B b)
{
	adder_type oper;
	test_header("addition", a, b, "a", "b");
	test_bank(a, b, oper, "+", "a", "b");
	os_nline;
}

template <typename A, typename B, typename C>
void Fixed_Point_Tests::addition(A a, B b, C c)
{
	adder_type oper;
	test_header("addition", a, b, c, "a", "b", "c");
	test_bank(a, b, c, oper, "+", "a", "b", "c");
	os_nline;
}


struct substracter_type
{
	template <typename A, typename B>
	auto operator()(const A& a, const B& b) -> decltype(a+b)
	{
		return a - b;
	}
};

template <typename A, typename B>
void Fixed_Point_Tests::subtraction(A a, B b)
{
	substracter_type oper;
	test_header("subtraction", a, b, "a", "b");
	test_bank(a, b, oper, "-", "a", "b");
	os_nline;
}

template <typename A, typename B, typename C>
void Fixed_Point_Tests::subtraction(A a, B b, C c)
{
	substracter_type oper;
	test_header("subtraction", a, b, c, "a", "b", "c");
	test_bank(a, b, c, oper, "-", "a", "b", "c");
	os_nline;
}


struct multiplicater_type
{
	template <typename A, typename B>
	auto operator()(const A& a, const B& b) -> decltype(a+b)
	{
		return a * b;
	}
};

template <typename A, typename B>
void Fixed_Point_Tests::multiplication(A a, B b)
{
	multiplicater_type oper;
	test_header("multiplication", a, b, "a", "b");
	test_bank(a, b, oper, "*", "a", "b");
	os_nline;
}

template <typename A, typename B, typename C>
void Fixed_Point_Tests::multiplication(A a, B b, C c)
{
	multiplicater_type oper;
	test_header("multiplication", a, b, c, "a", "b", "c");
	test_bank(a, b, c, oper, "*", "a", "b", "c");
	os_nline;
}


struct diviser_type
{
	template <typename A, typename B>
	auto operator()(const A& a, const B& b) -> decltype(a+b)
	{
		return a / b;
	}
};

template <typename A, typename B>
void Fixed_Point_Tests::division(A a, B b)
{
	diviser_type oper;
	test_header("division", a, b, "a", "b");
	test_bank(a, b, oper, "/", "a", "b");
	os_nline;
}

template <typename A, typename B, typename C>
void Fixed_Point_Tests::division(A a, B b, C c)
{
	diviser_type oper;
	test_header("division", a, b, c, "a", "b", "c");
	test_bank(a, b, c, oper, "/", "a", "b", "c");
	os_nline;
}


struct incrementer_type
{
	template <typename A>
	auto operator()(A a) -> decltype(a++)
	{
		return (++a);
	}
};

template <typename A>
void Fixed_Point_Tests::increment(A a)
{
	incrementer_type oper;
	test_header("increment", a, "a");
	test_bank(a, oper, "++", "a");
	os_nline;
}

template <typename A, typename B>
void Fixed_Point_Tests::increment(A a, B b)
{
	incrementer_type oper;
	test_header("increment", a, b, "a", "b");
	test_bank(a, oper, "++", "a");
	test_bank(b, oper, "++", "b");
	os_nline;
}

template <typename A, typename B, typename C>
void Fixed_Point_Tests::increment(A a, B b, C c)
{
	incrementer_type oper;
	test_header("increment", a, b, c, "a", "b", "c");
	test_bank(a, oper, "++", "a");
	test_bank(b, oper, "++", "b");
	test_bank(c, oper, "++", "c");
	os_nline;
}

struct decrementer_type
{
	template <typename A>
	auto operator()(A a) -> decltype(a--)
	{
		return (--a);
	}
};

template <typename A>
void Fixed_Point_Tests::decrement(A a)
{
	decrementer_type oper;
	test_header("decrement", a, "a");
	test_bank(a, oper, "--", "a");
	os_nline;
}

template <typename A, typename B>
void Fixed_Point_Tests::decrement(A a, B b)
{
	decrementer_type oper;
	test_header("decrement", a, b, "a", "b");
	test_bank(a, oper, "--", "a");
	test_bank(b, oper, "--", "b");
	os_nline;
}

template <typename A, typename B, typename C>
void Fixed_Point_Tests::decrement(A a, B b, C c)
{
	decrementer_type oper;
	test_header("decrement", a, b, c, "a", "b", "c");
	test_bank(a, oper, "--", "a");
	test_bank(b, oper, "--", "b");
	test_bank(c, oper, "--", "c");
	os_nline;
}


struct maximum_type
{
	template <typename A, typename B>
	auto operator()(const A& a, const B& b) -> decltype(std::max(a,b))
	{
		return std::max(a, b);
	}
};

template <typename A, typename B>
void Fixed_Point_Tests::maximum(A a, B b)
{
	maximum_type oper;
	test_header("maximum", a, b, "a", "b");
	test_bank(a, b, oper, "max", "a", "b");
	os_nline;
}

template <typename A, typename B, typename C>
void Fixed_Point_Tests::maximum(A a, B b, C c)
{
	maximum_type oper;
	test_header("maximum", a, b, c, "a", "b", "c");
	test_bank(a, b, c, oper, "max", "a", "b", "c");
	os_nline;
}


struct minimum_type
{
	template <typename A, typename B>
	auto operator()(const A& a, const B& b) -> decltype(std::min(a,b))
	{
		return std::min(a, b);
	}
};

template <typename A, typename B>
void Fixed_Point_Tests::minimum(A a, B b)
{
	minimum_type oper;
	test_header("minimum", a, b, "a", "b");
	test_bank(a, b, oper, "min", "a", "b");
	os_nline;
}

template <typename A, typename B, typename C>
void Fixed_Point_Tests::minimum(A a, B b, C c)
{
	minimum_type oper;
	test_header("minimum", a, b, c, "a", "b", "c");
	test_bank(a, b, c, oper, "min", "a", "b", "c");
	os_nline;
}


struct comp_equal_type
{
	template <typename A, typename B>
	auto operator()(const A& a, const B& b) -> decltype(a==b)
	{
		return a == b;
	}
};

template <typename A, typename B>
void Fixed_Point_Tests::comp_equal(A a, B b)
{
	comp_equal_type oper;
	test_header("comp_equal", a, b, "a", "b");
	test_bank(a, b, oper, "==", "a", "b");
	os_nline;
}

template <typename A, typename B, typename C>
void Fixed_Point_Tests::comp_equal(A a, B b, C c)
{
	comp_equal_type oper;
	test_header("comp_equal", a, b, c, "a", "b", "c");
	test_bank(a, b, c, oper, "==", "a", "b", "c");
	os_nline;
}


struct comp_high_type
{
	template <typename A, typename B>
	auto operator()(const A& a, const B& b) -> decltype(a>b)
	{
		return a > b;
	}
};

template <typename A, typename B>
void Fixed_Point_Tests::comp_high(A a, B b)
{
	comp_high_type oper;
	test_header("comp_high", a, b, "a", "b");
	test_bank(a, b, oper, ">", "a", "b");
	os_nline;
}

template <typename A, typename B, typename C>
void Fixed_Point_Tests::comp_high(A a, B b, C c)
{
	comp_high_type oper;
	test_header("comp_high", a, b, c, "a", "b", "c");
	test_bank(a, b, c, oper, ">", "a", "b", "c");
	os_nline;
}


struct comp_high_or_equal_type
{
	template <typename A, typename B>
	auto operator()(const A& a, const B& b) -> decltype(a>=b)
	{
		return a >= b;
	}
};

template <typename A, typename B>
void Fixed_Point_Tests::comp_high_or_equal(A a, B b)
{
	comp_high_or_equal_type oper;
	test_header("comp_high_or_equal", a, b, "a", "b");
	test_bank(a, b, oper, ">=", "a", "b");
	os_nline;
}

template <typename A, typename B, typename C>
void Fixed_Point_Tests::comp_high_or_equal(A a, B b, C c)
{
	comp_high_or_equal_type oper;
	test_header("comp_high_or_equal", a, b, c, "a", "b", "c");
	test_bank(a, b, c, oper, ">=", "a", "b", "c");
	os_nline;
}


struct comp_low_type
{
	template <typename A, typename B>
	auto operator()(const A& a, const B& b) -> decltype(a<b)
	{
		return a < b;
	}
};

template <typename A, typename B>
void Fixed_Point_Tests::comp_low(A a, B b)
{
	comp_low_type oper;
	test_header("comp_low", a, b, "a", "b");
	test_bank(a, b, oper, "<", "a", "b");
	os_nline;
}

template <typename A, typename B, typename C>
void Fixed_Point_Tests::comp_low(A a, B b, C c)
{
	comp_low_type oper;
	test_header("comp_low", a, b, c, "a", "b", "c");
	test_bank(a, b, c, oper, "<", "a", "b", "c");
	os_nline;
}


struct comp_low_or_equal_type
{
	template <typename A, typename B>
	auto operator()(const A& a, const B& b) -> decltype(a<=b)
	{
		return a <= b;
	}
};

template <typename A, typename B>
void Fixed_Point_Tests::comp_low_or_equal(A a, B b)
{
	comp_low_or_equal_type oper;
	test_header("comp_low_or_equal", a, b, "a", "b");
	test_bank(a, b, oper, "<=", "a", "b");
	os_nline;
}

template <typename A, typename B, typename C>
void Fixed_Point_Tests::comp_low_or_equal(A a, B b, C c)
{
	comp_low_or_equal_type oper;
	test_header("comp_low_or_equal", a, b, c, "a", "b", "c");
	test_bank(a, b, c, oper, "<=", "a", "b", "c");
	os_nline;
}

#endif /* FIXED_POINT_TESTS_HXX_ */
