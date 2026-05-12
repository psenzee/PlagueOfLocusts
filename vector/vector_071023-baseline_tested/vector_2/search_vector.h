#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include "stl.h"
#include "key.h"
#include "utils.h"

template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN, size_t LongKeyRanksN = 16>
struct search_vector
{
    typedef std::array<uint64_t, (N >> 6) * 2 + 1>           key_bits_t;
    typedef key_index<FloatT, N, IndexT, KeyRanksN>      key_index_t;
    typedef key<FloatT, N, IndexT, KeyRanksN>            key_t;
    typedef key<FloatT, N, IndexT, LongKeyRanksN>        long_key_t;
    typedef key_id<FloatT, N, IndexT, KeyRanksN>         key_id_t;
    typedef std::array<FloatT, N>                        flat_vector_t;

    const flat_vector_t &vector;
    key_id_t             key_id;
    long_key_t           long_key;
    key_bits_t           key_bits,
                         long_bits;
    
    inline search_vector(const flat_vector_t &search) : vector(search), key_id(key_t(search), 0), long_key(search), key_bits({ 0 }), long_bits({ 0 })
    {
        bits_encode_signed(key_id.key.ranks, key_bits);
        bits_encode_signed(key_id.key.ranks, long_bits);
    }

    inline size_t common_ranks_long(const key_t &other)    const { return bits_count_intersection(long_bits, other.ranks); }
    inline size_t common_ranks_long(const key_id_t &other) const { return common_ranks_long(other.key); }
    inline size_t common_ranks(const key_t &other)         const { return bits_count_intersection(key_bits, other.ranks); }
    inline size_t common_ranks(const key_id_t &other)      const { return common_ranks(other.key); }
};
