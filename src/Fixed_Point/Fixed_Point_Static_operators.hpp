#ifndef FIXED_POINT_STATIC_OPERATORS_HPP_
#define FIXED_POINT_STATIC_OPERATORS_HPP_

#include "Fixed_Point_Static.hpp"

/// *********************************************************************************************************************
/// Display operator that indicates the used quantification *************************************************************
/// *********************************************************************************************************************

template <size_t T, size_t F>
std::ostream& operator<<(std::ostream& os, const Fixed_Point_Static<T,F>& f);

/// *********************************************************************************************************************
/// Fixed point arithmetics and bitwise operators ***********************************************************************
/// *********************************************************************************************************************

namespace FP_numeric
{
	// add, sub, mul or div lhs and rhs
	// return the result with its automatically given size for the computation
	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	typename FP_numeric::add_res_type_d<Tl, Fl, Tr, Fr>::type
	add(const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs,
		typename std::enable_if < (Fl > Fr), bool>::type v = 0);

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	typename FP_numeric::add_res_type_d<Tl, Fl, Tr, Fr>::type
	add(const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs,
		typename std::enable_if < (Fl < Fr), bool>::type v = 0);

	template <size_t Tl, size_t Tr, size_t F>
	typename FP_numeric::add_res_type_d<Tl, F, Tr, F>::type
	add(const Fixed_Point_Static<Tl, F >& lhs, const Fixed_Point_Static<Tr, F >& rhs);

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	typename FP_numeric::sub_res_type_d<Tl, Fl, Tr, Fr>::type
	sub(const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs,
		typename std::enable_if < (Fl > Fr), bool>::type v = 0);

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	typename FP_numeric::sub_res_type_d<Tl, Fl, Tr, Fr>::type
	sub(const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs,
		typename std::enable_if < (Fl < Fr), bool>::type v = 0);

	template <size_t Tl, size_t Tr, size_t F>
	typename FP_numeric::sub_res_type_d<Tl, F, Tr, F>::type
	sub(const Fixed_Point_Static<Tl, F >& lhs, const Fixed_Point_Static<Tr, F >& rhs);

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	typename FP_numeric::mul_res_type_d<Tl, Fl, Tr, Fr>::type
	mul(const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs);

	template <size_t Tn, size_t Fn, size_t Td, size_t Fd>
	typename FP_numeric::div_res_type_d<Tn, Fn, Td, Fd>::type
	div(const Fixed_Point_Static<Tn, Fn>& numerator, const Fixed_Point_Static<Td, Fd>& denominator);

	// return the quotient in its given size and compute from that the remainder
	template <size_t Tn, size_t Fn, size_t Td, size_t Fd, size_t Tq, size_t Fq, size_t Tr, size_t Fr>
	void
	div(const Fixed_Point_Static<Tn, Fn>& numerator, const Fixed_Point_Static<Td, Fd>& denominator,
	          Fixed_Point_Static<Tq, Fq>& quotient ,       Fixed_Point_Static<Tr, Fr>& remainder);

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	typename FP_numeric::mod_res_type_d<Tl, Fl, Tr, Fr>::type
	mod(const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs);
}

template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
typename FP_numeric::add_res_type_d<Tl, Fl, Tr, Fr>::type
operator+ (const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs);

template <size_t T, size_t F, typename type>
typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator+ (const Fixed_Point_Static<T , F >& lhs, const type                        rhs);

template <size_t T, size_t F, typename type>
typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator+ (const type                        lhs, const Fixed_Point_Static<T , F >& rhs);

template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
typename FP_numeric::sub_res_type_d<Tl, Fl, Tr, Fr>::type
operator- (const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs);

template <size_t T, size_t F, typename type>
typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator- (const Fixed_Point_Static<T , F >& lhs, const type                        rhs);

template <size_t T, size_t F, typename type>
typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator- (const type                        lhs, const Fixed_Point_Static<T , F >& rhs);

template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
typename FP_numeric::mul_res_type_d<Tl, Fl, Tr, Fr>::type
operator* (const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs);

template <size_t T, size_t F, typename type>
typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator* (const Fixed_Point_Static<T , F >& lhs, const type                        rhs);

template <size_t T, size_t F, typename type>
typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator* (const type                        lhs, const Fixed_Point_Static<T , F >& rhs);

template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
typename FP_numeric::div_res_type_d<Tl, Fl, Tr, Fr>::type
operator/ (const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs);

template <size_t T, size_t F, typename type>
typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator/ (const Fixed_Point_Static<T , F >& lhs, const type                        rhs);

template <size_t T, size_t F, typename type>
typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator/ (const type                        lhs, const Fixed_Point_Static<T , F >& rhs);

template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
typename FP_numeric::mod_res_type_d<Tl, Fl, Tr, Fr>::type
operator% (const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs);

template <size_t T, size_t F, typename type>
typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator% (const Fixed_Point_Static<T , F >& lhs, const type                        rhs);

template <size_t T, size_t F, typename type>
typename FP_numeric::add_res_type_d<T, F, T, F>::type
operator% (const type                        lhs, const Fixed_Point_Static<T , F >& rhs);

// shift operators (Fixed_Point rhs is convert as an unsigned integer)
template <size_t Tl, size_t Fl, typename type>
Fixed_Point_Static<Tl, Fl>
operator>>(Fixed_Point_Static<Tl, Fl> lhs, const type& rhs);

template <size_t Tl, size_t Fl, typename type>
Fixed_Point_Static<Tl, Fl>
operator<<(Fixed_Point_Static<Tl, Fl> lhs, const type& rhs);

// binary operators between the raw of lhs and rhs
// return an integer whom the size is equivalent to the size of the biggest base_type size between lhs and rhs
template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
                                 >= sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type),
                                 typename Fixed_Point_Static<Tl, Fl>::base_type,
                                 typename Fixed_Point_Static<Tr, Fr>::base_type>::type
operator& (const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs);

template <size_t Tl, size_t Fl, typename type>
typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
                                 >= sizeof(type),
                                 typename Fixed_Point_Static<Tl, Fl>::base_type,
                                 type>::type
operator& (const Fixed_Point_Static<Tl, Fl>& lhs, const type                        rhs);

template <size_t Tr, size_t Fr, typename type>
typename std::conditional<sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type)
                                 >= sizeof(type),
                                 typename Fixed_Point_Static<Tr, Fr>::base_type,
                                 type>::type
operator& (const type lhs,                        const Fixed_Point_Static<Tr, Fr>& rhs);

template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
                                 >= sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type),
                                 typename Fixed_Point_Static<Tl, Fl>::base_type,
                                 typename Fixed_Point_Static<Tr, Fr>::base_type>::type
operator^ (const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs);

template <size_t Tl, size_t Fl, typename type>
typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
                                 >= sizeof(type),
                                 typename Fixed_Point_Static<Tl, Fl>::base_type,
                                 type>::type
operator^ (const Fixed_Point_Static<Tl, Fl>& lhs, const type                        rhs);

template <size_t Tr, size_t Fr, typename type>
typename std::conditional<sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type)
                                 >= sizeof(type),
                                 typename Fixed_Point_Static<Tr, Fr>::base_type,
                                 type>::type
operator^ (const type lhs,                        const Fixed_Point_Static<Tr, Fr>& rhs);

template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
                                 >= sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type),
                                 typename Fixed_Point_Static<Tl, Fl>::base_type,
                                 typename Fixed_Point_Static<Tr, Fr>::base_type>::type
operator| (const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs);

template <size_t Tl, size_t Fl, typename type>
typename std::conditional<sizeof(typename Fixed_Point_Static<Tl, Fl>::base_type)
                                 >= sizeof(type),
                                 typename Fixed_Point_Static<Tl, Fl>::base_type,
                                 type>::type
operator| (const Fixed_Point_Static<Tl, Fl>& lhs, const type                        rhs);

template <size_t Tr, size_t Fr, typename type>
typename std::conditional<sizeof(typename Fixed_Point_Static<Tr, Fr>::base_type)
                                 >= sizeof(type),
                                 typename Fixed_Point_Static<Tr, Fr>::base_type,
                                 type>::type
operator| (const type lhs,                        const Fixed_Point_Static<Tr, Fr>& rhs);

// boolean comparison of the raw of lhs and rhs
template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
bool
operator&&(const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs);

template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
bool
operator||(const Fixed_Point_Static<Tl, Fl>& lhs, const Fixed_Point_Static<Tr, Fr>& rhs);


/// ********************************************************************************************************************
/// standard library overloads *****************************************************************************************
/// ********************************************************************************************************************

namespace std
{
	template <size_t T, size_t F>
	Fixed_Point_Static<T,F> abs      (const Fixed_Point_Static<T,F>& __x);
	template <size_t T, size_t F>
	int                     signbit  (const Fixed_Point_Static<T,F>& __x);
	template <size_t T, size_t F>
	std::string             to_string(const Fixed_Point_Static<T,F>& __x);

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	typename FP_numeric::comp_res_type_d<Tl, Fl, Tr, Fr>::type
	max(const Fixed_Point_Static<Tl,Fl>& lhs, const Fixed_Point_Static<Tr,Fr>& rhs);

	template <size_t Tl, size_t Fl, size_t Tr, size_t Fr>
	typename FP_numeric::comp_res_type_d<Tl, Fl, Tr, Fr>::type
	min(const Fixed_Point_Static<Tl,Fl>& lhs, const Fixed_Point_Static<Tr,Fr>& rhs);

	template <size_t T, size_t F>
	Fixed_Point_Static<T,F>
	max(const Fixed_Point_Static<T,F>& lhs, const Fixed_Point_Static<T,F>& rhs);

	template <size_t T, size_t F>
	Fixed_Point_Static<T,F>
	min(const Fixed_Point_Static<T,F>& lhs, const Fixed_Point_Static<T,F>& rhs);

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
	void copy(const std::vector<_IT,                       _IA>& __in_vector, std::vector<Fixed_Point_Static<T ,F >, _OA>& __out_vector);

	template<typename _OT, size_t T, size_t F, typename _IA, typename _OA>
	void copy(const std::vector<Fixed_Point_Static<T ,F >, _IA>& __in_vector, std::vector<_OT,                       _OA>& __out_vector);

	template<size_t Ti, size_t Fi, size_t To, size_t Fo, typename _IA, typename _OA>
	void copy(const std::vector<Fixed_Point_Static<Ti,Fi>, _IA>& __in_vector, std::vector<Fixed_Point_Static<To,Fo>, _OA>& __out_vector);

	template<typename _IT, typename _OT, typename _IA, typename _OA>
	void copy(const std::vector<_IT,                       _IA>& __in_vector, std::vector<_OT,                       _OA>& __out_vector);

	template<typename _II, typename _OI>
	_OI copy(_II __first, _II __last, _OI __result);

	template<typename _IP, typename _OP>
	_OP* copy_ptr(const _IP* __first, const _IP* __last, _OP* __result);
}

#include "Fixed_Point_Static_operators.hxx"

#endif // FIXED_POINT_STATIC_OPERATORS_HPP_
