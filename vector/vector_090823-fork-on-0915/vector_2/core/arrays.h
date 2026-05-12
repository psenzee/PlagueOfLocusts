#pragma once

#include <iostream>
#include <limits>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstddef>

#include "simd.h"

namespace arrays
{

// for raw C arrays:

template <typename T>             inline T sq(T u)                                                         { return u * u; }
template <typename T>             inline T cube(T u)                                                       { return u * u * u; }
template <typename T>              inline T sum(const T *v, const T *end)                                   { T s(0); for (; v != end; ++v) s += *v; return s; }
template <typename T, typename Fn> inline T sum_func(const T *v, const T *end)                              { Fn f; T s(0); for (; v != end; ++v) s += f(*v); return s; }
template <typename T>              inline T sum_abs(const T *v, const T *end)                               { T s(0); for (; v != end; ++v) s += std::abs(*v); return s; }
template <typename T>             inline T mean(const T *v, const T *end)                                  { return (end - v) <= 0 ? T(0) : (sum(v, end) / (end - v)); }
template <typename T>              inline T variance(T mean, const T *v, const T *end)                      { T s(0); for (; v != end; ++v) s += sq(v - mean); return s / ((end - v) - 1); }
template <typename T>             inline T variance(const T *v, const T *end)                              { return variance(mean(v, end), v, end); }
template <typename T>             inline T skewness(const T *v, const T *end)                              { T m(mean(v, end)); return skewness(m, variance(m, v, end), v, end); }
template <typename T>             inline T kurtosis(const T *v, const T *end)                              { T m(mean(v, end)); return kurtosis(m, variance(m, v, end), v, end); }
template <typename T, typename U>  inline T dot(const T *a, const U *b, const T *a_end)                     { T s(0); for (; a != a_end; ++a, ++b) s += *a * *b; return s; }
template <typename T>              inline T dot(const T *u, const T *u_end)                                 { T s(0); for (; u != u_end; ++u) s += sq(*u); return s; } // same as dot(u, u) but more efficient

template <typename T>              inline void lerp(const T *a, T *b, const T *a_end, T t)                  { for (; a != a_end; ++a, ++b) *b = (*b - *a) * t + *a; }

template <typename T, typename U> inline T distance(const T *a, const U *b, const T *a_end)                { return sqrt(simd::dist_sq(a, b, a_end)); }
template <typename T, typename U> inline T distance_limit(const T *a, const U *b, const T *a_end, T limit) { return sqrt(simd::dist_sq_limit(a, b, a_end), limit * limit); }

/*
template <typename T, typename U> inline T distance_sq_limit(const T *a, const U *b, const T *a_end, T limit_sq)
{
    static constexpr T max(std::numeric_limits<T>::max());
    if (limit_sq == max) return distance_sq(a, b, a_end);
    T sum(0);
    for (; a != a_end; ++a, ++b) if ((sum += sq(*a - *b)) > limit_sq) return max;
    return sum;
}
 */

template <typename T> inline T normalize(const T *v, const T *end)
{
    T norm(sqrt(dot(v, end))), z(0), inv(0);
    if (norm == z) return z;
    inv = 1 / norm;
    for (; v != end; ++v) *v *= inv;
    return norm;
}

template <typename T> inline T skewness(T mean, T variance, const T *v, const T *end)
{
    if (end - v <= 0) return T(0);
    T n(end - v), factor = n / ((n - 1.0) * (n - 2.0)), sum(0), sd = sqrt(variance);
    for (; v != end; ++v) sum += cube((*v - mean) / sd);
    return sum * factor;
}

template <typename T> inline T kurtosis(T mean, T variance, const T *v, const T *end)
{
    if (end - v <= 0) return T(0);
    T n(end - v), factor = n * (n + 1) / ((n - 1.0) * (n - 2.0) * (n - 3.0)), sum(0), sd = sqrt(variance);
    for (; v != end; ++v) sum += sq(sq((*v - mean) / sd));
    return sum * factor - (3.0 * (n - 1.0) * (n - 1.0)) / ((n - 2.0) * (n - 3.0));
}

template <typename T> inline auto least_squares(const T *a, const T *b, const T *a_end)
{
    T sum_a(0), sum_b(0), sum_ab(0), sum_aa(0);
    size_t n(a_end - a);
    for (size_t i = 0; i != n; ++i) {
        T ai(a[i]), bi(b[i]);
        sum_a += ai; sum_b += bi;
        sum_ab += ai * bi; sum_aa += ai * ai;
    }
    T m = (n * sum_ab - sum_a * sum_b) / (n * sum_aa - sum_a * sum_a);
    return std::pair<T, T>(m, (sum_b - m * sum_a) / n);
}

template <typename T> std::ostream &print(std::ostream &os, const T *v, const T *end, bool elipsis = false)
{
    os << "[" << std::setprecision(2);
    for (auto u = v; u != end; ++u) os << (u == v ? "" : " ") << *u;
    return os << (elipsis ? " ..." : "") << "]";
}

// adapters for stl containers (C; C0, C1)

template <typename C0, typename C1> inline auto  _minsz(const C0 &a, const C1 &b) { return std::min(a.size(), b.size()); }
template <typename C0, typename C1> inline auto  _minend(const C0 &a, const C1 &b) { return a.data() + std::min(a.size(), b.size()); }

template <typename C>               inline auto  sum(const C &v) { return sum(v.data(), v.data() + v.size()); }
template <typename C>               inline auto  mean(const C &v) { return mean(v.data(), v.data() + v.size()); }
template <typename C>               inline auto  variance(const C &v) { return variance(v.data(), v.data() + v.size()); }
template <typename C>               inline auto  skewness(const C &v) { return skewness(v.data(), v.data() + v.size()); }
template <typename C>               inline auto  kurtosis(const C &v) { return skewness(v.data(), v.data() + v.size()); }
template <typename C>               inline auto  least_squares(const C &a, const C &b) { return least_squares(a.data(), b.data(), _minend(a, b)); }
template <typename C0, typename C1> inline auto  dot(const C0 &a, const C1 &b) { return dot(a.data(), b.data(), _minend(a, b)); }
template <typename C>               inline auto  dot(const C &u) { return dot(u.data(), u.data() + u.size()); }
template <typename C0, typename C1> inline void  lerp(const C0 &a, C1 &b, typename C0::value_type t) { return lerp(a.data(), b.data(), _minend(a, b), t); }
template <typename C>               inline auto  normalize(C &v) { return normalize(v.data(), v.data() + v.size()); }
template <typename C0, typename C1> inline auto  distance_sq(const C0 &a, const C1 &b) { return distance_sq(a.data(), b.data(), _minend(a, b)); }
template <typename C0, typename C1> inline auto  distance(const C0 &a, const C1 &b) { return distance(a.data(), b.data(), _minend(a, b)); }
template <typename C0, typename C1> inline auto  distance_sq_limit(const C0 &a, const C1 &b, typename C0::value_type limit_sq) { return simd::dist_sq_limit(a.data(), b.data(), _minend(a, b), limit_sq); }
template <typename C0, typename C1> inline auto  distance_limit(const C0 &a, const C1 &b, typename C0::value_type limit) { return simd::dist_sq_limit(a.data(), b.data(), _minend(a, b), limit); }
template <size_t M, typename T>     inline auto &fixed(const std::vector<T> &u, std::array<T, M> &v) { std::copy(u.begin(), u.begin() + _minsz(u, v), v.begin()); return v; }
template <size_t M, typename T>     inline auto &dynamic(const std::array<T, M> &u, std::vector<T> &v) { v.resize(u.size()); std::copy(u.begin(), u.end(), v.begin()); return v; }

template <typename T, size_t N>     inline T     sum(std::array<T, N> &v)               { T s(0); for (auto &u : v) s += u; return s; }
template <typename T, size_t N>     inline void  add(std::array<T, N> &v, T value)      { for (auto &u : v) u += value; }
template <typename T, size_t N>     inline void  subtract(std::array<T, N> &v, T value) { for (auto &u : v) u -= value; }
template <typename T, size_t N>     inline void  multiply(std::array<T, N> &v, T value) { for (auto &u : v) u *= value; }
template <typename T, size_t N>     inline void  divide(std::array<T, N> &v, T value)   { for (auto &u : v) u /= value; }
template <typename T, size_t N>     inline void  normalize(std::array<T, N> &v)         { multiply(v, T(1) / sqrt(dot(v, v))); }

template <typename T>               inline T     sum(std::vector<T> &v)                 { T s(0); for (auto &u : v) s += u; return s; }
template <typename T>               inline void  add(std::vector<T> &v, T value)        { for (auto &u : v) u += value; }
template <typename T>               inline void  subtract(std::vector<T> &v, T value)   { for (auto &u : v) u -= value; }
template <typename T>               inline void  multiply(std::vector<T> &v, T value)   { for (auto &u : v) u *= value; }
template <typename T>               inline void  divide(std::vector<T> &v, T value)     { for (auto &u : v) u /= value; }
template <typename T>               inline void  normalize(std::vector<T> &v)           { multiply(v, T(1) / sqrt(dot(v, v))); }

template <typename T, size_t N>     inline T     sum_normalized_abs(const std::array<T, N> &v) { std::array<T, N> c(v); normalize(c); return sum_abs(c); }
template <typename T>               inline T     sum_normalized_abs(const std::vector<T> &v)   { std::vector<T> c(v); normalize(c); return sum_abs(c); }

template <typename C> inline std::ostream &print(std::ostream &os, const C &v, size_t count = 0)
{
    count = count ? v.size() : count;
    return print(os, v.data(), v.data() + std::min(count, v.size()), count < v.size());
}

}
