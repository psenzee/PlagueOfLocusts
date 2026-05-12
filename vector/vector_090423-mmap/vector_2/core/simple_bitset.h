#pragma once

#include <array>


template <size_t N> inline void bit_set   (std::array<uint64_t, N> &data, size_t u) { data[u >> 6] |= (1ull << (u & 63)); }
template <size_t N> inline void bit_set   (std::array<uint32_t, N> &data, size_t u) { data[u >> 5] |= (1ul << (u & 31)); }
template <size_t N> inline void bit_set   (std::array<uint16_t, N> &data, size_t u) { data[u >> 4] |= (1 << (u & 15)); }

template <size_t N> inline void bit_unset (std::array<uint64_t, N> &data, size_t u) { data[u >> 6] &= ~(1ull << (u & 63)); }
template <size_t N> inline void bit_unset (std::array<uint32_t, N> &data, size_t u) { data[u >> 5] &= ~(1ul << (u & 31)); }
template <size_t N> inline void bit_unset (std::array<uint16_t, N> &data, size_t u) { data[u >> 4] &= ~(1 << (u & 15)); }

template <size_t N> inline bool bit_test  (const std::array<uint64_t, N> &data, size_t u) { return (data[u >> 6] & (1ull << (u & 63))) > 0; }
template <size_t N> inline bool bit_test  (const std::array<uint32_t, N> &data, size_t u) { return (data[u >> 5] & (1ul << (u & 31))) > 0; }
template <size_t N> inline bool bit_test  (const std::array<uint16_t, N> &data, size_t u) { return (data[u >> 4] & (1 << (u & 15))) > 0; }

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
