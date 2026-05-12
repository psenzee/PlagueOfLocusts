#pragma once

#include <array>
#include <vector>

namespace {
static constexpr size_t _lg(size_t n) { return n <= 1 ? 0 : (1 + _lg(n >> 1)); }
template <typename U> struct _traits { enum { BITS = sizeof(U) * 8, LG = _lg(BITS), MASK = BITS - 1 }; };
}

template <typename U>           inline void bit_set(U *d, size_t u)                       { typedef _traits<U> T; d[u >> T::LG] |= (U(1) << (u & T::MASK)); }
template <typename U>           inline void bit_unset(U *d, size_t u)                     { typedef _traits<U> T; d[u >> T::LG] &= ~(U(1) << (u & T::MASK)); }
template <typename U>           inline bool bit_test(const U *d, size_t u)                { typedef _traits<U> T; return (d[u >> T::LG] & (U(1) << (u & T::MASK))) > 0; }

template <typename U, size_t N> inline void bit_set(std::array<U, N> &d, size_t u)        { bit_set(d.data(), u); }
template <typename U, size_t N> inline void bit_unset(std::array<U, N> &d, size_t u)      { bit_unset(d.data(), u); }
template <typename U, size_t N> inline bool bit_test(const std::array<U, N> &d, size_t u) { return bit_test(d.data(), u); }

template <typename U>           inline void bit_set(std::vector<U> &d, size_t u)          { bit_set(d.data(), u); }
template <typename U>           inline void bit_unset(std::vector<U> &d, size_t u)        { bit_unset(d.data(), u); }
template <typename U>           inline bool bit_test(const std::vector<U> &d, size_t u)   { return bit_test(d.data(), u); }
