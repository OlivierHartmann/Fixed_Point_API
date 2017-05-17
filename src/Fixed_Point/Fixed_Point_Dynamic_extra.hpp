#ifndef FIXED_POINT_DYNAMIC_EXTRA_HPP_
#define FIXED_POINT_DYNAMIC_EXTRA_HPP_

#include "Fixed_Point_Dynamic.hpp"

/// *********************************************************************************************************************
/// Display operator that indicates the used quantification *************************************************************
/// *********************************************************************************************************************

inline std::ostream& operator<<(std::ostream& os, const Fixed_Point_Dynamic& f)
{
	os << f.display(6) << f.display_quantification() ;

	return os;
}

/// *********************************************************************************************************************
/// Fixed point arithmetics and bitwise operators ***********************************************************************
/// *********************************************************************************************************************

namespace FP_numeric
{
	// add, sub, mul or div lhs and rhs and set the result with the given configuration
	// if res_total_bits == 0 AND res_fractional_bits == 0 then return the result with its automatically given size for the computation
	inline Fixed_Point_Dynamic add(const Fixed_Point_Dynamic& lhs,
	                               const Fixed_Point_Dynamic& rhs)
	{
		// get the configuration for the operation
		size_t res_size_f, res_size_t;
		lhs.get_config_after_add(rhs, res_size_t, res_size_f);

		// get the data and align the commas
		Fixed_Point_Dynamic::base_type lhs_data = ((lhs.get_fractional_bits() != res_size_f) ? lhs.shift_fract(res_size_f)
		                                                                                     : lhs.to_raw());
		Fixed_Point_Dynamic::base_type rhs_data = ((rhs.get_fractional_bits() != res_size_f) ? rhs.shift_fract(res_size_f)
		                                                                                     : rhs.to_raw());

		// compute
		Fixed_Point_Dynamic res(res_size_t, res_size_f);
		res.set_data(lhs_data + rhs_data);

		return res;
	}
	inline Fixed_Point_Dynamic sub(const Fixed_Point_Dynamic& lhs,
	                               const Fixed_Point_Dynamic& rhs)
	{
		// get the configuration for the operation
		size_t res_size_f, res_size_t;
		lhs.get_config_after_sub(rhs, res_size_t, res_size_f);

		// get the data and align them
		Fixed_Point_Dynamic::base_type lhs_data = ((lhs.get_fractional_bits() != res_size_f) ? lhs.shift_fract(res_size_f)
		                                                                                     : lhs.to_raw());
		Fixed_Point_Dynamic::base_type rhs_data = ((rhs.get_fractional_bits() != res_size_f) ? rhs.shift_fract(res_size_f)
		                                                                                     : rhs.to_raw());

		// compute
		Fixed_Point_Dynamic res(res_size_t, res_size_f);
		res.set_data(lhs_data - rhs_data);

		return res;
	}
	inline Fixed_Point_Dynamic mul(const Fixed_Point_Dynamic& lhs,
	                               const Fixed_Point_Dynamic& rhs)
	{
		// get the configuration for the operation
		size_t res_size_f, res_size_t;
		lhs.get_config_after_mul(rhs, res_size_t, res_size_f);

		// compute
		Fixed_Point_Dynamic res(res_size_t, res_size_f);
		res.set_data(lhs.to_raw() * rhs.to_raw());

		return res;
	}
	inline Fixed_Point_Dynamic div(const Fixed_Point_Dynamic& numerator,
	                               const Fixed_Point_Dynamic& denominator)
	{
		if(!denominator)
			throw std::domain_error("Division by zero!");

		// get the configuration for the operation
		size_t res_size_f, res_size_t;
		numerator.get_config_after_div(denominator, res_size_t, res_size_f);

		// get the data and align them
		Fixed_Point_Dynamic::base_type num_data =   numerator.shift_fract(res_size_f*2);
		Fixed_Point_Dynamic::base_type den_data = denominator.shift_fract(res_size_f  );

		// compute
		Fixed_Point_Dynamic quotient(res_size_t, res_size_f);
		quotient.set_data(num_data / den_data);

		return quotient;
	}

	// return the quotient in its given size and compute from that the remainder
	inline void                div(const Fixed_Point_Dynamic& numerator,
	                               const Fixed_Point_Dynamic& denominator,
	                                     Fixed_Point_Dynamic& quotient,
	                                     Fixed_Point_Dynamic& remainder)
	{
		quotient  = FP_numeric::div(numerator, denominator);
		remainder = FP_numeric::sub(numerator, FP_numeric::mul(quotient, denominator));
	}
	inline Fixed_Point_Dynamic mod(const Fixed_Point_Dynamic& lhs,
	                               const Fixed_Point_Dynamic& rhs)
	{
		// get the configuration for the operation
		size_t res_size_f, res_size_t;
		lhs.get_config_after_mod(rhs, res_size_t, res_size_f);

		// get the data and align them
		Fixed_Point_Dynamic::base_type lhs_data = lhs.shift_fract(res_size_f);
		Fixed_Point_Dynamic::base_type rhs_data = rhs.shift_fract(res_size_f);

		// compute
		Fixed_Point_Dynamic result(res_size_t, res_size_f);
		result.set_data(lhs_data % rhs_data);

		return result;
	}

}

// add, sub, mul, div or mod lhs with rhs and set the result with the maximum possible dynamic function of lhs and rhs
inline Fixed_Point_Dynamic operator+ (const Fixed_Point_Dynamic& lhs, const Fixed_Point_Dynamic& rhs)
{
	return FP_numeric::add(lhs, rhs);
}
template <typename T>
inline Fixed_Point_Dynamic operator+ (const Fixed_Point_Dynamic& lhs, const T                    rhs)
{
	return lhs + (Fixed_Point_Dynamic)rhs;
}
template <typename T>
inline Fixed_Point_Dynamic operator+ (const T                    lhs, const Fixed_Point_Dynamic& rhs)
{
	return (Fixed_Point_Dynamic)lhs + rhs;
}

inline Fixed_Point_Dynamic operator- (const Fixed_Point_Dynamic& lhs, const Fixed_Point_Dynamic& rhs)
{
	return FP_numeric::sub(lhs, rhs);
}
template <typename T>
inline Fixed_Point_Dynamic operator- (const Fixed_Point_Dynamic& lhs, const T                    rhs)
{
	return lhs - (Fixed_Point_Dynamic)rhs;
}
template <typename T>
inline Fixed_Point_Dynamic operator- (const T                    lhs, const Fixed_Point_Dynamic& rhs)
{
	return (Fixed_Point_Dynamic)lhs - rhs;
}

inline Fixed_Point_Dynamic operator* (const Fixed_Point_Dynamic& lhs, const Fixed_Point_Dynamic& rhs)
{
	return FP_numeric::mul(lhs, rhs);
}
template <typename T>
inline Fixed_Point_Dynamic operator* (const Fixed_Point_Dynamic& lhs, const T                    rhs)
{
	return lhs * (Fixed_Point_Dynamic)rhs;
}
template <typename T>
inline Fixed_Point_Dynamic operator* (const T                    lhs, const Fixed_Point_Dynamic& rhs)
{
	return (Fixed_Point_Dynamic)lhs * rhs;
}

inline Fixed_Point_Dynamic operator/ (const Fixed_Point_Dynamic& lhs, const Fixed_Point_Dynamic& rhs)
{
	return FP_numeric::div(lhs, rhs);
}
template <typename T>
inline Fixed_Point_Dynamic operator/ (const Fixed_Point_Dynamic& lhs, const T                    rhs)
{
	return lhs / (Fixed_Point_Dynamic)rhs;
}
template <typename T>
inline Fixed_Point_Dynamic operator/ (const T                    lhs, const Fixed_Point_Dynamic& rhs)
{
	return (Fixed_Point_Dynamic)lhs / rhs;
}

inline Fixed_Point_Dynamic operator% (const Fixed_Point_Dynamic& lhs, const Fixed_Point_Dynamic& rhs)
{
	return FP_numeric::mod(lhs, rhs);
}
template <typename T>
inline Fixed_Point_Dynamic operator% (const Fixed_Point_Dynamic& lhs, const T                    rhs)
{
	return lhs % (Fixed_Point_Dynamic)rhs;
}
template <typename T>
inline Fixed_Point_Dynamic operator% (const T                    lhs, const Fixed_Point_Dynamic& rhs)
{
	return (Fixed_Point_Dynamic)lhs % rhs;
}

// shift operators (Fixed_Point_Dynamic rhs is convert as an unsigned integer)
template <typename T>
inline Fixed_Point_Dynamic operator>>(      Fixed_Point_Dynamic  lhs, const T                    rhs)
{
	return lhs >>= rhs;
}
template <typename T>
inline Fixed_Point_Dynamic operator<<(      Fixed_Point_Dynamic  lhs, const T                    rhs)
{
	return lhs <<= rhs;
}

// binary operators between the raw of lhs and rhs
inline Fixed_Point_Dynamic::base_type operator& (const Fixed_Point_Dynamic& lhs, const Fixed_Point_Dynamic& rhs)
{
	return lhs.to_raw() & rhs.to_raw();
}
template <typename T>
inline Fixed_Point_Dynamic::base_type operator& (const Fixed_Point_Dynamic& lhs, const T                    rhs)
{
	return lhs.to_raw() & (Fixed_Point_Dynamic::base_type)rhs;
}
template <typename T>
inline Fixed_Point_Dynamic::base_type operator& (const T                    lhs, const Fixed_Point_Dynamic& rhs)
{
	return (Fixed_Point_Dynamic::base_type)lhs & rhs.to_raw();
}

inline Fixed_Point_Dynamic::base_type operator^ (const Fixed_Point_Dynamic& lhs, const Fixed_Point_Dynamic& rhs)
{
	return lhs.to_raw() ^ rhs.to_raw();
}
template <typename T>
inline Fixed_Point_Dynamic::base_type operator^ (const Fixed_Point_Dynamic& lhs, const T rhs                   )
{
	return lhs.to_raw() ^ (Fixed_Point_Dynamic::base_type)rhs;
}
template <typename T>
inline Fixed_Point_Dynamic::base_type operator^ (const T                    lhs, const Fixed_Point_Dynamic& rhs)
{
	return (Fixed_Point_Dynamic::base_type)lhs ^ rhs.to_raw();
}

inline Fixed_Point_Dynamic::base_type operator| (const Fixed_Point_Dynamic& lhs, const Fixed_Point_Dynamic& rhs)
{
	return lhs.to_raw() | rhs.to_raw();
}
template <typename T>
inline Fixed_Point_Dynamic::base_type operator| (const Fixed_Point_Dynamic& lhs, const T                    rhs)
{
	return lhs.to_raw() | (Fixed_Point_Dynamic::base_type)rhs;
}
template <typename T>
inline Fixed_Point_Dynamic::base_type operator| (const T                    lhs, const Fixed_Point_Dynamic& rhs)
{
	return (Fixed_Point_Dynamic::base_type)lhs | rhs.to_raw();
}


// boolean comparison of the raw of lhs and rhs
inline bool operator&&(const Fixed_Point_Dynamic& lhs, const Fixed_Point_Dynamic& rhs)
{
	return lhs.to_raw() && rhs.to_raw();
}
//template <typename T>
//inline bool operator&&(const Fixed_Point_Dynamic& lhs, const T                    rhs)
//{
//	return lhs.to_raw() | (FP_numeric::base_type)rhs;
//}
//template <typename T>
//inline bool operator&&(const T                    lhs, const Fixed_Point_Dynamic& rhs)
//{
//	return (FP_numeric::base_type)lhs | rhs.to_raw();
//}

inline bool operator||(const Fixed_Point_Dynamic& lhs, const Fixed_Point_Dynamic& rhs)
{
	return lhs.to_raw() || rhs.to_raw();
}
//template <typename T>
//inline bool operator||(const Fixed_Point_Dynamic& lhs, const T                    rhs)
//{
//	return lhs.to_raw() | (FP_numeric::base_type)rhs;
//}
//template <typename T>
//inline bool operator||(const T                    lhs, const Fixed_Point_Dynamic& rhs)
//{
//	return (FP_numeric::base_type)lhs | rhs.to_raw();
//}


/// *********************************************************************************************************************
/// standard library overloads ******************************************************************************************
/// *********************************************************************************************************************
namespace std
{
	inline Fixed_Point_Dynamic abs      (const Fixed_Point_Dynamic& __x)
	{
		return __x.abs();
	}
	inline int                 signbit  (const Fixed_Point_Dynamic& __x)
	{
		return __x.sign();
	}
	inline std::string         to_string(const Fixed_Point_Dynamic& __x)
	{
		return std::to_string(__x.to_float());
	}

	template <>
	struct numeric_limits<Fixed_Point_Dynamic>
	{
		static _GLIBCXX_USE_CONSTEXPR bool is_specialized = true;

		static _GLIBCXX_CONSTEXPR Fixed_Point_Dynamic::base_type
		min() _GLIBCXX_USE_NOEXCEPT { return numeric_limits<Fixed_Point_Dynamic::base_type>::min(); }

		static _GLIBCXX_CONSTEXPR Fixed_Point_Dynamic::base_type
		max() _GLIBCXX_USE_NOEXCEPT { return numeric_limits<Fixed_Point_Dynamic::base_type>::max(); }

		static Fixed_Point_Dynamic::base_type
		min(const Fixed_Point_Dynamic& o) _GLIBCXX_USE_NOEXCEPT { return o.min(); }

		static Fixed_Point_Dynamic::base_type
		max(const Fixed_Point_Dynamic& o) _GLIBCXX_USE_NOEXCEPT { return o.max(); }

		static _GLIBCXX_CONSTEXPR Fixed_Point_Dynamic::base_type
		lowest() _GLIBCXX_USE_NOEXCEPT { return min(); }

		static Fixed_Point_Dynamic::base_type
		lowest(const Fixed_Point_Dynamic& o) _GLIBCXX_USE_NOEXCEPT { return min(o); }
	};
}


/// *********************************************************************************************************************
/// fixed points copy functions  ****************************************************************************************
/// *********************************************************************************************************************

#include <vector>

namespace FP_numeric
{
	/// check if it is a Fixed_Point_Dynamic
	template<typename>
	struct __is_FP_D_helper : public std::false_type { };
	template<>
	struct __is_FP_D_helper<Fixed_Point_Dynamic> : public std::true_type { };
	template<typename _Tp>
	struct   is_FP_D : public __is_FP_D_helper<typename std::remove_cv<_Tp>::type>::type { };

	/// check if it is a Fixed Point (Fixed_Point_Dynamic or Fixed_Point_Static)
	template<typename>
	struct __is_FP_helper : public std::false_type { };
	template<>
	struct __is_FP_helper<Fixed_Point_Dynamic> : public std::true_type { };
	template<size_t T, size_t F>
	struct __is_FP_helper<Fixed_Point_Static<T,F>> : public std::true_type { };
	template<typename _Tp>
	struct   is_FP : public __is_FP_helper<typename std::remove_cv<_Tp>::type>::type { };

	/// copy of two vectors
	template<typename _IT, typename _IA, typename _OA>
	void copy(const std::vector<_IT,                 _IA>& __in_vector, std::vector<Fixed_Point_Dynamic, _OA>& __out_vector)
	{
		assert(__in_vector.size() <= __out_vector.size());
		for (auto i = 0; i < (int)__in_vector.size(); i++)
			__out_vector[i].set_data(__in_vector[i]);
	}
	template<typename _OT, typename _IA, typename _OA>
	void copy(const std::vector<Fixed_Point_Dynamic, _IA>& __in_vector, std::vector<_OT,                 _OA>& __out_vector)
	{
		assert(__in_vector.size() <= __out_vector.size());
		for (auto i = 0; i < (int)__in_vector.size(); i++)
			__out_vector[i] = (_OT)__in_vector[i];
	}
	template<typename _IA, typename _OA>
	void copy(const std::vector<Fixed_Point_Dynamic, _IA>& __in_vector, std::vector<Fixed_Point_Dynamic, _OA>& __out_vector)
	{
		std::copy(__in_vector.begin(), __in_vector.end(), __out_vector.begin());
	}

}


#endif /* FIXED_POINT_DYNAMIC_EXTRA_HPP_ */
