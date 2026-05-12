#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string_view>
#include <limits>

struct pair;
struct func;
struct object;

struct tag
{
    enum Type : uint8_t  { PTR = 0, INT, FLOAT, FUNC, SYMBOL, OBJECT };
    
    static constexpr const size_t   TAG_BITS = 3;
    static constexpr const intptr_t TAG_MASK = 7;
    static constexpr const float    _NAN     = std::numeric_limits<float>::quiet_NaN();
    
    union { struct { uint32_t _; float f; }; intptr_t p; };

    tag() : p(0) {}
    explicit tag(int64_t v)       : p((v << TAG_BITS) | INT) {}
    explicit tag(float v)         : _(FLOAT), f(v) {}
    explicit tag(const object *v) : p(reinterpret_cast<intptr_t>(v) | OBJECT) {}

    Type                           type()                           const { return static_cast<Type>(p & TAG_MASK); }
    tag                           &set_type(Type t)                       { p &= ~TAG_MASK; p |= _int(t); return *this; }
    bool                           is(Type t)                       const { return (p & TAG_MASK) == t; }
    
    uint16_t                       sigs()                           const { return static_cast<uint16_t>(p & TAG_MASK); }
    uint16_t                       sigs(const tag &t)               const { return (sigs() << TAG_BITS)     | t.sigs(); }
    uint16_t                       sigs(const tag &t, const tag &u) const { return (sigs() << TAG_BITS * 2) | t.sigs(u); }
    
    void                           set(const tag &t)                      { p = t.p; }
    void                           set(const float &v)                    { f = v; _ = FLOAT; }
    void                           set(const uint64_t &v)                 { p = (v << TAG_BITS) | INT; }
    void                           set(const int64_t &v)                  { p = (v << TAG_BITS) | INT; }
    void                           set(const object *v)                   { p = reinterpret_cast<intptr_t>(v) | (v ? OBJECT : PTR); }
    void                           set(object *v)                         { p = reinterpret_cast<intptr_t>(v) | (v ? OBJECT : PTR); }
    void                           set(const pair *v)                     { p = reinterpret_cast<intptr_t>(v) | (v ? OBJECT : PTR); }
    void                           set(pair *v)                           { p = reinterpret_cast<intptr_t>(v) | (v ? OBJECT : PTR); }
    template <typename T> void     set(T v)                               { set(static_cast<int64_t>(v)); }
//  template <typename T> void     set(T *v)                              { p = reinterpret_cast<intptr_t>(v); }
//  template <typename T> void     set(const T *v)                        { p = reinterpret_cast<intptr_t>(v); }

    template <typename T> tag     &operator=(const T &v)                  { set(v); return *this; }

    float                          safe_float()                     const { return !is_number() ? _NAN : _ == FLOAT ? as_float() : _float(as_int()); }
    int64_t                        safe_int()                       const { return !is_number() ?    0 : _ == FLOAT ? _int(f)    : as_int(); }
    object                        *safe_object()                    const { return is(OBJECT)   ? reinterpret_cast<object *>(p & ~OBJECT) : nullptr; }
    pair                          *safe_pair()                      const { return _pair(safe_object()); }
    func                          *safe_func()                      const { return _func(safe_object()); }

    bool                           is_nil()                         const { return p == PTR || p == OBJECT; }
    bool                           is_int()                         const { return (p & TAG_MASK) == INT; }
    bool                           is_pointer()                     const { return !(p & TAG_MASK); }
    bool                           is_float()                       const { return _ == FLOAT; }
    bool                           is_number()                      const { auto t(p & TAG_MASK); return t == FLOAT || t == INT; }
    bool                           is_object()                      const { return is(OBJECT); }
    bool                           is_pair()                        const { return _pair(safe_object()) != nullptr; }
    bool                           is_func()                        const { return _func(safe_object()) != nullptr; }

    int64_t                        as_int()                         const { return _int(p >> TAG_BITS); }
    float                          as_float()                       const { return f; }
    
    template <typename T> T       *ptr()                            const { return reinterpret_cast<T *>(p); }
    template <typename T> const T *ptr()                            const { return reinterpret_cast<const T *>(p); }
    
    std::ostream                  &print_raw(std::ostream &os)      const;
    std::ostream                  &print(std::ostream &os)          const;

    bool                           is_zero()                        const { return (p >> TAG_BITS) == 0 || (_ == FLOAT && f == 0.f); }
    bool                           is_not_zero()                    const { return !is_zero(); }
    bool                           gt_zero()                        const { return (p >> TAG_BITS) >  0 || (_ == FLOAT && f >  0.f); }
    bool                           lt_zero()                        const { return (p >> TAG_BITS) <  0 || (_ == FLOAT && f <  0.f); }
    bool                           gte_zero()                       const { return (p >> TAG_BITS) >= 0 || (_ == FLOAT && f >= 0.f); }
    bool                           lte_zero()                       const { return (p >> TAG_BITS) <= 0 || (_ == FLOAT && f <= 0.f); }
    
    void                           add_const(int64_t iv, float fv)        { if (is_number()) { if (_ == FLOAT) f += fv; else p += (iv << TAG_BITS); } }
    void                           mul_const(int64_t iv, float fv)        { if (is_number()) { if (_ == FLOAT) f *= fv; else set(as_int() * iv);    } }
    void                           shr_const(int64_t c)                   { if (is(INT)) set(as_int() >> c); }
    void                           shl_const(int64_t c)                   { if (is(INT)) set(as_int() << c); }

    bool                           operator!()                      const { return !is_zero(); }
    tag                            operator~()                      const { return is(INT) ? tag(~as_int()) : *this; }
    tag                            operator-()                      const { return !is_number() ? *this : ((_ == FLOAT) ? tag(-f) : tag(-as_int())); }
    tag                           &operator++(int)                        { add_const( 1,  1.f); return *this; }
    tag                           &operator++()                           { add_const( 1,  1.f); return *this; }
    tag                           &operator--(int)                        { add_const(-1, -1.f); return *this; }
    tag                           &operator--()                           { add_const(-1, -1.f); return *this; }

    static const tag nil;
    
private:

    template <typename T> static constexpr float    _float(T v)      { return static_cast<float>(v); }
    template <typename T> static constexpr int64_t  _int(T v)        { return static_cast<int64_t>(v); }
    static                                 pair    *_pair(object *o);
    static                                 func    *_func(object *o);
};

inline std::ostream &operator<<(std::ostream &os, const tag &t) { return t.print(os); }

#include "tag_ops.h"
