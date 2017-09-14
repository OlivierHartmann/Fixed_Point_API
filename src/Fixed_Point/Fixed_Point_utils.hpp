#ifndef FIXED_POINT_UTILS_HPP_
#define FIXED_POINT_UTILS_HPP_

#include <stdint.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <cmath>
#include <type_traits>

#if defined(__GNUC__) && defined(__x86_64__)

	namespace std
	{
		string to_string( __int128_t __val);
		string to_string(__uint128_t __val);

	}

	template<class stream_class> stream_class& operator<<(stream_class& s,  __int128_t __val);
	template<class stream_class> stream_class& operator<<(stream_class& s, __uint128_t __val);
#endif

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

	template <typename T>
	std::string to_bit(const T data);

	template <typename T>
	std::string to_bit(const T data, const size_t n_bits);

	template <typename T>
	std::string to_hex(const T data);

	template <typename T>
	std::string to_hex(const T data, const size_t n_bits);


	template<size_t T>
	struct matching_type
	{
		using type = typename std::conditional<T<=sizeof(int8_t)*8, int8_t,
	                                   typename std::conditional<T<=sizeof(int16_t)*8, int16_t,
	                                   typename std::conditional<T<=sizeof(int32_t)*8, int32_t,
	                                   typename std::conditional<T<=sizeof(int64_t)*8, int64_t,
	                               #if defined(__GNUC__) && defined(__x86_64__)
	                                   typename std::conditional<T<=sizeof(__int128_t)*8, __int128_t, void>::type
	                               #else
	                                   void
	                               #endif
	                                   >::type
	                                   >::type
	                                   >::type
	                                 >::type;
	};

	template<size_t T>
	struct next_type_from_size
	{
		using type = typename std::conditional<std::is_void<typename matching_type<T*2>::type>::value,
		                                       typename matching_type<T>::type, // no type above
		                                       typename matching_type<T*2>::type>::type;
	};

	template<typename T>
	struct next_type
	{
		using type = typename next_type_from_size<sizeof(T)*8>::type;
	};

	template<typename T1, typename T2>
	struct biggest_type
	{
		using type = typename std::conditional<sizeof(T1) >= sizeof(T2), T1, T2>::type;
	};
}

#include "Fixed_Point_utils.hxx"

#endif /* FIXED_POINT_UTILS_HPP_ */
