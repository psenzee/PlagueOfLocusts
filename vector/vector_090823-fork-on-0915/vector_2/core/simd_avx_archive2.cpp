#include "simd_avx.h"

#include <limits>
#include <cmath>
#include <algorithm>
#include <cstddef>

#include <immintrin.h>

namespace simd_avx
{

template <typename T> inline static T _sq(T u) { return u * u; }
template <typename T> inline static T _ref_dist_sq(const T *p, const T *q, const T *p_end) { T r(0); for (; p != p_end; ++p, ++q) r += _sq(*p - *q); return r; }

typedef const float * _cfp;

template <typename T> struct _mm_traits {};
template <> struct _mm_traits<__m128> { typedef __m128 type; static constexpr size_t count = 4; };
template <> struct _mm_traits<__m256> { typedef __m256 type; static constexpr size_t count = 8; };

inline void   __load(const float *p, __m256 &r)      { r = _mm256_loadu_ps(p); }
inline void   __load(const float *p, __m128 &r)      { r = _mm_loadu_ps(p); }

inline void   __load_const(float v, __m256 &r)       { float u[] = { v, v, v, v, v, v, v, v }; __load(u, r); }
inline void   __load_const(float v, __m128 &r)       { float u[] = { v, v, v, v }; __load(u, r); }

inline void   __store(__m256 r, float *p)            { r = _mm256_loadu_ps(p); }
inline void   __store(__m128 r, float *p)            { r = _mm_loadu_ps(p); }

inline __m256 __add(__m256 a, __m256 b)              { return _mm256_add_ps(a, b); }
inline __m128 __add(__m128 a, __m128 b)              { return _mm_add_ps(a, b); }
inline __m256 __mul(__m256 a, __m256 b)              { return _mm256_mul_ps(a, b); }
inline __m128 __mul(__m128 a, __m128 b)              { return _mm_mul_ps(a, b); }
inline __m256 __sub(__m256 a, __m256 b)              { return _mm256_sub_ps(a, b); }
inline __m128 __sub(__m128 a, __m128 b)              { return _mm_sub_ps(a, b); }

inline void   __clear(__m256 &p)                     { p = _mm256_xor_si256(p, p); }
inline void   __clear(__m128 &p)                     { p = _mm_xor_si128(p, p); }

inline float  __horiz_add(__m128 r)                  { const __m128 r2 = _mm_add_ps(r, _mm_movehl_ps(r, r)); return _mm_cvtss_f32(_mm_add_ss(r2, _mm_movehdup_ps(r2))); }
inline float  __horiz_add(__m256 r)                  { return __horiz_add(_mm_add_ps(_mm256_castps256_ps128(r), _mm256_extractf128_ps(r, 1))); }

template <bool fma = false>
inline __m256 __fmadd(__m256 a, __m256 b, __m256 acc) { if constexpr(fma) return _mm256_fmadd_ps(a, b, acc); return _mm256_add_ps(_mm256_mul_ps(a, b), acc); }

inline __m128 __fmadd(__m128 a, __m128 b, __m128 acc) { return _mm_add_ps(_mm_mul_ps(a, b), acc); }

template <typename T> inline void __load_pq(const float *p, const float *q, size_t offset, T &a, T &b) { __load(p + offset, a); __load(q + offset, b); }

template <size_t N> inline void _bad_n_error() { static_assert(N == 1 || N == 2 || N == 4 || N == 8 || N == 16); }

template <size_t N> inline __m256 _sum    (__m256 *a) { _bad_n_error<N>(); }
template <>         inline __m256 _sum< 1>(__m256 *a) { return *a; }
template <>         inline __m256 _sum< 2>(__m256 *a) { return __add(a[0], a[1]); }
template <>         inline __m256 _sum< 4>(__m256 *a) { return __add(_sum<2>(a), _sum<2>(a + 2)); }
template <>         inline __m256 _sum< 8>(__m256 *a) { return __add(_sum<4>(a), _sum<4>(a + 4)); }
template <>         inline __m256 _sum<16>(__m256 *a) { return __add(_sum<8>(a), _sum<8>(a + 8)); }

template <size_t N> inline __m128 _sum    (__m128 *a) { _bad_n_error<N>(); }
template <>         inline __m128 _sum< 1>(__m128 *a) { return *a; }
template <>         inline __m128 _sum< 2>(__m128 *a) { return __add(a[0], a[1]); }
template <>         inline __m128 _sum< 4>(__m128 *a) { return __add(_sum<2>(a), _sum<2>(a + 2)); }
template <>         inline __m128 _sum< 8>(__m128 *a) { return __add(_sum<4>(a), _sum<4>(a + 4)); }
template <>         inline __m128 _sum<16>(__m128 *a) { return __add(_sum<8>(a), _sum<8>(a + 8)); }

template <size_t N> inline void  _clear    (__m256 *p) { _bad_n_error<N>(); }
template <>         inline void  _clear< 1>(__m256 *p) { __clear(*p); }
template <>         inline void  _clear< 2>(__m256 *p) { __clear(p[0]); __clear(p[1]); }
template <>         inline void  _clear< 4>(__m256 *p) { _clear<2>(p); _clear<2>(p + 2); }
template <>         inline void  _clear< 8>(__m256 *p) { _clear<4>(p); _clear<4>(p + 4); }
template <>         inline void  _clear<16>(__m256 *p) { _clear<8>(p); _clear<8>(p + 8); }

template <size_t N> inline void  _clear    (__m128 *p) { _bad_n_error<N>(); }
template <>         inline void  _clear< 1>(__m128 *p) { __clear(*p); }
template <>         inline void  _clear< 2>(__m128 *p) { __clear(p[0]); __clear(p[1]); }
template <>         inline void  _clear< 4>(__m128 *p) { _clear<2>(p); _clear<2>(p + 2); }
template <>         inline void  _clear< 8>(__m128 *p) { _clear<4>(p); _clear<4>(p + 4); }
template <>         inline void  _clear<16>(__m128 *p) { _clear<8>(p); _clear<8>(p + 8); }

template <size_t N, typename T> inline float _accumulate(T *a) { return __horiz_add(_sum<N>(a)); }

template <typename T> inline T __acc_dot(T &a, T &b, T &acc)                   { return __fmadd(a, b, acc); }
template <typename T> inline T __acc_dsq(T &a, T &b, T &acc)                   { a = __sub(a, b); return __fmadd(a, a, acc); }
template <typename T> inline T __acc_mul_dsq(T &ma, T &a, T &b, T &acc)        { a = __sub(__mul(a, ma), b); return __fmadd(a, a, acc); }
template <typename T> inline T __acc_mul_dsq(T &ma, T &mb, T &a, T &b, T &acc) { a = __sub(__mul(a, ma), __mul(b, mb)); return __fmadd(a, a, acc); }

// ----------
// primitives
// ----------

// __dot
template <size_t N> inline void __dot(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { _bad_n_error<N>(); }
template <> inline void __dot< 1>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { __load_pq(p, q, i * 8, a, b); r[i] = __fmadd(a, b, r[i]); }
template <> inline void __dot< 2>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { __dot<1>(p, q, i, a, b, r); __dot<1>(p, q, i + 1, a, b, r); }
template <> inline void __dot< 4>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { __dot<2>(p, q, i, a, b, r); __dot<2>(p, q, i + 2, a, b, r); }
template <> inline void __dot< 8>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { __dot<4>(p, q, i, a, b, r); __dot<4>(p, q, i + 4, a, b, r); }
template <> inline void __dot<16>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { __dot<8>(p, q, i, a, b, r); __dot<4>(p, q, i + 8, a, b, r); }

// __self_dot
template <size_t N> inline void __self_dot(const float *p, size_t i, __m256 &a, __m256 *r) { _bad_n_error<N>(); }
template <> inline void __self_dot< 1>(const float *p, size_t i, __m256 &a, __m256 *r) { __load(p + i * 8, a); r[i] = __fmadd(a, a, r[i]); }
template <> inline void __self_dot< 2>(const float *p, size_t i, __m256 &a, __m256 *r) { __self_dot<1>(p, i, a, r); __self_dot<1>(p, i + 1, a, r); }
template <> inline void __self_dot< 4>(const float *p, size_t i, __m256 &a, __m256 *r) { __self_dot<2>(p, i, a, r); __self_dot<2>(p, i + 2, a, r); }
template <> inline void __self_dot< 8>(const float *p, size_t i, __m256 &a, __m256 *r) { __self_dot<4>(p, i, a, r); __self_dot<4>(p, i + 4, a, r); }
template <> inline void __self_dot<16>(const float *p, size_t i, __m256 &a, __m256 *r) { __self_dot<8>(p, i, a, r); __self_dot<4>(p, i + 8, a, r); }

// __dsq
template <size_t N> inline void __dsq(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { _bad_n_error<N>(); }
template <> inline void __dsq< 1>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { __load_pq(p, q, i * 8, a, b); a = __sub(a, b); r[i] = __fmadd(a, a, r[i]); }
template <> inline void __dsq< 2>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { __dsq<1>(p, q, i, a, b, r); __dsq<1>(p, q, i + 1, a, b, r); }
template <> inline void __dsq< 4>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { __dsq<2>(p, q, i, a, b, r); __dsq<2>(p, q, i + 2, a, b, r); }
template <> inline void __dsq< 8>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { __dsq<4>(p, q, i, a, b, r); __dsq<4>(p, q, i + 4, a, b, r); }
template <> inline void __dsq<16>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { __dsq<8>(p, q, i, a, b, r); __dsq<4>(p, q, i + 8, a, b, r); }

template <size_t N> inline void __dsq(const float *p, const float *q, size_t i, __m128 &a, __m128 &b, __m128 *r) { _bad_n_error<N>(); }
template <> inline void __dsq< 1>(const float *p, const float *q, size_t i, __m128 &a, __m128 &b, __m128 *r) { __load_pq(p, q, i * 8, a, b); a = __sub(a, b); r[i] = __fmadd(a, a, r[i]); }
template <> inline void __dsq< 2>(const float *p, const float *q, size_t i, __m128 &a, __m128 &b, __m128 *r) { __dsq<1>(p, q, i, a, b, r); __dsq<1>(p, q, i + 1, a, b, r); }
template <> inline void __dsq< 4>(const float *p, const float *q, size_t i, __m128 &a, __m128 &b, __m128 *r) { __dsq<2>(p, q, i, a, b, r); __dsq<2>(p, q, i + 2, a, b, r); }
template <> inline void __dsq< 8>(const float *p, const float *q, size_t i, __m128 &a, __m128 &b, __m128 *r) { __dsq<4>(p, q, i, a, b, r); __dsq<4>(p, q, i + 4, a, b, r); }
template <> inline void __dsq<16>(const float *p, const float *q, size_t i, __m128 &a, __m128 &b, __m128 *r) { __dsq<8>(p, q, i, a, b, r); __dsq<4>(p, q, i + 8, a, b, r); }

// __mul2_dsq
template <size_t N> inline void __mul2_dsq(const float *p, const float *q, size_t i, __m256 &mp, __m256 &mq, __m256 &a, __m256 &b, __m256 *r) { _bad_n_error<N>(); }
template <> inline void __mul2_dsq< 1>(const float *p, const float *q, size_t i, __m256 &mp, __m256 &mq, __m256 &a, __m256 &b, __m256 *r) { __load_pq(p, q, i * 8, a, b); a = __sub(__mul(a, mp), __mul(b, mq)); r[i] = __fmadd(a, a, r[i]); }
template <> inline void __mul2_dsq< 2>(const float *p, const float *q, size_t i, __m256 &mp, __m256 &mq, __m256 &a, __m256 &b, __m256 *r) { __mul2_dsq<1>(p, q, i, mp, mq, a, b, r); __mul2_dsq<1>(p, q, i + 1, mp, mq, a, b, r); }
template <> inline void __mul2_dsq< 4>(const float *p, const float *q, size_t i, __m256 &mp, __m256 &mq, __m256 &a, __m256 &b, __m256 *r) { __mul2_dsq<2>(p, q, i, mp, mq, a, b, r); __mul2_dsq<2>(p, q, i + 2, mp, mq, a, b, r); }
template <> inline void __mul2_dsq< 8>(const float *p, const float *q, size_t i, __m256 &mp, __m256 &mq, __m256 &a, __m256 &b, __m256 *r) { __mul2_dsq<4>(p, q, i, mp, mq, a, b, r); __mul2_dsq<4>(p, q, i + 4, mp, mq, a, b, r); }
template <> inline void __mul2_dsq<16>(const float *p, const float *q, size_t i, __m256 &mp, __m256 &mq, __m256 &a, __m256 &b, __m256 *r) { __mul2_dsq<8>(p, q, i, mp, mq, a, b, r); __mul2_dsq<4>(p, q, i + 8, mp, mq, a, b, r); }

// __mul1_dsq
template <size_t N> inline void __mul1_dsq(const float *p, const float *q, size_t i, __m256 &mp, __m256 &a, __m256 &b, __m256 *r) { _bad_n_error<N>(); }
template <> inline void __mul1_dsq< 1>(const float *p, const float *q, size_t i, __m256 &mp, __m256 &a, __m256 &b, __m256 *r) { __load_pq(p, q, i * 8, a, b); a = __sub(__mul(a, mp), b); r[i] = __fmadd(a, a, r[i]); }
template <> inline void __mul1_dsq< 2>(const float *p, const float *q, size_t i, __m256 &mp, __m256 &a, __m256 &b, __m256 *r) { __mul1_dsq<1>(p, q, i, mp, a, b, r); __mul1_dsq<1>(p, q, i + 1, mp, a, b, r); }
template <> inline void __mul1_dsq< 4>(const float *p, const float *q, size_t i, __m256 &mp, __m256 &a, __m256 &b, __m256 *r) { __mul1_dsq<2>(p, q, i, mp, a, b, r); __mul1_dsq<2>(p, q, i + 2, mp, a, b, r); }
template <> inline void __mul1_dsq< 8>(const float *p, const float *q, size_t i, __m256 &mp, __m256 &a, __m256 &b, __m256 *r) { __mul1_dsq<4>(p, q, i, mp, a, b, r); __mul1_dsq<4>(p, q, i + 4, mp, a, b, r); }
template <> inline void __mul1_dsq<16>(const float *p, const float *q, size_t i, __m256 &mp, __m256 &a, __m256 &b, __m256 *r) { __mul1_dsq<8>(p, q, i, mp, a, b, r); __mul1_dsq<4>(p, q, i + 8, mp, a, b, r); }

// -------------

// mul
void mul(const float *p, const float *q, float *out, const float * const p_end)
{
    assert((p_end - p) % 8 == 0);
    __m256 a, b;
    for (; p < p_end; p += 8, q += 8, out += 8) {
        __load(p, a);
        __load(q, b);
        __store(__mul(a, b), out);
    }
}

void mul(const float *p, float q, float *out, const float * const p_end)
{
    assert((p_end - p) % 8 == 0);
    __m256 a, b;
    __load_const(q, b);
    for (; p < p_end; p += 8, q += 8, out += 8) {
        __load(p, a);
        __store(__mul(a, b), out);
    }
}

// dot
template <size_t N, typename T>
inline float _dot(const float *p, const float *q, const float * const p_end)
{
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    assert((p_end - p) % N == 0);
    T d[PARALLEL], a, b;
    _clear<PARALLEL>(d);
    for (; p < p_end; p += N, q += N)
        __dot<PARALLEL>(p, q, 0, a, b, d);
    return _accumulate<PARALLEL>(d);
}

float dot(const float *p, const float *q, const float * const p_end)
{
    size_t chunk = 64, count = (p_end - p) & ~(chunk - 1);
    float result(0);
    if (count == p_end - p) return _dot<64, __m256>(p, q, p_end);
    for (; p_end - p; p += count, q += count) {
        while ((chunk >> 1) > (p_end - p)) chunk >>= 1;
        count = (p_end - p) & ~(chunk - 1);
        float r(0);
        switch (chunk) {
        case 64: r = _dot<64, __m256>(p, q, p + count); break;
        case 32: r = _dot<32, __m256>(p, q, p + count); break;
        case 16: r = _dot<16, __m256>(p, q, p + count); break;
        case  8: r = _dot< 8, __m256>(p, q, p + count); break;
        default: for (; p != p_end; ++p, ++q) r += *p * *q; break;
        }
        result += r;
    }
    return result;
}

// self_dot
template <size_t N, typename T>
inline float _self_dot(const float *p, const float * const p_end)
{
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    assert((p_end - p) % N == 0);
    T d[PARALLEL], a, b;
    _clear<PARALLEL>(d);
    for (; p < p_end; p += N)
        __self_dot<PARALLEL>(p, 0, a, d);
    return _accumulate<PARALLEL>(d);
}

float self_dot(const float *p, const float * const p_end)
{
    size_t chunk = 64, count = (p_end - p) & ~(chunk - 1);
    float result(0);
    if (count == p_end - p) return _self_dot<64, __m256>(p, p_end);
    for (; p_end - p; p += count) {
        while ((chunk >> 1) > (p_end - p)) chunk >>= 1;
        count = (p_end - p) & ~(chunk - 1);
        float r(0);
        switch (chunk) {
        case 64: r = _self_dot<64, __m256>(p, p + count); break;
        case 32: r = _self_dot<32, __m256>(p, p + count); break;
        case 16: r = _self_dot<16, __m256>(p, p + count); break;
        case  8: r = _self_dot< 8, __m256>(p, p + count); break;
        default: for (; p != p_end; ++p) r += _sq(*p); break;
        }
        result += r;
    }
    return result;
}

// dist_sq
template <size_t N, typename T>
inline float _dist_sq(const float *p, const float *q, const float * const p_end)
{
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    assert((p_end - p) % N == 0);
    T d[PARALLEL], a, b;
    _clear<PARALLEL>(d);
    for (; p < p_end; p += N, q += N)
        __dsq<PARALLEL>(p, q, 0, a, b, d);
    return _accumulate<PARALLEL>(d);
}

float dist_sq(const float *p, const float *q, const float * const p_end)
{
    size_t chunk = 64, count = (p_end - p) & ~(chunk - 1);
    float result(0);
    if (count == p_end - p) return _dist_sq<64, __m256>(p, q, p_end);
    for (; p_end - p; p += count, q += count) {
        while ((chunk >> 1) > (p_end - p)) chunk >>= 1;
        count = (p_end - p) & ~(chunk - 1);
        float r(0);
        switch (chunk) {
        case 64: r = _dist_sq<64, __m256>(p, q, p + count); break;
        case 32: r = _dist_sq<32, __m256>(p, q, p + count); break;
        case 16: r = _dist_sq<16, __m256>(p, q, p + count); break;
        case  8: r = _dist_sq< 8, __m256>(p, q, p + count); break;
        default: for (; p != p_end; ++p, ++q) r += _sq(*p - *q); break;
        }
        result += r;
    }
    return result;
}

// dist_sq_limit
template <size_t N, typename T>
inline float _dist_sq_limit(const float *p, const float *q, const float * const p_end, float limit_sq) // about 10% faster than stride32
{
    static constexpr float MAX = std::numeric_limits<float>::max();
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    assert((p_end - p) % N == 0);
    T d[PARALLEL], a, b;
    float r(0);
    _clear<PARALLEL>(d);
    for (; p < p_end; p += N, q += N) {
        __dsq<PARALLEL>(p, q, 0, a, b, d);
        if ((r = _accumulate<PARALLEL>(d)) > limit_sq)
            return MAX;
    }
    return r;
}

// dist_sq_limit
/*
template <size_t N, typename T>
inline float _dist_sq_limit_(const float *p, const float *q, const float * const p_end, float limit_sq) // about 10% faster than stride32
{
    static constexpr float MAX = std::numeric_limits<float>::max();
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    size_t size = p_end - p;
    if (size < N) return !size ? float(0) : _dist_sq_limit_<N / 2, T>(p, q, p_end, limit_sq);
    size_t count = size & ~(N - 1);
    float result = _dist_sq_limit<N, T>(p, q, p + count, limit_sq);
    p += count; q += count;
    size = p_end - p;
    if (!size) return result;
    float r = _dist_sq_limit_<N / 2, T>(p, q, p_end, limit_sq);
    if (r == MAX) return r;
    return (result += r) > limit_sq ? MAX : result;
}
*/

template <size_t N, typename T>
inline float _dist_sq_limit_(const float *p, const float *q, const float * const p_end, float limit_sq) // about 10% faster than stride32
{
    static constexpr float MAX = std::numeric_limits<float>::max();
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    if (p_end == p) return float(0);
    if (p_end - p < N) return _dist_sq_limit_<N / 2, T>(p, q, p_end, limit_sq);
    size_t count = (p_end - p) & ~(N - 1);

    T d[PARALLEL], a, b;
    float result(0), r(0);
    _clear<PARALLEL>(d);
    for (const float *e = p + count; p < e; p += N, q += N) {
        __dsq<PARALLEL>(p, q, 0, a, b, d);
        if ((result = _accumulate<PARALLEL>(d)) > limit_sq)
            return MAX;
    }    
    if ((p_end - p) == 0) return result;
    if ((r = _dist_sq_limit_<N / 2, T>(p, q, p_end, limit_sq)) == MAX) return r;
    return (result += r) > limit_sq ? MAX : result;
}

// dist_sq_limit
template <> inline float _dist_sq_limit_<0, __m128>(const float *p, const float *q, const float * const p_end, float limit_sq) { return float(0); }
template <> inline float _dist_sq_limit_<0, __m256>(const float *p, const float *q, const float * const p_end, float limit_sq) { return float(0); }
template <> inline float _dist_sq_limit_<1, __m128>(const float *p, const float *q, const float * const p_end, float limit_sq) { float r(_ref_dist_sq(p, q, p_end)); return r > limit_sq ? std::numeric_limits<float>::max() : r; }
template <> inline float _dist_sq_limit_<1, __m256>(const float *p, const float *q, const float * const p_end, float limit_sq) { float r(_ref_dist_sq(p, q, p_end)); return r > limit_sq ? std::numeric_limits<float>::max() : r; }
template <> inline float _dist_sq_limit_<2, __m128>(const float *p, const float *q, const float * const p_end, float limit_sq) { return _dist_sq_limit_<1, __m128>(p, q, p_end, limit_sq); }
template <> inline float _dist_sq_limit_<2, __m256>(const float *p, const float *q, const float * const p_end, float limit_sq) { return _dist_sq_limit_<1, __m256>(p, q, p_end, limit_sq); }
template <> inline float _dist_sq_limit_<4, __m256>(const float *p, const float *q, const float * const p_end, float limit_sq) { return _dist_sq_limit_<4, __m128>(p, q, p_end, limit_sq); }

float dist_sq_limit(const float *p, const float *q, const float * const p_end, float limit_sq)
{
    return _dist_sq_limit_<64, __m256>(p, q, p_end, limit_sq);
    /*
    static constexpr float max = std::numeric_limits<float>::max();
    size_t chunk = 64, count = (p_end - p) & ~(chunk - 1);
    if (count == p_end - p) return _dist_sq_limit<64, __m256>(p, q, p_end, limit_sq);
    float result(0);
    for (; p_end - p; p += count, q += count) {
        while ((chunk >> 1) > (p_end - p)) chunk >>= 1;
        count = (p_end - p) & ~(chunk - 1);
        float r(0);
        switch (chunk) {
        case 64: r = _dist_sq_limit<64, __m256>(p, q, p + count, limit_sq); break;
        case 32: r = _dist_sq_limit<32, __m256>(p, q, p + count, limit_sq); break;
        case 16: r = _dist_sq_limit<16, __m256>(p, q, p + count, limit_sq); break;
        case  8: r = _dist_sq_limit< 8, __m256>(p, q, p + count, limit_sq); break;
        default: for (; p != p_end; ++p, ++q) r += _sq(*p - *q); break;
        }
        if (r == max) return r;
        result += r;
        if (result > limit_sq) return max;
    }
    return result > limit_sq ? max : result;
    */
}

// mul1_dist_sq
template <size_t N, typename T>
inline float _mul1_dist_sq(float mul_p, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    assert((p_end - p) % N == 0);
    T d[PARALLEL], a, b, ma, mb;
    __load_const(mul_p, ma);
    _clear<PARALLEL>(d);
    for (; p < p_end; p += N, q += N)
        __mul2_dsq<PARALLEL>(p, q, 0, ma, mb, a, b, d);
    return _accumulate<PARALLEL>(d);
}

float mul1_dist_sq(float mul_p, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    size_t chunk = 64, count = (p_end - p) & ~(chunk - 1);
    float result(0);
    if (count == p_end - p) return _mul1_dist_sq<64, __m256>(mul_p, p, q, p_end, limit_sq);
    for (; p_end - p; p += count, q += count) {
        while ((chunk >> 1) > (p_end - p)) chunk >>= 1;
        count = (p_end - p) & ~(chunk - 1);
        float r(0);
        switch (chunk) {
        case 64: r = _mul1_dist_sq<64, __m256>(mul_p, p, q, p + count, limit_sq); break;
        case 32: r = _mul1_dist_sq<32, __m256>(mul_p, p, q, p + count, limit_sq); break;
        case 16: r = _mul1_dist_sq<16, __m256>(mul_p, p, q, p + count, limit_sq); break;
        case  8: r = _mul1_dist_sq< 8, __m256>(mul_p, p, q, p + count, limit_sq); break;
        default: for (; p != p_end; ++p, ++q) r += _sq((*p * mul_p) - *q); break;
        }
        result += r;
    }
    return result;
}

// mul2_dist_sq
template <size_t N, typename T>
inline float _mul2_dist_sq(float mul_p, float mul_q, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    assert((p_end - p) % N == 0);
    T d[PARALLEL], a, b, ma, mb;
    __load_const(mul_p, ma);
    __load_const(mul_q, mb);
    _clear<PARALLEL>(d);
    for (; p < p_end; p += N, q += N)
        __mul2_dsq<PARALLEL>(p, q, 0, ma, mb, a, b, d);
    return _accumulate<PARALLEL>(d);
}

float mul2_dist_sq(float mul_p, float mul_q, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    size_t chunk = 64, count = (p_end - p) & ~(chunk - 1);
    float result(0);
    if (count == p_end - p) return _mul2_dist_sq<64, __m256>(mul_p, mul_q, p, q, p_end, limit_sq);
    for (; p_end - p; p += count, q += count) {
        while ((chunk >> 1) > (p_end - p)) chunk >>= 1;
        count = (p_end - p) & ~(chunk - 1);
        float r(0);
        switch (chunk) {
        case 64: r = _mul2_dist_sq<64, __m256>(mul_p, mul_q, p, q, p + count, limit_sq); break;
        case 32: r = _mul2_dist_sq<32, __m256>(mul_p, mul_q, p, q, p + count, limit_sq); break;
        case 16: r = _mul2_dist_sq<16, __m256>(mul_p, mul_q, p, q, p + count, limit_sq); break;
        case  8: r = _mul2_dist_sq< 8, __m256>(mul_p, mul_q, p, q, p + count, limit_sq); break;
        default: for (; p != p_end; ++p, ++q) r += _sq((*p * mul_p) - (*q * mul_q)); break;
        }
        result += r;
    }
    return result;
}

// mul1_dist_sq_limit
template <size_t N, typename T>
inline float _mul1_dist_sq_limit(float mul_p, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float MAX = std::numeric_limits<float>::max();
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    assert((p_end - p) % N == 0);
    T d[PARALLEL], a, b, ma, mb;
    float r(0);
    __load_const(mul_p, ma);
    _clear<PARALLEL>(d);
    for (; p < p_end; p += N, q += N) {
        __mul2_dsq<PARALLEL>(p, q, 0, ma, mb, a, b, d);
        if ((r = _accumulate<PARALLEL>(d)) > limit_sq)
            return MAX;
    }
    return _accumulate<PARALLEL>(d);
}

float mul1_dist_sq_limit(float mul_p, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float max = std::numeric_limits<float>::max();
    size_t chunk = 64, count = (p_end - p) & ~(chunk - 1);
    float result(0);
    if (count == p_end - p) return _mul1_dist_sq_limit<64, __m256>(mul_p, p, q, p_end, limit_sq);
    for (; p_end - p; p += count, q += count) {
        while ((chunk >> 1) > (p_end - p)) chunk >>= 1;
        count = (p_end - p) & ~(chunk - 1);
        float r(0);
        switch (chunk) {
        case 64: r = _mul1_dist_sq_limit<64, __m256>(mul_p, p, q, p + count, limit_sq); break;
        case 32: r = _mul1_dist_sq_limit<32, __m256>(mul_p, p, q, p + count, limit_sq); break;
        case 16: r = _mul1_dist_sq_limit<16, __m256>(mul_p, p, q, p + count, limit_sq); break;
        case  8: r = _mul1_dist_sq_limit< 8, __m256>(mul_p, p, q, p + count, limit_sq); break;
        default: for (; p != p_end; ++p, ++q) r += _sq((*p * mul_p) - *q); break;
        }
        if (r == max) return r;
        result += r;
        if (result > limit_sq) return max;
    }
    return result;
}

// mul2_dist_sq_limit
template <size_t N, typename T>
inline float _mul2_dist_sq_limit(float mul_p, float mul_q, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float MAX = std::numeric_limits<float>::max();
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    assert((p_end - p) % N == 0);
    T d[PARALLEL], a, b, ma, mb;
    float r(0);
    __load_const(mul_p, ma);
    __load_const(mul_q, mb);
    _clear<PARALLEL>(d);
    for (; p < p_end; p += N, q += N) {
        __mul2_dsq<PARALLEL>(p, q, 0, ma, mb, a, b, d);
        if ((r = _accumulate<PARALLEL>(d)) > limit_sq)
            return MAX;
    }
    return _accumulate<PARALLEL>(d);
}

float mul2_dist_sq_limit(float mul_p, float mul_q, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr float max = std::numeric_limits<float>::max();
    size_t chunk = 64, count = (p_end - p) & ~(chunk - 1);
    float result(0);
    if (count == p_end - p) return _mul2_dist_sq_limit<64, __m256>(mul_p, mul_q, p, q, p_end, limit_sq);
    for (; p_end - p; p += count, q += count) {
        while ((chunk >> 1) > (p_end - p)) chunk >>= 1;
        count = (p_end - p) & ~(chunk - 1);
        float r(0);
        switch (chunk) {
        case 64: r = _mul2_dist_sq_limit<64, __m256>(mul_p, mul_q, p, q, p + count, limit_sq); break;
        case 32: r = _mul2_dist_sq_limit<32, __m256>(mul_p, mul_q, p, q, p + count, limit_sq); break;
        case 16: r = _mul2_dist_sq_limit<16, __m256>(mul_p, mul_q, p, q, p + count, limit_sq); break;
        case  8: r = _mul2_dist_sq_limit< 8, __m256>(mul_p, mul_q, p, q, p + count, limit_sq); break;
        default: for (; p != p_end; ++p, ++q) r += _sq((*p * mul_p) - (*q * mul_q)); break;
        }
        if (r == max) return r;
        result += r;
        if (result > limit_sq) return max;
    }
    return result;
}

float normalize(const float *p, float *out, const float * const p_end)
{
    double norm = sqrt(double(self_dot(p, p_end))),
           invn = double(1.0) / norm;
    mul(p, float(invn), out, p_end);
    return float(norm);
}

}
