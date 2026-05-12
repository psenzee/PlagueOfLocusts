#pragma once

#include <cstddef>

namespace {
static constexpr size_t _lg(size_t n) { return n <= 1 ? 0 : (1 + _lg(n >> 1)); }
}

template <size_t N> struct int_for_bits      : public int_for_bits<N - 1> {};
template <>         struct int_for_bits<33>  { typedef int64_t signed_t; typedef uint64_t unsigned_t; };
template <>         struct int_for_bits<32>  { typedef int64_t signed_t; typedef uint32_t unsigned_t; };
template <>         struct int_for_bits<17>  { typedef int32_t signed_t; typedef uint32_t unsigned_t; };
template <>         struct int_for_bits<16>  { typedef int32_t signed_t; typedef uint16_t unsigned_t; };
template <>         struct int_for_bits<9>   { typedef int16_t signed_t; typedef uint16_t unsigned_t; };
template <>         struct int_for_bits<8>   { typedef int16_t signed_t; typedef uint8_t  unsigned_t; };
template <>         struct int_for_bits<0>   { typedef int8_t  signed_t; typedef uint8_t  unsigned_t; };

template <size_t N> struct int_for_max_value : public int_for_bits<_lg(N)> {};
