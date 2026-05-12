#pragma once

#include <limits>
#include <cmath>
#include <array>
#include <algorithm>
#include <cstddef>

#include "pack_bits.h"

template <size_t Dimensions, size_t Ranks, typename FloatT, typename IndexT>
struct ranks
{
    enum { RANKS = Ranks, DIMENSIONS = Dimensions };

    typedef FloatT                               float_t;
    typedef IndexT                               index_t;
    typedef std::array<float_t, DIMENSIONS>      vector_t;
    typedef std::pair<float_t, index_t>          rank_pair_t;
    typedef std::array<rank_pair_t, RANKS>       ranks_t;
    typedef std::array<int16_t, RANKS>           rank_ids_t;
    typedef std::array<rank_pair_t, DIMENSIONS>  ranks_full_t;

    ranks_t data;
    float_t magnitude_sq;

    inline ranks(const vector_t &vector) { from_vector(vector); }

    inline uint8_t magnitude_exp() const { int ex = 0; frexp(magnitude_sq, &ex); ex += 127; return ex; }

    void from_vector(const vector_t &vector)
    {
        ranks_full_t ranks;
        magnitude_sq = 0;
        for (size_t i = 0; i != DIMENSIONS; ++i) {
            float_t value = vector[i];
            ranks[i] = rank_pair_t(value, i);
            magnitude_sq += value * value;
        }
        pair_compare_first_greater_abs<rank_pair_t> c;
        auto begin = ranks.begin(), mid = begin + RANKS, end = ranks.end();
        std::nth_element(begin, mid, end, c);
        std::sort(begin, mid, c);
        std::copy(begin, mid, data.begin());
    }
    
    void sorted_ranks(rank_ids_t &pattern) const
    {
        for (size_t i = 0; i != RANKS; ++i)
            pattern[i] = (_value(data[i]) < 0 ? -1 : 1) * _index(data[i]);
        //std::sort(pattern.begin(), pattern.end());
    }
    
    inline std::array<uint64_t, 8> bits() const
    {
        rank_ids_t q;
        sorted_ranks(q);
        uint8_t schema[] = { 0, 1 };
        std::array<uint64_t, 8> bits = {
            /* we shift because schema only takes two bits */
            pack_bits_64_from_8s (schema[0], q[0], q[1], q[2], q[3], magnitude_exp()) << 6,
            pack_bits_64_from_8s (schema[0], q[1], q[2], q[3], q[4], magnitude_exp()) << 6,
            pack_bits_64_from_8s (schema[0], q[2], q[3], q[4], q[5], magnitude_exp()) << 6,
            pack_bits_64_from_8s (schema[0], q[3], q[4], q[5], q[6], magnitude_exp()) << 6,
            pack_bits_64_from_16s(schema[1], q[0], q[1],             magnitude_exp()) << 14,
            pack_bits_64_from_16s(schema[1], q[1], q[2],             magnitude_exp()) << 14,
            pack_bits_64_from_16s(schema[1], q[2], q[3],             magnitude_exp()) << 14,
            pack_bits_64_from_16s(schema[1], q[3], q[4],             magnitude_exp()) << 14,
        };
        return bits;
    }
};
