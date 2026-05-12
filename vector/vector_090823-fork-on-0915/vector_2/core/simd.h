#pragma once

#include <iostream>
#include <limits>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstddef>

#include <immintrin.h>

#include "simd_avx.h"

namespace simd
{

template <typename T> inline T sq(T u) { return u * u; }

inline float ref_dot(const float *a, const float *b, const float *a_end)
{
    float sum = 0;
    for (; a != a_end; ++a, ++b)
        sum += *a * *b;
    return sum;
}

inline float ref_dist_sq(const float *a, const float *b, const float *a_end)
{
    float sum = 0;
    for (; a != a_end; ++a, ++b)
        sum += sq(*a - *b);
    return sum;
}

inline float dist_sq(const float *a, const float *b, const float *a_end)
{
    return simd_avx::dist_sq(a, b, a_end);
}

inline float dist_sq_limit(const float *a, const float *b, const float *a_end, float limit_sq)
{
    return simd_avx::dist_sq(a, b, a_end, limit_sq);
}

// stl container adapters
template <size_t N> inline float dist_sq(const std::array<float, N> &a, const std::array<float, N> &b)
{
    return dist_sq(a.data(), b.data(), a.data() + a.size());
}

inline float dist_sq(const std::vector<float> &a, const std::vector<float> &b)
{
    return dist_sq(a.data(), b.data(), a.data() + a.size());
}

template <size_t N> inline float dist_sq_limit(const std::array<float, N> &a, const std::array<float, N> &b, float limit_sq)
{
    return dist_sq_limit(a.data(), b.data(), a.data() + a.size(), limit_sq);
}

inline float dist_sq_limit(const std::vector<float> &a, const std::vector<float> &b, float limit_sq)
{
    return dist_sq_limit(a.data(), b.data(), a.data() + a.size(), limit_sq);
}

}
