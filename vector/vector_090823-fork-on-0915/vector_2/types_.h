#pragma once

#include <array>
#include <vector>
#include <algorithm>

namespace types
{

struct type_id
{
    enum Type
    {
        TYPE_NONE = 0,  TYPE_VOID,
        TYPE_BOOL,
        TYPE_INT8,      TYPE_UINT8,     TYPE_INT16,     TYPE_UINT16,
        TYPE_INT32,     TYPE_UINT32,    TYPE_INT64,     TYPE_UINT64,
        TYPE_FLOAT,     TYPE_DOUBLE,
        TYPE_CONST_PTR, TYPE_CONST_CSTR, TYPE_PTR, TYPE_OBJECT, TYPE_STRING
    };
    
    enum Attribute
    {
        ATTR_NONE =   0, ATTR_INT   = 1,   ATTR_UNSIGNED   = 2,  ATTR_FLOAT  =    4,
        ATTR_8b   =   8, ATTR_16b   = 16,  ATTR_32b       = 32,  ATTR_64b    =   64,
        ATTR_PTR  = 128, ATTR_CONST = 256, ATTR_STRING   = 512,  ATTR_OBJECT = 1024,
        
        ATTR_UINT    = ATTR_INT    | ATTR_UNSIGNED, ATTR_CONST_PTR  = ATTR_PTR       | ATTR_CONST,
        ATTR_STD_STR = ATTR_OBJECT | ATTR_STRING,   ATTR_CONST_CSTR = ATTR_CONST_PTR | ATTR_STRING,
    };
    
    Type        type;
    uint32_t    attributes;
    const char *name;
    
    inline constexpr type_id(Type type = TYPE_NONE, uint32_t attributes = ATTR_NONE, const char *name = "none") : type(type), attributes(attributes), name(name) {}
};

inline const type_id &type_none()       { static constexpr type_id t(type_id::TYPE_NONE,       type_id::ATTR_NONE,                      "none");       return t; }
inline const type_id &type_void()       { static constexpr type_id t(type_id::TYPE_VOID,       type_id::ATTR_INT   | type_id::ATTR_8b,  "void");       return t; }
inline const type_id &type_bool()       { static constexpr type_id t(type_id::TYPE_BOOL,       type_id::ATTR_INT   | type_id::ATTR_8b,  "bool");       return t; }
inline const type_id &type_int8()       { static constexpr type_id t(type_id::TYPE_INT8,       type_id::ATTR_INT   | type_id::ATTR_8b,  "int8");       return t; }
inline const type_id &type_uint8()      { static constexpr type_id t(type_id::TYPE_UINT8,      type_id::ATTR_UINT  | type_id::ATTR_8b,  "uint8");      return t; }
inline const type_id &type_int16()      { static constexpr type_id t(type_id::TYPE_INT16,      type_id::ATTR_INT   | type_id::ATTR_16b, "int16");      return t; }
inline const type_id &type_uint16()     { static constexpr type_id t(type_id::TYPE_UINT16,     type_id::ATTR_UINT  | type_id::ATTR_16b, "uint16");     return t; }
inline const type_id &type_int32()      { static constexpr type_id t(type_id::TYPE_INT32,      type_id::ATTR_INT   | type_id::ATTR_32b, "int32");      return t; }
inline const type_id &type_uint32()     { static constexpr type_id t(type_id::TYPE_UINT32,     type_id::ATTR_UINT  | type_id::ATTR_32b, "uint32");     return t; }
inline const type_id &type_int64()      { static constexpr type_id t(type_id::TYPE_INT64,      type_id::ATTR_INT   | type_id::ATTR_64b, "int64");      return t; }
inline const type_id &type_uint64()     { static constexpr type_id t(type_id::TYPE_UINT64,     type_id::ATTR_UINT  | type_id::ATTR_64b, "uint64");     return t; }
inline const type_id &type_float()      { static constexpr type_id t(type_id::TYPE_FLOAT,      type_id::ATTR_FLOAT | type_id::ATTR_32b, "float");      return t; }
inline const type_id &type_double()     { static constexpr type_id t(type_id::TYPE_DOUBLE,     type_id::ATTR_FLOAT | type_id::ATTR_64b, "double");     return t; }
inline const type_id &type_const_ptr()  { static constexpr type_id t(type_id::TYPE_CONST_PTR,  type_id::ATTR_CONST_PTR,                 "const_ptr");  return t; }
inline const type_id &type_const_cstr() { static constexpr type_id t(type_id::TYPE_CONST_CSTR, type_id::ATTR_CONST_CSTR,                "const_cstr"); return t; }
inline const type_id &type_ptr()        { static constexpr type_id t(type_id::TYPE_PTR,        type_id::ATTR_PTR,                       "ptr");        return t; }
inline const type_id &type_object()     { static constexpr type_id t(type_id::TYPE_OBJECT,     type_id::ATTR_OBJECT,                    "object");     return t; }
inline const type_id &type_string()     { static constexpr type_id t(type_id::TYPE_STRING,     type_id::ATTR_OBJECT,                    "string");     return t; }

inline const type_id &type(type_id::Type t)
{
    static const type_id *types[] = {
        &type_none(),
        &type_void(),
        &type_bool(),
        &type_int8(),      &type_uint8(),
        &type_int16(),     &type_uint16(),
        &type_int32(),     &type_uint32(),
        &type_int64(),     &type_uint64(),
        &type_float(),     &type_double(),
        &type_const_ptr(), &type_const_cstr(),
        &type_ptr(),
        &type_object(),
        &type_string(),
    };
    static const size_t size = sizeof(types) / sizeof(types[0]);
    return *types[t < size ? t : 0];
}

template <typename T> inline const type_id &type()                  { return type_none(); }
template <>           inline const type_id &type<bool>()            { return type_bool(); }
template <>           inline const type_id &type<uint8_t>()         { return type_uint8(); }
template <>           inline const type_id &type<int8_t>()          { return type_int8(); }
template <>           inline const type_id &type<uint16_t>()        { return type_uint16(); }
template <>           inline const type_id &type<int16_t>()         { return type_int16(); }
template <>           inline const type_id &type<uint32_t>()        { return type_uint32(); }
template <>           inline const type_id &type<int32_t>()         { return type_int32(); }
template <>           inline const type_id &type<uint64_t>()        { return type_uint64(); }
template <>           inline const type_id &type<int64_t>()         { return type_int64(); }
template <>           inline const type_id &type<float>()           { return type_float(); }
template <>           inline const type_id &type<double>()          { return type_double(); }
template <>           inline const type_id &type<const char *>()    { return type_const_cstr(); }
template <>           inline const type_id &type<const void *>()    { return type_const_ptr(); }
template <>           inline const type_id &type<void *>()          { return type_ptr(); }
template <>           inline const type_id &type<std::string>()     { return type_object(); }

template <typename T> inline const type_id &type(const T &_)        { return type<T>(); }

}
