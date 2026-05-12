#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <utility>

namespace std_array
{

template <typename T, typename U, std::size_t N>
constexpr std::array<U, N> cast(const std::array<T, N> &a) {
    return std::apply([](auto&&... elems) { return std::array<U, sizeof...(elems)>{ { U(elems)... } }; }, a);
}
template <typename T, std::size_t N> constexpr auto ints(const std::array<T, N> &a) { return cast<T, int, N>(a); }
template <typename T, typename U, std::size_t N> constexpr std::array<T, N> fill(const U &a) { std::array<T, N> r; r.fill(a); return r; }

template <typename T, std::size_t N> constexpr auto zero()      { return fill<T, T, N>(T(0.)); }
template <typename T, std::size_t N> constexpr auto half()      { return fill<T, T, N>(T(0.5)); }
template <typename T, std::size_t N> constexpr auto tenth()     { return fill<T, T, N>(T(0.1)); }
template <typename T, std::size_t N> constexpr auto one()       { return fill<T, T, N>(T(1.)); }
template <typename T, std::size_t N> constexpr auto two()       { return fill<T, T, N>(T(2.)); }
template <typename T, std::size_t N> constexpr auto minus_one() { return fill<T, T, N>(T(-1.)); }
template <typename T, std::size_t N> constexpr auto e()         { return fill<T, T, N>(T(2.718281828459045)); }
template <typename T, std::size_t N> constexpr auto sqrt2()     { return fill<T, T, N>(T(1.414213562373095)); }
template <typename T, std::size_t N> constexpr auto sqrt3()     { return fill<T, T, N>(T(1.73205080757)); }
template <typename T, std::size_t N> constexpr auto pi()        { return fill<T, T, N>(T(3.141592653589793)); }
template <typename T, std::size_t N> constexpr auto tau()       { return fill<T, T, N>(T(3.141592653589793 * 2.)); }

#define CONSTEXPR_ARRAY_OP1(NAME, EXPR) \
template <typename T, std::size_t N> \
constexpr std::array<T, N> NAME(const std::array<T, N>& a) { \
return std::apply([](auto&&... elems) { return std::array<T, sizeof...(elems)>{ { (EXPR)... } }; }, a); }

CONSTEXPR_ARRAY_OP1(invert,     (T(1) / elems))
CONSTEXPR_ARRAY_OP1(negative,   -elems)
CONSTEXPR_ARRAY_OP1(complement, ~elems)
CONSTEXPR_ARRAY_OP1(square,     (elems * elems))
CONSTEXPR_ARRAY_OP1(cube,       (elems * elems * elems))
CONSTEXPR_ARRAY_OP1(sqrt,       std::sqrt(elems))
CONSTEXPR_ARRAY_OP1(cbrt,       std::cbrt(elems))
CONSTEXPR_ARRAY_OP1(cos,        std::cos(elems))
CONSTEXPR_ARRAY_OP1(sin,        std::sin(elems))
CONSTEXPR_ARRAY_OP1(tan,        std::tan(elems))
CONSTEXPR_ARRAY_OP1(isqrt,      (T(1) / std::sqrt(elems)))
CONSTEXPR_ARRAY_OP1(icbrt,      (T(1) / std::cbrt(elems)))
CONSTEXPR_ARRAY_OP1(floor,      std::floor(elems))
CONSTEXPR_ARRAY_OP1(round,      std::round(elems))
CONSTEXPR_ARRAY_OP1(ceil,       std::ceil(elems))
CONSTEXPR_ARRAY_OP1(exp,        std::exp(elems))
CONSTEXPR_ARRAY_OP1(abs,        std::abs(elems))

#define CONSTEXPR_ARRAY_OP2_INPL(NAME, EXPR) \
template <typename T, typename U, std::size_t N, std::size_t... I> \
constexpr std::array<T, N>& _##NAME##_impl(std::array<T, N>& a, const std::array<U, N>& b, std::index_sequence<I...>) { \
((EXPR), ...); return a; } \
template <typename T, typename U, std::size_t N> \
constexpr std::array<T, N>& NAME(std::array<T, N>& a, const std::array<U, N>& b) { \
return _##NAME##_impl(a, b, std::make_index_sequence<N>{}); }

CONSTEXPR_ARRAY_OP2_INPL(in_place_sub, a[I] -= b[I])
CONSTEXPR_ARRAY_OP2_INPL(in_place_add, a[I] += b[I])
CONSTEXPR_ARRAY_OP2_INPL(in_place_mul, a[I] *= b[I])
CONSTEXPR_ARRAY_OP2_INPL(in_place_div, a[I] /= b[I])
CONSTEXPR_ARRAY_OP2_INPL(in_place_mod, a[I] %= b[I])
CONSTEXPR_ARRAY_OP2_INPL(in_place_and, a[I] &= b[I])
CONSTEXPR_ARRAY_OP2_INPL(in_place_or,  a[I] |= b[I])
CONSTEXPR_ARRAY_OP2_INPL(in_place_xor, a[I] ^= b[I])

// ---

#define CONSTEXPR_ARRAY_OP2(NAME, EXPR2) \
template <typename T, typename U, std::size_t N, std::size_t... I> \
constexpr std::array<T, N> _##NAME##_impl(const std::array<T, N>& a, const std::array<U, N>& b, std::index_sequence<I...>) { \
return { { (EXPR2)... } }; } \
template <typename T, typename U, std::size_t N> \
constexpr std::array<T, N> NAME(const std::array<T, N>& a, const std::array<U, N>& b) { \
return _##NAME##_impl(a, b, std::make_index_sequence<N>{}); }

CONSTEXPR_ARRAY_OP2(pow,        std::pow(a[I], b[I]))
CONSTEXPR_ARRAY_OP2(min,        std::min(a[I], b[I]))
CONSTEXPR_ARRAY_OP2(max,        std::max(a[I], b[I]))
CONSTEXPR_ARRAY_OP2(subtract,   a[I] - b[I])
CONSTEXPR_ARRAY_OP2(add,        a[I] + b[I])
CONSTEXPR_ARRAY_OP2(multiply,   a[I] * b[I])
CONSTEXPR_ARRAY_OP2(divide,     a[I] / b[I])
CONSTEXPR_ARRAY_OP2(modulus,    a[I] % b[I])
CONSTEXPR_ARRAY_OP2(bit_and,    a[I] & b[I])
CONSTEXPR_ARRAY_OP2(bit_or,     a[I] | b[I])
CONSTEXPR_ARRAY_OP2(bit_xor,    a[I] ^ b[I])

}

#define CONSTEXPR_ARRAY_OPERATOR(OP, FUNCTION) \
template <typename T, typename U, std::size_t N> constexpr std::array<T, N> OP(const std::array<T, N> &a, const std::array<U, N> &b) { return FUNCTION(a, b); } \
template <typename T, typename U, std::size_t N> constexpr std::enable_if_t<std::is_arithmetic_v<U>, std::array<T, N>> OP(const std::array<T, N> &a, const U &b) { return FUNCTION(a, std_array::fill<T, U, N>(b)); } \
template <typename T, typename U, std::size_t N> constexpr std::enable_if_t<std::is_arithmetic_v<T>, std::array<U, N>> OP(const T &a, const std::array<U, N> &b) { return FUNCTION(std_array::fill<U, T, N>(a), b); }

CONSTEXPR_ARRAY_OPERATOR(operator-, std_array::subtract)
CONSTEXPR_ARRAY_OPERATOR(operator+, std_array::add)
CONSTEXPR_ARRAY_OPERATOR(operator*, std_array::multiply)
CONSTEXPR_ARRAY_OPERATOR(operator/, std_array::divide)
CONSTEXPR_ARRAY_OPERATOR(operator%, std_array::modulus)
CONSTEXPR_ARRAY_OPERATOR(operator&, std_array::bit_and)
CONSTEXPR_ARRAY_OPERATOR(operator|, std_array::bit_or)
CONSTEXPR_ARRAY_OPERATOR(operator^, std_array::bit_xor)

#define CONSTEXPR_ARRAY_OPERATOR_ASSIGN(OP, FUNCTION) \
template <typename T, typename U, std::size_t N> constexpr std::array<T, N> &OP(std::array<T, N> &a, const std::array<U, N> &b) { return FUNCTION(a, b); } \
template <typename T, typename U, std::size_t N> constexpr std::enable_if_t<std::is_arithmetic_v<U>, std::array<T, N>> &OP(std::array<T, N> &a, const U &b) { return FUNCTION(a, std_array::fill<T, U, N>(b)); }

CONSTEXPR_ARRAY_OPERATOR_ASSIGN(operator-=, std_array::in_place_sub)
CONSTEXPR_ARRAY_OPERATOR_ASSIGN(operator+=, std_array::in_place_add)
CONSTEXPR_ARRAY_OPERATOR_ASSIGN(operator*=, std_array::in_place_mul)
CONSTEXPR_ARRAY_OPERATOR_ASSIGN(operator/=, std_array::in_place_div)
CONSTEXPR_ARRAY_OPERATOR_ASSIGN(operator%=, std_array::in_place_mod)
CONSTEXPR_ARRAY_OPERATOR_ASSIGN(operator&=, std_array::in_place_and)
CONSTEXPR_ARRAY_OPERATOR_ASSIGN(operator|=, std_array::in_place_or)
CONSTEXPR_ARRAY_OPERATOR_ASSIGN(operator^=, std_array::in_place_xor)

template <typename T, std::size_t N> inline std::array<T, N> operator-(const std::array<T, N> &a) { return std_array::negative(a); }
template <typename T, std::size_t N> inline std::array<T, N> operator~(const std::array<T, N> &a) { return std_array::complement(a); }

#undef CONSTEXPR_ARRAY_OP1
#undef CONSTEXPR_ARRAY_OP2_INPL
#undef CONSTEXPR_ARRAY_OPERATOR
#undef CONSTEXPR_ARRAY_OPERATOR_ASSIGN

namespace std_array
{

template <typename T, std::size_t N> constexpr std::array<T, N>  bit_shl    (const std::array<T, N> &a, std::size_t b)                  { std::array<T, N> c(a); for (auto &v: c) v <<= b; return c; }
template <typename T, std::size_t N> constexpr std::array<T, N>  bit_shr    (const std::array<T, N> &a, std::size_t b)                  { std::array<T, N> c(a); for (auto &v: c) v >>= b; return c; }

template <typename T, std::size_t N> constexpr std::array<T, N> &in_place_shl (std::array<T, N> &a, std::size_t b)                      { for (auto &v: a) v <<= b; return a; }
template <typename T, std::size_t N> constexpr std::array<T, N> &in_place_shr (std::array<T, N> &a, std::size_t b)                      { for (auto &v: a) v >>= b; return a; }

template <typename T, std::size_t N> constexpr T                 min_element(const std::array<T, N> &a)                            { return std::min_element(a.begin(), a.end()); }
template <typename T, std::size_t N> constexpr T                 max_element(const std::array<T, N> &a)                            { return std::max_element(a.begin(), a.end()); }
template <typename T, std::size_t N, typename U = T> constexpr U mean(const std::array<T, N> &a)                                   { return sum(a) / U(N); }

namespace {
template <typename T, typename U, std::size_t N, std::size_t... I>
constexpr auto _dot_impl(const std::array<T, N> &a, const std::array<U, N> &b, std::index_sequence<I...>) {
    return ((a[I] * b[I]) + ...);
}
}
template <typename T, typename U, std::size_t N>
constexpr auto dot(const std::array<T, N>& a, const std::array<U, N>& b) {
    return _dot_impl(a, b, std::make_index_sequence<N>{});
}

namespace {
template <typename T, std::size_t N, std::size_t... I>
constexpr auto _sum_impl(const std::array<T, N> &a, std::index_sequence<I...>) { return (a[I] + ...); }
}
template <typename T, std::size_t N>
constexpr auto sum(const std::array<T, N> &a) { return _sum_impl(a, std::make_index_sequence<N>{}); }

template <typename T, std::size_t N>
constexpr T dot_self(const std::array<T, N> &a) {
    return dot(a, a);
}

template <typename T, typename U, std::size_t N>
constexpr auto distance_sq(const std::array<T, N> &a, const std::array<U, N> &b) {
    return dot_self(subtract(a, b));
}

template <typename T, std::size_t N>
constexpr auto magnitude(const std::array<T, N> &a) {
    return std::sqrt(dot_self(a));
}

template <typename T, typename U, std::size_t N>
constexpr auto distance(const std::array<T, N> &a, const std::array<U, N> &b) {
    return magnitude(subtract(a, b));
}

namespace {
template <typename T, std::size_t N, std::size_t... I>
constexpr auto _rchop_impl(const std::array<T, N> &a, std::index_sequence<I...>) {
    return std::array<T, N-1>{a[I]...};
}
}
template <typename T, std::size_t N>
constexpr auto rchop(const std::array<T, N> &a) {
    return _rchop_impl(a, std::make_index_sequence<N-1>{});
}

namespace {
template <typename T, std::size_t N, std::size_t... I>
constexpr auto _lchop_impl(const std::array<T, N> &a, std::index_sequence<I...>) {
    return std::array<T, N-1>{a[I+1]...};
}
}
template <typename T, std::size_t N>
constexpr auto lchop(const std::array<T, N> &a) {
    return _lchop_impl(a, std::make_index_sequence<N-1>{});
}

int test();

}
