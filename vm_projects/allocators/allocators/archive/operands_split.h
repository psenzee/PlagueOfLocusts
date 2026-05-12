#pragma once

#include "types.h"
#include "stack.h"
#include "functions.h"

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
};

#define RETURN_TAG_OP(TAG, RETURN_EXPR) \
{switch (TAG) { \
case Type::FLOAT: return tag_op(floats.RETURN_EXPR); \
case Type::INT:   return tag_op(ints.RETURN_EXPR);   \
case Type::FUNC:  return tag_op(funcs.RETURN_EXPR);  \
default:          return tag_op();                   \
}}

#define TAG_OPERATION(TAG, OPERATION) \
{switch (TAG) { \
case Type::FLOAT: floats.OPERATION;  break; \
case Type::INT:   ints.OPERATION;    break; \
case Type::FUNC:  funcs.OPERATION;   break; \
default:                             break; \
}}

#define PUSH_TAG_VALUE(TAG, FLOAT_VAL, INT_VAL, FUNC_VAL) \
{switch (TAG) { \
case Type::FLOAT: floats.push(FLOAT_VAL); break; \
case Type::INT:   ints.push(INT_VAL);     break; \
case Type::FUNC:  funcs.push(FUNC_VAL);   break; \
default:                                  break; \
}}

struct operands
{
    static constexpr const size_t TYPE_COUNT = 64, FLOAT_COUNT = 32, INT_COUNT = 32, FUNC_COUNT = 32;
    
    typedef stack<Type>              stack_tag_t;
    typedef stack<float>             stack_float_t;
    typedef stack<int64_t>           stack_int_t;
    typedef stack<const function *>  stack_func_t;
    /*
    typedef stack_n<Type,             TYPE_COUNT>  stack_tag_t;
    typedef stack_n<float,            FLOAT_COUNT> stack_float_t;
    typedef stack_n<int64_t,          INT_COUNT>   stack_int_t;
    typedef stack_n<const function *, FUNC_COUNT>  stack_func_t;
    */
    
    stack_n<Type,             TYPE_COUNT>  tags;
    stack_n<float,            FLOAT_COUNT> floats;
    stack_n<int64_t,          INT_COUNT>   ints;
    stack_n<const function *, FUNC_COUNT>  funcs;
    
    operands() {}

    bool           is_top(Type t)            const { return tags.top() == t; }
    bool           is_at(Type t, size_t n)   const { return tags.at(n) == t; }

    void           pushi(int64_t v)                { tags.push(Type::INT);   ints.push(v);   }
    void           pushf(float v)                  { tags.push(Type::FLOAT); floats.push(v); }
    void           push(const function *f)         { tags.push(Type::FUNC);  funcs.push(f);  }
    void           seti(int64_t v)                 { if (is_top(Type::INT))   ints.set(v);   else { drop(); pushi(v); } }
    void           setf(float v)                   { if (is_top(Type::FLOAT)) floats.set(v); else { drop(); pushf(v); } }
    void           set(const function *f)          { if (is_top(Type::FUNC))  funcs.set(f);  else { drop(); push(f);  } }
    void           drop()                          { TAG_OPERATION(tags.pop(), drop()) }
    void           swap()                          { if (tags.eq()) TAG_OPERATION(tags.top(), swap()) else tags.swap(); }
    void           over()                          { dupe_from(); }
    void           f2i()                           { _check(Type::FLOAT); tags.set(Type::INT);   ints.push(static_cast<int64_t>(floats.pop())); }
    void           i2f()                           { _check(Type::INT);   tags.set(Type::FLOAT); floats.push(static_cast<float>(ints.pop())); }
    tag_op         at(size_t n)              const { RETURN_TAG_OP(tags.at(n), at(n)) }
    tag_op         pop()                           { RETURN_TAG_OP(tags.pop(), pop()) }
    void           dupe(size_t n = 1)              { tags.dupe(n); TAG_OPERATION(tags.top(), dupe(n)) }
    void           dupe_from(size_t at = 1)        { Type type = tags.at(at); tags.push(type); PUSH_TAG_VALUE(type, floats.at(at), ints.at(at), funcs.at(at)) }
    void           push(const tag_op &v)           { tags.push(v.type); PUSH_TAG_VALUE(v.type, v.f, v.i, v.func) }

    std::ostream &print(std::ostream &os) const;
    
private:
    
    void _check(Type tag, size_t n = 0) { if (tag != tags.at(n)) { std::cout << "bad type at stack[" << n << "]\n"; } }

    stack_float_t &stack_for(const float &)        { return floats; }
    stack_int_t   &stack_for(const int64_t &)      { return ints; }
    stack_func_t  &stack_for(const function *)     { return funcs; }
    
    uint16_t _tag_at(size_t n, size_t sh = 0) const { return uint16_t(tags.at(n)) << (sh * 4); }

    uint16_t _sig1() const { return _tag_at(0); }
    uint16_t _sig2() const { return _tag_at(0) | _tag_at(1, 1); }
    uint16_t _sig3() const { return _tag_at(0) | _tag_at(1, 1) | _tag_at(2, 2); }
    uint16_t _sig4() const { return _tag_at(0) | _tag_at(1, 1) | _tag_at(2, 2) | _tag_at(3, 3); }
};

