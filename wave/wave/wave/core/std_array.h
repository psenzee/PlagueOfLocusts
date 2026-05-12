#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <iomanip>

namespace std_array
{

template <typename T, typename U, size_t N> inline std::array<U, N> cast(const std::array<T, N> &a)                        { std::array<U, N> c; auto i = c.begin(); for (const auto &v: a) *i++ = U(v); return c; }

template <typename T, size_t N> inline std::array<T, N>  negative   (const std::array<T, N> &a)                            { std::array<T, N> c(a); for (auto &v: c) v = -v; return c; }
template <typename T, size_t N> inline std::array<T, N>  complement (const std::array<T, N> &a)                            { std::array<T, N> c(a); for (auto &v: c) v = ~v; return c; }
template <typename T, size_t N> inline std::array<T, N>  invert     (const std::array<T, N> &a)                            { std::array<T, N> c(a); for (auto &v: c) v = T(1) / v; return c; }
template <typename T, size_t N> inline std::array<T, N>  square     (const std::array<T, N> &a)                            { std::array<T, N> c(a); for (auto &v: c) v *= v; return c; }
template <typename T, size_t N> inline std::array<T, N>  cube       (const std::array<T, N> &a)                            { std::array<T, N> c(a); for (auto &v: c) v *= v * v; return c; }
template <typename T, size_t N> inline std::array<T, N>  sqrt       (const std::array<T, N> &a)                            { std::array<T, N> c(a); for (auto &v: c) v = std::sqrt(v); return c; }
template <typename T, size_t N> inline std::array<T, N>  isqrt      (const std::array<T, N> &a)                            { std::array<T, N> c(a); for (auto &v: c) v = T(1) / std::sqrt(v); return c; }
template <typename T, size_t N> inline std::array<T, N>  cbrt       (const std::array<T, N> &a)                            { std::array<T, N> c(a); for (auto &v: c) v = std::cbrt(v); return c; }
template <typename T, size_t N> inline std::array<T, N>  floor      (const std::array<T, N> &a)                            { std::array<T, N> c(a); for (auto &v: c) v = std::floor(v); return c; }
template <typename T, size_t N> inline std::array<T, N>  round      (const std::array<T, N> &a)                            { std::array<T, N> c(a); for (auto &v: c) v = std::round(v); return c; }
template <typename T, size_t N> inline std::array<T, N>  ceil       (const std::array<T, N> &a)                            { std::array<T, N> c(a); for (auto &v: c) v = std::ceil(v); return c; }
template <typename T, size_t N> inline std::array<T, N>  exp        (const std::array<T, N> &a)                            { std::array<T, N> c(a); for (auto &v: c) v = std::exp(v); return c; }
template <typename T, size_t N> inline std::array<T, N>  abs        (const std::array<T, N> &a)                            { std::array<T, N> c(a); for (auto &v: c) v = std::abs(v); return c; }

template <typename T, size_t N> inline std::array<T, N>  pow        (const std::array<T, N> &a, const std::array<T, N> &b) { std::array<T, N> c(a); auto i = b.begin(); for (auto &v: c) v = pow(v, *i++); return c; }
template <typename T, size_t N> inline std::array<T, N>  subtract   (const std::array<T, N> &a, const std::array<T, N> &b) { std::array<T, N> c(a); auto i = b.begin(); for (auto &v: c) v -= *i++; return c; }
template <typename T, size_t N> inline std::array<T, N>  add        (const std::array<T, N> &a, const std::array<T, N> &b) { std::array<T, N> c(a); auto i = b.begin(); for (auto &v: c) v += *i++; return c; }
template <typename T, size_t N> inline std::array<T, N>  multiply   (const std::array<T, N> &a, const std::array<T, N> &b) { std::array<T, N> c(a); auto i = b.begin(); for (auto &v: c) v *= *i++; return c; }
template <typename T, size_t N> inline std::array<T, N>  divide     (const std::array<T, N> &a, const std::array<T, N> &b) { std::array<T, N> c(a); auto i = b.begin(); for (auto &v: c) v /= *i++; return c; }
template <typename T, size_t N> inline std::array<T, N>  modulus    (const std::array<T, N> &a, const std::array<T, N> &b) { std::array<T, N> c(a); auto i = b.begin(); for (auto &v: c) v %= *i++; return c; }
template <typename T, size_t N> inline std::array<T, N>  bit_and    (const std::array<T, N> &a, const std::array<T, N> &b) { std::array<T, N> c(a); auto i = b.begin(); for (auto &v: c) v &= *i++; return c; }
template <typename T, size_t N> inline std::array<T, N>  bit_or     (const std::array<T, N> &a, const std::array<T, N> &b) { std::array<T, N> c(a); auto i = b.begin(); for (auto &v: c) v |= *i++; return c; }
template <typename T, size_t N> inline std::array<T, N>  bit_xor    (const std::array<T, N> &a, const std::array<T, N> &b) { std::array<T, N> c(a); auto i = b.begin(); for (auto &v: c) v ^= *i++; return c; }
template <typename T, size_t N> inline std::array<T, N>  pow        (const std::array<T, N> &a, T b)                       { std::array<T, N> c(a); for (auto &v: c) v = std::pow(v, b); return c; }
template <typename T, size_t N> inline std::array<T, N>  subtract   (const std::array<T, N> &a, T b)                       { std::array<T, N> c(a); for (auto &v: c) v -= b; return c; }
template <typename T, size_t N> inline std::array<T, N>  add        (const std::array<T, N> &a, T b)                       { std::array<T, N> c(a); for (auto &v: c) v += b; return c; }
template <typename T, size_t N> inline std::array<T, N>  multiply   (const std::array<T, N> &a, T b)                       { std::array<T, N> c(a); for (auto &v: c) v *= b; return c; }
template <typename T, size_t N> inline std::array<T, N>  divide     (const std::array<T, N> &a, T b)                       { std::array<T, N> c(a); for (auto &v: c) v /= b; return c; }
template <typename T, size_t N> inline std::array<T, N>  modulus    (const std::array<T, N> &a, T b)                       { std::array<T, N> c(a); for (auto &v: c) v %= b; return c; }
template <typename T, size_t N> inline std::array<T, N>  bit_and    (const std::array<T, N> &a, T b)                       { std::array<T, N> c(a); for (auto &v: c) v &= b; return c; }
template <typename T, size_t N> inline std::array<T, N>  bit_or     (const std::array<T, N> &a, T b)                       { std::array<T, N> c(a); for (auto &v: c) v |= b; return c; }
template <typename T, size_t N> inline std::array<T, N>  bit_xor    (const std::array<T, N> &a, T b)                       { std::array<T, N> c(a); for (auto &v: c) v ^= b; return c; }
template <typename T, size_t N> inline std::array<T, N>  pow        (T a, const std::array<T, N>  &b)                      { std::array<T, N> c(b); for (auto &v: c) v = std::pow(a, v); return c; }
template <typename T, size_t N> inline std::array<T, N>  subtract   (T a, const std::array<T, N>  &b)                      { std::array<T, N> c(b); for (auto &v: c) v = a - v; return c; }
template <typename T, size_t N> inline std::array<T, N>  add        (T a, const std::array<T, N>  &b)                      { std::array<T, N> c(b); for (auto &v: c) v += a; return c; }
template <typename T, size_t N> inline std::array<T, N>  multiply   (T a, const std::array<T, N>  &b)                      { std::array<T, N> c(b); for (auto &v: c) v *= a; return c; }
template <typename T, size_t N> inline std::array<T, N>  divide     (T a, const std::array<T, N>  &b)                      { std::array<T, N> c(b); for (auto &v: c) v = a / v; return c; }
template <typename T, size_t N> inline std::array<T, N>  modulus    (T a, const std::array<T, N>  &b)                      { std::array<T, N> c(b); for (auto &v: c) v = a % v; return c; }
template <typename T, size_t N> inline std::array<T, N>  bit_and    (T a, const std::array<T, N>  &b)                      { std::array<T, N> c(b); for (auto &v: c) v &= a; return c; }
template <typename T, size_t N> inline std::array<T, N>  bit_or     (T a, const std::array<T, N>  &b)                      { std::array<T, N> c(b); for (auto &v: c) v |= a; return c; }
template <typename T, size_t N> inline std::array<T, N>  bit_xor    (T a, const std::array<T, N>  &b)                      { std::array<T, N> c(b); for (auto &v: c) v ^= a; return c; }
template <typename T, size_t N> inline std::array<T, N>  bit_shl    (const std::array<T, N> &a, size_t b)                  { std::array<T, N> c(a); for (auto &v: c) v <<= b; return c; }
template <typename T, size_t N> inline std::array<T, N>  bit_shr    (const std::array<T, N> &a, size_t b)                  { std::array<T, N> c(a); for (auto &v: c) v >>= b; return c; }

template <typename T, size_t N> inline std::array<T, N> &in_place_sub (std::array<T, N> &a, const std::array<T, N> &b)     { auto i = b.begin(); for (auto &v: a) v -= *i++; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_add (std::array<T, N> &a, const std::array<T, N> &b)     { auto i = b.begin(); for (auto &v: a) v += *i++; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_mul (std::array<T, N> &a, const std::array<T, N> &b)     { auto i = b.begin(); for (auto &v: a) v *= *i++; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_div (std::array<T, N> &a, const std::array<T, N> &b)     { auto i = b.begin(); for (auto &v: a) v /= *i++; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_mod (std::array<T, N> &a, const std::array<T, N> &b)     { auto i = b.begin(); for (auto &v: a) v %= *i++; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_and (std::array<T, N> &a, const std::array<T, N> &b)     { auto i = b.begin(); for (auto &v: a) v &= *i++; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_or  (std::array<T, N> &a, const std::array<T, N> &b)     { auto i = b.begin(); for (auto &v: a) v |= *i++; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_xor (std::array<T, N> &a, const std::array<T, N> &b)     { auto i = b.begin(); for (auto &v: a) v ^= *i++; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_sub (std::array<T, N> &a, T b)                           { for (auto &v: a) v  -= b; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_add (std::array<T, N> &a, T b)                           { for (auto &v: a) v  += b; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_mul (std::array<T, N> &a, T b)                           { for (auto &v: a) v  *= b; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_div (std::array<T, N> &a, T b)                           { for (auto &v: a) v  /= b; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_mod (std::array<T, N> &a, T b)                           { for (auto &v: a) v  %= b; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_and (std::array<T, N> &a, T b)                           { for (auto &v: a) v  &= b; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_or  (std::array<T, N> &a, T b)                           { for (auto &v: a) v  |= b; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_xor (std::array<T, N> &a, T b)                           { for (auto &v: a) v  ^= b; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_shl (std::array<T, N> &a, size_t b)                      { for (auto &v: a) v <<= b; return a; }
template <typename T, size_t N> inline std::array<T, N> &in_place_shr (std::array<T, N> &a, size_t b)                      { for (auto &v: a) v >>= b; return a; }

// todo update script to specialize these for few items
template <typename T, size_t N> inline T                 min_element(const std::array<T, N> &a)                            { return std::min_element(a.begin(), a.end()); }
template <typename T, size_t N> inline T                 max_element(const std::array<T, N> &a)                            { return std::max_element(a.begin(), a.end()); }
//template <typename T, size_t N> inline T               sum(const std::array<T, N> &a)                                    { return std::accumulate(a.begin(), a.end()); }
template <typename T, size_t N> inline T                 sum(const std::array<T, N> &a)                                    { T c(0); for (const auto &v: a) c += v; return c; }
template <typename T, size_t N, typename U = T> inline U mean(const std::array<T, N> &a)                                   { return sum(a) / U(N); }
// dot for one does not autovectorize well when embedded deeply in templates, at least in CLANG
template <typename T, size_t N> inline T                 dot(const std::array<T, N> &a, const std::array<T, N> &b)         { T acc = T(0); auto i = b.begin(); for (const auto &v: a) acc += v * *i++; return acc; }
template <typename T, size_t N> inline T                 dot_self(const std::array<T, N> &a)                               { T acc = T(0); for (const auto &v: a) acc += v * v; return acc; }
template <typename T, size_t N> inline T                 magnitude(const std::array<T, N> &a)                              { T acc = dot_self(a); return std::sqrt(acc); }

// todo: min max average dot lerp etc

#define _T_INT 0
#define _T_FLOAT 1
#define _T double
#include "std_array_incl.h"
#undef _T
#define _T float
#include "std_array_incl.h"
#undef _T
#undef _T_INT
#undef _T_FLOAT

#define _T_INT 1
#define _T_FLOAT 0
#define _T int64_t
#include "std_array_incl.h"
#undef _T
#define _T uint64_t
#include "std_array_incl.h"
#undef _T
#define _T int32_t
#include "std_array_incl.h"
#undef _T
#define _T uint32_t
#include "std_array_incl.h"
#undef _T
#define _T int16_t
#include "std_array_incl.h"
#undef _T
#define _T uint16_t
#include "std_array_incl.h"
#undef _T
#define _T int8_t
#include "std_array_incl.h"
#undef _T
#define _T uint8_t
#include "std_array_incl.h"
#undef _T
#define _T bool
#include "std_array_incl.h"
#undef _T
#undef _T_INT
#undef _T_FLOAT

}

/*
template <typename T, size_t N> inline std::array<T, N>  operator-(const std::array<T, N> &a)                              { return std_array::negative(a); }
template <typename T, size_t N> inline std::array<T, N>  operator~(const std::array<T, N> &a)                              { return std_array::complement(a); }

template <typename T, size_t N> inline std::array<T, N>  operator-(const std::array<T, N> &a, const std::array<T, N> &b)   { return std_array::subtract(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator+(const std::array<T, N> &a, const std::array<T, N> &b)   { return std_array::add(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator*(const std::array<T, N> &a, const std::array<T, N> &b)   { return std_array::multiply(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator/(const std::array<T, N> &a, const std::array<T, N> &b)   { return std_array::divide(a, b); }

template <typename T, size_t N> inline std::array<T, N>  operator%(const std::array<T, N> &a, const std::array<T, N> &b)   { return std_array::modulus(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator&(const std::array<T, N> &a, const std::array<T, N> &b)   { return std_array::bit_and(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator|(const std::array<T, N> &a, const std::array<T, N> &b)   { return std_array::bit_or(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator^(const std::array<T, N> &a, const std::array<T, N> &b)   { return std_array::bit_xor(a, b); }

template <typename T, size_t N> inline std::array<T, N>  operator-(const std::array<T, N> &a, T b)                         { return std_array::subtract(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator+(const std::array<T, N> &a, T b)                         { return std_array::add(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator*(const std::array<T, N> &a, T b)                         { return std_array::multiply(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator/(const std::array<T, N> &a, T b)                         { return std_array::divide(a, b); }

template <typename T, size_t N> inline std::array<T, N>  operator%(const std::array<T, N> &a, T b)                         { return std_array::modulus(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator&(const std::array<T, N> &a, T b)                         { return std_array::bit_and(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator|(const std::array<T, N> &a, T b)                         { return std_array::bit_or(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator^(const std::array<T, N> &a, T b)                         { return std_array::bit_xor(a, b); }

template <typename T, size_t N> inline std::array<T, N>  operator-(T a, const std::array<T, N> &b)                         { return std_array::subtract(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator+(T a, const std::array<T, N> &b)                         { return std_array::add(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator*(T a, const std::array<T, N> &b)                         { return std_array::multiply(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator/(T a, const std::array<T, N> &b)                         { return std_array::divide(a, b); }

template <typename T, size_t N> inline std::array<T, N>  operator%(T a, const std::array<T, N> &b)                         { return std_array::modulus(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator&(T a, const std::array<T, N> &b)                         { return std_array::bit_and(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator|(T a, const std::array<T, N> &b)                         { return std_array::bit_or(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator^(T a, const std::array<T, N> &b)                         { return std_array::bit_xor(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator<<(const std::array<T, N> &a, size_t b)                   { return std_array::bit_shl(a, b); }
template <typename T, size_t N> inline std::array<T, N>  operator>>(const std::array<T, N> &a, size_t b)                   { return std_array::bit_shr(a, b); }

template <typename T, size_t N> inline std::array<T, N> &operator-=(std::array<T, N> &a, T b)                              { return std_array::in_place_sub(a, b); }
template <typename T, size_t N> inline std::array<T, N> &operator+=(std::array<T, N> &a, T b)                              { return std_array::in_place_add(a, b); }
template <typename T, size_t N> inline std::array<T, N> &operator*=(std::array<T, N> &a, T b)                              { return std_array::in_place_mul(a, b); }
template <typename T, size_t N> inline std::array<T, N> &operator/=(std::array<T, N> &a, T b)                              { return std_array::in_place_div(a, b); }

template <typename T, size_t N> inline std::array<T, N> &operator%=(std::array<T, N> &a, T b)                              { return std_array::in_place_mod(a, b); }
template <typename T, size_t N> inline std::array<T, N> &operator&=(std::array<T, N> &a, T b)                              { return std_array::in_place_and(a, b); }
template <typename T, size_t N> inline std::array<T, N> &operator|=(std::array<T, N> &a, T b)                              { return std_array::in_place_or (a, b); }
template <typename T, size_t N> inline std::array<T, N> &operator^=(std::array<T, N> &a, T b)                              { return std_array::in_place_xor(a, b); }
template <typename T, size_t N> inline std::array<T, N> &operator<<=(std::array<T, N> &a, size_t b)                        { return std_array::in_place_shl(a, b); }
template <typename T, size_t N> inline std::array<T, N> &operator>>=(std::array<T, N> &a, size_t b)                        { return std_array::in_place_shr(a, b); }

template <typename T, size_t N> inline std::array<T, N> &operator-=(std::array<T, N> &a, const std::array<T, N> &b)        { return std_array::in_place_sub(a, b); }
template <typename T, size_t N> inline std::array<T, N> &operator+=(std::array<T, N> &a, const std::array<T, N> &b)        { return std_array::in_place_add(a, b); }
template <typename T, size_t N> inline std::array<T, N> &operator*=(std::array<T, N> &a, const std::array<T, N> &b)        { return std_array::in_place_mul(a, b); }
template <typename T, size_t N> inline std::array<T, N> &operator/=(std::array<T, N> &a, const std::array<T, N> &b)        { return std_array::in_place_div(a, b); }

template <typename T, size_t N> inline std::array<T, N> &operator%=(std::array<T, N> &a, const std::array<T, N> &b)        { return std_array::in_place_mod(a, b); }
template <typename T, size_t N> inline std::array<T, N> &operator&=(std::array<T, N> &a, const std::array<T, N> &b)        { return std_array::in_place_and(a, b); }
template <typename T, size_t N> inline std::array<T, N> &operator|=(std::array<T, N> &a, const std::array<T, N> &b)        { return std_array::in_place_or (a, b); }
template <typename T, size_t N> inline std::array<T, N> &operator^=(std::array<T, N> &a, const std::array<T, N> &b)        { return std_array::in_place_xor(a, b); }
*/
#include "std_array_1.h"
#include "std_array_2.h"
#include "std_array_3.h"
#include "std_array_4.h"
#include "std_array_5.h"
#include "std_array_6.h"
#include "std_array_7.h"
#include "std_array_8.h"
