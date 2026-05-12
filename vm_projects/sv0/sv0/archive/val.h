#pragma once

#include <iostream>
#include <cstddef>
#include <cstdint>

enum class Type : uint8_t  { NIL = 0, INT, FLOAT, FUNC, PTR };

struct env;

struct function
{
    typedef void (*fptr_t)(env *);
    
    fptr_t fptr;
};

inline                       Type type_for(const float &)    { return Type::FLOAT; }
inline                       Type type_for(const function *) { return Type::FUNC; }
template <typename T> inline Type type_for(const T &)        { return Type::INT; }
template <typename T> inline Type type_for(const T *)        { return Type::PTR; }

struct val
{
    template <typename T> static float   _float(const T &v) { return static_cast<float>(v); }
    template <typename T> static int64_t _int(const T &v)   { return static_cast<int64_t>(v); }
    
    union { float f; int64_t i; const function *func; void *ptr; };
    Type type;
    
    constexpr val()                           : type(Type::NIL), func(nullptr) {}
    template <typename T>
    constexpr explicit val(const T &v)        : type(Type::INT), i(_int(v)) {}
    constexpr explicit val(float v)           : type(Type::FLOAT), f(v) {}
    constexpr explicit val(bool v)            : type(Type::INT), i(v) {}
    constexpr explicit val(const function *f) : type(Type::FUNC), func(f) {}
    template <typename T>
    constexpr explicit val(const T *v)        : type(Type::PTR), ptr(v) {}
    
    template <typename T> T     value()                   const { return _int(i); }
    template <> float           value<float>()            const { return f; }
    template <> const function *value<const function *>() const { return func; }

    val                        &set_nil()                       { type = Type::NIL;   i = 0; return *this; }
    val                        &set(const float &v)             { type = Type::FLOAT; f = static_cast<float>(v); return *this; }
    val                        &set(const function *v)          { type = Type::FUNC;  func = v; return *this; }
    template <typename T> val  &set(const T &v)                 { type = Type::INT;   i = static_cast<int64_t>(v); return *this; }
    template <typename T> val  &set(T *v)                       { type = Type::PTR;   ptr = reinterpret_cast<void *>(v); return *this; }
    
    bool                       is(Type t)                 const { return type == t; }
    template <typename T> bool is()                       const { return false; }
    template <>           bool is<float>()                const { return is(Type::FLOAT); }
    template <>           bool is<int64_t>()              const { return is(Type::INT); }
    template <>           bool is<int>()                  const { return is(Type::INT); }
    template <>           bool is<const function *>()     const { return is(Type::FUNC); }
    
    bool                       type_eq(const val &b)      const { return type == b.type; }

    std::ostream &print(std::ostream &os) const;
};

template <typename Op> inline val &set_unary_result(val &a)
{
    constexpr const Op op;
    switch (a.type) {
    case Type::INT:   return a.set(op(a.i));
    case Type::FLOAT: return a.set(op(a.f));
    case Type::FUNC:  return a.set(op(a.func));
    case Type::NIL:
    default:          return a.set_nil();
    }
}

template <typename Op> inline val &set_binary_result(val &a, const val &b)
{
    constexpr const Op op;
    static constexpr const uint16_t I = uint16_t(Type::INT),   I_ = I << 4,
                                    F = uint16_t(Type::FLOAT), F_ = F << 4;
    uint16_t at = uint16_t(a.type), bt = uint16_t(b.type);
    switch (at << 4 | bt) {
    case I_ | I: return a.set(op(a.i, b.i));
    case I_ | F: return a.set(op(a.i, b.f));
    case F_ | I: return a.set(op(a.f, b.i));
    case F_ | F: return a.set(op(a.f, b.f));
    default: return a.set_nil();
    }
}
