#pragma once

#include <cstddef>

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
template <>         struct key_n_traits<24>  : public _key_n_small_size  {};
template <>         struct key_n_traits<16>  : public _key_n_small_size  {};
template <>         struct key_n_traits<12>  : public _key_n_small_size  {};
template <>         struct key_n_traits<8>   : public _key_n_small_size  {};
