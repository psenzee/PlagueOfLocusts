#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <variant>
#include <cstdint>

namespace
{
template <typename V, std::floating_point    T> T cast_as(const V &v) { return static_cast<T>(std::get<double>  (v)); }
template <typename V, std::unsigned_integral T> T cast_as(const V &v) { return static_cast<T>(std::get<uint64_t>(v)); }
template <typename V, std::signed_integral   T> T cast_as(const V &v) { return static_cast<T>(std::get<int64_t> (v)); }
template <typename V, std::floating_point    T> V variant(const T &u) { return V(static_cast<double>  (u)); }
template <typename V, std::unsigned_integral T> V variant(const T &u) { return V(static_cast<uint64_t>(u)); }
template <typename V, std::signed_integral   T> V variant(const T &u) { return V(static_cast<int64_t> (u)); }
}

struct numeric
{
    typedef std::variant<uint64_t, int64_t, double> variant_t;
    typedef std::array<uint8_t, sizeof(uint64_t)>   bytes_t;
    
    numeric() : _value(0ull), _size_of(0) {}
    numeric(const numeric &u) : _value(u._value), _size_of(u._size_of) {}

    template <typename T> explicit numeric(const T &u) : _value(variant<variant_t, T>(u)),  _size_of(sizeof(T)) {}
    template <typename T> numeric &operator=(const T &u) { _value = variant<variant_t, T>(u); _size_of = sizeof(T); return *this; }

    numeric &operator=(const numeric &u) { if (this != &u) { _value = u._value; _size_of = u._size_of; } return *this; }

    size_t                  index()                                      const { return _value.index(); }
    size_t                  size()                                       const { return _size_of; }
    const variant_t        &value()                                      const { return _value; }
    template <typename T> T as()                                         const { return cast_as<variant_t, T>(_value); }

    size_t                  bytes(uint8_t *into, size_t size = 0)        const;
    bytes_t                 bytes(size_t size)                           const;
    std::ostream           &print(std::ostream &os)                      const;
    std::ostream           &print_hex(std::ostream &os, size_t size = 0) const;
    std::ostream           &write(std::ostream &os, size_t size = 0)     const;

private:

    variant_t _value;
    uint8_t   _size_of;
};

inline std::ostream &operator<<(std::ostream &os, const numeric &n) { return n.print(os); }
