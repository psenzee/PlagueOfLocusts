#pragma once

#include <array>

template <size_t N>
struct simple_bitset
{
    enum { COUNT = N, BITS = 64, SHIFT = 6, WORDS = (N + BITS - 1) >> SHIFT };
    std::array<uint64_t, WORDS> data;

    inline simple_bitset() : data({ 0 }) {}

    inline void set(size_t u)          { data[u >> SHIFT] |= _mask(u); }
    inline void unset(size_t u)        { data[u >> SHIFT] &= ~_mask(u); }
    inline bool is_set(size_t u) const { return (data[u >> SHIFT] & _mask(u)) > 0; }
private:
    inline static uint64_t _mask(size_t u) { return uint64_t(1) << (u & (BITS - 1)); }
};
