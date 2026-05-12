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

namespace simd
{

template <typename T> inline T sq(T u) { return u * u; }

inline void sq4(float *data)
{
    __m128 f = _mm_loadu_ps(data);
    f = _mm_mul_ps(f, f);
    _mm_storeu_ps(data, f);
}

inline void sq8(float *data)
{
    __m256 f = _mm256_loadu_ps(data);
    f = _mm256_mul_ps(f, f);
    _mm256_storeu_ps(data, f);
}

inline float dot8(const float *a, const float *b)
{
    float v[8];
    __m256 c = _mm256_loadu_ps(a),
           d = _mm256_loadu_ps(b);
    c = _mm256_dp_ps(c, d, 0xff);
    _mm256_storeu_ps(v, c);
    return v[0] + v[4];
}

inline float dot8(const float *a)
{
    float v[8];
    __m256 c = _mm256_loadu_ps(a);
    c = _mm256_dp_ps(c, c, 0xff);
    _mm256_storeu_ps(v, c);
    return v[0] + v[4];
}

inline float ref_dot(const float *a, const float *b, const float *a_end)
{
    float sum = 0;
    for (; a != a_end; ++a, ++b)
        sum += *a * *b;
    return sum;
}

inline void add8(const float *a, const float *b, float *c)
{
    __m256 fa = _mm256_loadu_ps(a),
           fb = _mm256_loadu_ps(b);
    fa = _mm256_add_ps(fa, fb);
    _mm256_storeu_ps(c, fa);
}

inline void add8(const float *a, float *b)
{
    __m256 fa = _mm256_loadu_ps(a),
           fb = _mm256_loadu_ps(b);
    fa = _mm256_add_ps(fa, fb);
    _mm256_storeu_ps(b, fa);
}

inline void sub8(const float *a, const float *b, float *c)
{
    __m256 fa = _mm256_loadu_ps(a),
           fb = _mm256_loadu_ps(b);
    fa = _mm256_sub_ps(fa, fb);
    _mm256_storeu_ps(c, fa);
}

inline void sub8(const float *a, float *b)
{
    __m256 fa = _mm256_loadu_ps(a),
           fb = _mm256_loadu_ps(b);
    fa = _mm256_sub_ps(fa, fb);
    _mm256_storeu_ps(b, fa);
}

void simd_square(float *data, const float *end)
{
    for (; data < end; data += 8)
        sq8(data);
    if (data == end)
        return;
    sq4(data);
    data += 4;
    if (data == end)
        return;
    for (; data < end; data++)
        *data = sq(*data);
}

inline float dist_sq4(const float *a, const float *b)
{
    float v[4];
    __m128 c = _mm_loadu_ps(a),
           d = _mm_loadu_ps(b);
    c = _mm_sub_ps(c, d);
    c = _mm_dp_ps(c, c, 0xff);
    _mm_storeu_ps(v, c);
    return v[0];
}

inline float dist_sq8(const float *a, const float *b)
{
    float v[8];
    __m256 c = _mm256_loadu_ps(a),
           d = _mm256_loadu_ps(b);
    c = _mm256_sub_ps(c, d);
    c = _mm256_dp_ps(c, c, 0xff);
    _mm256_storeu_ps(v, c);
    return v[0] + v[4];
}

inline float dist_sq32(const float *a, const float *b)
{
    float r = 0, v[32];
    __m256 a0 = _mm256_loadu_ps(a +  0), b0 = _mm256_loadu_ps(b +  0),
           a1 = _mm256_loadu_ps(a +  8), b1 = _mm256_loadu_ps(b +  8),
           a2 = _mm256_loadu_ps(a + 16), b2 = _mm256_loadu_ps(b + 16),
           a3 = _mm256_loadu_ps(a + 24), b3 = _mm256_loadu_ps(b + 24);
    a0 = _mm256_sub_ps(a0, b0); a1 = _mm256_sub_ps(a1, b1);
    a2 = _mm256_sub_ps(a2, b2); a3 = _mm256_sub_ps(a3, b3);
    a0 = _mm256_dp_ps(a0, a0, 0xff); a1 = _mm256_dp_ps(a1, a1, 0xff);
    a2 = _mm256_dp_ps(a2, a2, 0xff); a3 = _mm256_dp_ps(a3, a3, 0xff);
    _mm256_storeu_ps(v + 0,  a0); _mm256_storeu_ps(v + 8,  a1);
    _mm256_storeu_ps(v + 16, a2); _mm256_storeu_ps(v + 24, a3);
    return v[0] + v[4] + v[8] + v[12] + v[16] + v[20] + v[24] + v[28];
}

inline float dist_sq16(const float *a, const float *b)
{
    float r = 0, v[16];
    __m256 a0 = _mm256_loadu_ps(a +  0), b0 = _mm256_loadu_ps(b +  0),
           a1 = _mm256_loadu_ps(a +  8), b1 = _mm256_loadu_ps(b +  8);
    a0 = _mm256_sub_ps(a0, b0); a1 = _mm256_sub_ps(a1, b1);
    a0 = _mm256_dp_ps(a0, a0, 0xff); a1 = _mm256_dp_ps(a1, a1, 0xff);
    _mm256_storeu_ps(v, a0); _mm256_storeu_ps(v + 8, a1);
    return v[0] + v[4] + v[8] + v[12];
}

inline float dist_sq(const float *a, const float *b, const float *a_end)
{
    float r = 0;
    for (; a <= a_end - 32; a += 32, b += 32)
        r += dist_sq32(a, b);
    if (a_end - a >= 16) {
        r += dist_sq16(a, b);
        a += 16; b += 16;
    }
    if (a_end - a >= 8) {
        r += dist_sq8(a, b);
        a += 8; b += 8;
    }
    if (a_end - a >= 4) {
        r += dist_sq4(a, b);
        a += 4; b += 4;
    }
    for (; a_end - a > 0; ++a, ++b)
        r += sq(*a - *b);
    return r;
}

template <size_t N> inline float dist_sq(const std::array<float, N> &a, const std::array<float, N> &b)
{
    return dist_sq(a.data(), b.data(), a.data() + a.size());
}

inline float dist_sq(const std::vector<float> &a, const std::vector<float> &b)
{
    return dist_sq(a.data(), b.data(), a.data() + a.size());
}
/*
inline float dist_sq_limit(const float *a, const float *b, const float *a_end, float limit_sq)
{
    static constexpr float max(std::numeric_limits<float>::max());
    float r = 0;
    for (; a <= a_end - 32; a += 32, b += 32) {
        r += dist_sq32(a, b);
        if (r >= limit_sq) return max;
    }
    if (a_end - a >= 16) {
        r += dist_sq16(a, b);
        if (r >= limit_sq) return max;
        a += 16; b += 16;
    }
    if (a_end - a >= 8) {
        r += dist_sq8(a, b);
        if (r >= limit_sq) return max;
        a += 8; b += 8;
    }
    if (a_end - a >= 4) {
        r += dist_sq4(a, b);
        if (r >= limit_sq) return max;
        a += 4; b += 4;
    }
    for (; a_end - a > 0; ++a, ++b)
        r += sq(*a - *b);
    return r;
}

template <size_t N> inline float dist_sq_limit(const std::array<float, N> &a, const std::array<float, N> &b, float limit_sq)
{
    return dist_sq_limit(a.data(), b.data(), a.data() + a.size(), limit_sq);
}

inline float dist_sq_limit(const std::vector<float> &a, const std::vector<float> &b, float limit_sq)
{
    return dist_sq_limit(a.data(), b.data(), a.data() + a.size(), limit_sq);
}
*/

inline float ref_dist_sq(const float *a, const float *b, const float *a_end)
{
    float sum = 0;
    for (; a != a_end; ++a, ++b)
        sum += sq(*a - *b);
    return sum;
}


// ////////////////////

// Horizontal sum of 4 lanes of the vector
inline float hadd_ps( __m128 r4 )
{
    // Add 4 values into 2
    const __m128 r2 = _mm_add_ps( r4, _mm_movehl_ps( r4, r4 ) );
    // Add 2 lower values into the final result
    const __m128 r1 = _mm_add_ss( r2, _mm_movehdup_ps( r2 ) );
    // Return the lowest lane of the result vector.
    // The intrinsic below compiles into noop, modern compilers return floats in the lowest lane of xmm0 register.
    return _mm_cvtss_f32( r1 );
}

// Horizontal sum of 8 lanes of the vector
inline float hadd_ps( __m256 r8 )
{
    const __m128 low = _mm256_castps256_ps128( r8 );
    const __m128 high = _mm256_extractf128_ps( r8, 1 );
    return hadd_ps( _mm_add_ps( low, high ) );
}

// Compute a * b + acc; compiles either into a single FMA instruction, or into 2 separate SSE 1 instructions.
template<bool fma>
inline __m128 fmadd_ps( __m128 a, __m128 b, __m128 acc )
{
    if constexpr( fma )
        return _mm_fmadd_ps( a, b, acc );
    else
        return _mm_add_ps( _mm_mul_ps( a, b ), acc );
}

// Same as above, for 8-wide AVX vectors
template<bool fma>
inline __m256 fmadd_ps( __m256 a, __m256 b, __m256 acc )
{
   // if constexpr( fma )
   //     return _mm256_fmadd_ps( a, b, acc );
   // else
        return _mm256_add_ps( _mm256_mul_ps( a, b ), acc );
}

// ==== Vertical SSE version, with single accumulator register ====

template<bool fma>
inline float dot__sse_vertical( const float* p1, const float* p2, size_t count )
{
    assert( 0 == count % 4 );
    const float* const p1End = p1 + count;

    __m128 acc;
    // For the first 4 values we don't have anything to add yet, just multiplying
    {
        const __m128 a = _mm_loadu_ps( p1 );
        const __m128 b = _mm_loadu_ps( p2 );
        acc = _mm_mul_ps( a, b );
        p1 += 4;
        p2 += 4;
    }
    for( ; p1 < p1End; p1 += 4, p2 += 4 )
    {
        const __m128 a = _mm_loadu_ps( p1 );
        const __m128 b = _mm_loadu_ps( p2 );
        acc = fmadd_ps<fma>( a, b, acc );
    }
    return hadd_ps( acc );
}

// ==== Vertical AVX version, with single accumulator register ====

template<bool fma>
inline float dot__avx_vertical( const float* p1, const float* p2, size_t count )
{
    assert( 0 == count % 8 );
    const float* const p1End = p1 + count;

    __m256 acc;
    // For the first 8 values we don't have anything to add yet, just multiplying
    {
        const __m256 a = _mm256_loadu_ps( p1 );
        const __m256 b = _mm256_loadu_ps( p2 );
        acc = _mm256_mul_ps( a, b );
        p1 += 8;
        p2 += 8;
    }

    for( ; p1 < p1End; p1 += 8, p2 += 8 )
    {
        const __m256 a = _mm256_loadu_ps( p1 );
        const __m256 b = _mm256_loadu_ps( p2 );
        acc = fmadd_ps<fma>( a, b, acc );
    }
    return hadd_ps( acc );
}

// ==== Vertical SSE version, with up to 4 independent accumulators ====

template<int accumulators, bool fma = true>
inline float dot__sse_vertical_multi( const float* p1, const float* p2, size_t count )
{
    static_assert( accumulators > 1 && accumulators <= 4 );
    constexpr int valuesPerLoop = accumulators * 4;
    assert( 0 == count % valuesPerLoop );
    const float* const p1End = p1 + count;

    // These independent accumulators.
    // Depending on the accumulators template argument, some are unused, "unreferenced local variable" warning is OK.
    __m128 dot0, dot1, dot2, dot3;

    // For the first few values we don't have anything to add yet, just multiplying
    {
        __m128 a = _mm_loadu_ps( p1 );
        __m128 b = _mm_loadu_ps( p2 );
        dot0 = _mm_mul_ps( a, b );
        if constexpr( accumulators > 1 )
        {
            a = _mm_loadu_ps( p1 + 4 );
            b = _mm_loadu_ps( p2 + 4 );
            dot1 = _mm_mul_ps( a, b );
        }
        if constexpr( accumulators > 2 )
        {
            a = _mm_loadu_ps( p1 + 8 );
            b = _mm_loadu_ps( p2 + 8 );
            dot2 = _mm_mul_ps( a, b );
        }
        if constexpr( accumulators > 3 )
        {
            a = _mm_loadu_ps( p1 + 12 );
            b = _mm_loadu_ps( p2 + 12 );
            dot3 = _mm_mul_ps( a, b );
        }
        p1 += valuesPerLoop;
        p2 += valuesPerLoop;
    }

    // The main loop, reads valuesPerLoop floats from both vectors.
    for( ; p1 < p1End; p1 += valuesPerLoop, p2 += valuesPerLoop )
    {
        __m128 a = _mm_loadu_ps( p1 );
        __m128 b = _mm_loadu_ps( p2 );
        dot0 = fmadd_ps<fma>( a, b, dot0 );
        if constexpr( accumulators > 1 )
        {
            a = _mm_loadu_ps( p1 + 4 );
            b = _mm_loadu_ps( p2 + 4 );
            dot1 = fmadd_ps<fma>( a, b, dot1 );
        }
        if constexpr( accumulators > 2 )
        {
            a = _mm_loadu_ps( p1 + 8 );
            b = _mm_loadu_ps( p2 + 8 );
            dot2 = fmadd_ps<fma>( a, b, dot2 );
        }
        if constexpr( accumulators > 3 )
        {
            a = _mm_loadu_ps( p1 + 12 );
            b = _mm_loadu_ps( p2 + 12 );
            dot3 = fmadd_ps<fma>( a, b, dot3 );
        }
    }

    // Add the accumulators together into dot0. Using pairwise approach for slightly better precision, with 4 accumulators we compute ( d0 + d1 ) + ( d2 + d3 ).
    if constexpr( accumulators > 1 )
        dot0 = _mm_add_ps( dot0, dot1 );
    if constexpr( accumulators > 3 )
        dot2 = _mm_add_ps( dot2, dot3 );
    if constexpr( accumulators > 2 )
        dot0 = _mm_add_ps( dot0, dot2 );
    // Compute horizontal sum of all 4 lanes in dot0
    return hadd_ps( dot0 );
}

// ==== Vertical AVX version, with up to 4 independent accumulators ====

template<int accumulators, bool fma = true>
inline float dot__avx_vertical_multi( const float* p1, const float* p2, size_t count )
{
    static_assert( accumulators > 1 && accumulators <= 4 );
    constexpr int valuesPerLoop = accumulators * 8;
    assert( 0 == count % valuesPerLoop );
    const float* const p1End = p1 + count;

    // These independent accumulators.
    // Depending on the accumulators template argument, some are unused, "unreferenced local variable" warning is OK.
    __m256 dot0, dot1, dot2, dot3;

    // For the first few values we don't have anything to add yet, just multiplying
    {
        __m256 a = _mm256_loadu_ps( p1 );
        __m256 b = _mm256_loadu_ps( p2 );
        dot0 = _mm256_mul_ps( a, b );
        if constexpr( accumulators > 1 )
        {
            a = _mm256_loadu_ps( p1 + 8 );
            b = _mm256_loadu_ps( p2 + 8 );
            dot1 = _mm256_mul_ps( a, b );
        }
        if constexpr( accumulators > 2 )
        {
            a = _mm256_loadu_ps( p1 + 16 );
            b = _mm256_loadu_ps( p2 + 16 );
            dot2 = _mm256_mul_ps( a, b );
        }
        if constexpr( accumulators > 3 )
        {
            a = _mm256_loadu_ps( p1 + 24 );
            b = _mm256_loadu_ps( p2 + 24 );
            dot3 = _mm256_mul_ps( a, b );
        }
        p1 += valuesPerLoop;
        p2 += valuesPerLoop;
    }

    for( ; p1 < p1End; p1 += valuesPerLoop, p2 += valuesPerLoop )
    {
        __m256 a = _mm256_loadu_ps( p1 );
        __m256 b = _mm256_loadu_ps( p2 );
        dot0 = fmadd_ps<fma>( a, b, dot0 );
        if constexpr( accumulators > 1 )
        {
            a = _mm256_loadu_ps( p1 + 8 );
            b = _mm256_loadu_ps( p2 + 8 );
            dot1 = fmadd_ps<fma>( a, b, dot1 );
        }
        if constexpr( accumulators > 2 )
        {
            a = _mm256_loadu_ps( p1 + 16 );
            b = _mm256_loadu_ps( p2 + 16 );
            dot2 = fmadd_ps<fma>( a, b, dot2 );
        }
        if constexpr( accumulators > 3 )
        {
            a = _mm256_loadu_ps( p1 + 24 );
            b = _mm256_loadu_ps( p2 + 24 );
            dot3 = fmadd_ps<fma>( a, b, dot3 );
        }
    }

    // Add the accumulators together into dot0. Using pairwise approach for slightly better precision, with 4 accumulators we compute ( d0 + d1 ) + ( d2 + d3 ).
    if constexpr( accumulators > 1 )
        dot0 = _mm256_add_ps( dot0, dot1 );
    if constexpr( accumulators > 3 )
        dot2 = _mm256_add_ps( dot2, dot3 );
    if constexpr( accumulators > 2 )
        dot0 = _mm256_add_ps( dot0, dot2 );
    // Return horizontal sum of all 8 lanes of dot0
    return hadd_ps( dot0 );
}

inline float dist_sq__avx_vertical_multi(const float *p, const float *q, const float * const p_end)
{
    static constexpr size_t stride = 32;
    assert(0 == (p_end - p) % stride);

    // These independent accumulators.
    // Depending on the accumulators template argument, some are unused, "unreferenced local variable" warning is OK.
    __m256 d[4];

    // For the first few values we don't have anything to add yet, just multiplying
    __m256 a = _mm256_loadu_ps(p);
    __m256 b = _mm256_loadu_ps(q);
    a = _mm256_sub_ps(a, b);
    d[0] = _mm256_mul_ps(a, a);

    a = _mm256_loadu_ps(p + 8);
    b = _mm256_loadu_ps(q + 8);
    a = _mm256_sub_ps(a, b);
    d[1] = _mm256_mul_ps(a, a);

    a = _mm256_loadu_ps(p + 16);
    b = _mm256_loadu_ps(q + 16);
    a = _mm256_sub_ps(a, b);
    d[2] = _mm256_mul_ps(a, a);

    a = _mm256_loadu_ps(p + 24);
    b = _mm256_loadu_ps(q + 24);
    a = _mm256_sub_ps(a, b);
    d[3] = _mm256_mul_ps(a, a);

    p += stride;
    q += stride;

    for (; p < p_end; p += stride, q += stride)
    {
        a = _mm256_loadu_ps(p);
        b = _mm256_loadu_ps(q);
        a = _mm256_sub_ps(a, b);
        d[0] = fmadd_ps<true>(a, a, d[0]);

        a = _mm256_loadu_ps(p + 8);
        b = _mm256_loadu_ps(q + 8);
        a = _mm256_sub_ps(a, b);
        d[1] = fmadd_ps<true>(a, a, d[1]);

        a = _mm256_loadu_ps(p + 16);
        b = _mm256_loadu_ps(q + 16);
        a = _mm256_sub_ps(a, b);
        d[2] = fmadd_ps<true>(a, a, d[2]);

        a = _mm256_loadu_ps(p + 24);
        b = _mm256_loadu_ps(q + 24);
        a = _mm256_sub_ps(a, b);
        d[3] = fmadd_ps<true>(a, a, d[3]);
    }

    // Add the accumulators together into dot0. Using pairwise approach for slightly better precision, with 4 accumulators we compute ( d0 + d1 ) + ( d2 + d3 ).
    d[0] = _mm256_add_ps(d[0], d[1]);
    d[2] = _mm256_add_ps(d[2], d[3]);
    d[0] = _mm256_add_ps(d[0], d[2]);
    // Return horizontal sum of all 8 lanes of dot0
    return hadd_ps(d[0]);
}

inline float dist_sq__avx_vertical_multi_limit(const float *p, const float *q, const float * const p_end, float limit_sq, size_t check_shift)
{
    static constexpr size_t stride = 32;
    size_t chunk_mask = (size_t(1) << check_shift) - 1;
    assert((p_end - p) % stride == 0);
    const float * const start = p;

    // These independent accumulators.
    // Depending on the accumulators template argument, some are unused, "unreferenced local variable" warning is OK.
    __m256 d[4];

    // For the first few values we don't have anything to add yet, just multiplying
    __m256 a = _mm256_loadu_ps(p);
    __m256 b = _mm256_loadu_ps(q);
    a = _mm256_sub_ps(a, b);
    d[0] = _mm256_mul_ps(a, a);

    a = _mm256_loadu_ps(p + 8);
    b = _mm256_loadu_ps(q + 8);
    a = _mm256_sub_ps(a, b);
    d[1] = _mm256_mul_ps(a, a);

    a = _mm256_loadu_ps(p + 16);
    b = _mm256_loadu_ps(q + 16);
    a = _mm256_sub_ps(a, b);
    d[2] = _mm256_mul_ps(a, a);

    a = _mm256_loadu_ps(p + 24);
    b = _mm256_loadu_ps(q + 24);
    a = _mm256_sub_ps(a, b);
    d[3] = _mm256_mul_ps(a, a);
    
    if ((((p - start) >> 5) & chunk_mask) == 0) {
        __m256 s1 = _mm256_add_ps(d[0], d[1]),
               s2 = _mm256_add_ps(d[2], d[3]);
        s1 = _mm256_add_ps(s1, s2);
        // Return horizontal sum of all 8 lanes of dot0
        float r = hadd_ps(s1);
        if (r > limit_sq)
            return r;
    }

    p += stride;
    q += stride;

    for (; p < p_end; p += stride, q += stride)
    {
        a = _mm256_loadu_ps(p);
        b = _mm256_loadu_ps(q);
        a = _mm256_sub_ps(a, b);
        d[0] = fmadd_ps<true>(a, a, d[0]);

        a = _mm256_loadu_ps(p + 8);
        b = _mm256_loadu_ps(q + 8);
        a = _mm256_sub_ps(a, b);
        d[1] = fmadd_ps<true>(a, a, d[1]);

        a = _mm256_loadu_ps(p + 16);
        b = _mm256_loadu_ps(q + 16);
        a = _mm256_sub_ps(a, b);
        d[2] = fmadd_ps<true>(a, a, d[2]);

        a = _mm256_loadu_ps(p + 24);
        b = _mm256_loadu_ps(q + 24);
        a = _mm256_sub_ps(a, b);
        d[3] = fmadd_ps<true>(a, a, d[3]);
        
        if ((((p - start) >> 5) & chunk_mask) == 0) {
            __m256 s1 = _mm256_add_ps(d[0], d[1]),
                   s2 = _mm256_add_ps(d[2], d[3]);
            s1 = _mm256_add_ps(s1, s2);
            // Return horizontal sum of all 8 lanes of dot0
            float r = hadd_ps(s1);
            if (r > limit_sq)
                return r;
        }
    }

    // Add the accumulators together into dot0. Using pairwise approach for slightly better precision, with 4 accumulators we compute ( d0 + d1 ) + ( d2 + d3 ).
    d[0] = _mm256_add_ps(d[0], d[1]);
    d[2] = _mm256_add_ps(d[2], d[3]);
    d[0] = _mm256_add_ps(d[0], d[2]);
    // Return horizontal sum of all 8 lanes of dot0
    return hadd_ps(d[0]);
}

inline float dist_sq_limit(const float *a, const float *b, const float *a_end, float limit_sq)
{
    return dist_sq__avx_vertical_multi_limit(a, b, a_end, limit_sq, 0);
    //return dist_sq__avx_vertical_multi(a, b, a_end);
}

template <size_t N> inline float dist_sq_limit(const std::array<float, N> &a, const std::array<float, N> &b, float limit_sq)
{
    return dist_sq_limit(a.data(), b.data(), a.data() + a.size(), limit_sq);
}

inline float dist_sq_limit(const std::vector<float> &a, const std::vector<float> &b, float limit_sq)
{
    return dist_sq_limit(a.data(), b.data(), a.data() + a.size(), limit_sq);
}

// ////////////////////

// for raw C arrays:

template <typename T>              inline T cube(T u)                                                       { return u * u * u; }
template <typename T>              inline T sum(const T *v, const T *end)                                   { T s(0); for (; v != end; ++v) s += *v; return s; }
template <typename T>              inline T sum_abs(const T *v, const T *end)                               { T s(0); for (; v != end; ++v) s += std::abs(*v); return s; }
template <typename T>              inline T mean(const T *v, const T *end)                                  { return (end - v) <= 0 ? T(0) : (sum(v, end) / (end - v)); }
template <typename T>              inline T variance(T mean, const T *v, const T *end)                      { T s(0); for (; v != end; ++v) s += sq(v - mean); return s / ((end - v) - 1); }
template <typename T>              inline T variance(const T *v, const T *end)                              { return variance(mean(v, end), v, end); }
template <typename T>              inline T skewness(const T *v, const T *end)                              { T m(mean(v, end)); return skewness(m, variance(m, v, end), v, end); }
template <typename T>              inline T kurtosis(const T *v, const T *end)                              { T m(mean(v, end)); return kurtosis(m, variance(m, v, end), v, end); }
template <typename T, typename U>  inline T dot(const T *a, const U *b, const T *a_end)                     { T s(0); for (; a != a_end; ++a, ++b) s += *a * *b; return s; }
template <typename T>              inline T dot(const T *u, const T *u_end)                                 { T s(0); for (; u != u_end; ++u) s += sq(*u); return s; } // same as dot(u, u) but more efficient

template <typename T>              inline void lerp(const T *a, T *b, const T *a_end, T t)                  { for (; a != a_end; ++a, ++b) *b = (*b - *a) * t + *a; }

template <typename T, typename U>  inline T distance_sq(const T *a, const U *b, const T *a_end)             { T s(0); for (; a != a_end; ++a, ++b) s += sq(*a - *b); return s; }
template <typename T, typename U>  inline T distance(const T *a, const U *b, const T *a_end)                { return sqrt(distance_sq(a, b, a_end)); }
template <typename T, typename U>  inline T distance_limit(const T *a, const U *b, const T *a_end, T limit) { return sqrt(distance_sq_limit(a, b, a_end), limit * limit); }

template <typename T, typename U>  inline T distance_sq_limit(const T *a, const U *b, const T *a_end, T limit_sq)
{
    static constexpr T max(std::numeric_limits<T>::max());
    if (limit_sq == max) return distance_sq(a, b, a_end);
    T sum(0);
    for (; a != a_end; ++a, ++b) if ((sum += sq(*a - *b)) > limit_sq) return max;
    return sum;
}

template <typename T> inline T normalize(const T *v, const T *end)
{
    T norm(sqrt(dot(v, end))), z(0), inv(0);
    if (norm == z) return z;
    inv = 1 / norm;
    for (; v != end; ++v) *v *= inv;
    return norm;
}

template <typename T> inline T skewness(T mean, T variance, const T *v, const T *end)
{
    if (end - v <= 0) return T(0);
    T n(end - v), factor = n / ((n - 1.0) * (n - 2.0)), sum(0), sd = sqrt(variance);
    for (; v != end; ++v) sum += cube((*v - mean) / sd);
    return sum * factor;
}

template <typename T> inline T kurtosis(T mean, T variance, const T *v, const T *end)
{
    if (end - v <= 0) return T(0);
    T n(end - v), factor = n * (n + 1) / ((n - 1.0) * (n - 2.0) * (n - 3.0)), sum(0), sd = sqrt(variance);
    for (; v != end; ++v) sum += sq(sq((*v - mean) / sd));
    return sum * factor - (3.0 * (n - 1.0) * (n - 1.0)) / ((n - 2.0) * (n - 3.0));
}

template <typename T> inline auto least_squares(const T *a, const T *b, const T *a_end)
{
    T sum_a(0), sum_b(0), sum_ab(0), sum_aa(0);
    size_t n(a_end - a);
    for (size_t i = 0; i != n; ++i) {
        T ai(a[i]), bi(b[i]);
        sum_a += ai; sum_b += bi;
        sum_ab += ai * bi; sum_aa += ai * ai;
    }
    T m = (n * sum_ab - sum_a * sum_b) / (n * sum_aa - sum_a * sum_a);
    return std::pair<T, T>(m, (sum_b - m * sum_a) / n);
}

}
