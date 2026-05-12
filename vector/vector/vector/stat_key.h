#pragma once

#include <iostream>
#include <limits>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>

#include "stl.h"
#include "dct.h"
#include "value_index_t.h"

template <typename Value> inline std::ostream &print_hex(std::ostream &os, const Value &v, size_t width = 0)
{
    const size_t SIZE = sizeof(v), BITS = SIZE * 8;
    uint64_t MASK = (uint64_t(1) << BITS) - 1;
    return print_hex(os, uint64_t(v) & MASK, width ? width : sizeof(v) * 2);
}

template <> inline std::ostream &print_hex(std::ostream &os, const uint64_t &v, size_t width)
{
    return os << std::hex << std::setfill('0') << std::setw(int(width ? width : (sizeof(v) * 2))) << v << std::dec;
}

template <typename Value, size_t N> inline std::ostream &print_hex(std::ostream &os, const std::array<Value, N> &k, size_t width = 0)
{
    for (size_t i = 0; i < N; i++) print_hex(os << (i == 0 ? "" : " "), k[i]);
    return os;
}

template <typename Value> inline Value clamp(const Value &v, const Value &lo, const Value &hi) { return std::max(std::min(v, hi), lo); }

struct stat_key
{
    typedef stat_key self_t;
    
    enum Type {
        TYPE_RDPFM    = 0x7f,
        TYPE_ZEROS    = 0x7e,
        TYPE_EQUIFLAT = 0x7d,
        TYPE_SPARSE   = 0x7c,
    };
    
    static constexpr float MAX_MAGNITUDE = float(0x0ffff);

    int16_t  _rank;
    uint8_t  _type, _density, _peaked, _flatness;
    uint16_t _magnitude;
       
    inline stat_key() : _type(TYPE_RDPFM), _rank(0x8000), _density(0), _peaked(0), _flatness(0), _magnitude(0) {}
       
    inline uint64_t bits() const
    {
        switch (_type) {
        case TYPE_RDPFM: return _bits_rdpfm();
        }
        return 0;
    }

    inline self_t &set_type(uint8_t type)                    { _type = type;  return *this; }
    inline self_t &set_rank(size_t index, float value = 0.f) { _rank = int16_t(index); if (value < 0) _rank = -_rank; return *this; }

    inline self_t &set_density(float value)                  { _density = value; return *this; }
    inline self_t &set_peaked(float value)                   { _peaked = value; return *this; }
    inline self_t &set_flatness(float value)                 { _flatness = value; return *this; }
    inline self_t &set_magnitude(uint16_t value)             { _magnitude = value; return *this; }
    inline self_t &set_magnitude(float value)                { return set_magnitude(uint16_t(clamp(value, 0.f, MAX_MAGNITUDE))); }
    
    inline std::ostream &print(std::ostream &os) const
    {
        print_hex(os << "r:", _rank)      << " ";;
        print_hex(os << "t:", _type)      << " ";;
        print_hex(os << "d:", _density)   << " ";;
        print_hex(os << "p:", _peaked)    << " ";;
        print_hex(os << "f:", _flatness)  << " ";;
        print_hex(os << "m:", _magnitude) << " ";
        return print_hex(os << " [", bits()) << "]";
    }
    
private:

    inline uint64_t _bits_rdpfm() const
    {
        uint64_t t(_type), r(_rank), d(_density), p(_peaked), f(_flatness), m(_magnitude);
        return (r << 48) | (t << 40) | (d << 32) | (p << 24) | (f << 16) | m;
    }
};
    
inline std::ostream &operator<<(std::ostream &os, const stat_key &k) { return k.print(os); }
