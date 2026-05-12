#pragma once

#include <array>
#include <cmath>

#define _AT   std::array<T, 1>
#define _AI   std::array<int, 1>
#define _ASZ  std::array<size_t, 1>
#define _AU   std::array<U, 1>

template <typename T, typename U> constexpr _AT  operator-(const _AT &a)                  { return _AT({ -a[0] }); }
template <typename T, typename U> constexpr _AT  operator~(const _AT &a)                  { return _AT({ ~a[0] }); }

template <typename T, typename U> constexpr _AT  operator+(const _AT &a, const _AU &b)    { return _AT({ a[0] + T(b[0]) }); }
template <typename T, typename U> constexpr _AT  operator*(const _AT &a, const _AU &b)    { return _AT({ a[0] * T(b[0]) }); }
template <typename T, typename U> constexpr _AT  operator-(const _AT &a, const _AU &b)    { return _AT({ a[0] - T(b[0]) }); }
template <typename T, typename U> constexpr _AT  operator/(const _AT &a, const _AU &b)    { return _AT({ a[0] / T(b[0]) }); }
template <typename T, typename U> constexpr _AT  operator%(const _AT &a, const _AU &b)    { return _AT({ a[0] % T(b[0]) }); }
template <typename T, typename U> constexpr _AT  operator|(const _AT &a, const _AU &b)    { return _AT({ a[0] | T(b[0]) }); }
template <typename T, typename U> constexpr _AT  operator&(const _AT &a, const _AU &b)    { return _AT({ a[0] & T(b[0]) }); }
template <typename T, typename U> constexpr _AT  operator^(const _AT &a, const _AU &b)    { return _AT({ a[0] ^ T(b[0]) }); }

template <typename T, typename U> constexpr _AT  operator+(const _AT &a, const U &b)      { return _AT({ a[0] + T(b) }); }
template <typename T, typename U> constexpr _AT  operator*(const _AT &a, const U &b)      { return _AT({ a[0] * T(b) }); }
template <typename T, typename U> constexpr _AT  operator-(const _AT &a, const U &b)      { return _AT({ a[0] - T(b) }); }
template <typename T, typename U> constexpr _AT  operator/(const _AT &a, const U &b)      { return _AT({ a[0] / T(b) }); }
template <typename T, typename U> constexpr _AT  operator%(const _AT &a, const U &b)      { return _AT({ a[0] % T(b) }); }
template <typename T, typename U> constexpr _AT  operator|(const _AT &a, const U &b)      { return _AT({ a[0] | T(b) }); }
template <typename T, typename U> constexpr _AT  operator&(const _AT &a, const U &b)      { return _AT({ a[0] & T(b) }); }
template <typename T, typename U> constexpr _AT  operator^(const _AT &a, const U &b)      { return _AT({ a[0] ^ T(b) }); }

template <typename T, typename U> constexpr _AT  operator+(const U &a, const _AT &b)      { return _AT({ T(a) + b[0] }); }
template <typename T, typename U> constexpr _AT  operator*(const U &a, const _AT &b)      { return _AT({ T(a) * b[0] }); }
template <typename T, typename U> constexpr _AT  operator-(const U &a, const _AT &b)      { return _AT({ T(a) - b[0] }); }
template <typename T, typename U> constexpr _AT  operator/(const U &a, const _AT &b)      { return _AT({ T(a) / b[0] }); }
template <typename T, typename U> constexpr _AT  operator%(const U &a, const _AT &b)      { return _AT({ T(a) % b[0] }); }
template <typename T, typename U> constexpr _AT  operator|(const U &a, const _AT &b)      { return _AT({ T(a) | b[0] }); }
template <typename T, typename U> constexpr _AT  operator&(const U &a, const _AT &b)      { return _AT({ T(a) & b[0] }); }
template <typename T, typename U> constexpr _AT  operator^(const U &a, const _AT &b)      { return _AT({ T(a) ^ b[0] }); }

template <typename T, typename U> constexpr _AT &operator+=(_AT &a, const U &b)           { a[0] += T(b); return a; }
template <typename T, typename U> constexpr _AT &operator*=(_AT &a, const U &b)           { a[0] *= T(b); return a; }
template <typename T, typename U> constexpr _AT &operator-=(_AT &a, const U &b)           { a[0] -= T(b); return a; }
template <typename T, typename U> constexpr _AT &operator/=(_AT &a, const U &b)           { a[0] /= T(b); return a; }
template <typename T, typename U> constexpr _AT &operator%=(_AT &a, const U &b)           { a[0] %= T(b); return a; }
template <typename T, typename U> constexpr _AT &operator|=(_AT &a, const U &b)           { a[0] |= T(b); return a; }
template <typename T, typename U> constexpr _AT &operator&=(_AT &a, const U &b)           { a[0] &= T(b); return a; }
template <typename T, typename U> constexpr _AT &operator^=(_AT &a, const U &b)           { a[0] ^= T(b); return a; }

template <typename T, typename U> constexpr _AT &operator+=(_AT &a, const _AU &b)         { a[0] += T(b[0]); return a; }
template <typename T, typename U> constexpr _AT &operator*=(_AT &a, const _AU &b)         { a[0] *= T(b[0]); return a; }
template <typename T, typename U> constexpr _AT &operator-=(_AT &a, const _AU &b)         { a[0] -= T(b[0]); return a; }
template <typename T, typename U> constexpr _AT &operator/=(_AT &a, const _AU &b)         { a[0] /= T(b[0]); return a; }
template <typename T, typename U> constexpr _AT &operator%=(_AT &a, const _AU &b)         { a[0] %= T(b[0]); return a; }
template <typename T, typename U> constexpr _AT &operator|=(_AT &a, const _AU &b)         { a[0] |= T(b[0]); return a; }
template <typename T, typename U> constexpr _AT &operator&=(_AT &a, const _AU &b)         { a[0] &= T(b[0]); return a; }
template <typename T, typename U> constexpr _AT &operator^=(_AT &a, const _AU &b)         { a[0] ^= T(b[0]); return a; }

template <typename T> constexpr std::ostream &operator<<(std::ostream &os, const _AT &a)  { return os << "[" << a[0] << "]"; }

template <typename T>             constexpr T     indices(const _AT &unused)              { return _AT({ T(0) }); }
template <typename T>             constexpr T     sum(const _AT &a)                       { return a[0]; }
template <typename T, typename U> constexpr T     dot(const _AT &a, const _AU &b)         { return a[0] * T(b[0]); }
template <typename T>             constexpr T     dot_self(const _AT &a)                  { return a[0] * a[0]; }
template <typename T>             constexpr T     magnitude(const _AT &a)                 { return std::abs(a[0]); }
template <typename T, typename U> constexpr T     distance_sq(const _AT &a, const _AU &b) { return (a[0] - b[0]) * (a[0] - b[0]); }
template <typename T, typename U> constexpr T     distance(const _AT &a, const _AU &b)    { return sqrt(distance_sq(a, b)); }
template <typename T>             constexpr _AT   normal(const _AT &a)                    { return a / magnitude(a); }
template <typename T>             constexpr _AI   ints(const _AT &a)                      { return _AI({ int(a[0]) }); }
template <typename T>             constexpr _AT   floor(const _AT &a)                     { return _AT({ std::floor(a[0]) }); }

#undef _AT
#undef _AI
#undef _ASZ
#undef _AU
