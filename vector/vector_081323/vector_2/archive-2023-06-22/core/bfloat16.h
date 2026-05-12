#pragma once

#include <cmath>
#include <limits>
#include <cstddef>

constexpr size_t ENDIAN = 1; // 1 for little, 0 for big

typedef uint16_t bfloat16;

inline bfloat16 f2bf(float value)
{
    union { float f; bfloat16 bf[2]; } fbf = { 0 };
    fbf.f = value;
    return fbf.bf[ENDIAN];
}

inline float bf2f(bfloat16 value)
{
    union { float f; bfloat16 bf[2]; } fbf = { 0 };
    fbf.bf[1 - ENDIAN] = 0;
    fbf.bf[ENDIAN] = value;
    return fbf.f;
}
