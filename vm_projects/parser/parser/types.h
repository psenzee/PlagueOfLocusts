#pragma once

#include <cstdint>
#include "strs.h"

class types
{
    static constexpr const char *_names[] =
    {
        "NIL", "STRING", "SYMBOL", "TOKEN", "INT32", "INT64", "FLOAT", "DOUBLE", "TYPE", "PAIR", "ARRAY", "QUOTE", "FRAME", "ERROR"
    };

public:
    enum Type : uint8_t
    {
        NIL,   STRING,   SYMBOL,   TOKEN,   INT32,   INT64,   FLOAT,    DOUBLE,   TYPE,   PAIR,   ARRAY,   QUOTE,   FRAME,   ERROR
    };

    static constexpr const char *type_name(Type type)       { return _names[type]; }
    static constexpr bool        is_atom(Type type)         { return type < PAIR; }
    static constexpr Type        type(const Type &v)        { return TYPE; }
    static constexpr Type        type(const int32_t &v)     { return INT32; }
    static constexpr Type        type(const int64_t &v)     { return INT64; }
    static constexpr Type        type(const float &v)       { return FLOAT; }
    static constexpr Type        type(const double &v)      { return DOUBLE; }
    
    static           Type        from_name(std::string_view s)
    {
        int r = str_find_ci(s, std::span(_names));
        return r == -1 ? ERROR : Type(r);
    }
};

inline std::ostream &operator<<(std::ostream &os, types::Type type) { return os << "@" << types::type_name(type); }

template <typename T> constexpr types::Type value_type()              { return types::NIL; }
template <>           constexpr types::Type value_type<types::Type>() { return types::TYPE; }
template <>           constexpr types::Type value_type<int32_t>()     { return types::INT32; }
template <>           constexpr types::Type value_type<int64_t>()     { return types::INT64; }
template <>           constexpr types::Type value_type<float>()       { return types::FLOAT; }
template <>           constexpr types::Type value_type<double>()      { return types::DOUBLE; }
