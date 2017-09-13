#ifndef FIXED_POINT_UTILS_HPP_
#define FIXED_POINT_UTILS_HPP_

#include <stdint.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <bitset>
#include <cmath>
#include <type_traits>

namespace FP_numeric
{
	// TODO : manage other arithmetic types cases
	enum  Arithmetic_type {SIGNED_SATURATED};//, UNSIGNED_SATURATED, CYCLIC};

	// return the given value val saturated (if needed) between min and max in function of the Arithmetic_type
	template <typename T>
	T saturate(const T val, const T min, const T max, const FP_numeric::Arithmetic_type ar_type);

	// return the given value val saturated (if needed) at max in function of the Arithmetic_type
	template <typename T>
	T saturate_max(const T val, const T max, const FP_numeric::Arithmetic_type ar_type);

	// return the given value val saturated (if needed) at min in function of the Arithmetic_type
	template <typename T>
	T saturate_min(const T val, const T min, const FP_numeric::Arithmetic_type ar_type);

	// return the value of data with the point set at the new_fract_bits position that is originally set at old_fract_bits
	template <typename T>
	T shift_fract(const T data, const size_t new_fract_bits, const size_t old_fract_bits);

	// return the given value on its T type and its given quantification but with its sign extended on the most
	// significant bits of T
	template <typename T>
	T sign_extension(T data, const size_t total_bits);

	template <typename FP>
	std::string display(const FP& n, int precision);

	template <typename FP>
	std::string display_quantification(const FP& n);

	//display resume of this fixed point
	template <typename FP>
	std::string       resume(const FP& n);

	template <typename FP>
	std::string short_resume(const FP& n);

}

#include "Fixed_Point_utils.hxx"

#endif /* FIXED_POINT_UTILS_HPP_ */
