#pragma once

#include <cmath>
#include <limits>
#include <cstddef>

typedef uint16_t bfloat16;

inline bfloat16 f2bf(float value)
{
    constexpr size_t ENDIAN = (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__);
    union { float f; bfloat16 bf[2]; } fbf = { 0 };
    fbf.f = value;
    return fbf.bf[ENDIAN];
}

inline float bf2f(bfloat16 value)
{
    constexpr size_t ENDIAN = (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__);
    union { float f; bfloat16 bf[2]; } fbf = { 0 };
    fbf.bf[1 - ENDIAN] = 0;
    fbf.bf[ENDIAN] = value;
    return fbf.f;
}
