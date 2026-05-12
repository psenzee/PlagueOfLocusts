#pragma once

#include <array>

template <typename T, typename U = T>           constexpr bool any_of(T v, U a, U b)                                    { return v == a || v == b; }
template <typename T, typename U = T>           constexpr bool any_of(T v, U a, U b, U c)                               { return v == a || v == b || v == c; }
template <typename T, typename U = T>           constexpr bool any_of(T v, U a, U b, U c, U d)                          { return v == a || v == b || v == c || v == d; }
template <typename T, typename U = T>           constexpr bool any_of(T v, U a, U b, U c, U d, U e)                     { return v == a || v == b || v == c || v == d || v == e; }
template <typename T, typename U = T>           constexpr bool any_of(T v, U a, U b, U c, U d, U e, U f)                { return v == a || v == b || v == c || v == d || v == e || v == f; }
template <typename T, typename U = T>           constexpr bool any_of(T v, U a, U b, U c, U d, U e, U f, U g)           { return v == a || v == b || v == c || v == d || v == e || v == f || v == g; }
template <typename T, typename U = T>           constexpr bool any_of(T v, U a, U b, U c, U d, U e, U f, U g, U h)      { return v == a || v == b || v == c || v == d || v == e || v == f || v == g || v == h; }
template <typename T, typename U = T>           constexpr bool any_of(T v, U a, U b, U c, U d, U e, U f, U g, U h, U i) { return v == a || v == b || v == c || v == d || v == e || v == f || v == g || v == h || v == i; }

template <typename T, typename U = T>           constexpr bool any_of(T v, const std::array<U, 1> &a)                   { return any_of(v, a[0]); }
template <typename T, typename U = T>           constexpr bool any_of(T v, const std::array<U, 2> &a)                   { return any_of(v, a[0], a[1]); }
template <typename T, typename U = T>           constexpr bool any_of(T v, const std::array<U, 3> &a)                   { return any_of(v, a[0], a[1], a[2]); }
template <typename T, typename U = T>           constexpr bool any_of(T v, const std::array<U, 4> &a)                   { return any_of(v, a[0], a[1], a[2], a[3]); }
template <typename T, typename U = T>           constexpr bool any_of(T v, const std::array<U, 5> &a)                   { return any_of(v, a[0], a[1], a[2], a[3], a[4]); }
template <typename T, typename U = T>           constexpr bool any_of(T v, const std::array<U, 6> &a)                   { return any_of(v, a[0], a[1], a[2], a[3], a[4], a[5]); }
template <typename T, typename U = T>           constexpr bool any_of(T v, const std::array<U, 7> &a)                   { return any_of(v, a[0], a[1], a[2], a[3], a[4], a[5], a[6]); }
template <typename T, typename U = T>           constexpr bool any_of(T v, const std::array<U, 8> &a)                   { return any_of(v, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]); }
template <typename T, typename U = T>           constexpr bool any_of(T v, const std::array<U, 9> &a)                   { return any_of(v, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8]); }

template <typename T, size_t N, typename U = T> constexpr bool any_of(T v, const std::array<U, N> &a)                   { for (const auto &u : a) if (v == u) return true; return false; }
