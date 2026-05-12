#pragma once

#include <cstddef>
#include <cstdint>

struct value_union
{
    union {
        uint8_t  u8 [8];
        int8_t   s8 [8];
        uint16_t u16[4];
        int16_t  s16[4];
        uint32_t u32[2];
        int32_t  s32[2];
        float    f32[2];
        uint64_t u64;
        int64_t  s64;
        double   f64;
        intptr_t p;
    };

    template <typename T> const value_union &cast(const T *p) { const value_union *vu = reinterpret_cast<const value_union *>(p); return *vu; }
    template <typename T>       value_union &cast(T *p)       { value_union *vu = reinterpret_cast<value_union *>(p); return *vu; }

    void set()           { u64    = 0; }
    void set(uint8_t  v) { u8[0]  = v; }
    void set(uint16_t v) { u16[0] = v; }
    void set(uint32_t v) { u32[0] = v; }
    void set(uint64_t v) { u64    = v; }
    void set(int8_t   v) { s8[0]  = v; }
    void set(int16_t  v) { s16[0] = v; }
    void set(int32_t  v) { s32[0] = v; }
    void set(int64_t  v) { s64    = v; }
    void set(float    v) { f32[0] = v; }
    void set(double   v) { f64    = v; }
    void set(intptr_t v) { p      = v; }
};

struct value_union_type
{
    enum { _NONE = 0, _PTR = 4, _REAL = 8, _UINT = 16, _SINT = 32 };

    enum Type : uint8_t
    {
        NONE =         0, PTR = _PTR  | 3,
        F32  = _REAL | 2, F64 = _REAL | 3,
        U8   = _UINT | 0, U16 = _UINT | 1, U32 = _UINT | 2, U64 = _UINT | 3,
        S8   = _SINT | 0, S16 = _SINT | 1, S32 = _SINT | 2, S64 = _SINT | 3
    };
    uint8_t type;
    
    static constexpr Type type_of(uint8_t  v) { return  U8; }
    static constexpr Type type_of(uint16_t v) { return U16; }
    static constexpr Type type_of(uint32_t v) { return U32; }
    static constexpr Type type_of(uint64_t v) { return U64; }
    static constexpr Type type_of(int8_t   v) { return  S8; }
    static constexpr Type type_of(int16_t  v) { return S16; }
    static constexpr Type type_of(int32_t  v) { return S32; }
    static constexpr Type type_of(int64_t  v) { return S64; }
    static constexpr Type type_of(float    v) { return F32; }
    static constexpr Type type_of(double   v) { return F64; }
    static constexpr Type type_of(intptr_t v) { return PTR; }

    template <typename T> static constexpr Type type_of(const T *v) { return PTR; }
    
    template <typename T> void set_from(T v) { type = type_of(v); }
   
    bool     is(Type a)                         const { return type == a; }
    
    size_t   size()                             const { return 1 << (type & 3); }
    
    bool     is_pointer()                       const { return is(PTR); }
    bool     is_real()                          const { return (type & _REAL) != 0; }
    bool     is_integral()                      const { return (type & (_UINT | _SINT)) != 0; }
    bool     is_signed_int()                    const { return (type & _SINT) != 0; }
    bool     is_unsigned_int()                  const { return (type & _UINT) != 0; }
};
