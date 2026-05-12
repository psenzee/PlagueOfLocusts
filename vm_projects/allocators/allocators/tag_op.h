#pragma once

#include "types.h"
#include <iostream>
#include <limits>
#include "math_operators.h"

class function;

struct tag_op
{
    union { float f; int64_t i; const function *func; };
    Type type;
    
    constexpr tag_op()                           : type(Type::NONE), func(nullptr) {}
    constexpr explicit tag_op(float v)           : type(Type::FLOAT), f(v) {}
    constexpr explicit tag_op(int64_t v)         : type(Type::INT), i(v) {}
    constexpr explicit tag_op(const function *f) : type(Type::FUNC), func(f) {}
    
    template <typename T> T     val()                   const { return T{}; }
    
    template <> float           val<float>()            const { return f; }
    template <> int64_t         val<int64_t>()          const { return i; }
    template <> const function *val<const function *>() const { return func; }
    
    std::ostream &print(std::ostream &os) const;
    
    enum PairSignature : uint16_t {
        I = uint16_t(Type::INT), F = uint16_t(Type::FLOAT), FUN = uint16_t(Type::FUNC), N = uint16_t(Type::NONE),
        II = (I << 4) | I, IF = (I << 4) | F, FI = (F << 4) | I, FF = (F << 4) | F,
    };
    
    static uint16_t signature(const tag_op &a, const tag_op &b) { return (uint16_t(a.type) << 4) | uint16_t(b.type); }

    static constexpr float nan() { return std::numeric_limits<float>::quiet_NaN(); }
    
    template <typename Op>
    tag_op &set_binary_arithmetic(const tag_op &b)
    {
        constexpr const Op op;
        switch ((uint16_t(type) << 4) | uint16_t(b.type)) {
        case II: i = op(i, b.i); return *this;
        case FI: f = op(f, b.i); return *this;
        case FF: f = op(f, b.f); return *this;
        case IF: return set(op(i, b.f));
        default: return set_none();
        }
    }
    
    template <typename Op>
    tag_op &set_binary_logical(const tag_op &b)
    {
        constexpr const Op op;
        switch ((uint16_t(type) << 4) | uint16_t(b.type)) {
        case II: i = op(i, b.i); return *this;
        case IF: i = op(i, b.f); return *this;
        case FI: return set(op(f, b.i));
        case FF: return set(op(f, b.f));
        default: return set_none();
        }
    }
    
    template <typename Op>
    tag_op &set_unary_logical()
    {
        constexpr const Op op;
        switch (type) {
        case Type::INT:   i = op(i); return *this;
        case Type::FLOAT: return set(op(f));
        case Type::FUNC:  return set(op(func));
        case Type::NONE:
        default:          return set_none();
        }
    }

    tag_op &set_none()                         { type = Type::NONE;  func = nullptr; return *this; }
    tag_op &set(bool v)                        { type = Type::INT;   i = v;    return *this; }
    tag_op &set(const int64_t &v)              { type = Type::INT;   i = v;    return *this; }
    tag_op &set(const float &v)                { type = Type::FLOAT; f = v;    return *this; }
    tag_op &set(const function *v)             { type = Type::FUNC;  func = v; return *this; }

    bool    is(Type t)                      const { return type == t; }
    template <typename T> bool is()         const { return false; }
    template <> bool is<float>()            const { return type == Type::FLOAT; }
    template <> bool is<int64_t>()          const { return type == Type::INT; }
    template <> bool is<int>()              const { return type == Type::INT; }
    template <> bool is<const function *>() const { return type == Type::FUNC; }
    
    bool    is_int()                     const { return type == Type::INT; }
    bool    is_float()                   const { return type == Type::FLOAT; }
    bool    is_number()                  const { return type == Type::FLOAT || type == Type::INT; }
    bool    is_func()                    const { return type == Type::FUNC; }
    bool    is_none()                    const { return type == Type::NONE; }
       
    bool    both_int(const tag_op &b)    const { return is_int() && b.is_int(); }
    bool    both_float(const tag_op &b)  const { return is_float() && b.is_float(); }
    
    float   as_float()                   const { return is_float() ? f : (is_int() ? static_cast<float>(i) : nan()); }
    
    tag_op &f2i()                              { return is_float() ? set(static_cast<int64_t>(f)) : set_none(); }
    tag_op &i2f()                              { return is_int()   ? set(static_cast<float>(i))   : set_none(); }
    
    tag_op &op_not()                           { return set(is_int() ? ~i : (func != nullptr)); }
    tag_op &op_neg()                           { return is_int() ? set(-i) : (is_float() ? set(-f) : *this);  }
    tag_op &op_shl(size_t n)                   { return is_int() ? set(i << n) : *this;  }
    tag_op &op_shr(size_t n)                   { return is_int() ? set(i >> n) : *this;  }

//    tag_op &pred_not ()                        { return unary_logical_fi<operator_logical_not>(); }
//    tag_op &pred_bool()                        { return unary_logical_fi<operator_bool       >(); }
};

template <typename Op>
inline tag_op &set_unary_result(tag_op &a)
{
    constexpr const Op op;
    switch (a.type) {
    case Type::INT:   return a.set(op(a.i));
    case Type::FLOAT: return a.set(op(a.f));
    case Type::FUNC:  return a.set(op(a.func));
    case Type::NONE:
    default:          return a.set_none();
    }
}

template <typename Op>
inline tag_op &set_binary_result(tag_op &a, const tag_op &b)
{
    constexpr const Op op;
    switch (tag_op::signature(a, b)) {
    case tag_op::II: return a.set(op(a.i, b.i));
    case tag_op::IF: return a.set(op(a.i, b.f));
    case tag_op::FI: return a.set(op(a.f, b.i));
    case tag_op::FF: return a.set(op(a.f, b.f));
    default: return a.set_none();
    }
}

inline tag_op &op_add   (tag_op &a, const tag_op &b) { return set_binary_result<op_real_add>(a, b); }
inline tag_op &op_sub   (tag_op &a, const tag_op &b) { return set_binary_result<op_real_sub>(a, b); }
inline tag_op &op_mul   (tag_op &a, const tag_op &b) { return set_binary_result<op_real_mul>(a, b); }
inline tag_op &op_div   (tag_op &a, const tag_op &b) { return set_binary_result<op_real_div>(a, b); }
inline tag_op &op_mod   (tag_op &a, const tag_op &b) { return set_binary_result<op_real_mod>(a, b); }
inline tag_op &bits_and (tag_op &a, const tag_op &b) { return set_binary_result<op_bits_and>(a, b); }
inline tag_op &bits_or  (tag_op &a, const tag_op &b) { return set_binary_result<op_bits_or >(a, b); }
inline tag_op &bits_xor (tag_op &a, const tag_op &b) { return set_binary_result<op_bits_xor>(a, b); }
inline tag_op &pred_lt  (tag_op &a, const tag_op &b) { return set_binary_result<op_pred_lt >(a, b); }
inline tag_op &pred_lte (tag_op &a, const tag_op &b) { return set_binary_result<op_pred_lte>(a, b); }
inline tag_op &pred_eq  (tag_op &a, const tag_op &b) { return set_binary_result<op_pred_eq >(a, b); }
inline tag_op &pred_gte (tag_op &a, const tag_op &b) { return set_binary_result<op_pred_gte>(a, b); }
inline tag_op &pred_gt  (tag_op &a, const tag_op &b) { return set_binary_result<op_pred_gt >(a, b); }
inline tag_op &pred_and (tag_op &a, const tag_op &b) { return set_binary_result<op_pred_and>(a, b); }
inline tag_op &pred_or  (tag_op &a, const tag_op &b) { return set_binary_result<op_pred_or >(a, b); }
