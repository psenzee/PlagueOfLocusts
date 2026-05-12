#pragma once

namespace tag_ops {

template <typename T> struct wrapper { T val; int64_t as_int() const { return static_cast<int64_t>(val); } };
template <typename T> wrapper<T> wrap(const T &v) { return wrapper<T> { v }; }
template <typename T> inline auto val(const wrapper<T> &u) { return u.val; }

struct op_neg     { template <typename T>             auto operator()(const T &a)             const { return -a; } };
struct op_inc     { template <typename T>             auto operator()(const T &a)             const { return a + T(1); } };
struct op_dec     { template <typename T>             auto operator()(const T &a)             const { return a - T(1); } };
struct op_not     { template <typename T>             auto operator()(const T &a)             const { return static_cast<int64_t>( !a); } };
struct op_bool    { template <typename T>             auto operator()(const T &a)             const { return static_cast<int64_t>(!!a); } };
struct op_bit_not { template <typename T>             auto operator()(const T &a)             const { return ~static_cast<int64_t>(a); } };

struct op_add     { template <typename T, typename U> auto operator()(const T &a, const U &b) const { return val(a) + val(b); } };
struct op_sub     { template <typename T, typename U> auto operator()(const T &a, const U &b) const { return val(a) - val(b); } };
struct op_mul     { template <typename T, typename U> auto operator()(const T &a, const U &b) const { return val(a) * val(b); } };
struct op_div     { template <typename T, typename U> auto operator()(const T &a, const U &b) const { return val(a) / val(b); } };

struct op_bit_and { template <typename T, typename U> auto operator()(const T &a, const U &b) const { return a.as_int() & b.as_int(); } };
struct op_bit_or  { template <typename T, typename U> auto operator()(const T &a, const U &b) const { return a.as_int() | b.as_int(); } };
struct op_bit_xor { template <typename T, typename U> auto operator()(const T &a, const U &b) const { return a.as_int() ^ b.as_int(); } };

struct op_gt      { template <typename T, typename U> bool operator()(const T &a, const U &b) const { return val(a) >  val(b); } };
struct op_lt      { template <typename T, typename U> bool operator()(const T &a, const U &b) const { return val(a) <  val(b); } };
struct op_gte     { template <typename T, typename U> bool operator()(const T &a, const U &b) const { return val(a) >= val(b); } };
struct op_lte     { template <typename T, typename U> bool operator()(const T &a, const U &b) const { return val(a) <= val(b); } };
struct op_eq      { template <typename T, typename U> bool operator()(const T &a, const U &b) const { return val(a) == val(b); } };
struct op_neq     { template <typename T, typename U> bool operator()(const T &a, const U &b) const { return val(a) != val(b); } };

template <typename Op> inline void destructive_op1(tag &a)
{
    constexpr const Op op;
    switch (a.type()) {
    case tag::INT:   a.set(op(a.as_int()));   break;
    case tag::FLOAT: a.set(op(a.as_float())); break;
    default:         break;
    }
}

template <typename Op> inline bool compare_op2(const tag &a, const tag &b)
{
    constexpr const Op op;
    static constexpr const uint16_t I = tag::INT,   I_ = I << tag::TAG_BITS,
                                    F = tag::FLOAT, F_ = F << tag::TAG_BITS;
    switch (a.sigs(b)) {
    case I_ | I: return op(wrap(a.as_int()),   wrap(b.as_int()));
    case I_ | F: return op(wrap(a.as_int()),   wrap(b.as_float()));
    case F_ | I: return op(wrap(a.as_float()), wrap(b.as_int()));
    case F_ | F: return op(wrap(a.as_float()), wrap(b.as_float()));
    default:     return false;
    }
}

template <typename Op> inline void destructive_op2(tag &a, const tag &b)
{
    constexpr const Op op;
    static constexpr const uint16_t I = tag::INT,   I_ = I << tag::TAG_BITS,
                                    F = tag::FLOAT, F_ = F << tag::TAG_BITS;
    switch (a.sigs(b)) {
    case I_ | I: a.set(op(wrap(a.as_int()),   wrap(b.as_int())));   break;
    case I_ | F: a.set(op(wrap(a.as_int()),   wrap(b.as_float()))); break;
    case F_ | I: a.set(op(wrap(a.as_float()), wrap(b.as_int())));   break;
    case F_ | F: a.set(op(wrap(a.as_float()), wrap(b.as_float()))); break;
    default:     break;
    }
}

template <typename Op> inline tag op1(const tag &a)               { tag c(a); destructive_op1<Op>(c);    return c; }
template <typename Op> inline tag op2(const tag &a, const tag &b) { tag c(a); destructive_op2<Op>(c, b); return c; }
}

inline tag operator+(const tag &a, const tag &b) { return tag_ops::op2<tag_ops::op_add>(a, b); }
inline tag operator-(const tag &a, const tag &b) { return tag_ops::op2<tag_ops::op_sub>(a, b); }
inline tag operator*(const tag &a, const tag &b) { return tag_ops::op2<tag_ops::op_mul>(a, b); }
inline tag operator/(const tag &a, const tag &b) { return tag_ops::op2<tag_ops::op_div>(a, b); }
inline tag operator&(const tag &a, const tag &b) { return tag_ops::op2<tag_ops::op_bit_and>(a, b); }
inline tag operator|(const tag &a, const tag &b) { return tag_ops::op2<tag_ops::op_bit_or> (a, b); }
inline tag operator^(const tag &a, const tag &b) { return tag_ops::op2<tag_ops::op_bit_xor>(a, b); }

inline tag &operator+=(tag &a, const tag &b) { tag_ops::destructive_op2<tag_ops::op_add>(a, b); return a; }
inline tag &operator-=(tag &a, const tag &b) { tag_ops::destructive_op2<tag_ops::op_sub>(a, b); return a; }
inline tag &operator*=(tag &a, const tag &b) { tag_ops::destructive_op2<tag_ops::op_mul>(a, b); return a; }
inline tag &operator/=(tag &a, const tag &b) { tag_ops::destructive_op2<tag_ops::op_div>(a, b); return a; }
inline tag &operator&=(tag &a, const tag &b) { tag_ops::destructive_op2<tag_ops::op_bit_and>(a, b); return a; }
inline tag &operator|=(tag &a, const tag &b) { tag_ops::destructive_op2<tag_ops::op_bit_or> (a, b); return a; }
inline tag &operator^=(tag &a, const tag &b) { tag_ops::destructive_op2<tag_ops::op_bit_xor>(a, b); return a; }

inline bool operator< (const tag &a, const tag &b) { return tag_ops::compare_op2<tag_ops::op_lt>(a, b); }
inline bool operator> (const tag &a, const tag &b) { return tag_ops::compare_op2<tag_ops::op_gt>(a, b); }
inline bool operator<=(const tag &a, const tag &b) { return tag_ops::compare_op2<tag_ops::op_lte>(a, b); }
inline bool operator>=(const tag &a, const tag &b) { return tag_ops::compare_op2<tag_ops::op_gte>(a, b); }
inline bool operator==(const tag &a, const tag &b) { return tag_ops::compare_op2<tag_ops::op_eq>(a, b); }
inline bool operator!=(const tag &a, const tag &b) { return tag_ops::compare_op2<tag_ops::op_neq>(a, b); }
