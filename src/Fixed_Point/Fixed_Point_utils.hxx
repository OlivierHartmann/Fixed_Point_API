#ifndef FIXED_POINT_UTILS_HXX_
#define FIXED_POINT_UTILS_HXX_

#if defined(__GNUC__) && defined(__x86_64__)

inline std::string std::to_string(__int128_t __val)
{
	__uint128_t tmp = __val < 0 ? -__val : __val;
    char buffer[ 128 ];
    char* d = std::end( buffer );
    do
    {
        -- d;
        *d = "0123456789"[ tmp % 10 ];
        tmp /= 10;
    } while ( tmp != 0 );

    if ( __val < 0 )
    {
        -- d;
        *d = '-';
    }
    int len = std::end( buffer ) - d;

    string s(d, len);
	return s;
}

inline std::string std::to_string(__uint128_t __val)
{
	string s = __gnu_cxx::__to_xstring<string>(&std::vsnprintf, 4 * sizeof(__int128)/2,
				   "%d", (uint64_t)__val);
	s += __gnu_cxx::__to_xstring<string>(&std::vsnprintf, 4 * sizeof(__int128)/2,
				   "%d", (uint64_t)(__val >> 64));

	return s;
}

template<class stream_class>
inline stream_class& operator<<(stream_class& s, __int128_t __val)
{
	s << (int64_t)(__val >> 64);
	s << (int64_t)__val;

	return s;
}

template<class stream_class>
inline stream_class& operator<<(stream_class& s, __uint128_t __val)
{
	s << (uint64_t)(__val >> 64);
	s << (uint64_t)__val;

	return s;
}

#endif

// return the given value val saturated (if needed) between min and max in function of the Arithmetic_type
template <typename T>
inline T FP_numeric::saturate(const T val, const T min, const T max, const FP_numeric::Arithmetic_type ar_type)
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
inline T FP_numeric::saturate_max(const T val, const T max, const FP_numeric::Arithmetic_type ar_type)
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
inline T FP_numeric::saturate_min(const T val, const T min, const FP_numeric::Arithmetic_type ar_type)
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
inline T FP_numeric::shift_fract(const T data, const size_t new_fract_bits, const size_t old_fract_bits)
{
	return ((old_fract_bits > new_fract_bits) ? (data >> (old_fract_bits - new_fract_bits))
	                                          : (data << (new_fract_bits - old_fract_bits)));
}

// return the given value on its T type and its given quantification but with its sign extended on the most
// significant bits of T
template <typename T>
inline T FP_numeric::sign_extension(T data, const size_t total_bits)
{
	const T extension_mask = (~T(0)) << total_bits;

	if (data & (T(1) << (total_bits-1))) // if signed
		data |= extension_mask;
	else
		data &= ~extension_mask;

	return data;
}

template <typename FP>
inline std::string FP_numeric::display(const FP& n, int precision)
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
inline std::string FP_numeric::display_quantification(const FP& n)
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
inline std::string FP_numeric::resume(const FP& n)
{
	std::string str;
	str += "value  = "; str += std::to_string(n.to_double()) + n.display_quantification() + std::string("\n");
	str += "sign   = "; str += (n.sign() ? std::string("neg") : std::string("pos"));
	str += ", abs = " ; str += std::to_string(n.abs().to_double()) + std::string("\n");
	str += "bin    = "; str += n.to_bin() + std::string(" (") + std::to_string(n.get_data()) + std::string(")\n");
	str += "hex    = "; str += n.to_hex() + std::string("\n");
	str += "f mask = "; str += to_bin(n.get_fractional_mask())
	                        +  std::string(" (") + std::to_string(n.get_fractional_bits()) + std::string(")\n");
	str += "i mask = "; str += to_bin(n.get_integer_mask())
	                        +  std::string(" (") + std::to_string(n.get_integer_bits()) + std::string(")\n");
	str += "n mask = "; str += to_bin(n.get_number_mask()) + std::string("\n");
	str += "one    = "; str += n.one().to_bin()
	                        +  std::string(" (") + std::to_string(n.one().to_double()) + std::string(")\n");
	str += "max    = "; str += n.max().to_bin()
	                        +  std::string(" (") + std::to_string(n.max().to_double()) + std::string(")\n");
	str += "min    = "; str += n.min().to_bin()
	                        +  std::string(" (") + std::to_string(n.min().to_double()) + std::string(")\n");
	str += std::string("\n");
	return str;
}

template <typename FP>
inline std::string FP_numeric::short_resume(const FP& n)
{
	std::string str;
	str += "value  = "; str += std::to_string(n.to_double()) + n.display_quantification() + std::string("\n");
	str += "bin    = "; str += n.to_sbin() +  std::string(" (") + std::to_string(n.get_data()) + std::string(")\n");

	str += "mask   = "; str += "S"; str.append(n.get_integer_bits(), 'I');  str.append(n.get_fractional_bits(), 'F');
	                    str += std::string("\n");
	str += "hex    = "; str += n.to_shex() + std::string("\n");

	str += std::string("\n");
	return str;
}


template <typename T>
inline std::string FP_numeric::to_bin(const T data)
{
	return to_bin(data, sizeof(T)*8);
}

template <typename T>
inline std::string FP_numeric::to_bin(const T data, const size_t n_bits)
{
	std::string s(n_bits, '0');

	auto s_it = s.end() -1;

	for(unsigned i = 0; i < n_bits; i++)
	{
		if(data >> i & (T)1)
			*s_it = '1';
		s_it--;
	}

	return s;
}

template <typename T>
inline std::string FP_numeric::to_hex(const T data)
{
	return to_hex(data, sizeof(T)*8);
}

template <typename T>
inline std::string FP_numeric::to_hex(const T data, const size_t n_bits)
{
	size_t n_mibytes = n_bits/4 + (n_bits%4 ? 1 : 0);

	std::stringstream ss;
	ss << std::setfill('0') << std::setw(n_mibytes) << std::hex
	   << (static_cast<typename std::conditional<sizeof(T) == 1, int, T>::type>(data));

	std::string str;
	ss >> str;

	return str.substr(str.size()-n_mibytes);
}
#endif /* FIXED_POINT_UTILS_HXX_ */
