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

                      inline const type_id &type_none()             { static constexpr type_id t(type_id::TYPE_NONE,       type_id::ATTR_NONE,                      "none");       return t; }
                      inline const type_id &type_void()             { static constexpr type_id t(type_id::TYPE_VOID,       type_id::ATTR_INT   | type_id::ATTR_8b,  "void");       return t; }
                      inline const type_id &type_object()           { static constexpr type_id t(type_id::TYPE_OBJECT,     type_id::ATTR_OBJECT,                    "object");     return t; }

template <typename T> inline const type_id &type()                  { return type_none(); }
template <>           inline const type_id &type<bool>()            { static constexpr type_id t(type_id::TYPE_BOOL,       type_id::ATTR_INT   | type_id::ATTR_8b,  "bool");       return t; }
template <>           inline const type_id &type<uint8_t>()         { static constexpr type_id t(type_id::TYPE_UINT8,      type_id::ATTR_UINT  | type_id::ATTR_8b,  "uint8");      return t; }
template <>           inline const type_id &type<int8_t>()          { static constexpr type_id t(type_id::TYPE_INT8,       type_id::ATTR_INT   | type_id::ATTR_8b,  "int8");       return t; }
template <>           inline const type_id &type<uint16_t>()        { static constexpr type_id t(type_id::TYPE_UINT16,     type_id::ATTR_UINT  | type_id::ATTR_16b, "uint16");     return t; }
template <>           inline const type_id &type<int16_t>()         { static constexpr type_id t(type_id::TYPE_INT16,      type_id::ATTR_INT   | type_id::ATTR_16b, "int16");      return t; }
template <>           inline const type_id &type<uint32_t>()        { static constexpr type_id t(type_id::TYPE_UINT32,     type_id::ATTR_UINT  | type_id::ATTR_32b, "uint32");     return t; }
template <>           inline const type_id &type<int32_t>()         { static constexpr type_id t(type_id::TYPE_INT32,      type_id::ATTR_INT   | type_id::ATTR_32b, "int32");      return t; }
template <>           inline const type_id &type<uint64_t>()        { static constexpr type_id t(type_id::TYPE_UINT64,     type_id::ATTR_UINT  | type_id::ATTR_64b, "uint64");     return t; }
template <>           inline const type_id &type<int64_t>()         { static constexpr type_id t(type_id::TYPE_INT64,      type_id::ATTR_INT   | type_id::ATTR_64b, "int64");      return t; }
template <>           inline const type_id &type<float>()           { static constexpr type_id t(type_id::TYPE_FLOAT,      type_id::ATTR_FLOAT | type_id::ATTR_32b, "float");      return t; }
template <>           inline const type_id &type<double>()          { static constexpr type_id t(type_id::TYPE_DOUBLE,     type_id::ATTR_FLOAT | type_id::ATTR_64b, "double");     return t; }
template <>           inline const type_id &type<const char *>()    { static constexpr type_id t(type_id::TYPE_CONST_CSTR, type_id::ATTR_CONST_CSTR,                "const_cstr"); return t; }
template <>           inline const type_id &type<const void *>()    { static constexpr type_id t(type_id::TYPE_CONST_PTR,  type_id::ATTR_CONST_PTR,                 "const_ptr");  return t; }
template <>           inline const type_id &type<void *>()          { static constexpr type_id t(type_id::TYPE_PTR,        type_id::ATTR_PTR,                       "ptr");        return t; }
template <>           inline const type_id &type<std::string>()     { static constexpr type_id t(type_id::TYPE_STRING,     type_id::ATTR_OBJECT,                    "string");     return t; }

inline const type_id &type(type_id::Type t)
{
    // same order as the Type enum
    static const type_id *types[] = {
        &type_none(),          &type_void(),       &type<bool>(),    &type<int8_t>(),   &type<uint8_t>(),  &type<int16_t>(),  &type<uint16_t>(),
        &type<int32_t>(),      &type<uint32_t>(),  &type<int64_t>(), &type<uint64_t>(), &type<float>(),    &type<double>(),   &type<const void *>(),
        &type<const char *>(), &type<void *>(),    &type_object(),   &type<std::string>(),
    };
    static const size_t size = sizeof(types) / sizeof(types[0]);
    return *types[t < size ? t : 0];
}

template <typename T> inline const type_id &type(const T &_)        { return type<T>(); }

}
