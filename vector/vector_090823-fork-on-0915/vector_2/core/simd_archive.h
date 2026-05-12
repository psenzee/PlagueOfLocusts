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

inline float ref_dot(const float *a, const float *b, const float *a_end)
{
    float sum = 0;
    for (; a != a_end; ++a, ++b)
        sum += *a * *b;
    return sum;
}

inline void _f32_sq(const float *in, float *out, __m128 &f)                            { f = _mm_loadu_ps(in); f = _mm_mul_ps(f, f); _mm_storeu_ps(out, f); }
inline void _f32_sq(const float *in, float *out, __m256 &f)                            { f = _mm256_loadu_ps(in); f = _mm256_mul_ps(f, f); _mm256_storeu_ps(out, f); }
inline void _f32_add(const float *a, const float *b, float *c, __m256 &fa, __m256 &fb) { fa = _mm256_loadu_ps(a); fb = _mm256_loadu_ps(b); fa = _mm256_add_ps(fa, fb); _mm256_storeu_ps(c, fa); }
inline void _f32_add(const float *a, const float *b, float *c, __m128 &fa, __m128 &fb) { fa = _mm_loadu_ps(a); fb = _mm_loadu_ps(b); fa = _mm_add_ps(fa, fb); _mm_storeu_ps(c, fa); }
inline void _f32_sub(const float *a, const float *b, float *c, __m256 &fa, __m256 &fb) { fa = _mm256_loadu_ps(a); fb = _mm256_loadu_ps(b); fa = _mm256_sub_ps(fa, fb); _mm256_storeu_ps(c, fa); }
inline void _f32_sub(const float *a, const float *b, float *c, __m128 &fa, __m128 &fb) { fa = _mm_loadu_ps(a); fb = _mm_loadu_ps(b); fa = _mm_sub_ps(fa, fb); _mm_storeu_ps(c, fa); }
inline void _f32_mul(const float *a, const float *b, float *c, __m256 &fa, __m256 &fb) { fa = _mm256_loadu_ps(a); fb = _mm256_loadu_ps(b); fa = _mm256_mul_ps(fa, fb); _mm256_storeu_ps(c, fa); }
inline void _f32_mul(const float *a, const float *b, float *c, __m128 &fa, __m128 &fb) { fa = _mm_loadu_ps(a); fb = _mm_loadu_ps(b); fa = _mm_mul_ps(fa, fb); _mm_storeu_ps(c, fa); }

inline float ref_dist_sq(const float *a, const float *b, const float *a_end)
{
    float sum = 0;
    for (; a != a_end; ++a, ++b)
        sum += sq(*a - *b);
    return sum;
}

// ////////////////////

// Horizontal sum of 4 lanes of the vector
inline float _f32_hadd_ps(__m128 r4)
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
inline float _f32_hadd_ps(__m256 r8)
{
    const __m128 low = _mm256_castps256_ps128( r8 );
    const __m128 high = _mm256_extractf128_ps( r8, 1 );
    return _f32_hadd_ps( _mm_add_ps( low, high ) );
}

// Compute a * b + acc; compiles either into a single FMA instruction, or into 2 separate SSE 1 instructions.
template <bool fma = false>
inline __m128 _f32_fmadd_ps(__m128 a, __m128 b, __m128 acc)
{
    if constexpr(fma)
        return _mm_fmadd_ps(a, b, acc);
    else
        return _mm_add_ps(_mm_mul_ps(a, b), acc);
}

// Same as above, for 8-wide AVX vectors
template <bool fma = false>
inline __m256 _f32_fmadd_ps(__m256 a, __m256 b, __m256 acc)
{
    if constexpr(fma)
        return _mm256_fmadd_ps(a, b, acc);
    else
        return _mm256_add_ps(_mm256_mul_ps(a, b), acc);
}

// sse

inline void _f32_dot(const float *p, const float *q, size_t chunk_index, __m128 &a, __m128 &b, __m128 &result)
{
    chunk_index *= 4;
    a = _mm_loadu_ps(p + chunk_index);
    b = _mm_loadu_ps(q + chunk_index);
    result = _f32_fmadd_ps(a, b, result);
}

inline void _f32_dot(const float *p, const float *q, size_t chunk_index, __m256 &a, __m256 &b, __m256 &result)
{
    chunk_index *= 8;
    a = _mm256_loadu_ps(p + chunk_index);
    b = _mm256_loadu_ps(q + chunk_index);
    result = _f32_fmadd_ps(a, b, result);
}

inline void _f32_self_dot(const float *p, size_t chunk_index, __m128 &a, __m128 &result)
{
    chunk_index *= 8;
    a = _mm_loadu_ps(p + chunk_index);
    result = _f32_fmadd_ps(a, a, result);
}

inline void _f32_self_dot(const float *p, size_t chunk_index, __m256 &a, __m256 &result)
{
    chunk_index *= 8;
    a = _mm256_loadu_ps(p + chunk_index);
    result = _f32_fmadd_ps(a, a, result);
}

inline void _f32_dist_sq(const float *p, const float *q, size_t chunk_index, __m128 &a, __m128 &b, __m128 &result)
{
    chunk_index *= 4;
    a = _mm_loadu_ps(p + chunk_index);
    b = _mm_loadu_ps(q + chunk_index);
    a = _mm_sub_ps(a, b);
    result = _f32_fmadd_ps(a, a, result);
}

inline void _f32_dist_sq(const float *p, const float *q, size_t chunk_index, __m256 &a, __m256 &b, __m256 &result)
{
    chunk_index *= 8;
    a = _mm256_loadu_ps(p + chunk_index);
    b = _mm256_loadu_ps(q + chunk_index);
    a = _mm256_sub_ps(a, b);
    result = _f32_fmadd_ps(a, a, result);
}

inline void  _f32_clear(__m256 *p, size_t count)                         { for (size_t i = 0; i < count; ++i) p[i] = _mm256_xor_si256(p[i], p[i]); }
inline void  _f32_clear(__m128 *p, size_t count)                         { for (size_t i = 0; i < count; ++i) p[i] = _mm_xor_si128(p[i], p[i]); }
inline float _f32_accumulate(__m128 &a)                                  { return _f32_hadd_ps(a); }
inline float _f32_accumulate(__m256 &a)                                  { return _f32_hadd_ps(a); }
inline float _f32_accumulate(__m128 &a, __m128 &b)                       { return _f32_hadd_ps(_mm_add_ps(a, b)); }
inline float _f32_accumulate(__m256 &a, __m256 &b)                       { return _f32_hadd_ps(_mm256_add_ps(a, b)); }
inline float _f32_accumulate(__m128 &a, __m128 &b, __m128 &c, __m128 &d) { return _f32_hadd_ps(_mm_add_ps(_mm_add_ps(a, b), _mm_add_ps(c, d))); }
inline float _f32_accumulate(__m256 &a, __m256 &b, __m256 &c, __m256 &d) { return _f32_hadd_ps(_mm256_add_ps(_mm256_add_ps(a, b), _mm256_add_ps(c, d))); }
inline float _f32_accumulate8(__m256 *r)
{
    return _f32_hadd_ps(_mm256_add_ps(_mm256_add_ps(_mm256_add_ps(r[0], r[1]), _mm256_add_ps(r[2], r[3])),
                                      _mm256_add_ps(_mm256_add_ps(r[4], r[5]), _mm256_add_ps(r[6], r[7]))));
}
inline float _f32_accumulate16(__m256 *r)
{
    return _f32_hadd_ps(
               _mm256_add_ps(
                        _mm256_add_ps(_mm256_add_ps(_mm256_add_ps(r[0], r[1]), _mm256_add_ps(r[2], r[3])),
                                      _mm256_add_ps(_mm256_add_ps(r[4], r[5]), _mm256_add_ps(r[6], r[7]))),
                        _mm256_add_ps(_mm256_add_ps(_mm256_add_ps(r[10], r[11]), _mm256_add_ps(r[8], r[9])),
                                      _mm256_add_ps(_mm256_add_ps(r[14], r[15]), _mm256_add_ps(r[12], r[13])))));
}

inline void f32_mul__avx(const float *p, float factor, float *out, const float * const p_end)
{
    assert((p_end - p) % 8 == 0);
    float factors[8] = { factor, factor, factor, factor, factor, factor, factor, factor };
    __m256 a, b, f = _mm256_loadu_ps(factors);
    for (; p < p_end; p += 8, out += 8) {
        a = _mm256_loadu_ps(p);
        b = _mm256_mul_ps(a, f);
        _mm256_storeu_ps(out, b);
    }
}

inline void f32_mul__avx(const float *p, const float *q, float *out, const float * const p_end)
{
    assert((p_end - p) % 8 == 0);
    __m256 a, b;
    for (; p < p_end; p += 8, q += 8, out += 8) {
        a = _mm256_loadu_ps(p);
        b = _mm256_loadu_ps(q);
        a = _mm256_mul_ps(a, b);
        _mm256_storeu_ps(out, a);
    }
}

inline float f32_dist_sq__stride64_avx(const float *p, const float *q, const float * const p_end)
{
    assert((p_end - p) % 64 == 0);
    __m256 d[8], a, b;
    _f32_clear(d, 8);
    for (; p < p_end; p += 64, q += 64) {
        _f32_dist_sq(p, q, 0, a, b, d[0]);
        _f32_dist_sq(p, q, 1, a, b, d[1]);
        _f32_dist_sq(p, q, 2, a, b, d[2]);
        _f32_dist_sq(p, q, 3, a, b, d[3]);
        _f32_dist_sq(p, q, 4, a, b, d[4]);
        _f32_dist_sq(p, q, 5, a, b, d[5]);
        _f32_dist_sq(p, q, 6, a, b, d[6]);
        _f32_dist_sq(p, q, 7, a, b, d[7]);
    }
    return _f32_accumulate8(d);
}

inline float f32_dist_sq__stride32_avx(const float *p, const float *q, const float * const p_end)
{
    assert((p_end - p) % 32 == 0);
    __m256 d[4], a, b;
    _f32_clear(d, 4);
    for (; p < p_end; p += 32, q += 32) {
        _f32_dist_sq(p, q, 0, a, b, d[0]);
        _f32_dist_sq(p, q, 1, a, b, d[1]);
        _f32_dist_sq(p, q, 2, a, b, d[2]);
        _f32_dist_sq(p, q, 3, a, b, d[3]);
    }
    return _f32_accumulate(d[0], d[1], d[2], d[3]);
}

inline float f32_dist_sq__stride8_avx(const float *p, const float *q, const float * const p_end)
{
    assert((p_end - p) % 8 == 0);
    __m256 a, b, d[1];
    _f32_clear(d, 1);
    for (; p < p_end; p += 8, q += 8)
        _f32_dist_sq(p, q, 0, a, b, d[0]);
    return _f32_accumulate(d[0]);
}

inline float f32_dot__stride32_avx(const float *p, const float *q, const float * const p_end)
{
    assert((p_end - p) % 32 == 0);
    __m256 d[4], a, b;
    _f32_clear(d, 4);
    for (; p < p_end; p += 32, q += 32) {
        _f32_dot(p, q, 0, a, b, d[0]);
        _f32_dot(p, q, 1, a, b, d[1]);
        _f32_dot(p, q, 2, a, b, d[2]);
        _f32_dot(p, q, 3, a, b, d[3]);
    }
    return _f32_accumulate(d[0], d[1], d[2], d[3]);
}

inline float f32_dot__stride8_avx(const float *p, const float *q, const float * const p_end)
{
    assert((p_end - p) % 8 == 0);
    __m256 a, b, d[1];
    _f32_clear(d, 1);
    for (; p < p_end; p += 8, q += 8)
        _f32_dot(p, q, 0, a, b, d[0]);
    return _f32_accumulate(d[0]);
}

inline float f32_self_dot__stride32_avx(const float *p, const float * const p_end)
{
    assert((p_end - p) % 32 == 0);
    __m256 d[4], a;
    _f32_clear(d, 4);
    for (; p < p_end; p += 32) {
        _f32_self_dot(p, 0, a, d[0]);
        _f32_self_dot(p, 1, a, d[1]);
        _f32_self_dot(p, 2, a, d[2]);
        _f32_self_dot(p, 3, a, d[3]);
    }
    return _f32_accumulate(d[0], d[1], d[2], d[3]);
}

inline float f32_self_dot__stride8_avx(const float *p, const float * const p_end)
{
    assert((p_end - p) % 8 == 0);
    __m256 a, d[1];
    _f32_clear(d, 1);
    for (; p < p_end; p += 8)
        _f32_self_dot(p, 0, a, d[0]);
    return _f32_accumulate(d[0]);
}

inline float f32_normalize__stride32_avx(const float *p, float *out, const float * const p_end)
{
    float norm = sqrt(f32_self_dot__stride32_avx(p, p_end));
    f32_mul__avx(p, float(1) / norm, out, p_end);
    return norm;
}

inline float f32_normalize__stride8_avx(const float *p, float *out, const float * const p_end)
{
    float norm = sqrt(f32_self_dot__stride32_avx(p, p_end));
    f32_mul__avx(p, float(1) / norm, out, p_end);
    return norm;
}

inline float f32_dist_sq_limit__stride32_avx(const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float max = std::numeric_limits<float>::max();
    assert((p_end - p) % 32 == 0);
    __m256 d[4], a, b;
    float r = 0.f;
    _f32_clear(d, 4);
    for (; p < p_end; p += 32, q += 32) {
        _f32_dist_sq(p, q, 0, a, b, d[0]);
        _f32_dist_sq(p, q, 1, a, b, d[1]);
        _f32_dist_sq(p, q, 2, a, b, d[2]);
        _f32_dist_sq(p, q, 3, a, b, d[3]);
        if ((r = _f32_accumulate(d[0], d[1], d[2], d[3])) > limit_sq)
            return max;
    }
    return r;
}

inline float f32_dist_sq_limit__stride64_avx(const float *p, const float *q, const float * const p_end, float limit_sq) // about 10% faster than stride32
{
    static constexpr float max = std::numeric_limits<float>::max();
    assert((p_end - p) % 64 == 0);
    __m256 d[8], a, b;
    float r = 0.f;
    _f32_clear(d, 8);
    for (; p < p_end; p += 64, q += 64) {
        _f32_dist_sq(p, q, 0, a, b, d[0]);
        _f32_dist_sq(p, q, 1, a, b, d[1]);
        _f32_dist_sq(p, q, 2, a, b, d[2]);
        _f32_dist_sq(p, q, 3, a, b, d[3]);
        _f32_dist_sq(p, q, 4, a, b, d[4]);
        _f32_dist_sq(p, q, 5, a, b, d[5]);
        _f32_dist_sq(p, q, 6, a, b, d[6]);
        _f32_dist_sq(p, q, 7, a, b, d[7]);
        if ((r = _f32_accumulate8(d)) > limit_sq)
            return max;
    }
    return r;
}

inline float f32_dist_sq_limit__stride64_2_avx(const float *p, const float *q, const float * const p_end, float limit_sq) // this is about 10% slower than the unrolled version
{
    static constexpr float max = std::numeric_limits<float>::max();
    assert((p_end - p) % 64 == 0);
    __m256 d[8], a, b, c;
    float r = 0.f;
    _f32_clear(d, 8);
    for (; p < p_end;) {
        for (size_t i = 0; i != 8; ++i, p += 8, q += 8) {
            a    = _mm256_loadu_ps(p);
            b    = _mm256_loadu_ps(q);
            a    = _mm256_sub_ps(a, b);
            b    = _mm256_mul_ps(a, a);
            d[i] = _mm256_add_ps(b, d[i]);
        }
        a = _mm256_add_ps(d[0], d[1]);
        b = _mm256_add_ps(d[2], d[3]);
        a = _mm256_add_ps(a, b);
        b = _mm256_add_ps(d[4], d[5]);
        c = _mm256_add_ps(d[6], d[7]);
        b = _mm256_add_ps(b, c);
        a = _mm256_add_ps(a, b);
        r = _f32_hadd_ps(a);
        if (r > limit_sq)
            return max;
    }
    return r;
}

inline float f32_dist_sq_limit__stride64_3_avx(const float *p, const float *q, const float * const p_end, float limit_sq) // this is no faster than f32_dist_sq_limit__stride64_avx
{
    static constexpr float max = std::numeric_limits<float>::max();
    assert((p_end - p) % 64 == 0);
    __m256 d[8], a, b, c;
    float r = 0.f;
    _f32_clear(d, 8);
    for (; p < p_end; p += 64, q += 64) {

        a    = _mm256_loadu_ps(p);
        b    = _mm256_loadu_ps(q);
        a    = _mm256_sub_ps(a, b);
        b    = _mm256_mul_ps(a, a);
        d[0] = _mm256_add_ps(b, d[0]);
        
        a    = _mm256_loadu_ps(p + 8);
        b    = _mm256_loadu_ps(q + 8);
        a    = _mm256_sub_ps(a, b);
        b    = _mm256_mul_ps(a, a);
        d[1] = _mm256_add_ps(b, d[1]);
        
        a    = _mm256_loadu_ps(p + 16);
        b    = _mm256_loadu_ps(q + 16);
        a    = _mm256_sub_ps(a, b);
        b    = _mm256_mul_ps(a, a);
        d[2] = _mm256_add_ps(b, d[2]);
        
        a    = _mm256_loadu_ps(p + 24);
        b    = _mm256_loadu_ps(q + 24);
        a    = _mm256_sub_ps(a, b);
        b    = _mm256_mul_ps(a, a);
        d[3] = _mm256_add_ps(b, d[3]);
        
        a    = _mm256_loadu_ps(p + 32);
        b    = _mm256_loadu_ps(q + 32);
        a    = _mm256_sub_ps(a, b);
        b    = _mm256_mul_ps(a, a);
        d[4] = _mm256_add_ps(b, d[4]);
        
        a    = _mm256_loadu_ps(p + 40);
        b    = _mm256_loadu_ps(q + 40);
        a    = _mm256_sub_ps(a, b);
        b    = _mm256_mul_ps(a, a);
        d[5] = _mm256_add_ps(b, d[5]);
        
        a    = _mm256_loadu_ps(p + 48);
        b    = _mm256_loadu_ps(q + 48);
        a    = _mm256_sub_ps(a, b);
        b    = _mm256_mul_ps(a, a);
        d[6] = _mm256_add_ps(b, d[6]);
        
        a    = _mm256_loadu_ps(p + 56);
        b    = _mm256_loadu_ps(q + 56);
        a    = _mm256_sub_ps(a, b);
        b    = _mm256_mul_ps(a, a);
        d[7] = _mm256_add_ps(b, d[7]);

        a = _mm256_add_ps(d[0], d[1]);
        b = _mm256_add_ps(d[2], d[3]);
        a = _mm256_add_ps(a, b);
        b = _mm256_add_ps(d[4], d[5]);
        c = _mm256_add_ps(d[6], d[7]);
        b = _mm256_add_ps(b, c);
        a = _mm256_add_ps(a, b);
        r = _f32_hadd_ps(a);
        if (r > limit_sq)
            return max;
    }
    return r;
}

inline float f32_dist_sq_limit__stride128_avx(const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float max = std::numeric_limits<float>::max();
    assert((p_end - p) % 128 == 0);
    __m256 d[16], a, b;
    float r = 0.f;
    _f32_clear(d, 16);
    for (; p < p_end; p += 128, q += 128) {
        _f32_dist_sq(p, q,  0, a, b, d[ 0]);
        _f32_dist_sq(p, q,  1, a, b, d[ 1]);
        _f32_dist_sq(p, q,  2, a, b, d[ 2]);
        _f32_dist_sq(p, q,  3, a, b, d[ 3]);
        _f32_dist_sq(p, q,  4, a, b, d[ 4]);
        _f32_dist_sq(p, q,  5, a, b, d[ 5]);
        _f32_dist_sq(p, q,  6, a, b, d[ 6]);
        _f32_dist_sq(p, q,  7, a, b, d[ 7]);
        _f32_dist_sq(p, q,  8, a, b, d[ 8]);
        _f32_dist_sq(p, q,  9, a, b, d[ 9]);
        _f32_dist_sq(p, q, 10, a, b, d[10]);
        _f32_dist_sq(p, q, 11, a, b, d[11]);
        _f32_dist_sq(p, q, 12, a, b, d[12]);
        _f32_dist_sq(p, q, 13, a, b, d[13]);
        _f32_dist_sq(p, q, 14, a, b, d[14]);
        _f32_dist_sq(p, q, 15, a, b, d[15]);
        if ((r = _f32_accumulate16(d)) > limit_sq)
            return max;
    }
    return r;
}

inline float f32_dist_sq_limit__stride16_avx(const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float max = std::numeric_limits<float>::max();
    assert((p_end - p) % 16 == 0);
    __m256 d[2], a, b;
    float r = 0.f;
    _f32_clear(d, 2);
    for (; p < p_end; p += 16, q += 16) {
        _f32_dist_sq(p, q, 0, a, b, d[0]);
        _f32_dist_sq(p, q, 1, a, b, d[1]);
        if ((r = _f32_accumulate(d[0], d[1])) > limit_sq)
            return max;
    }
    return r;
}

inline float f32_dist_sq_limit__stride8_avx(const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float max = std::numeric_limits<float>::max();
    assert((p_end - p) % 8 == 0);
    __m256 a, b, d[1];
    _f32_clear(d, 1);
    float r = 0.f;
    for (; p < p_end; p += 8, q += 8) {
        _f32_dist_sq(p, q, 0, a, b, d[0]);
        if ((r = _f32_accumulate(d[0])) > limit_sq)
            return max;
    }
    return r;
}

// -----------

inline float dist_sq(const float *a, const float *b, const float *a_end)
{
    return f32_dist_sq__stride64_avx(a, b, a_end);
}

inline float dist_sq_limit(const float *a, const float *b, const float *a_end, float limit_sq)
{
    //return f32_dist_sq_limit__stride64_3_avx(a, b, a_end, limit_sq);
    return f32_dist_sq_limit__stride64_avx(a, b, a_end, limit_sq);
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
