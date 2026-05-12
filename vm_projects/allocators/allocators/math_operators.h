#pragma once

#include <cmath>
#include <algorithm>
#include <limits>

#define BINARY_OP(NAME, EXPR) namespace { struct NAME { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return EXPR; } }; }

#define REAL_BINARY_OP(NAME, EXPR) BINARY_OP(NAME, EXPR) typedef _real_binary_op<NAME> op##NAME;
#define BITS_BINARY_OP(NAME, EXPR) BINARY_OP(NAME, EXPR) typedef _bits_binary_op<NAME> op##NAME;
#define PRED_BINARY_OP(NAME, EXPR) BINARY_OP(NAME, EXPR) typedef _pred_binary_op<NAME> op##NAME;

namespace {
template <typename T> inline float   _float(const T &v) { return static_cast<float>  (v); }
template <typename T> inline int64_t _int64(const T &v) { return static_cast<int64_t>(v); }

template <typename Op>
struct _real_binary_op
{
    static constexpr const Op op;
    constexpr int64_t operator()(const int64_t &a, const int64_t &b) const { return op(a, b); }
    constexpr float   operator()(const float   &a, const int64_t &b) const { return op(a, _float(b)); }
    constexpr float   operator()(const float   &a, const float   &b) const { return op(a, b); }
    constexpr float   operator()(const int64_t &a, const float   &b) const { return op(_float(a), b); }
};

template <typename Op>
struct _bits_binary_op
{
    static constexpr const Op op;
    constexpr int64_t operator()(const int64_t &a, const int64_t &b) const { return op(a, b); }
    constexpr int64_t operator()(const float   &a, const int64_t &b) const { return op(_int64(a), b); }
    constexpr int64_t operator()(const float   &a, const float   &b) const { return op(_int64(a), _int64(b)); }
    constexpr int64_t operator()(const int64_t &a, const float   &b) const { return op(a, _int64(b)); }
};

template <typename Op>
struct _pred_binary_op
{
    static constexpr const Op op;
    constexpr bool operator()(const int64_t &a, const int64_t &b) const { return op(a, b); }
    constexpr bool operator()(const float   &a, const int64_t &b) const { return op(a, _float(b)); }
    constexpr bool operator()(const float   &a, const float   &b) const { return op(a, b); }
    constexpr bool operator()(const int64_t &a, const float   &b) const { return op(_float(a), b); }
};

template <typename Op>
struct _bits_unary_op
{
    static constexpr const Op op;
    constexpr int64_t operator()(const int64_t &a) const { return op(a); }
    constexpr int64_t operator()(const float   &a) const { return op(a); }
};

template <typename Op>
struct _pred_unary_op
{
    static constexpr const Op op;
    constexpr bool operator()(const int64_t &a) const { return op(a); }
    constexpr bool operator()(const float   &a) const { return op(a); }
};
}

REAL_BINARY_OP(_real_add, a + b)          REAL_BINARY_OP(_real_mul, a * b)              REAL_BINARY_OP(_real_div, a / b)           REAL_BINARY_OP(_real_sub, a - b)
REAL_BINARY_OP(_real_min, std::min(a, b)) REAL_BINARY_OP(_real_max, std::max(a, b))

BITS_BINARY_OP(_bits_and, a & b)          BITS_BINARY_OP(_bits_nand, ~(a & b))          BITS_BINARY_OP(_bits_na_and, ~(a & b))     BITS_BINARY_OP(_bits_nb_and, a & ~b)
BITS_BINARY_OP(_bits_or,  a | b)          BITS_BINARY_OP(_bits_nor,  ~(a | b))          BITS_BINARY_OP(_bits_na_or,  ~(a | b))     BITS_BINARY_OP(_bits_nb_or,  a | ~b)
BITS_BINARY_OP(_bits_xor, a ^ b)          BITS_BINARY_OP(_bits_xnor, ~(a ^ b))          BITS_BINARY_OP(_bits_na_xor, ~(a ^ b))     BITS_BINARY_OP(_bits_nb_xor, a ^ ~b)

PRED_BINARY_OP(_pred_and, a && b)         PRED_BINARY_OP(_pred_nand, !(a && b))         PRED_BINARY_OP(_pred_or, a || b)           PRED_BINARY_OP(_pred_nor, !(a || b))
PRED_BINARY_OP(_pred_lt,  a <  b)         PRED_BINARY_OP(_pred_lte, a <= b)             PRED_BINARY_OP(_pred_gt, a >  b)           PRED_BINARY_OP(_pred_gte, a >= b)
PRED_BINARY_OP(_pred_eq,  a == b)         PRED_BINARY_OP(_pred_neq, a != b)

namespace {
struct _mod {
    template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return std::fmodf(_float(a), _float(b)); }
    template <> constexpr auto operator()<int64_t, int64_t>(const int64_t &a, const int64_t &b) const { return a % b; }
};
}
typedef _real_binary_op<_mod> op_real_mod;

struct op_real_neg    { template <typename A> constexpr A       operator()(const A &a) const { return -a; } };
struct op_bits_not    { template <typename A> constexpr int64_t operator()(const A &a) const { return ~_int64(a); } };
struct op_pred_not    { template <typename A> constexpr bool    operator()(const A &a) const { return !a; } };
struct op_pred_bool   { template <typename A> constexpr bool    operator()(const A &a) const { return !!a; } };
