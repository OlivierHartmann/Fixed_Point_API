#ifndef FIXED_POINT_STATIC_HPP_
#define FIXED_POINT_STATIC_HPP_

#include <stdint.h>
#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include <iomanip>
#include <type_traits>
#include <cmath>
#include <limits>

#include "Fixed_Point_utils.hpp"

/// Fixed_Point_Static class ******************************************************************************************
template <size_t T, size_t F>
class Fixed_Point_Static
{
public:
	using base_type = typename FP_numeric::matching_type<T>::type;

	static_assert(!std::is_void<base_type>::value, "Error in Fixed_Point_Static : T > "
	                                 #if defined(__GNUC__) && defined(__x86_64__)
	                                        "128"
	                                 #else
	                                        "64"
	                                 #endif
	                                        " bits!"
	);

	static_assert(T > 0, "Error in Fixed_Point_Static : T == 0!");
	static_assert(T > F, "Error in Fixed_Point_Static : T <= F!"); // TODO: if signed, else can be ==

	static const size_t Tb       = T;
	static const size_t Fb       = F;
	static const size_t capacity = sizeof(base_type)*8; // maximum size of T before needing to rise the size of base_type

private: //given
	static const size_t __total_bits      = Tb;
	static const size_t __fractional_bits = Fb;

	static const FP_numeric::Arithmetic_type __ar_type  = FP_numeric::Arithmetic_type::SIGNED_SATURATED;

private: //automatically calculated
	static const base_type __one             =      base_type(1)  << __fractional_bits;
	static const size_t    __integer_bits    = __total_bits - __fractional_bits - ((__ar_type == FP_numeric::Arithmetic_type::SIGNED_SATURATED)?1:0);
	static const base_type __fractional_mask =  ~((~base_type(0)) << __fractional_bits);
	static const base_type __integer_mask    = (~((~base_type(0)) << __integer_bits   )) << __fractional_bits;
	static const base_type __max_            = __integer_mask | __fractional_mask;
	static const base_type __min_            = ((__ar_type == FP_numeric::Arithmetic_type::SIGNED_SATURATED)? -__max_ : 0); // symmetric value range dynamic
	static const base_type __number_mask     = ((__ar_type == FP_numeric::Arithmetic_type::SIGNED_SATURATED)? ((__integer_mask | __fractional_mask) << 1) +1
	                                                                                                        :  (__integer_mask | __fractional_mask));

	//static_assert(__integer_bits >  0, "Error in Fixed_Point_Static : __integer_bits == 0!");
	static_assert(__integer_bits <= T, "Error in Fixed_Point_Static : __integer_bits > T!" );


private : //the data
	base_type __data;

public: // constructors
	Fixed_Point_Static();

	template <typename type> Fixed_Point_Static(type data);

	virtual ~Fixed_Point_Static();

public:
	// return the given value val saturated (if needed) with the configuration of this Fixed_Point_Static
	template <typename type> base_type saturate(const type val) const;

	// return the __data value of this Fixed_Point_Static with the point set at the new_fract_bits position
	base_type shift_fract(const size_t new_fract_bits) const;

public: // relational operators : TODO : manage risks of overflow in extrem cases
	template <size_t To, size_t Fo> bool operator> (const Fixed_Point_Static<To, Fo>& o) const;
	template <size_t To           > bool operator> (const Fixed_Point_Static<To, F >& o) const;
	template <size_t To, size_t Fo> bool operator< (const Fixed_Point_Static<To, Fo>& o) const;
	template <size_t To, size_t Fo> bool operator<=(const Fixed_Point_Static<To, Fo>& o) const;
	template <size_t To, size_t Fo> bool operator>=(const Fixed_Point_Static<To, Fo>& o) const;

	template <typename type> bool operator> (const type n) const;
	template <typename type> bool operator< (const type n) const;
	template <typename type> bool operator<=(const type n) const;
	template <typename type> bool operator>=(const type n) const;

	template <size_t To, size_t Fo> bool operator==(const Fixed_Point_Static<To, Fo>& o) const;
	template <size_t To           > bool operator==(const Fixed_Point_Static<To, F >& o) const;
	template <size_t To, size_t Fo> bool operator!=(const Fixed_Point_Static<To, Fo>& o) const;

	template <typename type> bool operator==(const type n) const;
	template <typename type> bool operator!=(const type n) const;

	bool operator! ( ) const; // true if the this fixed point is null

	// compare the quantification of this Fixed_Point_Static with the given one
	bool same_quantification(size_t total_bits, size_t fractional_bits) const;
	template <size_t To, size_t Fo>
	bool same_quantification(const Fixed_Point_Static<To, Fo>& o      ) const;

public: // binary operators: they do not care about the quantification of the number in __data
	Fixed_Point_Static<T,F>  operator~  ( ) const;

	template <size_t To, size_t Fo> Fixed_Point_Static<T,F>& operator&= (const Fixed_Point_Static<To, Fo>& n);
	template <size_t To, size_t Fo> Fixed_Point_Static<T,F>& operator|= (const Fixed_Point_Static<To, Fo>& n);
	template <size_t To, size_t Fo> Fixed_Point_Static<T,F>& operator^= (const Fixed_Point_Static<To, Fo>& n);

	template <typename type> Fixed_Point_Static<T,F>& operator&= (const type n);
	template <typename type> Fixed_Point_Static<T,F>& operator|= (const type n);
	template <typename type> Fixed_Point_Static<T,F>& operator^= (const type n);

	template <size_t To, size_t Fo>	Fixed_Point_Static<T,F>& operator>>=(const Fixed_Point_Static<To, Fo>& n);
	template <size_t To, size_t Fo>	Fixed_Point_Static<T,F>& operator<<=(const Fixed_Point_Static<To, Fo>& n);


	template <typename integral_t>
	typename std::enable_if <std::is_integral<integral_t>::value, Fixed_Point_Static<T,F>>::type&
	operator>>=(const integral_t n);

	template <typename integral_t>
	typename std::enable_if <std::is_integral<integral_t>::value, Fixed_Point_Static<T,F>>::type&
	operator<<=(const integral_t n);

	template <typename floating_t>
	typename std::enable_if <std::is_floating_point<floating_t>::value, Fixed_Point_Static<T,F>>::type&
	operator>>=(const floating_t n);

	template <typename floating_t>
	typename std::enable_if <std::is_floating_point<floating_t>::value, Fixed_Point_Static<T,F>>::type&
	operator<<=(const floating_t n);

public: // basic math operators
	const Fixed_Point_Static<T,F>& operator+ () const;
	      Fixed_Point_Static<T,F>  operator- () const;

	// if __total_bits is null then takes the quantification of the result of the operation
	Fixed_Point_Static<T,F>& operator++ (   );
	Fixed_Point_Static<T,F>& operator-- (   );
	Fixed_Point_Static<T,F>& operator++ (int);
	Fixed_Point_Static<T,F>& operator-- (int);

	// if __total_bits is null then takes the quantification of the result of the operation
	template <typename type> Fixed_Point_Static<T,F>& operator+= (const type n);
	template <typename type> Fixed_Point_Static<T,F>& operator-= (const type n);
	template <typename type> Fixed_Point_Static<T,F>& operator*= (const type n);
	template <typename type> Fixed_Point_Static<T,F>& operator/= (const type n);
	template <typename type> Fixed_Point_Static<T,F>& operator%= (const type n);

public: // math functions
	// check if this fixed point is (positive or null) or is (strictly positive)
	bool is_pos () const;
	bool is_spos() const;

	// check if this fixed point is (negative or null) or (strictly negative)
	bool is_neg () const;
	bool is_sneg() const;

	// check if this fixed point equals zero, or one, or if it's saturated at max or min
	bool is_zero() const;
	bool is_one () const;
	bool is_min () const;
	bool is_max () const;
	bool is_sat () const;

	Fixed_Point_Static<T,F> abs () const;
	int                     sign() const; // return 0 if positive else (-1)

public: // conversion to common types
	//convert properly __data taking into account the comma position
	template <typename uintegral_t>
	typename std::enable_if< std::is_integral<uintegral_t>::value && std::is_unsigned<uintegral_t>::value, uintegral_t>::type
	convert() const; // to unsigned integer

	template <typename sintegral_t>
	typename std::enable_if< std::is_integral<sintegral_t>::value && std::is_signed<sintegral_t>::value, sintegral_t>::type
	convert() const; // to signed integer

	template <typename floating_t>
	typename std::enable_if< std::is_floating_point<floating_t>::value, floating_t>::type
	convert() const; // to floating point number

	// directly cast __data to the given type
	template <typename type>     type cast() const;
	template <typename type> operator type() const; // static cast

	base_type to_raw   () const;
	  int32_t to_int   () const;
	 uint32_t to_uint  () const;
	  int64_t to_lint  () const;
	 uint64_t to_luint () const;
	    float to_float () const;
	   double to_double() const;

	// return a binary representation of the data value.
	// to_bin  return a sizeof(base_type)*8 bits length number
	// to_sbin return a __total_bits bits length number
	std::string  to_bin () const;
	std::string  to_sbin() const;

	// return a hexdecimal representation of the data value.
	// to_hex  return a sizeof(base_type)*2 hexadecimal length number
	// to_shex return a ceil(__total_bits/4) hexadecimal length number
	std::string  to_hex () const;
	std::string  to_shex() const;

public: // basic getters
	size_t    get_integer_bits   () const;
	size_t    get_fractional_bits() const;
	size_t    get_total_bits     () const;
	base_type get_number_mask    () const;
	base_type get_fractional_mask() const;
	base_type get_integer_mask   () const;
	base_type get_one            () const;
	base_type get_max            () const;
	base_type get_min            () const;
	base_type get_data           () const;

	FP_numeric::Arithmetic_type get_arithmetic_type() const;

	// return min, max, one or zero values with the quantification of this fixed point
	// if there is none then return an empty Fixed_Point_Static
	static constexpr Fixed_Point_Static<T,F> min ();
	static constexpr Fixed_Point_Static<T,F> max ();
	static constexpr Fixed_Point_Static<T,F> one ();
	static constexpr Fixed_Point_Static<T,F> zero();

public: // setters
	// the following methods and operators change only the data value in function of the already set quantification
	template <typename integral_t>
	typename std::enable_if <std::is_integral<integral_t>::value, Fixed_Point_Static<T,F>>::type&
	operator=(const integral_t& o);

	template <typename floating_t>
	typename std::enable_if <std::is_floating_point<floating_t>::value, Fixed_Point_Static<T,F>>::type&
	operator=(const floating_t& o);

	template <size_t To, size_t Fo>
	Fixed_Point_Static<T,F>&
	operator=(const Fixed_Point_Static<To,Fo>& o);

	// the following methods and operators change only the data value with an original quantification
	// in function of the already set quantification (manage the possible need of a sign extension with the input data)
	template <typename type>
	void
	change_data(const type data);

	template <typename integral_t>
	typename std::enable_if <std::is_integral<integral_t>::value, void>::type
	change_data(      integral_t data, size_t data_total_bits, size_t data_fractional_bits);

	template <typename floating_t>
	typename std::enable_if <std::is_floating_point<floating_t>::value, void>::type&
	change_data(const floating_t data, size_t data_total_bits, size_t data_fractional_bits);

	                                void change_data(const Fixed_Point_Static<T ,F >& o);
	template <size_t To           > void change_data(const Fixed_Point_Static<To,F >& o);
	template <size_t To, size_t Fo> void change_data(const Fixed_Point_Static<To,Fo>& o);

	// the following methods set the given argument as new quantification OR arithmetic type OR data
	// without touching the others
	template <typename integer_t  > void set_data(integer_t data                     );
	template <size_t To, size_t Fo> void set_data(const Fixed_Point_Static<To, Fo>& o);

	                                void set_arithmetic_type(FP_numeric::Arithmetic_type ar_type);
	template <size_t To, size_t Fo>	void set_arithmetic_type(const Fixed_Point_Static<To, Fo>& o);

public: // display methods
	std::string display(int precision) const;

	std::string display_quantification() const;

	//display resume of this fixed point
	std::string       resume() const;
	std::string short_resume() const;
};

/// ********************************************************************************************************************
/// Tools to get the type of an operation result ***********************************************************************
/// ********************************************************************************************************************

namespace FP_numeric
{
	// TODO: take care of the arithmetic type in the computation of the result size
	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	struct add_res_type_d;

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	struct sub_res_type_d;

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	struct mul_res_type_d;

	template <size_t Tn, size_t Fn, size_t Td, size_t Fd>
	struct div_res_type_d;

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	struct mod_res_type_d;

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	struct comp_res_type_d;


	template <typename FPl, typename FPr>
	struct add_res_type;

	template <typename FPl, typename FPr>
	struct sub_res_type;

	template <typename FPl, typename FPr>
	struct mul_res_type;

	template <typename FPl, typename FPr>
	struct div_res_type;

	template <typename FPl, typename FPr>
	struct mod_res_type;

	template <typename FPl, typename FPr>
	struct comp_res_type;
}

template <size_t T, size_t F>
using FP_S = Fixed_Point_Static<T,F>;

#include "Fixed_Point_Static.hxx"
#include "Fixed_Point_Static_operators.hpp"

#endif // FIXED_POINT_STATIC_HPP_
