#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include "stl.h"
#include "key.h"
#include "utils.h"

template <typename FloatT, size_t N, size_t KeyRanksN>
struct search_vector
{
    typedef FloatT                               real_t;
    typedef key_index<FloatT, N, KeyRanksN>      key_index_t;
    typedef key<FloatT, N, KeyRanksN>            key_t;
    typedef typename key_t::key_ranks_t          key_ranks_t;
    typedef typename key_t::key_values_t         key_values_t;
    typedef key_id<FloatT, N, KeyRanksN>         key_id_t;
    typedef std::array<FloatT, N>                flat_vector_t;
    typedef std::array<FloatT, KeyRanksN + 1>    ratios_t;

    const flat_vector_t &vector;
    key_id_t             key_id;
    key_ranks_t          ranks_sorted; // so far unnecessary
    key_values_t         abs_values; // may be unnecessary-- probably does help performance a bit
    ratios_t             ratios;

    inline search_vector(const flat_vector_t &search) : vector(search), key_id(key_t(search), 0), ranks_sorted(key_id.key.ranks), abs_values(key_id.key.values), ratios({ 0 })
    {
        std::sort(ranks_sorted.begin(), ranks_sorted.end());
        real_t sq_sum = 0.0;
        for (size_t i = 0, sz = ratios.size(); i != sz; i++) {
            sq_sum += _sq(abs_values[i] = abs(abs_values[i]));
            ratios[i] = sqrt(sq_sum) / key_id.key.norm;
        }
    }

    inline real_t norm() const { return key_id.key.norm; }
};
