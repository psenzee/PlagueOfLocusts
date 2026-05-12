#include "simd_avx.h"

#include <limits>
#include <cmath>
#include <algorithm>
#include <cstddef>

#include <immintrin.h>

namespace simd_avx
{

template <typename T> inline static T _sq(T u) { return u * u; }

template <typename T> struct _mm_traits {};
template <> struct _mm_traits<__m128> { static constexpr size_t count = 4; };
template <> struct _mm_traits<__m256> { static constexpr size_t count = 8; };

inline void _f32_sq(const float *in, float *out, __m128 &f)                            { f = _mm_loadu_ps(in); f = _mm_mul_ps(f, f); _mm_storeu_ps(out, f); }
inline void _f32_sq(const float *in, float *out, __m256 &f)                            { f = _mm256_loadu_ps(in); f = _mm256_mul_ps(f, f); _mm256_storeu_ps(out, f); }
inline void _f32_add(const float *a, const float *b, float *c, __m256 &fa, __m256 &fb) { fa = _mm256_loadu_ps(a); fb = _mm256_loadu_ps(b); fa = _mm256_add_ps(fa, fb); _mm256_storeu_ps(c, fa); }
inline void _f32_add(const float *a, const float *b, float *c, __m128 &fa, __m128 &fb) { fa = _mm_loadu_ps(a); fb = _mm_loadu_ps(b); fa = _mm_add_ps(fa, fb); _mm_storeu_ps(c, fa); }
inline void _f32_sub(const float *a, const float *b, float *c, __m256 &fa, __m256 &fb) { fa = _mm256_loadu_ps(a); fb = _mm256_loadu_ps(b); fa = _mm256_sub_ps(fa, fb); _mm256_storeu_ps(c, fa); }
inline void _f32_sub(const float *a, const float *b, float *c, __m128 &fa, __m128 &fb) { fa = _mm_loadu_ps(a); fb = _mm_loadu_ps(b); fa = _mm_sub_ps(fa, fb); _mm_storeu_ps(c, fa); }
inline void _f32_mul(const float *a, const float *b, float *c, __m256 &fa, __m256 &fb) { fa = _mm256_loadu_ps(a); fb = _mm256_loadu_ps(b); fa = _mm256_mul_ps(fa, fb); _mm256_storeu_ps(c, fa); }
inline void _f32_mul(const float *a, const float *b, float *c, __m128 &fa, __m128 &fb) { fa = _mm_loadu_ps(a); fb = _mm_loadu_ps(b); fa = _mm_mul_ps(fa, fb); _mm_storeu_ps(c, fa); }

inline float _f32_hadd_ps(__m128 r4)
{
    // Add 4 values into 2
    const __m128 r2 = _mm_add_ps(r4, _mm_movehl_ps(r4, r4));
    // Add 2 lower values into the final result
    const __m128 r1 = _mm_add_ss(r2, _mm_movehdup_ps(r2));
    // Return the lowest lane of the result vector.
    // The intrinsic below compiles into noop, modern compilers return floats in the lowest lane of xmm0 register.
    return _mm_cvtss_f32(r1);
}

inline float _f32_hadd_ps(__m256 r8)
{
    const __m128 low = _mm256_castps256_ps128(r8);
    const __m128 high = _mm256_extractf128_ps(r8, 1);
    return _f32_hadd_ps(_mm_add_ps(low, high));
}

template <bool fma = false>
inline __m256 _f32_fmadd_ps(__m256 a, __m256 b, __m256 acc)
{
    if constexpr(fma)
        return _mm256_fmadd_ps(a, b, acc);
    else
        return _mm256_add_ps(_mm256_mul_ps(a, b), acc);
}

inline void _f32_dot(const float *p, const float *q, size_t chunk_index, __m256 &a, __m256 &b, __m256 &result)
{
    chunk_index *= 8;
    a = _mm256_loadu_ps(p + chunk_index);
    b = _mm256_loadu_ps(q + chunk_index);
    result = _f32_fmadd_ps(a, b, result);
}

inline void _f32_self_dot(const float *p, size_t chunk_index, __m256 &a, __m256 &result)
{
    chunk_index *= 8;
    a = _mm256_loadu_ps(p + chunk_index);
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

template <size_t N> inline void _dist_sq    (const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) {}
template <> inline void _dist_sq< 1>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r)
{
    a = _mm256_loadu_ps(p + i * 8);
    b = _mm256_loadu_ps(q + i * 8);
    a = _mm256_sub_ps(a, b);
    r[i] = _f32_fmadd_ps(a, a, r[i]);
}
template <> inline void _dist_sq< 2>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { _dist_sq<1>(p, q, i, a, b, r); _dist_sq<1>(p, q, i + 1, a, b, r); }
template <> inline void _dist_sq< 4>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { _dist_sq<2>(p, q, i, a, b, r); _dist_sq<2>(p, q, i + 2, a, b, r); }
template <> inline void _dist_sq< 8>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { _dist_sq<4>(p, q, i, a, b, r); _dist_sq<4>(p, q, i + 4, a, b, r); }
template <> inline void _dist_sq<16>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { _dist_sq<8>(p, q, i, a, b, r); _dist_sq<4>(p, q, i + 8, a, b, r); }

inline void _f32_mul_dist_sq(const float *p, const float *q, size_t chunk_index, __m256 &mul_p, __m256 &mul_q, __m256 &a, __m256 &b, __m256 &result)
{
    chunk_index *= 8;
    a = _mm256_loadu_ps(p + chunk_index);
    b = _mm256_loadu_ps(q + chunk_index);
    a = _mm256_mul_ps(a, mul_p);
    b = _mm256_mul_ps(b, mul_q);
    a = _mm256_sub_ps(a, b);
    result = _f32_fmadd_ps(a, a, result);
}

template <size_t N> inline void  _clear    (__m256 *p) { for (size_t i = 0; i < N; ++i) p[i] = _mm256_xor_si256(p[i], p[i]); }
template <>         inline void  _clear< 1>(__m256 *p) { *p = _mm256_xor_si256(*p, *p); }
template <>         inline void  _clear< 2>(__m256 *p) { _clear<1>(p); _clear<1>(p + 1); }
template <>         inline void  _clear< 4>(__m256 *p) { _clear<2>(p); _clear<2>(p + 2); }
template <>         inline void  _clear< 8>(__m256 *p) { _clear<4>(p); _clear<4>(p + 4); }
template <>         inline void  _clear<16>(__m256 *p) { _clear<8>(p); _clear<8>(p + 8); }

template <size_t N> inline float _accumulate    (__m256 *a) { return float(0); }
template <>         inline float _accumulate< 1>(__m256 *a) { return _f32_hadd_ps(*a); }
template <>         inline float _accumulate< 2>(__m256 *a) { return _f32_hadd_ps(_mm256_add_ps(a[0], a[1])); }
template <>         inline float _accumulate< 4>(__m256 *a) { return _f32_hadd_ps(_mm256_add_ps(_mm256_add_ps(a[0], a[1]), _mm256_add_ps(a[2], a[3]))); }
template <>         inline float _accumulate< 8>(__m256 *a) {
    return _f32_hadd_ps(_mm256_add_ps(_mm256_add_ps(_mm256_add_ps(a[0], a[1]), _mm256_add_ps(a[2], a[3])), _mm256_add_ps(_mm256_add_ps(a[4], a[5]), _mm256_add_ps(a[6], a[7]))));
}

void f32_mul(const float *p, float factor, float *out, const float * const p_end)
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

void f32_mul(const float *p, const float *q, float *out, const float * const p_end)
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

float f32_dot__stride32(const float *p, const float *q, const float * const p_end)
{
    assert((p_end - p) % 32 == 0);
    __m256 d[4], a, b;
    _clear<4>(d);
    for (; p < p_end; p += 32, q += 32) {
        _f32_dot(p, q, 0, a, b, d[0]);
        _f32_dot(p, q, 1, a, b, d[1]);
        _f32_dot(p, q, 2, a, b, d[2]);
        _f32_dot(p, q, 3, a, b, d[3]);
    }
    return _accumulate<4>(d);
}

float f32_dot__stride8(const float *p, const float *q, const float * const p_end)
{
    assert((p_end - p) % 8 == 0);
    __m256 a, b, d[1];
    _clear<1>(d);
    for (; p < p_end; p += 8, q += 8)
        _f32_dot(p, q, 0, a, b, d[0]);
    return _accumulate<1>(d);
}

float f32_self_dot__stride32(const float *p, const float * const p_end)
{
    assert((p_end - p) % 32 == 0);
    __m256 d[4], a;
    _clear<4>(d);
    for (; p < p_end; p += 32) {
        _f32_self_dot(p, 0, a, d[0]);
        _f32_self_dot(p, 1, a, d[1]);
        _f32_self_dot(p, 2, a, d[2]);
        _f32_self_dot(p, 3, a, d[3]);
    }
    return _accumulate<4>(d);
}

float f32_self_dot__stride8(const float *p, const float * const p_end)
{
    assert((p_end - p) % 8 == 0);
    __m256 a, d[1];
    _clear<1>(d);
    for (; p < p_end; p += 8)
        _f32_self_dot(p, 0, a, d[0]);
    return _accumulate<1>(d);
}

float f32_normalize__stride32(const float *p, float *out, const float * const p_end)
{
    double norm = sqrt(double(f32_self_dot__stride32(p, p_end))),
           invn = double(1.0) / norm;
    f32_mul(p, float(invn), out, p_end);
    return float(norm);
}


// f32_dist_sq

float f32_dist_sq__stride64(const float *p, const float *q, const float * const p_end)
{
    assert((p_end - p) % 64 == 0);
    __m256 d[8], a, b;
    _clear<8>(d);
    for (; p < p_end; p += 64, q += 64)
        _dist_sq<8>(p, q, 0, a, b, d);
    return _accumulate<8>(d);
}

float f32_dist_sq__stride32(const float *p, const float *q, const float * const p_end)
{
    assert((p_end - p) % 32 == 0);
    __m256 d[4], a, b;
    _clear<4>(d);
    for (; p < p_end; p += 32, q += 32) {
        _f32_dist_sq(p, q, 0, a, b, d[0]);
        _f32_dist_sq(p, q, 1, a, b, d[1]);
        _f32_dist_sq(p, q, 2, a, b, d[2]);
        _f32_dist_sq(p, q, 3, a, b, d[3]);
    }
    return _accumulate<4>(d);
}

float f32_dist_sq__stride16(const float *p, const float *q, const float * const p_end)
{
    assert((p_end - p) % 16 == 0);
    __m256 d[2], a, b;
    _clear<2>(d);
    for (; p < p_end; p += 16, q += 16) {
        _f32_dist_sq(p, q, 0, a, b, d[0]);
        _f32_dist_sq(p, q, 1, a, b, d[1]);
    }
    return _accumulate<2>(d);
}

float f32_dist_sq__stride8(const float *p, const float *q, const float * const p_end)
{
    assert((p_end - p) % 8 == 0);
    __m256 a, b, d[1];
    _clear<1>(d);
    for (; p < p_end; p += 8, q += 8)
        _f32_dist_sq(p, q, 0, a, b, d[0]);
    return _accumulate<1>(d);
}

float f32_dist_sq(const float *p, const float *q, const float * const p_end)
{
    size_t chunk = 64, count = (p_end - p) & ~(chunk - 1);
    float result(0);
    if (count == p_end - p) return f32_dist_sq__stride64(p, q, p + count);
    for (; p_end - p; p += count, q += count) {
        while ((chunk >> 1) > (p_end - p)) chunk >>= 1;
        count = (p_end - p) & ~(chunk - 1);
        float r(0);
        switch (chunk) {
        case 64: r = f32_dist_sq__stride64(p, q, p + count); break;
        case 32: r = f32_dist_sq__stride32(p, q, p + count); break;
        case 16: r = f32_dist_sq__stride16(p, q, p + count); break;
        case  8: r = f32_dist_sq__stride8 (p, q, p + count); break;
        default: for (; p != p_end; ++p, ++q) r += _sq(*p - *q); break;
        }
        result += r;
    }
    return result;
}

// f32_dist_sq_limit

template <size_t N, typename T>
inline float f32_dist_sq_limit_(const float *p, const float *q, const float * const p_end, float limit_sq) // about 10% faster than stride32
{
    static constexpr float MAX = std::numeric_limits<float>::max();
    static constexpr size_t ELEMS = _mm_traits<T>::count, COUNT = N / ELEMS;
    assert((p_end - p) % N == 0);
    T d[COUNT], a, b;
    float r(0);
    _clear<COUNT>(d);
    for (; p < p_end; p += N, q += N) {
        _dist_sq<COUNT>(p, q, 0, a, b, d);
        if ((r = _accumulate<COUNT>(d)) > limit_sq)
            return MAX;
    }
    return r;
}

float f32_dist_sq_limit__stride64(const float *p, const float *q, const float * const p_end, float limit_sq) // about 10% faster than stride32
{
    static constexpr float max = std::numeric_limits<float>::max();
    assert((p_end - p) % 64 == 0);
    __m256 d[8], a, b;
    float r = 0.f;
    _clear<8>(d);
    //_clear(d, 8);
    for (; p < p_end; p += 64, q += 64) {
        _dist_sq<8>(p, q, 0, a, b, d);
        /*
        _f32_dist_sq(p, q, 0, a, b, d[0]); _f32_dist_sq(p, q, 1, a, b, d[1]);
        _f32_dist_sq(p, q, 2, a, b, d[2]); _f32_dist_sq(p, q, 3, a, b, d[3]);
        _f32_dist_sq(p, q, 4, a, b, d[4]); _f32_dist_sq(p, q, 5, a, b, d[5]);
        _f32_dist_sq(p, q, 6, a, b, d[6]); _f32_dist_sq(p, q, 7, a, b, d[7]);
        */
        if ((r = _accumulate<8>(d)) > limit_sq)
            return max;
    }
    return r;
}

float f32_dist_sq_limit__stride32(const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float max = std::numeric_limits<float>::max();
    assert((p_end - p) % 32 == 0);
    __m256 d[4], a, b;
    float r = 0.f;
    _clear<4>(d);
    for (; p < p_end; p += 32, q += 32) {
        _f32_dist_sq(p, q, 0, a, b, d[0]);
        _f32_dist_sq(p, q, 1, a, b, d[1]);
        _f32_dist_sq(p, q, 2, a, b, d[2]);
        _f32_dist_sq(p, q, 3, a, b, d[3]);
        if ((r = _accumulate<4>(d)) > limit_sq)
            return max;
    }
    return r;
}

float f32_dist_sq_limit__stride16(const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float max = std::numeric_limits<float>::max();
    assert((p_end - p) % 16 == 0);
    __m256 d[2], a, b;
    float r = 0.f;
    _clear<2>(d);
    for (; p < p_end; p += 16, q += 16) {
        _f32_dist_sq(p, q, 0, a, b, d[0]);
        _f32_dist_sq(p, q, 1, a, b, d[1]);
        if ((r = _accumulate<2>(d)) > limit_sq)
            return max;
    }
    return r;
}

float f32_dist_sq_limit__stride8(const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float max = std::numeric_limits<float>::max();
    assert((p_end - p) % 8 == 0);
    __m256 a, b, d[1];
    _clear<1>(d);
    float r = 0.f;
    for (; p < p_end; p += 8, q += 8) {
        _f32_dist_sq(p, q, 0, a, b, d[0]);
        if ((r = _accumulate<1>(d)) > limit_sq)
            return max;
    }
    return r;
}

float f32_dist_sq_limit(const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float max = std::numeric_limits<float>::max();
    size_t chunk = 64, count = (p_end - p) & ~(chunk - 1);
    if (count == p_end - p) return f32_dist_sq_limit_<64, __m256>(p, q, p_end, limit_sq);
    float result(0);
    for (; p_end - p; p += count, q += count) {
        while ((chunk >> 1) > (p_end - p)) chunk >>= 1;
        count = (p_end - p) & ~(chunk - 1);
        float r(0);
        switch (chunk) {
        //case 64: r = f32_dist_sq_limit__stride64(p, q, p + count, limit_sq); break;
        //case 32: r = f32_dist_sq_limit__stride32(p, q, p + count, limit_sq); break;
        //case 16: r = f32_dist_sq_limit__stride16(p, q, p + count, limit_sq); break;
        //case  8: r = f32_dist_sq_limit__stride8 (p, q, p + count, limit_sq); break;
        case 64: r = f32_dist_sq_limit_<64, __m256>(p, q, p + count, limit_sq); break;
        case 32: r = f32_dist_sq_limit_<32, __m256>(p, q, p + count, limit_sq); break;
        case 16: r = f32_dist_sq_limit_<16, __m256>(p, q, p + count, limit_sq); break;
        case  8: r = f32_dist_sq_limit_< 8, __m256>(p, q, p + count, limit_sq); break;
        default: for (; p != p_end; ++p, ++q) r += _sq(*p - *q); break;
        }
        if (r == max) return r;
        result += r;
        if (result > limit_sq) return max;
    }
    return result > limit_sq ? max : result;
}

// f32_mul_dist_sq_limit

float f32_mul_dist_sq_limit__stride64(float mul_p, float mul_q, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float max = std::numeric_limits<float>::max();
    assert((p_end - p) % 64 == 0);
    float mp[] = { mul_p, mul_p, mul_p, mul_p, mul_p, mul_p, mul_p, mul_p },
          mq[] = { mul_q, mul_q, mul_q, mul_q, mul_q, mul_q, mul_q, mul_q };
    __m256 d[8], a, b, fa = _mm256_loadu_ps(mp), fb = _mm256_loadu_ps(mq);
    float r(0);
    _clear<8>(d);
    for (; p < p_end; p += 64, q += 64) {
        _f32_mul_dist_sq(p, q, 0, fa, fb, a, b, d[0]);
        _f32_mul_dist_sq(p, q, 1, fa, fb, a, b, d[1]);
        _f32_mul_dist_sq(p, q, 2, fa, fb, a, b, d[2]);
        _f32_mul_dist_sq(p, q, 3, fa, fb, a, b, d[3]);
        _f32_mul_dist_sq(p, q, 4, fa, fb, a, b, d[4]);
        _f32_mul_dist_sq(p, q, 5, fa, fb, a, b, d[5]);
        _f32_mul_dist_sq(p, q, 6, fa, fb, a, b, d[6]);
        _f32_mul_dist_sq(p, q, 7, fa, fb, a, b, d[7]);
        if ((r = _accumulate<8>(d)) > limit_sq)
            return max;
    }
    return r;
}

float f32_mul_dist_sq_limit__stride32(float mul_p, float mul_q, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float max = std::numeric_limits<float>::max();
    assert((p_end - p) % 32 == 0);
    float mp[] = { mul_p, mul_p, mul_p, mul_p, mul_p, mul_p, mul_p, mul_p },
          mq[] = { mul_q, mul_q, mul_q, mul_q, mul_q, mul_q, mul_q, mul_q };
    __m256 d[4], a, b, fa = _mm256_loadu_ps(mp), fb = _mm256_loadu_ps(mq);
    float r(0);
    _clear<4>(d);
    for (; p < p_end; p += 32, q += 32) {
        _f32_mul_dist_sq(p, q, 0, fa, fb, a, b, d[0]);
        _f32_mul_dist_sq(p, q, 1, fa, fb, a, b, d[1]);
        _f32_mul_dist_sq(p, q, 2, fa, fb, a, b, d[2]);
        _f32_mul_dist_sq(p, q, 3, fa, fb, a, b, d[3]);
        if ((r = _accumulate<4>(d)) > limit_sq)
            return max;
    }
    return r;
}

float f32_mul_dist_sq_limit__stride16(float mul_p, float mul_q, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float max = std::numeric_limits<float>::max();
    assert((p_end - p) % 16 == 0);
    float mp[] = { mul_p, mul_p, mul_p, mul_p, mul_p, mul_p, mul_p, mul_p },
          mq[] = { mul_q, mul_q, mul_q, mul_q, mul_q, mul_q, mul_q, mul_q };
    __m256 d[2], a, b, fa = _mm256_loadu_ps(mp), fb = _mm256_loadu_ps(mq);
    float r(0);
    _clear<2>(d);
    for (; p < p_end; p += 16, q += 16) {
        _f32_mul_dist_sq(p, q, 0, fa, fb, a, b, d[0]);
        _f32_mul_dist_sq(p, q, 1, fa, fb, a, b, d[1]);
        if ((r = _accumulate<2>(d)) > limit_sq)
            return max;
    }
    return r;
}

float f32_mul_dist_sq_limit__stride8(float mul_p, float mul_q, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float max = std::numeric_limits<float>::max();
    assert((p_end - p) % 16 == 0);
    float mp[] = { mul_p, mul_p, mul_p, mul_p, mul_p, mul_p, mul_p, mul_p },
          mq[] = { mul_q, mul_q, mul_q, mul_q, mul_q, mul_q, mul_q, mul_q };
    __m256 d[1], a, b, fa = _mm256_loadu_ps(mp), fb = _mm256_loadu_ps(mq);
    float r(0);
    _clear<1>(d);
    for (; p < p_end; p += 8, q += 8) {
        _f32_mul_dist_sq(p, q, 0, fa, fb, a, b, d[0]);
        if ((r = _accumulate<1>(d)) > limit_sq)
            return max;
    }
    return r;
}

float f32_mul_dist_sq_limit(float mul_p, float mul_q, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float max = std::numeric_limits<float>::max();
    size_t chunk = 64, count = (p_end - p) & ~(chunk - 1);
    if (count == p_end - p) return f32_mul_dist_sq_limit__stride64(mul_p, mul_q, p, q, p_end, limit_sq);
    float result(0);
    for (; p_end - p; p += count, q += count) {
        while ((chunk >> 1) > (p_end - p)) chunk >>= 1;
        count = (p_end - p) & ~(chunk - 1);
        float r(0);
        switch (chunk) {
        case 64: r = f32_mul_dist_sq_limit__stride64(mul_p, mul_q, p, q, p + count, limit_sq); break;
        case 32: r = f32_mul_dist_sq_limit__stride32(mul_p, mul_q, p, q, p + count, limit_sq); break;
        case 16: r = f32_mul_dist_sq_limit__stride16(mul_p, mul_q, p, q, p + count, limit_sq); break;
        case  8: r = f32_mul_dist_sq_limit__stride8 (mul_p, mul_q, p, q, p + count, limit_sq); break;
        default: for (; p != p_end; ++p, ++q) r += _sq((*p * mul_p) - (*q * mul_q)); break;
        }
        if (r == max) return r;
        result += r;
        if (result > limit_sq) return max;
    }
    return result > limit_sq ? max : result;
}

}
