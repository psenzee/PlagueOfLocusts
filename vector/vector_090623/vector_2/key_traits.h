#pragma once

#include <cstddef>


namespace {
struct _int8  { typedef int8_t  int_t; };
struct _int16 { typedef int16_t int_t; };
struct _int32 { typedef int32_t int_t; };
}

template <size_t N> struct int_lg             : public _int32 {};
template <>         struct int_lg<15>         : public _int16 {};
template <>         struct int_lg<14>         : public _int16 {};
template <>         struct int_lg<13>         : public _int16 {};
template <>         struct int_lg<12>         : public _int16 {};
template <>         struct int_lg<11>         : public _int16 {};
template <>         struct int_lg<10>         : public _int16 {};
template <>         struct int_lg<9>          : public _int16 {};
template <>         struct int_lg<8>          : public _int16 {};
template <>         struct int_lg<7>          : public _int8  {};
template <>         struct int_lg<6>          : public _int8  {};
template <>         struct int_lg<5>          : public _int8  {};
template <>         struct int_lg<4>          : public _int8  {};
template <>         struct int_lg<3>          : public _int8  {};
template <>         struct int_lg<2>          : public _int8  {};
template <>         struct int_lg<1>          : public _int8  {};

/*
namespace {
struct _signed_int8  { typedef int8_t  int_t; };
struct _signed_int16 { typedef int16_t int_t; };
}

template <size_t N> struct _signed_traits    : public _signed_int16 {};
template <>         struct key_n_traits<128> : public _signed_int8  {};
template <>         struct key_n_traits<120> : public _signed_int8  {};
template <>         struct key_n_traits<112> : public _signed_int8  {};
template <>         struct key_n_traits<96>  : public _signed_int8  {};
template <>         struct key_n_traits<80>  : public _signed_int8  {};
template <>         struct key_n_traits<64>  : public _key_n_small_size  {};
template <>         struct key_n_traits<56>  : public _key_n_small_size  {};
template <>         struct key_n_traits<48>  : public _key_n_small_size  {};
template <>         struct key_n_traits<40>  : public _key_n_small_size  {};
template <>         struct key_n_traits<32>  : public _key_n_small_size  {};
template <>         struct key_n_traits<28>  : public _key_n_small_size  {};
template <>         struct key_n_traits<27>  : public _key_n_small_size  {};
template <>         struct key_n_traits<26>  : public _key_n_small_size  {};
template <>         struct key_n_traits<25>  : public _key_n_small_size  {};
template <>         struct key_n_traits<24>  : public _key_n_small_size  {};
template <>         struct key_n_traits<23>  : public _key_n_small_size  {};
template <>         struct key_n_traits<22>  : public _key_n_small_size  {};
template <>         struct key_n_traits<21>  : public _key_n_small_size  {};
template <>         struct key_n_traits<20>  : public _key_n_small_size  {};
template <>         struct key_n_traits<19>  : public _key_n_small_size  {};
template <>         struct key_n_traits<18>  : public _key_n_small_size  {};
template <>         struct key_n_traits<17>  : public _key_n_small_size  {};
template <>         struct key_n_traits<16>  : public _key_n_small_size  {};
template <>         struct key_n_traits<15>  : public _key_n_small_size  {};
template <>         struct key_n_traits<14>  : public _key_n_small_size  {};
template <>         struct key_n_traits<13>  : public _key_n_small_size  {};
template <>         struct key_n_traits<12>  : public _key_n_small_size  {};
template <>         struct key_n_traits<11>  : public _key_n_small_size  {};
template <>         struct key_n_traits<10>  : public _key_n_small_size  {};
template <>         struct key_n_traits<9>   : public _key_n_small_size  {};
template <>         struct key_n_traits<8>   : public _key_n_small_size  {};
template <>         struct key_n_traits<7>   : public _key_n_small_size  {};
template <>         struct key_n_traits<6>   : public _key_n_small_size  {};
template <>         struct key_n_traits<5>   : public _key_n_small_size  {};
template <>         struct key_n_traits<4>   : public _key_n_small_size  {};
template <>         struct key_n_traits<3>   : public _key_n_small_size  {};
template <>         struct key_n_traits<2>   : public _key_n_small_size  {};
template <>         struct key_n_traits<1>   : public _key_n_small_size  {};


namespace {
struct _key_n_small_size  { typedef int8_t  index_t; };
struct _key_n_medium_size { typedef int16_t index_t; };
}

template <size_t N> struct key_n_traits      : public _key_n_medium_size {};
template <>         struct key_n_traits<128> : public _key_n_small_size  {};
template <>         struct key_n_traits<120> : public _key_n_small_size  {};
template <>         struct key_n_traits<112> : public _key_n_small_size  {};
template <>         struct key_n_traits<96>  : public _key_n_small_size  {};
template <>         struct key_n_traits<80>  : public _key_n_small_size  {};
template <>         struct key_n_traits<64>  : public _key_n_small_size  {};
template <>         struct key_n_traits<56>  : public _key_n_small_size  {};
template <>         struct key_n_traits<48>  : public _key_n_small_size  {};
template <>         struct key_n_traits<40>  : public _key_n_small_size  {};
template <>         struct key_n_traits<32>  : public _key_n_small_size  {};
template <>         struct key_n_traits<28>  : public _key_n_small_size  {};
template <>         struct key_n_traits<27>  : public _key_n_small_size  {};
template <>         struct key_n_traits<26>  : public _key_n_small_size  {};
template <>         struct key_n_traits<25>  : public _key_n_small_size  {};
template <>         struct key_n_traits<24>  : public _key_n_small_size  {};
template <>         struct key_n_traits<23>  : public _key_n_small_size  {};
template <>         struct key_n_traits<22>  : public _key_n_small_size  {};
template <>         struct key_n_traits<21>  : public _key_n_small_size  {};
template <>         struct key_n_traits<20>  : public _key_n_small_size  {};
template <>         struct key_n_traits<19>  : public _key_n_small_size  {};
template <>         struct key_n_traits<18>  : public _key_n_small_size  {};
template <>         struct key_n_traits<17>  : public _key_n_small_size  {};
template <>         struct key_n_traits<16>  : public _key_n_small_size  {};
template <>         struct key_n_traits<15>  : public _key_n_small_size  {};
template <>         struct key_n_traits<14>  : public _key_n_small_size  {};
template <>         struct key_n_traits<13>  : public _key_n_small_size  {};
template <>         struct key_n_traits<12>  : public _key_n_small_size  {};
template <>         struct key_n_traits<11>  : public _key_n_small_size  {};
template <>         struct key_n_traits<10>  : public _key_n_small_size  {};
template <>         struct key_n_traits<9>   : public _key_n_small_size  {};
template <>         struct key_n_traits<8>   : public _key_n_small_size  {};
template <>         struct key_n_traits<7>   : public _key_n_small_size  {};
template <>         struct key_n_traits<6>   : public _key_n_small_size  {};
template <>         struct key_n_traits<5>   : public _key_n_small_size  {};
template <>         struct key_n_traits<4>   : public _key_n_small_size  {};
template <>         struct key_n_traits<3>   : public _key_n_small_size  {};
template <>         struct key_n_traits<2>   : public _key_n_small_size  {};
template <>         struct key_n_traits<1>   : public _key_n_small_size  {};

*/
