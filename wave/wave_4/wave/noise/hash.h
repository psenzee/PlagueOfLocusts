#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <deque>
#include <string>
#include <string_view>

namespace
{

constexpr const uint8_t ROT = 5;

inline uint64_t _scale(float v)  { return uint64_t(v * 65535); }
inline uint64_t _scale(double v) { return uint64_t(v * 65535); }

template <typename T> inline uint64_t _scale(const T &v) { return uint64_t(v); }
template <typename C> inline uint64_t hash_container(const C &c) { uint64_t t(1); for (auto i : c) t = (std::rotr(t, ROT) ^ (_scale(i) * 311)) + 863; return t; }

}

namespace noise
{

// just add others if there are more containers to be added
template <typename T, size_t N> inline uint64_t hash(const std::array<T, N> &v)          { return hash_container< std::array<T, N> >(v); }
template <typename T>           inline uint64_t hash(const std::vector<T> &v)            { return hash_container< std::vector<T> >(v); }
template <typename T>           inline uint64_t hash(const std::deque<T> &v)             { return hash_container< std::deque<T> >(v); }
template <typename T>           inline uint64_t hash(const std::basic_string<T> &v)      { return hash_container< std::basic_string<T> >(v); }
template <typename T>           inline uint64_t hash(const std::basic_string_view<T> &v) { return hash_container< std::basic_string_view<T> >(v); }

}
