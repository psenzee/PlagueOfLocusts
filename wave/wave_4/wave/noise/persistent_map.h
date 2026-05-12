#pragma once

#include <algorithm>
#include <random>
#include <cstdint>
#include "hash.h"

namespace noise
{

template <typename T, size_t N, typename U = T, size_t M = N>
struct persistent_map
{
    typedef std::minstd_rand rng_t;
    typedef std::array<T, N> value_t;
    typedef std::array<U, M> result_t;
    
    uint64_t _seed;
    
    inline persistent_map(uint64_t seed) : _seed(seed) {}
    
    inline result_t operator()(const value_t &v) const
    {
        rng_t rng(unsigned(hash(v) + _seed));
        result_t r;
        for (size_t i = 0; i < M; i++) {
            r[i] = U(rng());
        }
        return r;
    }
};

}
