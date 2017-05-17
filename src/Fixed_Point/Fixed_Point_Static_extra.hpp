#ifndef FIXED_POINT_STATIC_EXTRA_HPP_
#define FIXED_POINT_STATIC_EXTRA_HPP_

#include "Fixed_Point_Static.hpp"

/// *********************************************************************************************************************
/// Display operator that indicates the used quantification *************************************************************
/// *********************************************************************************************************************

template <size_t T, size_t F>
inline std::ostream& operator<<(std::ostream& os, const Fixed_Point_Static<T,F>& f)
{
	os << f.display(6) << f.display_quantification() ;

	return os;
}

/// *********************************************************************************************************************
/// Fixed point arithmetics and bitwise operators ***********************************************************************
/// *********************************************************************************************************************

namespace FP_numeric
{
	// add, sub, mul or div lhs and rhs
	// return the result with its automatically given size for the computation
	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	inline typename FP_numeric::add_res_type_d<Tl, Fl, Tr, Fr>::type
	add(const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs,
		typename std::enable_if < (Fl > Fr), bool>::type v = 0)
	{
		using __res_t = typename FP_numeric::add_res_type_d<Tl, Fl, Tr, Fr>::type;
		__res_t res;
		res.set_data((typename __res_t::base_type)(lhs.to_raw())
		             + FP_numeric::shift_fract((typename __res_t::base_type)(rhs.to_raw()), Fl, Fr));
		return res;
	}
	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	inline typename FP_numeric::add_res_type_d<Tl, Fl, Tr, Fr>::type
	add(const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs,
		typename std::enable_if < (Fl < Fr), bool>::type v = 0)
	{
		using __res_t = typename FP_numeric::add_res_type_d<Tl, Fl, Tr, Fr>::type;
		__res_t res;
		res.set_data(FP_numeric::shift_fract((typename __res_t::base_type)(lhs.to_raw()), Fr, Fl)
		             + (typename __res_t::base_type)(rhs.to_raw()));
		return res;
	}
	template <size_t Tl, size_t Tr, size_t F>
	inline typename FP_numeric::add_res_type_d<Tl, F, Tr, F>::type
	add(const Fixed_Point_Static<Tl, F >& lhs, const Fixed_Point_Static<Tr, F >& rhs)
	{
		using __res_t = typename FP_numeric::add_res_type_d<Tl, F, Tr, F>::type;
		__res_t res;
		res.set_data((typename __res_t::base_type)(lhs.to_raw())
		           + (typename __res_t::base_type)(rhs.to_raw()));
		return res;
	}

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	inline typename FP_numeric::sub_res_type_d<Tl, Fl, Tr, Fr>::type
	sub(const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs,
		typename std::enable_if < (Fl > Fr), bool>::type v = 0)
	{
		using __res_t = typename FP_numeric::sub_res_type_d<Tl, Fl, Tr, Fr>::type;
		__res_t res;
		res.set_data((typename __res_t::base_type)(lhs.to_raw())
		             - FP_numeric::shift_fract((typename __res_t::base_type)(rhs.to_raw()), Fl, Fr));
		return res;
	}
	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	inline typename FP_numeric::sub_res_type_d<Tl, Fl, Tr, Fr>::type
	sub(const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs,
		typename std::enable_if < (Fl < Fr), bool>::type v = 0)
	{
		using __res_t = typename FP_numeric::sub_res_type_d<Tl, Fl, Tr, Fr>::type;
		__res_t res;
		res.set_data(FP_numeric::shift_fract((typename __res_t::base_type)(lhs.to_raw()), Fr, Fl)
		             - (typename __res_t::base_type)(rhs.to_raw()));
		return res;
	}
	template <size_t Tl, size_t Tr, size_t F>
	inline typename FP_numeric::sub_res_type_d<Tl, F, Tr, F>::type
	sub(const Fixed_Point_Static<Tl, F >& lhs, const Fixed_Point_Static<Tr, F >& rhs)
	{
		using __res_t = typename FP_numeric::add_res_type_d<Tl, F, Tr, F>::type;
		__res_t res;
		res.set_data((typename __res_t::base_type)(lhs.to_raw())
		           - (typename __res_t::base_type)(rhs.to_raw()));
		return res;
	}

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	inline typename FP_numeric::mul_res_type_d<Tl, Fl, Tr, Fr>::type
	mul(const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs)
	{
		using __res_t = typename FP_numeric::mul_res_type_d<Tl, Fl, Tr, Fr>::type;
		__res_t res;
		res.set_data((typename __res_t::base_type)(lhs.to_raw())
		           * (typename __res_t::base_type)(rhs.to_raw()));
		return res;
	}

	template <size_t Tn, size_t Fn, size_t Td, size_t Fd>
	inline typename FP_numeric::div_res_type_d<Tn, Fn, Td, Fd>::type
	div(const Fixed_Point_Static<Tn, Fn>& numerator, const Fixed_Point_Static<Td, Fd>& denominator)
	{
		if(!denominator)
			throw std::domain_error("Division by zero!");

		using __res_t  = typename FP_numeric::div_res_type_d<Tn, Fn, Td, Fd>::type;
		using big_type = typename __res_t::base_type;
		__res_t quotient;

		// get the data and align them
		// TODO : manage risk of overflow when shifting
		big_type num_data = FP_numeric::shift_fract((big_type)numerator.  get_data(), __res_t::Fb*2, Fn); // resize fractional part
		big_type den_data = FP_numeric::shift_fract((big_type)denominator.get_data(), __res_t::Fb,   Fd); // resize fractional part

		// compute
		quotient.set_data(num_data / den_data);

		return quotient;
	}

	// return the quotient in its given size and compute from that the remainder
	template <size_t Tn, size_t Fn, size_t Td, size_t Fd, size_t Tq, size_t Fq, size_t Tr, size_t Fr>
	inline void
	div(const Fixed_Point_Static<Tn, Fn>& numerator, const Fixed_Point_Static<Td, Fd>& denominator,
	          Fixed_Point_Static<Tq, Fq>& quotient ,       Fixed_Point_Static<Tr, Fr>& remainder)
	{
		quotient  = numerator/denominator;
		remainder = FP_numeric::sub(numerator, FP_numeric::mul(quotient, denominator));
	}

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	inline typename FP_numeric::mod_res_type_d<Tl, Fl, Tr, Fr>::type
	mod(const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs)
	{
		using __res_t = typename FP_numeric::mod_res_type_d<Tl, Fl, Tr, Fr>::type;
		__res_t res;
		// TODO : manage risk of overflow when shifting
		res.set_data((__res_t::base_type)(lhs.shift_fract(res.get_fractional_bits()))
		           % (__res_t::base_type)(rhs.shift_fract(res.get_fractional_bits())));
		return res;
	}

}

template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
inline typename FP_numeric::add_res_type_d<Tl, Fl, Tr, Fr>::type
operator+ (const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs)
{
	return FP_numeric::add(lhs, rhs);
}
template <size_t T, size_t F, typename type>
inline typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator+ (const Fixed_Point_Static<T , F >& lhs, const type                        rhs)
{
	return lhs + (Fixed_Point_Static<T, F>)rhs;
}
template <size_t T, size_t F, typename type>
inline typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator+ (const type                        lhs, const Fixed_Point_Static<T , F >& rhs)
{
	return (Fixed_Point_Static<T, F>)lhs + rhs;
}

template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
inline typename FP_numeric::sub_res_type_d<Tl, Fl, Tr, Fr>::type
operator- (const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs)
{
	return FP_numeric::sub(lhs, rhs);
}
template <size_t T, size_t F, typename type>
inline typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator- (const Fixed_Point_Static<T , F >& lhs, const type                        rhs)
{
	return lhs - (Fixed_Point_Static<T, F>)rhs;
}
template <size_t T, size_t F, typename type>
inline typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator- (const type                        lhs, const Fixed_Point_Static<T , F >& rhs)
{
	return (Fixed_Point_Static<T, F>)lhs - rhs;
}

template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
inline typename FP_numeric::mul_res_type_d<Tl, Fl, Tr, Fr>::type
operator* (const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs)
{
	return FP_numeric::mul(lhs, rhs);
}
template <size_t T, size_t F, typename type>
inline typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator* (const Fixed_Point_Static<T , F >& lhs, const type                        rhs)
{
	return lhs * (Fixed_Point_Static<T, F>)rhs;
}
template <size_t T, size_t F, typename type>
inline typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator* (const type                        lhs, const Fixed_Point_Static<T , F >& rhs)
{
	return (Fixed_Point_Static<T, F>)lhs * rhs;
}

template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
inline typename FP_numeric::div_res_type_d<Tl, Fl, Tr, Fr>::type
operator/ (const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs)
{
	return FP_numeric::div(lhs, rhs);
}
template <size_t T, size_t F, typename type>
inline typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator/ (const Fixed_Point_Static<T , F >& lhs, const type                        rhs)
{
	return lhs / (Fixed_Point_Static<T, F>)rhs;
}
template <size_t T, size_t F, typename type>
inline typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator/ (const type                        lhs, const Fixed_Point_Static<T , F >& rhs)
{
	return (Fixed_Point_Static<T, F>)lhs / rhs;
}

template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
inline typename FP_numeric::mod_res_type_d<Tl, Fl, Tr, Fr>::type
operator% (const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs)
{
	return FP_numeric::mod(lhs, rhs);
}
template <size_t T, size_t F, typename type>
inline typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator% (const Fixed_Point_Static<T , F >& lhs, const type                        rhs)
{
	return lhs % (Fixed_Point_Static<T, F>)rhs;
}
template <size_t T, size_t F, typename type>
inline typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator% (const type                        lhs, const Fixed_Point_Static<T , F >& rhs)
{
	return (Fixed_Point_Static<T, F>)lhs % rhs;
}

// shift operators (Fixed_Point rhs is convert as an unsigned integer)
template <size_t Tl, size_t Fl, typename type>
inline Fixed_Point_Static<Tl, Fl>
operator>>(Fixed_Point_Static<Tl, Fl> lhs, const type& rhs)
{
	return lhs >>= rhs;
}
template <size_t Tl, size_t Fl, typename type>
inline Fixed_Point_Static<Tl, Fl>
operator<<(Fixed_Point_Static<Tl, Fl> lhs, const type& rhs)
{
	return lhs <<= rhs;
}

// binary operators between the raw of lhs and rhs
// return an integer whom the size is equivalent to the size of the biggest base_type size between lhs and rhs
template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
inline typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
                                 >= sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type),
                                 typename Fixed_Point_Static<Tl, Fl>::base_type,
                                 typename Fixed_Point_Static<Tr, Fr>::base_type>::type
operator& (const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs)
{
	using __res_t = typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
	                                          >= sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type),
	                                          typename Fixed_Point_Static<Tl, Fl>::base_type,
	                                          typename Fixed_Point_Static<Tr, Fr>::base_type>::type;
	return (__res_t)(lhs.to_raw()) & (__res_t)(rhs.to_raw());
}
template <size_t Tl, size_t Fl, typename type>
inline typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
                                 >= sizeof(type),
                                 typename Fixed_Point_Static<Tl, Fl>::base_type,
                                 type>::type
operator& (const Fixed_Point_Static<Tl, Fl>& lhs, const type                        rhs)
{
	using __res_t = typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
	                                          >= sizeof(type),
	                                          typename Fixed_Point_Static<Tl, Fl>::base_type,
	                                          type>::type;
	return (__res_t)(lhs.to_raw()) & (__res_t)rhs;
}
template <size_t Tr, size_t Fr, typename type>
inline typename std::conditional<sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type)
                                 >= sizeof(type),
                                 typename Fixed_Point_Static<Tr, Fr>::base_type,
                                 type>::type
operator& (const type lhs,                        const Fixed_Point_Static<Tr, Fr>& rhs)
{
	using __res_t = typename std::conditional<sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type)
	                                          >= sizeof(type),
	                                          typename Fixed_Point_Static<Tr, Fr>::base_type,
	                                          type>::type;
	return (__res_t)lhs & (__res_t)(rhs.to_raw());
}

template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
inline typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
                                 >= sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type),
                                 typename Fixed_Point_Static<Tl, Fl>::base_type,
                                 typename Fixed_Point_Static<Tr, Fr>::base_type>::type
operator^ (const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs)
{
	using __res_t = typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
	                                          >= sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type),
	                                          typename Fixed_Point_Static<Tl, Fl>::base_type,
	                                          typename Fixed_Point_Static<Tr, Fr>::base_type>::type;
	return (__res_t)(lhs.to_raw()) ^ (__res_t)(rhs.to_raw());
}
template <size_t Tl, size_t Fl, typename type>
inline typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
                                 >= sizeof(type),
                                 typename Fixed_Point_Static<Tl, Fl>::base_type,
                                 type>::type
operator^ (const Fixed_Point_Static<Tl, Fl>& lhs, const type                        rhs)
{
	using __res_t = typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
	                                          >= sizeof(type),
	                                          typename Fixed_Point_Static<Tl, Fl>::base_type,
	                                          type>::type;
	return (__res_t)(lhs.to_raw()) ^ (__res_t)rhs;
}
template <size_t Tr, size_t Fr, typename type>
inline typename std::conditional<sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type)
                                 >= sizeof(type),
                                 typename Fixed_Point_Static<Tr, Fr>::base_type,
                                 type>::type
operator^ (const type lhs,                        const Fixed_Point_Static<Tr, Fr>& rhs)
{
	using __res_t = typename std::conditional<sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type)
	                                          >= sizeof(type),
	                                          typename Fixed_Point_Static<Tr, Fr>::base_type,
	                                          type>::type;
	return (__res_t)lhs ^ (__res_t)(rhs.to_raw());
}

template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
inline typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
                                 >= sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type),
                                 typename Fixed_Point_Static<Tl, Fl>::base_type,
                                 typename Fixed_Point_Static<Tr, Fr>::base_type>::type
operator| (const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs)
{
	using __res_t = typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
	                                          >= sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type),
	                                          typename Fixed_Point_Static<Tl, Fl>::base_type,
	                                          typename Fixed_Point_Static<Tr, Fr>::base_type>::type;
	return (__res_t)(lhs.to_raw()) | (__res_t)(rhs.to_raw());
}
template <size_t Tl, size_t Fl, typename type>
inline typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
                                 >= sizeof(type),
                                 typename Fixed_Point_Static<Tl, Fl>::base_type,
                                 type>::type
operator| (const Fixed_Point_Static<Tl, Fl>& lhs, const type                        rhs)
{
	using __res_t = typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
	                                          >= sizeof(type),
	                                          typename Fixed_Point_Static<Tl, Fl>::base_type,
	                                          type>::type;
	return (__res_t)(lhs.to_raw()) | (__res_t)rhs;
}
template <size_t Tr, size_t Fr, typename type>
inline typename std::conditional<sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type)
                                 >= sizeof(type),
                                 typename Fixed_Point_Static<Tr, Fr>::base_type,
                                 type>::type
operator| (const type lhs,                        const Fixed_Point_Static<Tr, Fr>& rhs)
{
	using __res_t = typename std::conditional<sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type)
	                                          >= sizeof(type),
	                                          typename Fixed_Point_Static<Tr, Fr>::base_type,
	                                          type>::type;
	return (__res_t)lhs | (__res_t)(rhs.to_raw());
}

// boolean comparison of the raw of lhs and rhs
template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
inline bool
operator&&(const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs)
{
	return lhs.to_raw() && rhs.to_raw();
}
//template <size_t Tl, size_t Fl, typename type>
//inline bool
//operator&&(const Fixed_Point_Static<Tl, Fl>& lhs, const type                        rhs)
//{
//	return lhs.to_raw() && (FP_numeric::base_type)rhs;
//}
//template <size_t Tr, size_t Fr, typename type>
//inline bool
//operator&&(const type                        lhs, const Fixed_Point_Static<Tr, Fr>& rhs)
//{
//	return (FP_numeric::base_type)lhs && rhs.to_raw();
//}

template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
inline bool
operator||(const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs)
{
	return lhs.to_raw() || rhs.to_raw();
}
//template <size_t Tl, size_t Fl, typename type>
//inline bool
//operator||(const Fixed_Point_Static<Tl, Fl>& lhs, const type                        rhs)
//{
//	return lhs.to_raw() || (FP_numeric::base_type)rhs;
//}
//template <size_t Tr, size_t Fr, typename type>
//inline bool
//operator||(const type                        lhs, const Fixed_Point_Static<Tr, Fr>& rhs)
//{
//	return (FP_numeric::base_type)lhs || rhs.to_raw();
//}


/// *********************************************************************************************************************
/// standard library overloads ******************************************************************************************
/// *********************************************************************************************************************
namespace std
{
	template <size_t T, size_t F>
	inline Fixed_Point_Static<T,F> abs      (const Fixed_Point_Static<T,F>& __x)
	{
		return __x.abs();
	}
	template <size_t T, size_t F>
	inline int                     signbit  (const Fixed_Point_Static<T,F>& __x)
	{
		return __x.sign();
	}
	template <size_t T, size_t F>
	inline std::string             to_string(const Fixed_Point_Static<T,F>& __x)
	{
		return std::to_string(__x.to_float());
	}

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	inline typename FP_numeric::max_res_type_d<Tl, Fl, Tr, Fr>::type
	max(const Fixed_Point_Static<Tl,Fl>& lhs, const Fixed_Point_Static<Tr,Fr>& rhs)
	{
		typename FP_numeric::max_res_type_d<Tl, Fl, Tr, Fr>::type res;
		if(lhs > rhs)
			res = lhs;
		else
			res = rhs;
		return res;
	}
	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	inline typename FP_numeric::min_res_type_d<Tl, Fl, Tr, Fr>::type
	min(const Fixed_Point_Static<Tl,Fl>& lhs, const Fixed_Point_Static<Tr,Fr>& rhs)
	{
		typename FP_numeric::min_res_type_d<Tl, Fl, Tr, Fr>::type res;
		if(lhs < rhs)
			res = lhs;
		else
			res = rhs;
		return res;
	}

	template <size_t T, size_t F>
	inline Fixed_Point_Static<T,F>
	max(const Fixed_Point_Static<T,F>& lhs, const Fixed_Point_Static<T,F>& rhs)
	{
		return (lhs > rhs)? lhs : rhs;
	}
	template <size_t T, size_t F>
	inline Fixed_Point_Static<T,F>
	min(const Fixed_Point_Static<T,F>& lhs, const Fixed_Point_Static<T,F>& rhs)
	{
		return (lhs < rhs)? lhs : rhs;
	}


	template<size_t T, size_t F>
	struct numeric_limits<Fixed_Point_Static<T,F>>
	{
		static _GLIBCXX_USE_CONSTEXPR bool is_specialized = true;

		static _GLIBCXX_CONSTEXPR Fixed_Point_Static<T,F>
		min() _GLIBCXX_USE_NOEXCEPT { return Fixed_Point_Static<T,F>::min(); }

		static _GLIBCXX_CONSTEXPR Fixed_Point_Static<T,F>
		max() _GLIBCXX_USE_NOEXCEPT { return Fixed_Point_Static<T,F>::max(); }

		static _GLIBCXX_CONSTEXPR Fixed_Point_Static<T,F>
		lowest() _GLIBCXX_USE_NOEXCEPT { return min(); }
	};
}


/// *********************************************************************************************************************
/// fixed points copy functions  ****************************************************************************************
/// *********************************************************************************************************************
#include <vector>

namespace FP_numeric
{
	/// helpers to check if it is a Fixed_Point_Static
	template<typename>
	struct __is_FP_S_helper : public std::false_type { };
	template<size_t T, size_t F>
	struct __is_FP_S_helper<Fixed_Point_Static<T,F>> : public std::true_type { };
	/// check if it is a Fixed_Point_Static
	template<typename _Tp>
	struct   is_FP_S : public __is_FP_S_helper<typename std::remove_cv<_Tp>::type>::type { };

	/// copy of two vectors
	template<typename _IT, size_t T, size_t F, typename _IA, typename _OA>
	void copy(const std::vector<_IT,                       _IA>& __in_vector, std::vector<Fixed_Point_Static<T ,F >, _OA>& __out_vector)
	{
		assert(__in_vector.size() <= __out_vector.size());
		for (auto i = 0; i < (int)__in_vector.size(); i++)
			__out_vector[i].set_data(__in_vector[i]);
	}
	template<typename _OT, size_t T, size_t F, typename _IA, typename _OA>
	void copy(const std::vector<Fixed_Point_Static<T ,F >, _IA>& __in_vector, std::vector<_OT,                       _OA>& __out_vector)
	{
		assert(__in_vector.size() <= __out_vector.size());
		for (auto i = 0; i < (int)__in_vector.size(); i++)
			__out_vector[i] = (_OT)__in_vector[i];
	}
	template<size_t Ti, size_t Fi, size_t To, size_t Fo, typename _IA, typename _OA>
	void copy(const std::vector<Fixed_Point_Static<Ti,Fi>, _IA>& __in_vector, std::vector<Fixed_Point_Static<To,Fo>, _OA>& __out_vector)
	{
		std::copy(__in_vector.begin(), __in_vector.end(), __out_vector.begin());
	}
	template<typename _IT, typename _OT, typename _IA, typename _OA>
	void copy(const std::vector<_IT,                       _IA>& __in_vector, std::vector<_OT,                       _OA>& __out_vector)
	{
		std::copy(__in_vector.begin(), __in_vector.end(), __out_vector.begin());
	}

	template<typename _II, typename _OI, bool _II_is_FPS, bool _OI_is_FPS>
	struct copy_FPS
	{
		static _OI copy(_II __first, _II __last, _OI __result)
		{
			return std::copy(__first, __last, __result);
		}

		static _OI* copy(const _II* __first, const _II* __last, _OI* __result)
		{
			return std::copy(__first, __last, __result);
		}
	};
	template<typename _II, typename _OI>
	struct copy_FPS<_II,_OI,false,true>
	{
		static _OI copy(_II __first, _II __last, _OI __result)
		{
			// concept requirements
			__glibcxx_function_requires(_InputIteratorConcept<_II>)
			__glibcxx_requires_valid_range(__first, __last);

			for (; __first != __last; ++__result, ++__first)
				__result->set_data(*__first);

			return __result;
		}

		static _OI* copy(const _II* __first, const _II* __last, _OI* __result)
		{
			for (; __first != __last; ++__result, ++__first)
				__result->set_data(*__first);

			return __result;
		}
	};
	template<typename _II, typename _OI>
	struct copy_FPS<_II,_OI,true,false>
	{
		static _OI copy(_II __first, _II __last, _OI __result)
		{
			// concept requirements
			__glibcxx_function_requires(_InputIteratorConcept<_II>)
			__glibcxx_requires_valid_range(__first, __last);

			for (; __first != __last; ++__result, ++__first)
				*__result = (typename _OI::value_type)(*__first);

			return __result;
		}

		static _OI* copy(const _II* __first, const _II* __last, _OI* __result)
		{
			for (; __first != __last; ++__result, ++__first)
				*__result = (_OI)(*__first);

			return __result;
		}
	};

	template<typename _II, typename _OI>
	inline _OI copy(_II __first, _II __last, _OI __result)
	{
		return copy_FPS<_II,_OI,FP_numeric::is_FP_S<typename _II::value_type>::value,FP_numeric::is_FP_S<typename _OI::value_type>::value>
		            ::copy(__first, __last, __result);
	}

	template<typename _IP, typename _OP>
	inline _OP* copy_ptr(const _IP* __first, const _IP* __last, _OP* __result)
	{
		return copy_FPS<_IP,_OP,FP_numeric::is_FP_S<_IP>::value,FP_numeric::is_FP_S<_OP>::value>
		            ::copy(__first, __last, __result);
	}
}

#endif // FIXED_POINT_STATIC_EXTRA_HPP_
