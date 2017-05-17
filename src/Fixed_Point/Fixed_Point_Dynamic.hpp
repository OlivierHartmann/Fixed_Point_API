#ifndef FIXED_POINT_DYNAMIC_HPP_
#define FIXED_POINT_DYNAMIC_HPP_

#include <stdint.h>
#include <iostream>
#include <exception>
#include <string>
#include <bitset>
#include <type_traits>
#include <cmath>
#include <limits>

#include "Fixed_Point_Utils.hpp"
#include "Fixed_Point_Static.hpp"

class Fixed_Point_Dynamic;

std::ostream& operator<<(std::ostream& os, const Fixed_Point_Dynamic& f);

//declaration of the class
class Fixed_Point_Dynamic
{
public:
	using base_type = int_fast64_t;
	static const size_t capacity = sizeof(base_type)*8; // maximum size of T before needing to rise the size of base_type

private: //given
	size_t                      __total_bits;
	size_t                      __fractional_bits;
	FP_numeric::Arithmetic_type __ar_type;
	base_type                   __data;

private: //automatically calculated
	size_t    __integer_bits;
	base_type __fractional_mask;
	base_type __integer_mask;
	base_type __number_mask;
	base_type __one;
	base_type __max_;
	base_type __min_;

public: // constructors
	Fixed_Point_Dynamic(                                                            FP_numeric::Arithmetic_type ar_type = FP_numeric::Arithmetic_type::SIGNED_SATURATED)
		: __total_bits      (0                                 ),
		  __fractional_bits (0                                 ),
		  __ar_type         (ar_type                           ),
		  __data            (base_type(0)                      ),
		  __integer_bits    (0                                 ),
		  __fractional_mask (0                                 ),
		  __integer_mask    (0                                 ),
		  __number_mask     (0                                 ),
		  __one             (1                                 ),
		  __max_            (0                                 ),
		  __min_            (0                                 )
		{ }
	Fixed_Point_Dynamic(                 size_t total_bits, size_t fractional_bits, FP_numeric::Arithmetic_type ar_type = FP_numeric::Arithmetic_type::SIGNED_SATURATED)
		: __total_bits      (total_bits                        ),
		  __fractional_bits (fractional_bits                   ),
		  __ar_type         (ar_type                           ),
		  __data            (base_type(0)                      )
		{ init(); }
	template <typename integral_t>
	Fixed_Point_Dynamic(integral_t data, size_t total_bits, size_t fractional_bits, FP_numeric::Arithmetic_type ar_type = FP_numeric::Arithmetic_type::SIGNED_SATURATED,
	                   typename std::enable_if < std::is_integral<integral_t>::value, bool>::type v = 0)
		: __total_bits      (total_bits                        ),
		  __fractional_bits (fractional_bits                   ),
		  __ar_type         (ar_type                           ),
		  __data            (base_type(data) << fractional_bits)
		{ init(); __data = saturate(__data);}
	template <typename floating_t>
	Fixed_Point_Dynamic(floating_t data, size_t total_bits, size_t fractional_bits, FP_numeric::Arithmetic_type ar_type = FP_numeric::Arithmetic_type::SIGNED_SATURATED,
	                   typename std::enable_if < std::is_floating_point<floating_t>::value, bool>::type v = 0)
		: __total_bits      (total_bits                        ),
		  __fractional_bits (fractional_bits                   ),
		  __ar_type         (ar_type                           )
		{ init(); *this = data; }

	template <typename integral_t>
	Fixed_Point_Dynamic(integral_t data, FP_numeric::Arithmetic_type ar_type = FP_numeric::Arithmetic_type::SIGNED_SATURATED,
	                   typename std::enable_if < std::is_integral<integral_t>::value, bool>::type = 0)
		: __total_bits      (sizeof(base_type)*8),
		  __fractional_bits (0                  ),
		  __ar_type         (ar_type            ),
		  __data            (base_type(data)    )
		{ init(); shrink_to_fit();}
	template <typename floating_t>
	Fixed_Point_Dynamic(floating_t data, FP_numeric::Arithmetic_type ar_type = FP_numeric::Arithmetic_type::SIGNED_SATURATED,
	                   typename std::enable_if < std::is_floating_point<floating_t>::value, bool>::type = 0)
		: __total_bits      (sizeof(base_type)*8),
		  __fractional_bits (sizeof(base_type)*4),
		  __ar_type         (ar_type            )
		{ init(); *this = data; shrink_to_fit();}

	Fixed_Point_Dynamic(const Fixed_Point_Dynamic& o, FP_numeric::Arithmetic_type ar_type)
	: __total_bits      (o.__total_bits     ),
	  __fractional_bits (o.__fractional_bits),
	  __ar_type         (ar_type            ),
	  __data            (o.__data           ),
	  __integer_bits    (o.__integer_bits   ),
	  __fractional_mask (o.__fractional_mask),
	  __integer_mask    (o.__integer_mask   ),
	  __number_mask     (o.__number_mask    ),
	  __one             (o.__one            ),
	  __max_            (o.__max_            ),
	  __min_            (o.__min_            )
		{ }
	Fixed_Point_Dynamic(const Fixed_Point_Dynamic& o)
	: __total_bits      (o.__total_bits     ),
	  __fractional_bits (o.__fractional_bits),
	  __ar_type         (o.__ar_type        ),
	  __data            (o.__data           ),
	  __integer_bits    (o.__integer_bits   ),
	  __fractional_mask (o.__fractional_mask),
	  __integer_mask    (o.__integer_mask   ),
	  __number_mask     (o.__number_mask    ),
	  __one             (o.__one            ),
	  __max_            (o.__max_            ),
	  __min_            (o.__min_            )
		{ }

	template<size_t T, size_t F>
	Fixed_Point_Dynamic(const Fixed_Point_Static<T,F>& o, FP_numeric::Arithmetic_type ar_type)
	: __total_bits      (T                      ),
	  __fractional_bits (F                      ),
	  __ar_type         (ar_type                ),
	  __data            (o.get_data()           ),
	  __integer_bits    (o.get_integer_bits()   ),
	  __fractional_mask (o.get_fractional_mask()),
	  __integer_mask    (o.get_integer_mask()   ),
	  __number_mask     (o.get_number_mask()    ),
	  __one             (o.get_one()            ),
	  __max_            (o.get_max()            ),
	  __min_            (o.get_min()            )
		{ }
	template<size_t T, size_t F>
	Fixed_Point_Dynamic(const Fixed_Point_Static<T,F>& o)
	: __total_bits      (T                      ),
	  __fractional_bits (F                      ),
	  __ar_type         (o.get_arithmetic_type()),
	  __data            (o.get_data()           ),
	  __integer_bits    (o.get_integer_bits()   ),
	  __fractional_mask (o.get_fractional_mask()),
	  __integer_mask    (o.get_integer_mask()   ),
	  __number_mask     (o.get_number_mask()    ),
	  __one             (o.get_one()            ),
	  __max_            (o.get_max()            ),
	  __min_            (o.get_min()            )
		{ }

	virtual ~Fixed_Point_Dynamic(){}

private:
	inline void init ()
	{
		if(__total_bits > sizeof(base_type)*8)
		{
			std::string excp_mes = "Fixed_Point_Dynamic Error: total_bits > " + std::to_string(sizeof(base_type)*8) +
			                       " --> total_bits = " + std::to_string(__total_bits) + ".";
			throw std::length_error(excp_mes);
		}

		if(__total_bits == 0)
		{
			std::string excp_mes = "Fixed_Point_Dynamic Error: total_bits == 0.";
			throw std::length_error(excp_mes);
		}

		if(__fractional_bits > __total_bits)
		{
			std::string excp_mes = "Fixed_Point_Dynamic Error: fractional_bits > total_bits: --> total_bits = " +
			                       std::to_string(__total_bits) + ", fractional_bits = " +
			                       std::to_string(__fractional_bits) + ".";
			throw std::length_error(excp_mes);
		}

		__integer_bits = __total_bits - __fractional_bits;
		if(__ar_type == FP_numeric::Arithmetic_type::SIGNED_SATURATED)
		{
			if(__integer_bits == 0)
			{
				std::cerr << "Error : Ask for SIGNED_SATURATED but no place for signed bit:" << std::endl;
				std::cerr << "--> total_bits = "       << __total_bits
				          << ", integer_bits = "   << __integer_bits
				          << ", fractional_bits = "<< __fractional_bits
				          << std::endl;

				std::string excp_mes = "Fixed_Point_Dynamic Error: Ask for SIGNED_SATURATED but no place for signed bit: "
				                       " --> total_bits = " + std::to_string(__total_bits) + ", integer_bits = " +
				                       std::to_string(__integer_bits) +  ", fractional_bits = " +
				                       std::to_string(__fractional_bits) + ".";
				throw std::length_error(excp_mes);
			}
			__integer_bits--; // include a sign bit
		}

		if(__integer_bits > __total_bits)
		{
			std::string excp_mes = "Fixed_Point_Dynamic Error: integer_bits > total_bits: "
			                       " --> total_bits = " + std::to_string(__total_bits) + ", integer_bits = " +
			                       std::to_string(__integer_bits) +  ", fractional_bits = " +
			                       std::to_string(__fractional_bits) + ".";
			throw std::length_error(excp_mes);
		}

		__fractional_mask =  ~((~base_type(0)) << __fractional_bits);
		__integer_mask    = (~((~base_type(0)) << __integer_bits   )) << __fractional_bits;
		__one             =      base_type(1)  << __fractional_bits;
		__max_            = __integer_mask | __fractional_mask;
		__number_mask     = __integer_mask | __fractional_mask;

		switch(__ar_type)
		{
//		case FP_numeric::Arithmetic_type::UNSIGNED_SATURATED:

		case FP_numeric::Arithmetic_type::SIGNED_SATURATED:
			__number_mask = __number_mask + 1 + __number_mask; // add the signed bit
			__min_  = -__max_;
			break;

//		case FP_numeric::Arithmetic_type::CYCLIC:
		}
	}

public:
	// return the given value val saturated (if needed) with the configuration of this Fixed_Point_Dynamic
	inline base_type saturate(const base_type val) const
	{
		return FP_numeric::saturate(val, __min_, __max_, __ar_type);
	}

	// return the __data value of this Fixed_Point_Dynamic with the point set at the new_fract_bits position
	inline base_type shift_fract(const size_t new_fract_bits) const
	{
		return FP_numeric::shift_fract(__data, new_fract_bits, __fractional_bits);
	}
	inline base_type shift_fract(const size_t new_fract_bits, const size_t old_fract_bits) const
	{
		return FP_numeric::shift_fract(__data, new_fract_bits, old_fract_bits);
	}

public: // relational operators

	inline bool operator> (const Fixed_Point_Dynamic& o) const
	{
		// can't align fractional parts to compare directly because of the risk of overflow
		// so compare first integer part then fractional part
		const base_type this_i =   __data >>   __fractional_bits;
		const base_type o_i    = o.__data >> o.__fractional_bits;

		if(this_i != o_i)
			return this_i > o_i;

		int64_t   f_diff = __fractional_bits - o.__fractional_bits;
		base_type this_f = __data, o_f = o.__data;

		if(f_diff < 0)
			this_f <<= -f_diff;
		else
			o_f    <<=  f_diff;

		return (this_f > o_f);
	}
	inline bool operator< (const Fixed_Point_Dynamic& o) const { return       o > *this;}
	inline bool operator<=(const Fixed_Point_Dynamic& o) const { return !(*this > o);   }
	inline bool operator>=(const Fixed_Point_Dynamic& o) const { return !(*this < o);   }

	// TODO : manage risk of overflow
	template <typename T>
	inline bool operator> (const T n) const { Fixed_Point_Dynamic t(n); return *this >  t;}
	template <typename T>
	inline bool operator< (const T n) const { Fixed_Point_Dynamic t(n); return *this <  t;}
	template <typename T>
	inline bool operator<=(const T n) const { Fixed_Point_Dynamic t(n); return *this <= t;}
	template <typename T>
	inline bool operator>=(const T n) const { Fixed_Point_Dynamic t(n); return *this >= t;}

	inline bool operator==(const Fixed_Point_Dynamic& o) const
	{
		// can't align fractional parts to compare directly because of the risk of overflow
		// so compare first integer part then fractional part
		const base_type this_i =   __data >>   __fractional_bits;
		const base_type o_i    = o.__data >> o.__fractional_bits;

		if(this_i != o_i)
			return false;

		int64_t   f_diff = __fractional_bits - o.__fractional_bits;
		base_type this_f = __data, o_f = o.__data;

		if(f_diff < 0)
			this_f <<= -f_diff;
		else
			o_f    <<=  f_diff;

		return (this_f == o_f);
	}
	inline bool operator!=(const Fixed_Point_Dynamic& o) const { return !(*this == o);}

	template <typename T>
	inline bool operator==(const T n) const { Fixed_Point_Dynamic t(n); return *this == t;}
	template <typename T>
	inline bool operator!=(const T n) const { Fixed_Point_Dynamic t(n); return *this != t;}

	inline bool operator! ( ) const { return !__data;}

	// compare the quantification of this Fixed_Point_Dynamic with the given one
	inline bool same_quantification(size_t total_bits, size_t fractional_bits) const
	{
		return (total_bits == __total_bits && fractional_bits == __fractional_bits);
	}
	inline bool same_quantification(const Fixed_Point_Dynamic& o             ) const
	{
		return same_quantification(o.get_total_bits(), o.get_fractional_bits());
	}

public: // binary operators: they do not care about the quantification of the number in __data
	inline Fixed_Point_Dynamic  operator~  ( ) const
	{
		Fixed_Point_Dynamic t(*this);
		t.__data = ~t.__data;
		return t;
	}

	inline Fixed_Point_Dynamic& operator&= (const Fixed_Point_Dynamic& n) { __data &= n.__data; return *this;}
	inline Fixed_Point_Dynamic& operator|= (const Fixed_Point_Dynamic& n) { __data |= n.__data; return *this;}
	inline Fixed_Point_Dynamic& operator^= (const Fixed_Point_Dynamic& n) { __data ^= n.__data; return *this;}

	template <typename T>
	inline Fixed_Point_Dynamic& operator&= (const T n) { __data &= (base_type)n; return *this;}
	template <typename T>
	inline Fixed_Point_Dynamic& operator|= (const T n) { __data |= (base_type)n; return *this;}
	template <typename T>
	inline Fixed_Point_Dynamic& operator^= (const T n) { __data ^= (base_type)n; return *this;}

	inline Fixed_Point_Dynamic& operator>>=(const Fixed_Point_Dynamic& n)
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
	inline Fixed_Point_Dynamic& operator<<=(const Fixed_Point_Dynamic& n)
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
	inline typename std::enable_if <std::is_integral<integral_t>::value, Fixed_Point_Dynamic>::type&
	operator>>=(const integral_t n) { __data >>= n;                            return *this;}
	template <typename integral_t>
	inline typename std::enable_if <std::is_integral<integral_t>::value, Fixed_Point_Dynamic>::type&
	operator<<=(const integral_t n) { __data <<= n; __data = saturate(__data); return *this;}
	template <typename floating_t>
	inline typename std::enable_if <std::is_floating_point<floating_t>::value, Fixed_Point_Dynamic>::type&
	operator>>=(const floating_t n) {  *this >>= ((n>0)? (int)n : -(int)n);    return *this;}
	template <typename floating_t>
	inline typename std::enable_if <std::is_floating_point<floating_t>::value, Fixed_Point_Dynamic>::type&
	operator<<=(const floating_t n) {  *this <<= ((n>0)? (int)n : -(int)n);    return *this;}

public: // basic math operators
	inline const Fixed_Point_Dynamic& operator+ () const { return *this;}
	inline       Fixed_Point_Dynamic  operator- () const
	{
		Fixed_Point_Dynamic t(*this);
		t.__data = -t.__data;
		return t;
	}

	// if __total_bits is null then takes the quantification of the result of the operation
	inline Fixed_Point_Dynamic& operator++ (   ) { *this += one(); return *this;}
	inline Fixed_Point_Dynamic& operator-- (   ) { *this -= one(); return *this;}
	inline Fixed_Point_Dynamic& operator++ (int) { *this += one(); return *this;}
	inline Fixed_Point_Dynamic& operator-- (int) { *this -= one(); return *this;}

	// if __total_bits is null then takes the quantification of the result of the operation
	template <typename T>
	inline Fixed_Point_Dynamic& operator+= (const T n) { *this = (*this)+n; return *this;}
	template <typename T>
	inline Fixed_Point_Dynamic& operator-= (const T n) { *this = (*this)-n; return *this;}
	template <typename T>
	inline Fixed_Point_Dynamic& operator*= (const T n) { *this = (*this)*n; return *this;}
	template <typename T>
	inline Fixed_Point_Dynamic& operator/= (const T n) { *this = (*this)/n; return *this;}
	template <typename T>
	inline Fixed_Point_Dynamic& operator%= (const T n) { *this = (*this)%n; return *this;}

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

	inline Fixed_Point_Dynamic abs () const { return is_sneg() ? -(*this) : *this;}
	inline int                 sign() const { return is_sneg() ? -1       : 0;    }

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
		return static_cast<floating_t>(__data) / __one;
	}

	template <typename T>
	inline T       cast      () const { return (T)__data;}
	template <typename T>
	               operator T() const { return cast<T>();} // static cast

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
	// if there is none then return an empty Fixed_Point_Dynamic
	inline Fixed_Point_Dynamic min () const
	{
		Fixed_Point_Dynamic min(*this);
		min.set_data(__min_);
		return min;
	}
	inline Fixed_Point_Dynamic max () const
	{
		Fixed_Point_Dynamic max(*this);
		max.set_data(__max_);
		return max;
	}
	inline Fixed_Point_Dynamic one () const
	{
		Fixed_Point_Dynamic one(*this);
		one.set_data(__one);
		return one;
	}
	inline Fixed_Point_Dynamic zero() const
	{
		Fixed_Point_Dynamic zero(*this);
		zero.set_data(0);
		return zero;
	}

public: // setters
	// the following methods and operators change only the data value in function of the already set quantification
	// except if __total_bits is null, then it resizes automatically the quantification to fit to the data
	template <typename integral_t>
	inline typename std::enable_if <std::is_integral<integral_t>::value, Fixed_Point_Dynamic>::type&
	operator=(const integral_t& o         )
	{
		if (__total_bits)
		{
			// TODO : manage risk of overflow
			__data = base_type(o) << __fractional_bits;
			__data = saturate (__data);
		}
		else
		{
			if(!o) // o is null
				return *this;

			// resize automatically
			__total_bits      = sizeof(base_type)*8;
			__fractional_bits = 0;
			init();
			__data = base_type(o);
			shrink_to_fit();
		}
		return *this;
	}
	// take care of the weight following lost bit in function of the sign of the value
	// round the fractional part to the nearest value (arithmetic not absolute)
	template <typename floating_t>
	inline typename std::enable_if <std::is_floating_point<floating_t>::value, Fixed_Point_Dynamic>::type&
	operator=(const floating_t& o         )
	{
		if (__total_bits)
		{
			__data = static_cast<base_type> (o * (__one<<1));
			__data = (__data >> 1) + (__data & (base_type)1);
			__data = saturate(__data);
		}
		else
		{
			if(o == 0.f)
				return *this;

			__total_bits      = sizeof(base_type)*8;
			__fractional_bits = __total_bits/2;
			init();
			__data = static_cast<base_type>(o * __one);
			shrink_to_fit();
		}
		return *this;
	}
	// same than above but if __total_bits is null, then it takes o's quantification
	inline Fixed_Point_Dynamic&
	operator=(const Fixed_Point_Dynamic& o)
	{
		change_data(o);
		return *this;
	}

	// the following methods and operators change only the data value with an original quantification
	// in function of the already set quantification
	// except if __total_bits == 0, then it takes the given quantification
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
		if (__total_bits)
		{
			// TODO : manage overflow __int128_t
			data = FP_numeric::sign_extension(data, data_total_bits);
			base_type temp = data;
			__data = data;
			if(data < 0)
				temp = -temp;

			if(data_fractional_bits != __fractional_bits)
				temp = shift_fract(__fractional_bits, data_fractional_bits); // resize fractional part

			// data is >= 0
			__data = FP_numeric::saturate_max(temp, __max_, __ar_type);

			if(data < 0)
				__data = -__data;
		}
		else
		{
			set_quantification(data_total_bits, data_fractional_bits);
			__data = saturate(base_type(data));
		}
	}
	template <typename floating_t>
	inline typename std::enable_if <std::is_floating_point<floating_t>::value, void>::type&
	change_data(const floating_t data, size_t data_total_bits, size_t data_fractional_bits)
	{
		if (__total_bits)
		{
			*this = data;
		}
		else
		{
			set_quantification(data_total_bits, data_fractional_bits);
			*this = data;
		}
	}
	inline void
	change_data(const Fixed_Point_Dynamic& o)
	{
		if (__total_bits)
		{
			base_type data = o.get_data();
			if(o < 0)
				data = -data;

			if(o.get_fractional_bits() != __fractional_bits)
				data = FP_numeric::shift_fract(data, __fractional_bits, o.get_fractional_bits()); // resize fractional part

			// o_data is >= 0
			data = FP_numeric::saturate_max(data, __max_, __ar_type);

			if(o < 0)
				__data = -data;
			else
				__data =  data;

		}
		else
		{
			set_quantification(o);
			__data = o.get_data();
		}
	}

	// change the quantification of the data, meaning that the data is touched in function of the old quantification
	inline void change_quantification(size_t total_bits, size_t fractional_bits)
	{
		if(fractional_bits != __fractional_bits)
			__data = shift_fract(fractional_bits); // resize fractional part
		else
			if(total_bits == __total_bits)
			{
				__data = saturate(__data); // if set_data is used before, __data may be over the saturation values
				return; // same quantification
			}

		// resize integer part
		set_quantification(total_bits, fractional_bits);
		__data = saturate(__data);
	}

	// the following methods set the given argument as new quantification OR arithmetic type OR data
	// without touching the others
	inline void set_quantification (size_t total_bits, size_t fractional_bits)
	{
		__total_bits      = total_bits;
		__fractional_bits = fractional_bits;
		init();
	}
	inline void set_arithmetic_type(FP_numeric::Arithmetic_type ar_type      )
	{
		__ar_type = ar_type;
	}
	template <typename integer_t>
	inline void set_data           (integer_t data                     )
	{
		__data = FP_numeric::sign_extension((base_type)data, __total_bits);
	}

	// same than those above but copy the configuration of the given Fixed_Point_Dynamic
	inline void change_quantification(const Fixed_Point_Dynamic& o)
	{
		change_quantification(o.__total_bits, o.__fractional_bits);
	}
	inline void set_quantification   (const Fixed_Point_Dynamic& o)
	{
		set_quantification(o.__total_bits, o.__fractional_bits);
	}
	inline void set_arithmetic_type  (const Fixed_Point_Dynamic& o)
	{
		set_arithmetic_type(o.__ar_type);
	}
	inline void set_data             (const Fixed_Point_Dynamic& o)
	{
		set_data(o.__data);
	}

public:
	// swap data and configurations of the two Fixed_Point_Dynamics
	inline void swap(Fixed_Point_Dynamic& rhs)
	{
		std::swap(__data,            rhs.__data           );
		std::swap(__total_bits,      rhs.__total_bits     );
		std::swap(__fractional_bits, rhs.__fractional_bits);
		std::swap(__integer_bits,    rhs.__integer_bits   );
		std::swap(__fractional_mask, rhs.__fractional_mask);
		std::swap(__integer_mask,    rhs.__integer_mask   );
		std::swap(__number_mask,     rhs.__number_mask    );
		std::swap(__one,             rhs.__one            );
		std::swap(__max_,            rhs.__max_           );
		std::swap(__min_,            rhs.__min_           );
	}

	// take the minimum size possible to contain the actual value of data
	inline void shrink_to_fit()
	{
		if(!__data)
		{
			Fixed_Point_Dynamic null_fp;
			this->swap(null_fp);
			return;
		}

		// resize integer part
		base_type mask             = (base_type)1 << (__total_bits-1);
		size_t    i                = 0;
		size_t    sign             = 0;
		size_t    new_integer_bits = 0;

		if (__ar_type == FP_numeric::Arithmetic_type::SIGNED_SATURATED)
		{
			new_integer_bits++; //take into account the sign bit
			sign = (__data & mask)? 1 : 0;
			mask >>= 1;
		}

		for (; i < __integer_bits; i++)
		{
			if (((__data & mask)? 1 : 0) != sign)
				break;
			mask >>= 1;
		}
		new_integer_bits += __integer_bits - i;

		// resize fractional part
		mask = 1;
		i    = 0;
		for (; i < __fractional_bits; i++)
		{
			if (__data & mask)
				break;
			mask <<= 1;
		}
		size_t new_fractional_bits = __fractional_bits - i;

		change_quantification(new_integer_bits+new_fractional_bits, new_fractional_bits);
	}


public:
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

public: // to get the configuration of the result of an operation
	inline void get_config_after_add(const Fixed_Point_Dynamic& o, size_t& res_total_bits, size_t& res_fractional_bits) const
	{
		res_fractional_bits = std::max(get_fractional_bits(), o.get_fractional_bits());
		res_total_bits      = std::max(get_integer_bits   (), o.get_integer_bits   ()) + res_fractional_bits + 1; // increase size of one
		if (__ar_type == FP_numeric::Arithmetic_type::SIGNED_SATURATED || o.get_arithmetic_type() == FP_numeric::Arithmetic_type::SIGNED_SATURATED)
			res_total_bits++; // add a sign bit
	}
	inline void get_config_after_sub(const Fixed_Point_Dynamic& o, size_t& res_total_bits, size_t& res_fractional_bits) const
	{
		res_fractional_bits = std::max(get_fractional_bits(), o.get_fractional_bits());
		res_total_bits      = std::max(get_integer_bits   (), o.get_integer_bits   ()) + res_fractional_bits + 1; // increase size of one
		if (__ar_type == FP_numeric::Arithmetic_type::SIGNED_SATURATED || o.get_arithmetic_type() == FP_numeric::Arithmetic_type::SIGNED_SATURATED)
			res_total_bits++; // add a sign bit
	}
	inline void get_config_after_mul(const Fixed_Point_Dynamic& o, size_t& res_total_bits, size_t& res_fractional_bits) const
	{
		res_fractional_bits = get_fractional_bits() + o.get_fractional_bits();
		res_total_bits      = get_integer_bits   () + o.get_integer_bits   () + res_fractional_bits;
		if (__ar_type == FP_numeric::Arithmetic_type::SIGNED_SATURATED || o.get_arithmetic_type() == FP_numeric::Arithmetic_type::SIGNED_SATURATED)
			res_total_bits++; // add sign bit
	}
	inline void get_config_after_div(const Fixed_Point_Dynamic& o, size_t& res_total_bits, size_t& res_fractional_bits) const
	{
		get_config_after_mul(o, res_total_bits, res_fractional_bits);
	}
	inline void get_config_after_mod(const Fixed_Point_Dynamic& o, size_t& res_total_bits, size_t& res_fractional_bits) const
	{
		res_fractional_bits = std::max(get_fractional_bits(), o.get_fractional_bits());
		res_total_bits      = std::max(get_integer_bits   (), o.get_integer_bits   ()) + res_fractional_bits +1;
		if (__ar_type == FP_numeric::Arithmetic_type::SIGNED_SATURATED || o.get_arithmetic_type() == FP_numeric::Arithmetic_type::SIGNED_SATURATED)
			res_total_bits++; // add sign bit
	}

	inline void set_config_after_add(const Fixed_Point_Dynamic& lhs, const Fixed_Point_Dynamic& rhs)
	{
		// get the new configuration
		size_t res_size_f, res_size_t;
		lhs.get_config_after_add(rhs, res_size_t, res_size_f);
		change_quantification(res_size_t, res_size_f);
	}
	inline void set_config_after_sub(const Fixed_Point_Dynamic& lhs, const Fixed_Point_Dynamic& rhs)
	{
		// get the new configuration
		size_t res_size_f, res_size_t;
		lhs.get_config_after_sub(rhs, res_size_t, res_size_f);
		change_quantification(res_size_t, res_size_f);
	}
	inline void set_config_after_mul(const Fixed_Point_Dynamic& lhs, const Fixed_Point_Dynamic& rhs)
	{
		// get the new configuration
		size_t res_size_f, res_size_t;
		lhs.get_config_after_mul(rhs, res_size_t, res_size_f);
		change_quantification(res_size_t, res_size_f);
	}
	inline void set_config_after_div(const Fixed_Point_Dynamic& lhs, const Fixed_Point_Dynamic& rhs)
	{
		// get the new configuration
		size_t res_size_f, res_size_t;
		lhs.get_config_after_div(rhs, res_size_t, res_size_f);
		change_quantification(res_size_t, res_size_f);
	}
	inline void set_config_after_mod(const Fixed_Point_Dynamic& lhs, const Fixed_Point_Dynamic& rhs)
	{
		// get the new configuration
		size_t res_size_f, res_size_t;
		lhs.get_config_after_mod(rhs, res_size_t, res_size_f);
		change_quantification(res_size_t, res_size_f);
	}
};

using FP_D = Fixed_Point_Dynamic;

#include "Fixed_Point_Dynamic_extra.hpp"

#endif /* FIXED_POINT_DYNAMIC_HPP_ */
