#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include "stl.h"
#include "bfloat16.h"
#include "container.h"
#include "simple_timer.h"
#include "key_.h"

inline unsigned _count1s(uint64_t x) { return __builtin_popcountl(x); }

template <size_t N, size_t M, typename IndexT = int16_t>
void encode(const std::array<IndexT, M> &indices, std::array<uint64_t, N> &into)
{
    for (const auto &i : indices) { size_t v(abs(i)); into[v >> 6] |= 1 << (v & 0x3f); }
}

template <size_t N>
size_t count_intersection(const std::array<uint64_t, N> &a, const std::array<uint64_t, N> &b)
{
    size_t count = 0; for (size_t i = 0; i != N; ++i) count += _count1s(a[i] & b[i]); return count;
}

template <size_t N, size_t M, typename IndexT = int16_t>
size_t count_intersection(const std::array<uint64_t, N> &a, const std::array<IndexT, M> &indices)
{
    std::array<uint64_t, N> t({ 0 }); encode(indices, t); return count_intersection(a, t);
}

template <size_t N, size_t RanksN, typename IndexT = int16_t>
struct expand_bitset
{
    typedef expand_bitset<N, RanksN, IndexT> self_t;
    typedef std::array<uint64_t, (N >> 6)>   data_t;
    typedef std::array<IndexT, RanksN>       in_t;

    data_t data;

    inline expand_bitset(const in_t &source)  : data({ 0 }) { for (const auto &i : source) { IndexT v(abs(i)); data[v >> 6] |= 1 << (v & 0x3f); } }
    inline expand_bitset(const data_t &data)  : data(data) {}
    inline expand_bitset(const self_t &other) : data(other.data) {}
    
    inline self_t &bit_or (const self_t &from)               { for (size_t i = 0, sz = data.size(); i != sz; ++i) data[i] |= from.data[i]; return *this; }
    inline self_t &bit_and(const self_t &from)               { for (size_t i = 0, sz = data.size(); i != sz; ++i) data[i] &= from.data[i]; return *this; }
    inline self_t &bit_xor(const self_t &from)               { for (size_t i = 0, sz = data.size(); i != sz; ++i) data[i] ^= from.data[i]; return *this; }
    inline size_t  bit_count()                         const { size_t count = 0; for (size_t i = 0, sz = data.size(); i != sz; ++i) count += _count1s(data[i]); return count; }
    
    inline size_t  count_intersection(const self_t &a) const { size_t count = 0; for (size_t i = 0, sz = data.size(); i != sz; ++i) count += _count1s(data[i] & a.data[i]); return count; }
    inline size_t  count_union(const self_t &a)        const { size_t count = 0; for (size_t i = 0, sz = data.size(); i != sz; ++i) count += _count1s(data[i] | a.data[i]); return count; }
};
