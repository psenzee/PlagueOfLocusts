#pragma once

#include <cstddef>


namespace {

struct _int8  { typedef int8_t  int_t; };
struct _int16 { typedef int16_t int_t; };
struct _int32 { typedef int32_t int_t; };
struct _int64 { typedef int64_t int_t; };

static constexpr size_t _lg(size_t n) { return n <= 1 ? 0 : (1 + _lg(n >> 1)); }

template <size_t N> struct _int_lg     : public _int64 {};
template <>         struct _int_lg<31> : public _int32 {};
template <>         struct _int_lg<30> : public _int32 {};
template <>         struct _int_lg<29> : public _int32 {};
template <>         struct _int_lg<28> : public _int32 {};
template <>         struct _int_lg<27> : public _int32 {};
template <>         struct _int_lg<26> : public _int32 {};
template <>         struct _int_lg<25> : public _int32 {};
template <>         struct _int_lg<24> : public _int32 {};
template <>         struct _int_lg<23> : public _int32 {};
template <>         struct _int_lg<22> : public _int32 {};
template <>         struct _int_lg<21> : public _int32 {};
template <>         struct _int_lg<20> : public _int32 {};
template <>         struct _int_lg<19> : public _int32 {};
template <>         struct _int_lg<18> : public _int32 {};
template <>         struct _int_lg<17> : public _int32 {};
template <>         struct _int_lg<16> : public _int32 {};
template <>         struct _int_lg<15> : public _int16 {};
template <>         struct _int_lg<14> : public _int16 {};
template <>         struct _int_lg<13> : public _int16 {};
template <>         struct _int_lg<12> : public _int16 {};
template <>         struct _int_lg<11> : public _int16 {};
template <>         struct _int_lg<10> : public _int16 {};
template <>         struct _int_lg<9>  : public _int16 {};
template <>         struct _int_lg<8>  : public _int16 {};
template <>         struct _int_lg<7>  : public _int8  {};
template <>         struct _int_lg<6>  : public _int8  {};
template <>         struct _int_lg<5>  : public _int8  {};
template <>         struct _int_lg<4>  : public _int8  {};
template <>         struct _int_lg<3>  : public _int8  {};
template <>         struct _int_lg<2>  : public _int8  {};
template <>         struct _int_lg<1>  : public _int8  {};

}

template <size_t N> struct int_t_for_max : public _int_lg<_lg(N)> {};
