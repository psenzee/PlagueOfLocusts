#pragma once

#include <array>
#include <cmath>
#include <algorithm>

//static constexpr size_t _lg(size_t n) { return n <= 1 ? 0 : (1 + _lg(n >> 1)); }

template <typename T>
inline std::pair<T, T> range_intersect(const std::pair<T, T> &a, const std::pair<T, T> &b)
    { return std::pair<T, T>(std::max(a.first, b.first), std::min(a.second, b.second)); }

template <typename T>
inline std::pair<T, T> range_union(const std::pair<T, T> &a, const std::pair<T, T> &b)
    { return std::pair<T, T>(std::min(a.first, b.first), std::max(a.second, b.second)); }

template <typename T>
inline size_t range_size(const std::pair<T, T> &a) { return size_t(a.second - a.first); }

inline size_t bits_count1s(uint64_t x) { return size_t(__builtin_popcountl(x)); }

template <size_t N>
inline size_t bits_count1s(const std::array<uint64_t, N> &x)
    { size_t count = 0; for (const auto &i : x) count += bits_count1s(i); return count; }

template <size_t WordsN, size_t IndicesN, typename IndexT = int16_t>
inline void bits_encode(const std::array<IndexT, IndicesN> &indices, std::array<uint64_t, WordsN> &into)
    { for (const auto &i : indices) { size_t v(abs(i)); into[v >> 6] |= 1ull << (v & 0x3f); } }

template <size_t WordsN, size_t IndicesN, typename IndexT = int16_t>
inline void bits_encode_signed(const std::array<IndexT, IndicesN> &indices, std::array<uint64_t, WordsN> &into)
    { for (const auto &i : indices) { size_t v(abs(i) + (i < 0) * ((WordsN / 2) << 6)); into[v >> 6] |= 1ull << (v & 0x3f); } }

template <size_t N>
inline size_t bits_count_intersection(const std::array<uint64_t, N> &a, const std::array<uint64_t, N> &b)
    { size_t count = 0; for (size_t i = 0; i != N; ++i) count += bits_count1s(a[i] & b[i]); return count; }

template <size_t N, size_t M, typename IndexT = int16_t>
inline size_t bits_count_intersection(const std::array<uint64_t, N> &a, const std::array<IndexT, M> &indices)
    { std::array<uint64_t, N> t({ 0 }); bits_encode(indices, t); return bits_count_intersection(a, t); }
