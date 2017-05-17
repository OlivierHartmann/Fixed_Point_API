#ifndef FIXED_POINT_STATIC_HPP_
#define FIXED_POINT_STATIC_HPP_

#include <stdint.h>
#include <iostream>
#include <exception>
#include <string>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <type_traits>
#include <cmath>
#include <limits>

#include "Fixed_Point_Utils.hpp"

template <size_t T, size_t F>
class Fixed_Point_Static;

/// *********************************************************************************************************************
/// Tools to get the type of an operation result*************************************************************************
/// *********************************************************************************************************************

namespace FP_numeric
{
	// TODO: take care of the arithmetic type in the computation of the result size
	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	struct add_res_type_d
	{
		using type = typename std::conditional<(Tl - Fl) >= (Tr - Fr), // compare integral part
		                                      typename std::conditional<Fl >= Fr, Fixed_Point_Static<Tl+1,       Fl>,
		                                                                          Fixed_Point_Static<Tl-Fl+Fr+1, Fr>>::type,
		                                      typename std::conditional<Fl >= Fr, Fixed_Point_Static<Tr-Fr+Fl+1, Fl>,
		                                                                          Fixed_Point_Static<Tr+1,       Fr>>::type
		                                      >::type;
	};

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	struct sub_res_type_d
	{
		using type = typename std::conditional<(Tl - Fl) >= (Tr - Fr), // compare integral part
		                                      typename std::conditional<Fl >= Fr, Fixed_Point_Static<Tl+1,       Fl>,
		                                                                          Fixed_Point_Static<Tl-Fl+Fr+1, Fr>>::type,
		                                      typename std::conditional<Fl >= Fr, Fixed_Point_Static<Tr-Fr+Fl+1, Fl>,
		                                                                          Fixed_Point_Static<Tr+1,       Fr>>::type
		                                      >::type;
	};

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	struct mul_res_type_d
	{
		using type = Fixed_Point_Static<Tl+Tr-1, Fl+Fr>; // "-1" if SIGNED_SATURATED
	};

	template <size_t Tn, size_t Fn, size_t Td, size_t Fd>
	struct div_res_type_d
	{
		using type = Fixed_Point_Static<Tn+Td+1, Fn+Td-Fd>; // I = Tn-Fn+Fd+1 ; F = Fn+Td-Fd ; T = I+F = Tn+Td+1
	};

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	struct mod_res_type_d
	{
		using type = typename std::conditional<(Tl - Fl) >= (Tr - Fr), // compare integral part
		                                      typename std::conditional<Fl >= Fr, Fixed_Point_Static<Tl+1,       Fl>,
		                                                                          Fixed_Point_Static<Tl-Fl+Fr+1, Fr>>::type,
		                                      typename std::conditional<Fl >= Fr, Fixed_Point_Static<Tr-Fr+Fl+1, Fl>,
		                                                                          Fixed_Point_Static<Tr+1,       Fr>>::type
		                                      >::type;
	};

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	struct max_res_type_d
	{
		using type = typename std::conditional<(Tl - Fl) >= (Tr - Fr), // compare integral part
		                                      typename std::conditional<Fl >= Fr, Fixed_Point_Static<Tl,       Fl>,
		                                                                          Fixed_Point_Static<Tl-Fl+Fr, Fr>>::type,
		                                      typename std::conditional<Fl >= Fr, Fixed_Point_Static<Tr-Fr+Fl, Fl>,
		                                                                          Fixed_Point_Static<Tr,       Fr>>::type
		                                      >::type;
	};

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	struct min_res_type_d
	{
		using type = typename std::conditional<(Tl - Fl) >= (Tr - Fr), // compare integral part
		                                      typename std::conditional<Fl >= Fr, Fixed_Point_Static<Tl,       Fl>,
		                                                                          Fixed_Point_Static<Tl-Fl+Fr, Fr>>::type,
		                                      typename std::conditional<Fl >= Fr, Fixed_Point_Static<Tr-Fr+Fl, Fl>,
		                                                                          Fixed_Point_Static<Tr,       Fr>>::type
		                                      >::type;
	};


	template <typename FPl, typename FPr>
	struct add_res_type
	{
		using type = typename add_res_type_d<FPl::Tb, FPl::Fb, FPr::Tb, FPr::Fb>::type;
	};

	template <typename FPl, typename FPr>
	struct sub_res_type
	{
		using type = typename sub_res_type_d<FPl::Tb, FPl::Fb, FPr::Tb, FPr::Fb>::type;
	};

	template <typename FPl, typename FPr>
	struct mul_res_type
	{
		using type = typename mul_res_type_d<FPl::Tb, FPl::Fb, FPr::Tb, FPr::Fb>::type;
	};

	template <typename FPl, typename FPr>
	struct div_res_type
	{
		using type = typename div_res_type_d<FPl::Tb, FPl::Fb, FPr::Tb, FPr::Fb>::type;
	};

	template <typename FPl, typename FPr>
	struct mod_res_type
	{
		using type = typename mod_res_type_d<FPl::Tb, FPl::Fb, FPr::Tb, FPr::Fb>::type;
	};

	template <typename FPl, typename FPr>
	struct max_res_type
	{
		using type = typename max_res_type_d<FPl::Tb, FPl::Fb, FPr::Tb, FPr::Fb>::type;
	};

	template <typename FPl, typename FPr>
	struct min_res_type
	{
		using type = typename min_res_type_d<FPl::Tb, FPl::Fb, FPr::Tb, FPr::Fb>::type;
	};
}

/// Fixed_Point_Static class ******************************************************************************************
template <size_t T, size_t F>
class Fixed_Point_Static
{
public:
	using base_type = typename std::conditional<T<=sizeof(int8_t)*8, int8_t,
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

	static_assert(!std::is_void<base_type>::value, "Error in Fixed_Point_Static : T > "
	                                 #if defined(__GNUC__) && defined(__x86_64__)
	                                        "128"
	                                 #else
	                                        "64"
	                                 #endif
	                                        " bits!"
	);

	static_assert(T >  0, "Error in Fixed_Point_Static : T == 0!");
	static_assert(T >= F, "Error in Fixed_Point_Static : T < F!");

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
	static const base_type __min_            = ((__ar_type == FP_numeric::Arithmetic_type::SIGNED_SATURATED)? -__max_ : 0);
	static const base_type __number_mask     = ((__ar_type == FP_numeric::Arithmetic_type::SIGNED_SATURATED)? ((__integer_mask | __fractional_mask) << 1) +1
	                                                                                                        :  (__integer_mask | __fractional_mask));

	//static_assert(__integer_bits >  0, "Error in Fixed_Point_Static : __integer_bits == 0!");
	static_assert(__integer_bits <= T, "Error in Fixed_Point_Static : __integer_bits > T!" );


private : //the data
	base_type __data;

public: // constructors
	Fixed_Point_Static()
		: __data    (base_type(0))
		{ }
	template <typename type>
	Fixed_Point_Static(type data)
		{ change_data(data); }

	virtual ~Fixed_Point_Static(){}

public:
	// return the given value val saturated (if needed) with the configuration of this Fixed_Point_Static
	template <typename type>
	inline type saturate(const type val) const
	{
		return FP_numeric::saturate(val, (type)__min_, (type)__max_, __ar_type);
	}

	// return the __data value of this Fixed_Point_Static with the point set at the new_fract_bits position
	inline base_type shift_fract(const size_t new_fract_bits) const
	{
		return FP_numeric::shift_fract(__data, new_fract_bits, __fractional_bits);
	}
	inline base_type shift_fract(const size_t new_fract_bits, const size_t old_fract_bits) const
	{
		return FP_numeric::shift_fract(__data, new_fract_bits, old_fract_bits);
	}

public: // relational operators

	template <size_t To, size_t Fo>
	inline bool operator> (const Fixed_Point_Static<To, Fo>& o) const
	{
		using comp_type = typename FP_numeric::max_res_type_d<T, F, To, Fo>::type;
		using big_type  = typename comp_type::base_type;

		big_type t_d = FP_numeric::shift_fract(big_type(__data      ), comp_type::Fb, F );
		big_type o_d = FP_numeric::shift_fract(big_type(o.get_data()), comp_type::Fb, Fo);
		return (t_d > o_d);
	}
	template <size_t To>
	inline bool operator> (const Fixed_Point_Static<To, F >& o) const
	{
		return (__data > o.get_data());
	}
	template <size_t To, size_t Fo>
	inline bool operator< (const Fixed_Point_Static<To, Fo>& o) const { return       o > *this;}
	template <size_t To, size_t Fo>
	inline bool operator<=(const Fixed_Point_Static<To, Fo>& o) const { return !(*this > o);   }
	template <size_t To, size_t Fo>
	inline bool operator>=(const Fixed_Point_Static<To, Fo>& o) const { return !(*this < o);   }

	// TODO : manage risk of overflow
	template <typename type>
	inline bool operator> (const type n) const { Fixed_Point_Static<T,F> t(n); return *this >  t;}
	template <typename type>
	inline bool operator< (const type n) const { Fixed_Point_Static<T,F> t(n); return *this <  t;}
	template <typename type>
	inline bool operator<=(const type n) const { Fixed_Point_Static<T,F> t(n); return *this <= t;}
	template <typename type>
	inline bool operator>=(const type n) const { Fixed_Point_Static<T,F> t(n); return *this >= t;}

	template <size_t To, size_t Fo>
	inline bool operator==(const Fixed_Point_Static<To, Fo>& o) const
	{
		using comp_type = typename FP_numeric::max_res_type_d<T, F, To, Fo>::type;
		using big_type  = typename comp_type::base_type;

		big_type t_d = FP_numeric::shift_fract(big_type(__data      ), comp_type::Fb, F );
		big_type o_d = FP_numeric::shift_fract(big_type(o.get_data()), comp_type::Fb, Fo);
		return (t_d == o_d);

//		// can't align fractional parts to compare directly because of the risk of overflow
//		// so compare first integer part then fractional part
//		const big_type this_i =     __data   >>     __fractional_bits;
//		const big_type o_i    = o.get_data() >> o.get_fractional_bits();
//
//		if(this_i != o_i)
//			return false;
//
//		int64_t   f_diff = __fractional_bits - o.get_fractional_bits();
//		base_type this_f = __data, o_f = o.get_data();
//
//		if(f_diff < 0)
//			this_f <<= -f_diff;
//		else
//			o_f    <<=  f_diff;
//
//		return (this_f == o_f);
	}
	template <size_t To>
	inline bool operator==(const Fixed_Point_Static<To, F >& o) const
	{
		return (__data == o.get_data());
	}
	template <size_t To, size_t Fo>
	inline bool operator!=(const Fixed_Point_Static<To, Fo>& o) const { return !(*this == o);}

	template <typename type>
	inline bool operator==(const type n) const { Fixed_Point_Static<T, F> t(n); return *this == t;}
	template <typename type>
	inline bool operator!=(const type n) const { Fixed_Point_Static<T, F> t(n); return *this != t;}

	inline bool operator! ( ) const { return !__data;}

	// compare the quantification of this Fixed_Point_Static with the given one
	inline bool same_quantification(size_t total_bits, size_t fractional_bits) const
	{
		return (total_bits == __total_bits && fractional_bits == __fractional_bits);
	}
	template <size_t To, size_t Fo>
	inline bool same_quantification(const Fixed_Point_Static<To, Fo>& o      ) const
	{
		return std::is_same<Fixed_Point_Static<T, F>,Fixed_Point_Static<To, Fo>>::value;
	}

public: // binary operators: they do not care about the quantification of the number in __data
	inline Fixed_Point_Static<T,F>  operator~  ( ) const
	{
		Fixed_Point_Static<T,F> t(*this);
		t.__data = ~t.__data;
		return t;
	}

	template <size_t To, size_t Fo>
	inline Fixed_Point_Static<T,F>& operator&= (const Fixed_Point_Static<To, Fo>& n) { __data &= n.get_data(); return *this;}
	template <size_t To, size_t Fo>
	inline Fixed_Point_Static<T,F>& operator|= (const Fixed_Point_Static<To, Fo>& n) { __data |= n.get_data(); return *this;}
	template <size_t To, size_t Fo>
	inline Fixed_Point_Static<T,F>& operator^= (const Fixed_Point_Static<To, Fo>& n) { __data ^= n.get_data(); return *this;}

	template <typename type>
	inline Fixed_Point_Static<T,F>& operator&= (const type n) { __data &= (base_type)n; return *this;}
	template <typename type>
	inline Fixed_Point_Static<T,F>& operator|= (const type n) { __data |= (base_type)n; return *this;}
	template <typename type>
	inline Fixed_Point_Static<T,F>& operator^= (const type n) { __data ^= (base_type)n; return *this;}

	template <size_t To, size_t Fo>
	inline Fixed_Point_Static<T,F>& operator>>=(const Fixed_Point_Static<To, Fo>& n)
	{
		if(n.is_pos())
			__data >>=  n.to_int();
		else
		{
			__data <<= -n.to_int();
			__data = saturate(__data);
		}

		return *this;
	}
	template <size_t To, size_t Fo>
	inline Fixed_Point_Static<T,F>& operator<<=(const Fixed_Point_Static<To, Fo>& n)
	{
		if(n.is_pos())
		{
			__data <<=  n.to_int();
			__data = saturate(__data);
		}
		else
			__data >>= -n.to_int();

		return *this;
	}


	template <typename integral_t>
	inline typename std::enable_if <std::is_integral<integral_t>::value, Fixed_Point_Static<T,F>>::type&
	operator>>=(const integral_t n) { __data >>= n;                            return *this;}
	template <typename integral_t>
	inline typename std::enable_if <std::is_integral<integral_t>::value, Fixed_Point_Static<T,F>>::type&
	operator<<=(const integral_t n) { __data <<= n; __data = saturate(__data); return *this;}
	template <typename floating_t>
	inline typename std::enable_if <std::is_floating_point<floating_t>::value, Fixed_Point_Static<T,F>>::type&
	operator>>=(const floating_t n) {  *this >>= ((n>0)? (int)n : -(int)n);    return *this;}
	template <typename floating_t>
	inline typename std::enable_if <std::is_floating_point<floating_t>::value, Fixed_Point_Static<T,F>>::type&
	operator<<=(const floating_t n) {  *this <<= ((n>0)? (int)n : -(int)n);    return *this;}

public: // basic math operators
	inline const Fixed_Point_Static<T,F>& operator+ () const { return *this;}
	inline       Fixed_Point_Static<T,F>  operator- () const
	{
		Fixed_Point_Static<T,F> t(*this);
		t.__data = -t.__data;
		return t;
	}

	// if __total_bits is null then takes the quantification of the result of the operation
	inline Fixed_Point_Static<T,F>& operator++ (   ) { *this += one(); return *this;}
	inline Fixed_Point_Static<T,F>& operator-- (   ) { *this -= one(); return *this;}
	inline Fixed_Point_Static<T,F>& operator++ (int) { *this += one(); return *this;}
	inline Fixed_Point_Static<T,F>& operator-- (int) { *this -= one(); return *this;}

	// if __total_bits is null then takes the quantification of the result of the operation
	template <typename type>
	inline Fixed_Point_Static<T,F>& operator+= (const type n) { *this = (*this)+n; return *this;}
	template <typename type>
	inline Fixed_Point_Static<T,F>& operator-= (const type n) { *this = (*this)-n; return *this;}
	template <typename type>
	inline Fixed_Point_Static<T,F>& operator*= (const type n) { *this = (*this)*n; return *this;}
	template <typename type>
	inline Fixed_Point_Static<T,F>& operator/= (const type n) { *this = (*this)/n; return *this;}
	template <typename type>
	inline Fixed_Point_Static<T,F>& operator%= (const type n) { *this = (*this)%n; return *this;}

public: // math functions
	// check if this fixed point is (positive or null) or is (strictly positive)
	inline bool is_pos () const { return ( *this >= 0);}
	inline bool is_spos() const { return ( *this >  0);}

	// check if this fixed point is (negative or null) or (strictly negative)
	inline bool is_neg () const { return ( *this <= 0);}
	inline bool is_sneg() const { return ( *this <  0);}

	// check if this fixed point equals zero, or one, or if it's saturated at max or min
	inline bool is_zero() const { return   __data ==   0  ;   }
	inline bool is_one () const { return   __data == __one;   }
	inline bool is_min () const { return   __data == __min_;  }
	inline bool is_max () const { return   __data == __max_;  }
	inline bool is_sat () const { return is_max() || is_min();}

	inline Fixed_Point_Static<T,F> abs () const { return is_sneg() ? -(*this) : *this;}
	inline int                     sign() const { return is_sneg() ? -1       : 0;    }

public: // conversion to common types
	//convert properly __data taking into account the comma position
	template <typename uintegral_t>
	inline typename std::enable_if< std::is_integral<uintegral_t>::value && std::is_unsigned<uintegral_t>::value, uintegral_t>::type
	convert() const // to unsigned integer
	{
		if((__data & __fractional_mask) >> (__fractional_bits-1))
			return (uintegral_t)((saturate(__data + __one) & __number_mask) >> __fractional_bits); // rounded upper
		else
			return (uintegral_t)((__data & __number_mask) >> __fractional_bits); // rounded lower
	}
	template <typename sintegral_t>
	inline typename std::enable_if< std::is_integral<sintegral_t>::value && std::is_signed<sintegral_t>::value, sintegral_t>::type
	convert() const // to signed integer
	{
		if((__data & __fractional_mask) >> (__fractional_bits-1))
			return (sintegral_t)(saturate(__data + __one) >> __fractional_bits); // rounded upper
		else
			return (sintegral_t)(__data >> __fractional_bits); // rounded lower
	}
	template <typename floating_t>
	inline typename std::enable_if< std::is_floating_point<floating_t>::value, floating_t>::type
	convert() const // to floating point number
	{
		return static_cast<floating_t>(__data) / static_cast<floating_t>(__one);
	}

	// directly cast __data to the given type
	template <typename type>
	inline type    cast         () const { return (type)__data;}
	template <typename type>
	               operator type() const { return cast<type>();} // static cast

	inline base_type to_raw   () const { return __data;             }
	inline   int32_t to_int   () const { return convert< int32_t>();}
	inline  uint32_t to_uint  () const { return convert<uint32_t>();}
	inline   int64_t to_lint  () const { return convert< int64_t>();}
	inline  uint64_t to_luint () const { return convert<uint64_t>();}
	inline     float to_float () const { return convert<   float>();}
	inline    double to_double() const { return convert<  double>();}

	// return a binary representation of the data value.
	// to_bin  return a sizeof(base_type)*8 bits length number
	// to_sbin return a __total_bits bits length number
	inline std::string  to_bin () const
	{
		return std::bitset<sizeof(base_type)*8>(__data).to_string();
	}
	inline std::string  to_sbin() const
	{
		std::string str = to_bin();
		return str.substr(sizeof(base_type)*8 - __total_bits);
	}

	// return a hexdecimal representation of the data value.
	// to_hex  return a sizeof(base_type)*2 hexadecimal length number
	// to_shex return a ceil(__total_bits/4) hexadecimal length number
	inline std::string  to_hex () const
	{
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(sizeof(base_type)*2) << std::hex
		   << (static_cast<typename std::conditional<sizeof(base_type) == sizeof(char), int, base_type>::type>(__data));

		std::string str;
		ss >> str;

		return str;
	}
	inline std::string  to_shex() const
	{
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(std::ceil(__total_bits/4)) << std::hex
		   << (static_cast<typename std::conditional<sizeof(base_type) == sizeof(char), int, base_type>::type>(__data));

		std::string str;
		ss >> str;

		return str;
	}

public: // basic getters
	inline size_t    get_integer_bits   () const { return __integer_bits;   }
	inline size_t    get_fractional_bits() const { return __fractional_bits;}
	inline size_t    get_total_bits     () const { return __total_bits;     }
	inline base_type get_number_mask    () const { return __number_mask;    }
	inline base_type get_fractional_mask() const { return __fractional_mask;}
	inline base_type get_integer_mask   () const { return __integer_mask;   }
	inline base_type get_one            () const { return __one;            }
	inline base_type get_max            () const { return __max_;           }
	inline base_type get_min            () const { return __min_;           }
	inline base_type get_data           () const { return __data;           }
	inline FP_numeric::Arithmetic_type
	                 get_arithmetic_type() const { return __ar_type;        }

	// return min, max, one or zero values with the quantification of this fixed point
	// if there is none then return an empty Fixed_Point_Static
	static constexpr Fixed_Point_Static<T,F> min ()
	{
		Fixed_Point_Static<T,F> min;
		min.set_data(__min_);
		return min;
	}
	static constexpr Fixed_Point_Static<T,F> max ()
	{
		Fixed_Point_Static<T,F> max;
		max.set_data(__max_);
		return max;
	}
	static constexpr Fixed_Point_Static<T,F> one ()
	{
		Fixed_Point_Static<T,F> one;
		one.set_data(__one);
		return one;
	}
	static constexpr Fixed_Point_Static<T,F> zero()
	{
		Fixed_Point_Static<T,F> zero;
		zero.set_data((Fixed_Point_Static<T,F>::base_type)0);
		return zero;
	}

public: // setters
	// the following methods and operators change only the data value in function of the already set quantification
	template <typename integral_t>
	inline typename std::enable_if <std::is_integral<integral_t>::value, Fixed_Point_Static<T,F>>::type&
	operator=(const integral_t& o               )
	{
		// TODO : manage risk of overflow
		int64_t temp = (int64_t)o << __fractional_bits;
		__data = (base_type)saturate<int64_t>(temp);
		return *this;
	}
	template <typename floating_t>
	inline typename std::enable_if <std::is_floating_point<floating_t>::value, Fixed_Point_Static<T,F>>::type&
	operator=(const floating_t& o               )
	{
		// TODO : manage risk of overflow
		int64_t cast = static_cast<int64_t> (o * (__one<<1));
		cast = (cast >> 1) + (cast & (int64_t)1);
		__data = (base_type)saturate<int64_t>(cast);
		return *this;
	}
	template <size_t To, size_t Fo>
	inline Fixed_Point_Static<T,F>&
	operator=(const Fixed_Point_Static<To,Fo>& o)
	{
		change_data(o);
		return *this;
	}

	// the following methods and operators change only the data value with an original quantification
	// in function of the already set quantification (manage the possible need of a sign extension with the input data)
	template <typename type>
	inline void
	change_data(const type       data)
	{
		*this = data;
	}
	template <typename integral_t>
	inline typename std::enable_if <std::is_integral<integral_t>::value, void>::type
	change_data(      integral_t data, size_t data_total_bits, size_t data_fractional_bits)
	{
		// TODO : manage overflow __int128_t
		data = FP_numeric::sign_extension(data, data_total_bits);
		base_type temp = data;
		if(data < 0)
			temp = -temp;

		if(data_fractional_bits != __fractional_bits)
			temp = FP_numeric::shift_fract(temp, __fractional_bits, data_fractional_bits); // resize fractional part

		// temp is >= 0
		__data = FP_numeric::saturate_max(temp, __max_,
		                                  FP_numeric::Arithmetic_type::SIGNED_SATURATED);

		if(data < 0)
			__data = -__data;
	}
	template <typename floating_t>
	inline typename std::enable_if <std::is_floating_point<floating_t>::value, void>::type&
	change_data(const floating_t data, size_t data_total_bits, size_t data_fractional_bits)
	{
		*this = data;
	}

	inline void change_data(const Fixed_Point_Static<T ,F >& o)
	{
		__data = o.get_data();
	}
	template <size_t To>
	inline void change_data(const Fixed_Point_Static<To,F >& o)
	{
		using big_type = typename std::conditional<T >= To, base_type, typename Fixed_Point_Static<To,F>::base_type>::type;

		__data = (base_type)FP_numeric::saturate((big_type)o.get_data(),
		                                         (big_type)__min_,
		                                         (big_type)__max_,
		                                         FP_numeric::Arithmetic_type::SIGNED_SATURATED);
	}
	template <size_t To, size_t Fo>
	inline void change_data(const Fixed_Point_Static<To,Fo>& o)
	{
		using big_type = typename FP_numeric::max_res_type_d<T, F, To, Fo>::type::base_type;

		big_type o_data = o.get_data();
		if(o < 0)
			o_data = -o_data;

		o_data = FP_numeric::shift_fract(o_data, F, Fo); // resize fractional part

		// o_data is >= 0
		o_data = FP_numeric::saturate_max(o_data, (big_type)__max_,
		                                  FP_numeric::Arithmetic_type::SIGNED_SATURATED);

		if(o < 0)
			__data = -o_data;
		else
			__data =  o_data;
	}

	// the following methods set the given argument as new quantification OR arithmetic type OR data
	// without touching the others
	template <typename integer_t>
	inline void set_data           (integer_t data                     )
	{
		__data = FP_numeric::sign_extension((base_type)data, __total_bits);
	}

	template <size_t To, size_t Fo>
	inline void set_data           (const Fixed_Point_Static<To, Fo>& o)
	{
		set_data(o.get_data());
	}
	inline void set_arithmetic_type(FP_numeric::Arithmetic_type ar_type)
	{
		__ar_type = ar_type;
	}
	template <size_t To, size_t Fo>
	inline void set_arithmetic_type(const Fixed_Point_Static<To, Fo>& o)
	{
		set_arithmetic_type(o.get_arithmetic_type());
	}

public: // display methods
	inline std::string display(int precision) const
	{
		return FP_numeric::display(*this, precision);
	}

	inline std::string display_quantification() const
	{
		return FP_numeric::display_quantification(*this);
	}

	//display resume of this fixed point
	inline std::string       resume() const
	{
		return FP_numeric::resume(*this);
	}
	inline std::string short_resume() const
	{
		return FP_numeric::short_resume(*this);
	}
};


template <size_t T, size_t F>
using FP_S = Fixed_Point_Static<T,F>;

#include "Fixed_Point_Static_extra.hpp"

#endif // FIXED_POINT_STATIC_HPP_
