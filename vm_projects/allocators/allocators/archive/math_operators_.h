#pragma once

#include <cmath>
#include <algorithm>
#include <limits>

#define REAL_BINARY_OP(NAME, EXPR) \
namespace { struct NAME { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return EXPR; } }; } \
typedef _real_binary_op<NAME> op_real##NAME;

#define BITS_BINARY_OP(NAME, EXPR) \
namespace { struct NAME { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return EXPR; } }; } \
typedef _bits_binary_op<NAME> op_bits##NAME;

#define PRED_BINARY_OP(NAME, EXPR) \
namespace { struct NAME { template <typename A, typename B> constexpr bool operator()(const A &a, const B &b) const { return EXPR; } }; } \
typedef _pred_binary_op<NAME> op_pred##NAME;

namespace {
template <typename T> inline float   _float(const T &v) { return static_cast<float>  (v); }
template <typename T> inline int64_t _int64(const T &v) { return static_cast<int64_t>(v); }

template <typename Op>
struct _real_binary_op
{
    static constexpr const Op op;
    int64_t operator()(const int64_t &a, const int64_t &b) { return op(a, b); }
    float   operator()(const float   &a, const int64_t &b) { return op(a, _float(b)); }
    float   operator()(const float   &a, const float   &b) { return op(a, b); }
    float   operator()(const int64_t &a, const float   &b) { return op(_float(a), b); }
};

template <typename Op>
struct _bits_binary_op
{
    static constexpr const Op op;
    int64_t operator()(const int64_t &a, const int64_t &b) { return op(a, b); }
    int64_t operator()(const float   &a, const int64_t &b) { return op(_int64(a), b); }
    int64_t operator()(const float   &a, const float   &b) { return op(_int64(a), _int64(b)); }
    int64_t operator()(const int64_t &a, const float   &b) { return op(a, _int64(b)); }
};

template <typename Op>
struct _pred_binary_op
{
    static constexpr const Op op;
    bool operator()(const int64_t &a, const int64_t &b) { return op(a, b); }
    bool operator()(const float   &a, const int64_t &b) { return op(a, _float(b)); }
    bool operator()(const float   &a, const float   &b) { return op(a, b); }
    bool operator()(const int64_t &a, const float   &b) { return op(_float(a), b); }
};

template <typename Op>
struct _bits_unary_op
{
    static constexpr const Op op;
    int64_t operator()(const int64_t &a) { return op(a); }
    int64_t operator()(const float   &a) { return op(a); }
};

template <typename Op>
struct _pred_unary_op
{
    static constexpr const Op op;
    bool operator()(const int64_t &a) { return op(a); }
    bool operator()(const float   &a) { return op(a); }
};
}

REAL_BINARY_OP(_add, a + b)

namespace {
struct _add         { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return a + b; } };
struct _mul         { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return a * b; } };
struct _div         { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return a / b; } };
struct _sub         { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return a - b; } };
struct _max         { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return std::max(a, b); } };
struct _min         { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return std::min(a, b); } };
struct _mod         {
    template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return std::fmodf(_float(a), _float(b)); }
    template <> constexpr auto operator()<int64_t, int64_t>(const int64_t &a, const int64_t &b) const { return a % b; }
};
struct _bits_and    { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return a & b; } };
struct _bits_nand   { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return ~(a & b); } };
struct _bits_na_and { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return ~a & b; } };
struct _bits_nb_and { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return a & ~b; } };
struct _bits_or     { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return a | b; } };
struct _bits_nor    { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return ~(a | b); } };
struct _bits_na_or  { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return ~a | b; } };
struct _bits_nb_or  { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return a | ~b; } };
struct _bits_xor    { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return a ^ b; } };
struct _bits_nxor   { template <typename A, typename B> constexpr auto operator()(const A &a, const B &b) const { return ~(a ^ b); } };

struct _pred_and    { template <typename A, typename B> constexpr bool operator()(const A &a, const B &b) const { return a && b; } };
struct _pred_nand   { template <typename A, typename B> constexpr bool operator()(const A &a, const B &b) const { return !(a && b); } };
struct _pred_or     { template <typename A, typename B> constexpr bool operator()(const A &a, const B &b) const { return a || b; } };
struct _pred_nor    { template <typename A, typename B> constexpr bool operator()(const A &a, const B &b) const { return !(a || b); } };
struct _pred_lt     { template <typename A, typename B> constexpr bool operator()(const A &a, const B &b) const { return a < b; } };
struct _pred_lte    { template <typename A, typename B> constexpr bool operator()(const A &a, const B &b) const { return a <= b; } };
struct _pred_eq     { template <typename A, typename B> constexpr bool operator()(const A &a, const B &b) const { return a == b; } };
struct _pred_neq    { template <typename A, typename B> constexpr bool operator()(const A &a, const B &b) const { return a != b; } };
struct _pred_gte    { template <typename A, typename B> constexpr bool operator()(const A &a, const B &b) const { return a >= b; } };
struct _pred_gt     { template <typename A, typename B> constexpr bool operator()(const A &a, const B &b) const { return a > b; } };

struct _real_neg    { template <typename A> constexpr A       operator()(const A &a) const { return -a; } };
struct _bits_not    { template <typename A> constexpr int64_t operator()(const A &a) const { return ~_int64(a); } };
struct _pred_not    { template <typename A> constexpr bool    operator()(const A &a) const { return !a; } };

}

// arithmetic returning int64_t/float
typedef _real_binary_op<_add>            real_add;
typedef _real_binary_op<_mul>            real_mul;
typedef _real_binary_op<_div>            real_div;
typedef _real_binary_op<_sub>            real_sub;
typedef _real_binary_op<_min>            real_min;
typedef _real_binary_op<_max>            real_max;
typedef _real_binary_op<_mod>            real_mod;

// bitwise returning int64_t
typedef _bits_binary_op<_bits_and>       bits_and;
typedef _bits_binary_op<_bits_nand>      bits_nand;
typedef _bits_binary_op<_bits_or>        bits_or;
typedef _bits_binary_op<_bits_nor>       bits_nor;
typedef _bits_binary_op<_bits_xor>       bits_xor;
typedef _bits_binary_op<_bits_nxor>      bits_nxor;

// predicates returning int64_t
typedef _pred_binary_op<_pred_lt>        pred_lt;
typedef _pred_binary_op<_pred_lte>       pred_lte;
typedef _pred_binary_op<_pred_eq>        pred_eq;
typedef _pred_binary_op<_pred_neq>       pred_neq;
typedef _pred_binary_op<_pred_gte>       pred_gte;
typedef _pred_binary_op<_pred_gt>        pred_gt;
typedef _pred_binary_op<_pred_and>       pred_and;
typedef _pred_binary_op<_pred_nand>      pred_nand;
typedef _pred_binary_op<_pred_or>        pred_or;
typedef _pred_binary_op<_pred_nand>      pred_nor;
