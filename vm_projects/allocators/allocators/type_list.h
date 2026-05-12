#pragma once

#include <cstddef>
#include <cstdint>

template <typename Type, size_t N = 4>
struct type_list
{
    typedef Type                  type_t;
    typedef std::array<type_t, N> types_t;
    
    types_t types;
    uint8_t count;

    constexpr type_list() : types({ }), count(0) {}
    constexpr type_list(type_t t0) : types({ t0 }), count(1) {}
    constexpr type_list(type_t t0, type_t t1) : types({ t0, t1 }), count(2) {}
    constexpr type_list(type_t t0, type_t t1, type_t t2) : types({ t0, t1, t2 }), count(3) {}
    constexpr type_list(type_t t0, type_t t1, type_t t2, type_t t3) : types({ t0, t1, t2, t3 }), count(4) {}
    constexpr type_list(const types_t &types, uint8_t count) : types(types), count(count) {}
};
