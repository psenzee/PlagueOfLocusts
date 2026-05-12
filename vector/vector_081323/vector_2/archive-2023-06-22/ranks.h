#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include "stl.h"
#include "bfloat16.h"

// okay if key has
// n ranks [n = 4]
// xx let's sort the whole search key
// starting from the index_key.ranks[0]..[n] look up the corresponding values in search key (search[index_key.ranks[i]] - index_key.rvalues[i])^2
// start from key and

template <typename FloatT, size_t N, typename IndexT = int16_t>
struct ranks
{
    typedef FloatT                                    real_t;
    typedef ranks<FloatT, N, IndexT>                  self_t;
    typedef std::pair<FloatT, IndexT>                 value_index_t;
    typedef std::array<FloatT, N>                     vector_t;
    typedef std::array<value_index_t, N>              ranks_t;

    real_t   _norm;
    vector_t _values;
    ranks_t  _ranks;

    static void set_ranks(const vector_t &values, ranks_t &ranks, size_t partition_n = 0)
    {
        for (size_t i = 0, sz = values.size(); i != sz; ++i)
            ranks[i] = value_index_t(values[i], i);
        pair_compare_first_greater_abs_second_less<self_t> c;
        if (partition_n) {
            std::sort(ranks.begin(), ranks.end(), c);
            return;
        }
        std::nth_element(ranks.begin(), ranks.begin() + partition_n, ranks.end(), c);
        std::sort(ranks.begin(), ranks.begin() + partition_n, c);
    }

    inline static real_t calculate_norm_sq(const vector_t &values) { real_t sum = 0; for (auto v : values) sum += v * v; return sum; }

    inline ranks(const vector_t &v, size_t partition_n = 32) : _values(v), _norm(sqrt(calculate_norm_sq(v))) { set_ranks(v, _ranks, partition_n); }

    struct rank_comparator {
        uint16_t _rank;
        inline rank_comparator(uint16_t r) : _rank(r) {}
        inline bool operator()(const self_t &a, const self_t &b) const { return a._ranks[_rank] < b._ranks[_rank]; }
    };
};
