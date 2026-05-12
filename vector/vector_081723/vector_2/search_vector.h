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
    typedef FloatT                                       real_t;
    typedef key_index<FloatT, N, IndexT, KeyRanksN>      key_index_t;
    typedef key<FloatT, N, IndexT, KeyRanksN>            key_t;
    typedef typename key_t::key_ranks_t                  key_ranks_t;
    typedef typename key_t::key_values_t                 key_values_t;
    typedef key_id<FloatT, N, IndexT, KeyRanksN>         key_id_t;
    typedef std::array<FloatT, N>                        flat_vector_t;

    const flat_vector_t &vector;
    key_id_t             key_id;
    key_ranks_t          ranks_sorted;
    key_values_t         abs_values;
    real_t               dist_values;
    real_t               ratio;

    inline search_vector(const flat_vector_t &search) : vector(search), key_id(key_t(search), 0), ranks_sorted(key_id.key.ranks), abs_values(key_id.key.values), dist_values(0.0)
    {
        std::sort(ranks_sorted.begin(), ranks_sorted.end());
        real_t sq_sum = 0.0;
        for (auto &v : abs_values) {
            v = abs(v);
            sq_sum += v * v;
        }
        dist_values = sqrt(sq_sum);
        ratio = dist_values / norm();
    }

    inline real_t norm() const { return key_id.key.norm; }
};
