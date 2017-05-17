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
	inline T saturate(const T val, const T min, const T max, const FP_numeric::Arithmetic_type ar_type)
	{
		switch(ar_type)
		{
	//		case FP_numeric::Arithmetic_type::CYCLIC:
	//			return __data & __max;

	//		case FP_numeric::Arithmetic_type::UNSIGNED_SATURATED:
			case FP_numeric::Arithmetic_type::SIGNED_SATURATED:
				return std::min(std::max(val, min), max);
		}

		return val;
	}
	// return the given value val saturated (if needed) at max in function of the Arithmetic_type
	template <typename T>
	inline T saturate_max(const T val, const T max, const FP_numeric::Arithmetic_type ar_type)
	{
		switch(ar_type)
		{
	//		case FP_numeric::Arithmetic_type::CYCLIC:
	//			return __data & __max;

	//		case FP_numeric::Arithmetic_type::UNSIGNED_SATURATED:
			case FP_numeric::Arithmetic_type::SIGNED_SATURATED:
				return std::min(val, max);
		}

		return val;
	}
	// return the given value val saturated (if needed) at min in function of the Arithmetic_type
	template <typename T>
	inline T saturate_min(const T val, const T min, const FP_numeric::Arithmetic_type ar_type)
	{
		switch(ar_type)
		{
	//		case FP_numeric::Arithmetic_type::CYCLIC:
	//			return __data & __max;

	//		case FP_numeric::Arithmetic_type::UNSIGNED_SATURATED:
			case FP_numeric::Arithmetic_type::SIGNED_SATURATED:
				return std::max(val, min);
		}

		return val;
	}

	// return the value of data with the point set at the new_fract_bits position that is originally set at old_fract_bits
	template <typename T>
	inline T shift_fract(const T data, const size_t new_fract_bits, const size_t old_fract_bits)
	{
		return ((old_fract_bits > new_fract_bits) ? (data >> (old_fract_bits - new_fract_bits))
		                                          : (data << (new_fract_bits - old_fract_bits)));
	}

	// return the given value on its T type and its given quantification but with its sign extended on the most
	// significant bits of T
	template <typename T>
	inline T sign_extension(T data, const size_t total_bits)
	{
		const T extension_mask = (~T(0)) << total_bits;

		if (data & (T(1) << (total_bits-1))) // if signed
			data |= extension_mask;
		else
			data &= ~extension_mask;

		return data;
	}

	template <typename FP>
	inline std::string display(const FP& n, int precision)
	{
		std::stringstream sstream;

		sstream << std::setprecision(precision) << std::setw(precision+2) << n.to_double();
		std::string value_string = sstream.str();

		size_t pos = value_string.find('e');
		if (pos != std::string::npos) // then scientific notation has been used (too big number)
			value_string = value_string.substr(0, precision-2) + value_string.substr(pos);
		else
			value_string = value_string.substr(0, precision+2);

		return value_string;
	}

	template <typename FP>
	inline std::string display_quantification(const FP& n)
	{
		std::string str = std::string("(") + ((n.get_total_bits()<10) ? std::string(" "): std::string(""))
		                + std::to_string(n.get_total_bits()) + std::string(",")
		                + ((n.get_fractional_bits()<10) ? std::string(" "): std::string(""))
		                + std::to_string(n.get_fractional_bits()) + std::string(",");
		switch(n.get_arithmetic_type())
		{
	//		case FP_numeric::Arithmetic_type::CYCLIC:
	//			str += std::string(" c");
	//		break;

	//		case FP_numeric::Arithmetic_type::UNSIGNED_SATURATED:
	//			str += std::string(" u");
	//		break;
		case FP_numeric::Arithmetic_type::SIGNED_SATURATED:
			str += std::string(" s");
		break;
		}

		str += std::string(")" );

		return str;
	}

	//display resume of this fixed point
	template <typename FP>
	inline std::string       resume(const FP& n)
	{
		std::string str;
		str += "value  = "; str += std::to_string(n.to_double()) + n.display_quantification() + std::string("\n");
		str += "sign   = "; str += (n.sign() ? std::string("neg") : std::string("pos"));
		str += ", abs = " ; str += std::to_string(n.abs().to_double()) + std::string("\n");
		str += "int    = "; str += std::to_string(n.to_int());
		str += ", uint = "; str += std::to_string(n.to_uint()) + std::string("\n");
		str += "bin    = "; str += std::bitset<FP::capacity>(n.get_data()).to_string()
		                        +  std::string(" (") + std::to_string((int64_t)(n.get_data())) + std::string(")\n");
		str += "hex    = "; str += n.to_hex() + std::string("\n");
		str += "f mask = "; str += std::bitset<FP::capacity>(n.get_fractional_mask()).to_string()
		                        +  std::string(" (") + std::to_string(n.get_fractional_bits()) + std::string(")\n");
		str += "i mask = "; str += std::bitset<FP::capacity>(n.get_integer_mask()).to_string()
		                        +  std::string(" (") + std::to_string(n.get_integer_bits()) + std::string(")\n");
		str += "n mask = "; str += std::bitset<FP::capacity>(n.get_number_mask()).to_string() +std::string("\n");
		str += "one    = "; str += std::bitset<FP::capacity>(n.get_one()).to_string()
		                        +  std::string(" (") + std::to_string(n.one().to_double()) + std::string(")\n");
		str += "max    = "; str += std::bitset<FP::capacity>(n.get_max()).to_string()
		                        +  std::string(" (") + std::to_string(n.max().to_double()) + std::string(")\n");
		str += "min    = "; str += std::bitset<FP::capacity>(n.get_min()).to_string()
		                        +  std::string(" (") + std::to_string(n.min().to_double()) + std::string(")\n");
		str += std::string("\n");
		return str;
	}
	template <typename FP>
	inline std::string short_resume(const FP& n)
	{
		std::string str;
		str += "value  = "; str += std::to_string(n.to_double()) + n.display_quantification() + std::string("\n");
		str += "bin    = "; str += std::bitset<FP::capacity>((int64_t)n.get_data()).to_string()
		                        +  std::string(" (") + std::to_string(n.get_data()) + std::string(")\n");

		str += "mask   = ";

		for (int var = sizeof(typename FP::base_type)*8-1; var >= 0; --var) {
			typename FP::base_type mask = (typename FP::base_type)1 << var;

			if(n.get_number_mask() & mask)
			{
				if(n.get_fractional_mask() & mask)
					str += "F";

				else if(n.get_integer_mask() & mask)
					str += "I";

				else
					str += "S";
			}
			else
				str += "0";
		}
		str += std::string("\n");
		str += std::string("\n");
		return str;
	}

}

#endif /* FIXED_POINT_UTILS_HPP_ */
