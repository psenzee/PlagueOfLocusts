#pragma once

#include <iostream>
#include <iomanip>
#include <variant>
#include <string>
#include <cstdint>

struct token
{
    enum Type { SYMBOL = 0, FLOAT, INT8, INT16, INT32, INT64, UINT8, UINT16, UINT32, UINT64, NONE };
    
    struct none {};

    std::variant<std::string, double, int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, none> var;

    token() : var() {}
    template <typename T> token(const T &v) : var(v) {}

    std::ostream &print(std::ostream &os) const;
    
    Type type()       const { return static_cast<Type>(var.index()); }

    bool is(Type t)   const { return type() == t; }
    
    bool is_number()  const { auto t = type(); return t >= FLOAT && t <= UINT64; }
    bool is_integer() const { auto t = type(); return t >= INT8 && t <= UINT64; }

    template <typename T>           const T &as()       const { return std::get<T>(var); }
    template <typename T, Type TID>       T  cast()     const { return static_cast<T>(std::get<TID>(var)); }
    
    template <typename T> T as_float() const { return cast<T, FLOAT>(); }

    template <typename T> T as_int()   const
    {
        switch (type()) {
        case INT8:   return cast<T, INT8>  ();
        case INT16:  return cast<T, INT16> ();
        case INT32:  return cast<T, INT32> ();
        case INT64:  return cast<T, INT64> ();
        case UINT8:  return cast<T, UINT8> ();
        case UINT16: return cast<T, UINT16>();
        case UINT32: return cast<T, UINT32>();
        case UINT64: return cast<T, UINT64>();
        default:     return T{};
        }
    }
    
    bool is_symbol_starting_with(char ch) const { return is(SYMBOL) && as<std::string>()[0] == ch; }
};

/*
struct token
{
    enum Type { SYMBOL = 0, FLOAT, INT, UINT, NONE };
    enum IntSize { ANY, L8, INT16, INT32, INT64, UINT8, UINT16, UINT32, UINT64 };
    
    struct none {};

    uint8_t size_of;
    std::variant<std::string, double, int64_t, uint64_t, none> var;

    token() : var() {}
    template <typename T> explicit token(const T &v) : var(v), size_of(sizeof(T)) {}

    std::ostream &print(std::ostream &os) const;
    
    Type type()       const { return static_cast<Type>(var.index()); }

    bool is(Type t)   const { return type() == t; }
    
    bool is_number()  const { auto t = type(); return t >= FLOAT && t <= UINT; }
    bool is_integer() const { auto t = type(); return is(INT) || is(UINT); }

    template <typename T>           const T &as()       const { return std::get<T>(var); }
    template <typename T, Type TID>       T  cast()     const { return static_cast<T>(std::get<TID>(var)); }
    template <typename T>                 T  as_float() const { return cast<T, FLOAT>(); }
    
    bool is_symbol_starting_with(char ch) const { return is(SYMBOL) && as<std::string>()[0] == ch; }
};
 */
inline std::ostream &operator<<(std::ostream &os, const token &t) { return t.print(os); }

/*
#include <iostream>
#include <iomanip>
#include <string_view>

struct token
{
    enum Type { SYMBOL = 0, FLOAT, INT8, INT16, INT32, INT64, UINT8, UINT16, UINT32, UINT64, NONE };
    
    struct none {};

    std::variant<std::string, double, int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, none> var;

    template <typename T> static constexpr Type get_type              (const T &v)           { return NONE;   }
    template <>                  constexpr Type get_type<uint8_t>     (const uint8_t &v)     { return UINT8;  }
    template <>                  constexpr Type get_type<uint16_t>    (const uint16_t &v)    { return UINT16; }
    template <>                  constexpr Type get_type<uint32_t>    (const uint32_t &v)    { return UINT32; }
    template <>                  constexpr Type get_type<uint64_t>    (const uint64_t &v)    { return UINT64; }
    template <>                  constexpr Type get_type<int8_t>      (const int8_t &v)      { return INT8;   }
    template <>                  constexpr Type get_type<int16_t>     (const int16_t &v)     { return INT16;  }
    template <>                  constexpr Type get_type<int32_t>     (const int32_t &v)     { return INT32;  }
    template <>                  constexpr Type get_type<int64_t>     (const int64_t &v)     { return INT64;  }
    template <>                  constexpr Type get_type<float>       (const float &v)       { return FLOAT;  }
    template <>                  constexpr Type get_type<std::string> (const std::string &v) { return SYMBOL; }
    
    token()                                 : var() {}
    template <typename T> token(const T &v) : var(v) {}

    std::ostream &print(std::ostream &os) const;
    
    Type type()                           const { return static_cast<Type>(var.index()); }
    bool is(Type t)                       const { return type() == t; }
    template <typename T> const T &as()   const { return std::get< get_type<T> >(var); }
    /*
    const std::string &as_symbol()        const { return std::get<SYMBOL>(var); }
    float              as_float()         const { return std::get<FLOAT> (var); }
    uint8_t            as_uint8()         const { return std::get<UINT8> (var); }
    uint16_t           as_uint16()        const { return std::get<UINT16>(var); }
    uint32_t           as_uint32()        const { return std::get<UINT32>(var); }
    uint64_t           as_uint64()        const { return std::get<UINT64>(var); }
    int8_t             as_int8()          const { return std::get<INT8>  (var); }
    int16_t            as_int16()         const { return std::get<INT16> (var); }
    int32_t            as_int32()         const { return std::get<INT32> (var); }
    int64_t            as_int64()         const { return std::get<INT64> (var); }

    int64_t            as_integer()       const
    {
        switch (type()) {
        case INT8:   return std::get<token::INT8>  (var);
        case UINT8:  return std::get<token::UINT8> (var);
        case INT16:  return std::get<token::INT16> (var);
        case UINT16: return std::get<token::UINT16>(var);
        case INT32:  return std::get<token::INT32> (var);
        case UINT32: return std::get<token::UINT32>(var);
        case INT64:  return std::get<token::INT64> (var);
        case UINT64: return std::get<token::UINT64>(var);
        }
        return 0;
    }
    * /

    bool is_none()                        const { return var.index() == NONE; }
    bool is_float()                       const { return var.index() == FLOAT; }
    bool is_symbol()                      const { return var.index() == SYMBOL; }
    bool is_number()                      const { auto t = type(); return t >= FLOAT && t <= UINT64; }
    bool is_integer()                     const { auto t = type(); return t >= INT8  && t <= UINT64; }
    bool is_unsigned()                    const { auto t = type(); return t >= UINT8 && t <= UINT64; }
    bool is_signed()                      const { auto t = type(); return t >= INT8  && t <= INT64; }
    bool is_symbol_starting_with(char ch) const { return is_symbol() && as_symbol()[0] == ch; }
};

#include <iostream>
#include <iomanip>
#include <variant>
#include <string>

struct token
{
    enum Type { SYMBOL = 0, FLOAT, INT8, INT16, INT32, INT64, UINT8, UINT16, UINT32, UINT64, NONE };
    
    struct none {};

    std::variant<std::string, double, int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, none> var;

    template <typename T> static constexpr Type get_type              (const T &v)           { return NONE;   }
    template <>                  constexpr Type get_type<uint8_t>     (const uint8_t &v)     { return UINT8;  }
    template <>                  constexpr Type get_type<uint16_t>    (const uint16_t &v)    { return UINT16; }
    template <>                  constexpr Type get_type<uint32_t>    (const uint32_t &v)    { return UINT32; }
    template <>                  constexpr Type get_type<uint64_t>    (const uint64_t &v)    { return UINT64; }
    template <>                  constexpr Type get_type<int8_t>      (const int8_t &v)      { return INT8;   }
    template <>                  constexpr Type get_type<int16_t>     (const int16_t &v)     { return INT16;  }
    template <>                  constexpr Type get_type<int32_t>     (const int32_t &v)     { return INT32;  }
    template <>                  constexpr Type get_type<int64_t>     (const int64_t &v)     { return INT64;  }
    template <>                  constexpr Type get_type<float>       (const float &v)       { return FLOAT;  }
    template <>                  constexpr Type get_type<std::string> (const std::string &v) { return SYMBOL; }
    
    token()                                 : var() {}
    template <typename T> token(const T &v) : var(v) {}

    std::ostream &print(std::ostream &os) const;
    
    Type type()                           const { return static_cast<Type>(var.index()); }
    bool is(Type t)                       const { return type() == t; }
    template <typename T> const T &as()   const { return std::get< get_type<T> >(var); }
};
*/
