#include "simd_avx.h"

#include <limits>
#include <cmath>
#include <algorithm>
#include <cstddef>

#include <immintrin.h>

namespace simd_avx
{

static constexpr float MAX = std::numeric_limits<float>::max();
static constexpr float _0 = float(0);
static constexpr float _1 = float(1);

template <typename T> inline static T _sq(T u)                                                                                { return u * u; }
template <typename T> inline static T _ref_dist_sq           (const T *p, const T *q, const T *p_end)                         { T r(0); for (; p != p_end; ++p, ++q) r += _sq(*p - *q); return r; }
template <typename T> inline static T _ref_dist_sq_limit     (const T *p, const T *q, const T *p_end, T limit_sq)             { T r(0); for (; p != p_end; ++p, ++q) if ((r += _sq(*p - *q)) > limit_sq) return MAX; return r; }
template <typename T> inline static T _ref_mul1_dist_sq      (T mp, const T *p, const T *q, const T *p_end)                   { T r(0); for (; p != p_end; ++p, ++q) r += _sq(*p * mp - *q); return r; }
template <typename T> inline static T _ref_mul1_dist_sq_limit(T mp, const T *p, const T *q, const T *p_end, T limit_sq)       { T r(0); for (; p != p_end; ++p, ++q) if ((r += _sq(*p * mp - *q)) > limit_sq) return MAX; return r; }
template <typename T> inline static T _ref_mul2_dist_sq      (T mp, T mq, const T *p, const T *q, const T *p_end)             { T r(0); for (; p != p_end; ++p, ++q) r += _sq(*p * mp - *q * mq); return r; }
template <typename T> inline static T _ref_mul2_dist_sq_limit(T mp, T mq, const T *p, const T *q, const T *p_end, T limit_sq) { T r(0); for (; p != p_end; ++p, ++q) if ((r += _sq(*p * mp - *q * mq)) > limit_sq) return MAX; return r; }
template <typename T> inline static T _ref_dot               (const T *p, const T *q, const T *p_end)                         { T r(0); for (; p != p_end; ++p) r += *p * *q; return r; }
template <typename T> inline static T _ref_dot_limit         (const T *p, const T *q, const T *p_end, T limit)                { T r(0); for (; p != p_end; ++p) if ((r += *p * *q) > limit) return MAX; return r; }
template <typename T> inline static T _ref_self_dot          (const T *p, const T *p_end)                                     { T r(0); for (; p != p_end; ++p) r += _sq(*p); return r; }
template <typename T> inline static T _ref_self_dot_limit    (const T *p, const T *p_end, T limit)                            { T r(0); for (; p != p_end; ++p) if ((r += _sq(*p)) > limit) return MAX; return r; }

template <typename T> inline T ref_dist_sq(T mp, T mq, const T *p, const T *q, const T * const p_end, T limit_sq)
{
    T r(0);
    for (; p != p_end; ++p)
        if ((r += _sq(*p * mp - *q * mq)) > limit_sq)
            return MAX;
    return r;
}

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

template <size_t N, typename T> inline T _sum(T *a) { return __add(_sum<N / 2>(a), _sum<N - (N / 2)>(a + N / 2)); }
template <> inline __m256 _sum< 1, __m256>(__m256 *a) { return *a; }
template <> inline __m256 _sum< 2, __m256>(__m256 *a) { return __add(a[0], a[1]); }
template <> inline __m128 _sum< 1, __m128>(__m128 *a) { return *a; }
template <> inline __m128 _sum< 2, __m128>(__m128 *a) { return __add(a[0], a[1]); }

template <size_t N, typename T> inline void  _clear(T *p) { _clear<N / 2>(p); _clear<N - (N / 2)>(p + N / 2); }
template <> inline void  _clear< 1, __m256>(__m256 *p) { __clear(*p); }
template <> inline void  _clear< 1, __m128>(__m128 *p) { __clear(*p); }

template <size_t N, typename T> inline float _accumulate(T *a) { return __horiz_add(_sum<N>(a)); }

template <typename T> inline T __acc_dot(T &a, T &b, T &acc)                   { return __fmadd(a, b, acc); }
template <typename T> inline T __acc_dsq(T &a, T &b, T &acc)                   { a = __sub(a, b); return __fmadd(a, a, acc); }
template <typename T> inline T __acc_mul_dsq(T &ma, T &a, T &b, T &acc)        { a = __sub(__mul(a, ma), b); return __fmadd(a, a, acc); }
template <typename T> inline T __acc_mul_dsq(T &ma, T &mb, T &a, T &b, T &acc) { a = __sub(__mul(a, ma), __mul(b, mb)); return __fmadd(a, a, acc); }

// ----------
// primitives
// ----------

// __dot
template <size_t N, typename T> inline void __dot(const float *p, const float *q, size_t i, T &a, T &b, T *r) { __dot<N / 2>(p, q, i, a, b, r); __dot<N - (N / 2)>(p, q, i + (N / 2), a, b, r); }
template <> inline void __dot< 1, __m256>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { __load_pq(p, q, i * 8, a, b); r[i] = __fmadd(a, b, r[i]); }
template <> inline void __dot< 1, __m128>(const float *p, const float *q, size_t i, __m128 &a, __m128 &b, __m128 *r) { __load_pq(p, q, i * 4, a, b); r[i] = __fmadd(a, b, r[i]); }

// __self_dot
template <size_t N, typename T> inline void __self_dot(const float *p, size_t i, T &a, T *r) { __self_dot<N / 2>(p, i, a, r); __self_dot<N - (N / 2)>(p, i + (N / 2), a, r); }
template <> inline void __self_dot< 1, __m256>(const float *p, size_t i, __m256 &a, __m256 *r) { __load(p + i * 8, a); r[i] = __fmadd(a, a, r[i]); }
template <> inline void __self_dot< 1, __m128>(const float *p, size_t i, __m128 &a, __m128 *r) { __load(p + i * 4, a); r[i] = __fmadd(a, a, r[i]); }

// __dsq
template <size_t N, typename T> inline void __dsq(const float *p, const float *q, size_t i, T &a, T &b, T *r) { __dsq<N / 2>(p, q, i, a, b, r); __dsq<N - (N / 2)>(p, q, i + (N / 2), a, b, r); }
template <> inline void __dsq< 1, __m256>(const float *p, const float *q, size_t i, __m256 &a, __m256 &b, __m256 *r) { __load_pq(p, q, i * 8, a, b); a = __sub(a, b); r[i] = __fmadd(a, a, r[i]); }
template <> inline void __dsq< 1, __m128>(const float *p, const float *q, size_t i, __m128 &a, __m128 &b, __m128 *r) { __load_pq(p, q, i * 4, a, b); a = __sub(a, b); r[i] = __fmadd(a, a, r[i]); }

// __mul2_dsq
template <size_t N, typename T> inline void __mul2_dsq(const float *p, const float *q, size_t i, T &mp, T &mq, T &a, T &b, T *r) { __mul2_dsq<N / 2>(p, q, i, mp, mq, a, b, r); __mul2_dsq<N - (N / 2)>(p, q, i + (N / 2), mp, mq, a, b, r); }
template <> inline void __mul2_dsq< 1, __m256>(const float *p, const float *q, size_t i, __m256 &mp, __m256 &mq, __m256 &a, __m256 &b, __m256 *r) { __load_pq(p, q, i * 8, a, b); a = __sub(__mul(a, mp), __mul(b, mq)); r[i] = __fmadd(a, a, r[i]); }
template <> inline void __mul2_dsq< 1, __m128>(const float *p, const float *q, size_t i, __m128 &mp, __m128 &mq, __m128 &a, __m128 &b, __m128 *r) { __load_pq(p, q, i * 4, a, b); a = __sub(__mul(a, mp), __mul(b, mq)); r[i] = __fmadd(a, a, r[i]); }

// __mul1_dsq
template <size_t N, typename T> inline void __mul1_dsq(const float *p, const float *q, size_t i, T &mp, T &a, T &b, T *r) { __mul1_dsq<N / 2>(p, q, i, mp, a, b, r); __mul1_dsq<N - (N / 2)>(p, q, i + (N / 2), mp, a, b, r); }
template <> inline void __mul1_dsq< 1, __m256>(const float *p, const float *q, size_t i, __m256 &mp, __m256 &a, __m256 &b, __m256 *r) { __load_pq(p, q, i * 8, a, b); a = __sub(__mul(a, mp), b); r[i] = __fmadd(a, a, r[i]); }
template <> inline void __mul1_dsq< 1, __m128>(const float *p, const float *q, size_t i, __m128 &mp, __m128 &a, __m128 &b, __m128 *r) { __load_pq(p, q, i * 4, a, b); a = __sub(__mul(a, mp), b); r[i] = __fmadd(a, a, r[i]); }

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
    if (p_end == p) return _0;
    if (p_end - p < N) return _dot<N / 2, T>(p, q, p_end);
    size_t count = (p_end - p) & ~(N - 1);
    T d[PARALLEL], a, b;
    float result(0);
    _clear<PARALLEL>(d);
    for (const float *e = p + count; p < e; p += N, q += N) {
        __dot<PARALLEL>(p, q, 0, a, b, d);
        result = _accumulate<PARALLEL>(d);
    }
    if ((p_end - p) == 0) return result;
    return result + _dot<N / 2, T>(p, q, p_end);
}

template <> inline float _dot<0, __m128>(const float *p, const float *q, const float * const p_end) { return _0; }
template <> inline float _dot<0, __m256>(const float *p, const float *q, const float * const p_end) { return _0; }
template <> inline float _dot<1, __m128>(const float *p, const float *q, const float * const p_end) { return _ref_dot(p, q, p_end); }
template <> inline float _dot<1, __m256>(const float *p, const float *q, const float * const p_end) { return _ref_dot(p, q, p_end); }
template <> inline float _dot<2, __m128>(const float *p, const float *q, const float * const p_end) { return _dot<1, __m128>(p, q, p_end); }
template <> inline float _dot<2, __m256>(const float *p, const float *q, const float * const p_end) { return _dot<1, __m256>(p, q, p_end); }
template <> inline float _dot<4, __m256>(const float *p, const float *q, const float * const p_end) { return _dot<4, __m128>(p, q, p_end); }

// dot_limit
template <size_t N, typename T>
inline float _dot_limit(const float *p, const float *q, const float * const p_end, float limit)
{
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    if (p_end == p) return _0;
    if (p_end - p < N) return _dot_limit<N / 2, T>(p, q, p_end, limit);
    size_t count = (p_end - p) & ~(N - 1);
    T d[PARALLEL], a, b;
    float result(0), r(0);
    _clear<PARALLEL>(d);
    for (const float *e = p + count; p < e; p += N, q += N) {
        __dot<PARALLEL>(p, q, 0, a, b, d);
        if ((result = _accumulate<PARALLEL>(d)) > limit)
            return MAX;
    }
    if ((p_end - p) == 0) return result;
    if ((r = _dot_limit<N / 2, T>(p, q, p_end, limit)) == MAX) return r;
    return (result += r) > limit ? MAX : result;
}

template <> inline float _dot_limit<0, __m128>(const float *p, const float *q, const float * const p_end, float limit) { return _0; }
template <> inline float _dot_limit<0, __m256>(const float *p, const float *q, const float * const p_end, float limit) { return _0; }
template <> inline float _dot_limit<1, __m128>(const float *p, const float *q, const float * const p_end, float limit) { return _ref_dot_limit(p, q, p_end, limit); }
template <> inline float _dot_limit<1, __m256>(const float *p, const float *q, const float * const p_end, float limit) { return _ref_dot_limit(p, q, p_end, limit); }
template <> inline float _dot_limit<2, __m128>(const float *p, const float *q, const float * const p_end, float limit) { return _dot_limit<1, __m128>(p, q, p_end, limit); }
template <> inline float _dot_limit<2, __m256>(const float *p, const float *q, const float * const p_end, float limit) { return _dot_limit<1, __m256>(p, q, p_end, limit); }
template <> inline float _dot_limit<4, __m256>(const float *p, const float *q, const float * const p_end, float limit) { return _dot_limit<4, __m128>(p, q, p_end, limit); }

// self_dot
template <size_t N, typename T>
inline float _self_dot(const float *p, const float * const p_end)
{
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    if (p_end == p) return _0;
    if (p_end - p < N) return _self_dot<N / 2, T>(p, p_end);
    size_t count = (p_end - p) & ~(N - 1);
    T d[PARALLEL], a;
    float result(0);
    _clear<PARALLEL>(d);
    for (const float *e = p + count; p < e; p += N) {
        __self_dot<PARALLEL>(p, 0, a, d);
        result = _accumulate<PARALLEL>(d);
    }
    if ((p_end - p) == 0) return result;
    return result + _self_dot<N / 2, T>(p, p_end);
}

template <> inline float _self_dot<0, __m128>(const float *p, const float * const p_end) { return _0; }
template <> inline float _self_dot<0, __m256>(const float *p, const float * const p_end) { return _0; }
template <> inline float _self_dot<1, __m128>(const float *p, const float * const p_end) { return _ref_self_dot(p, p_end); }
template <> inline float _self_dot<1, __m256>(const float *p, const float * const p_end) { return _ref_self_dot(p, p_end); }
template <> inline float _self_dot<2, __m128>(const float *p, const float * const p_end) { return _self_dot<1, __m128>(p, p_end); }
template <> inline float _self_dot<2, __m256>(const float *p, const float * const p_end) { return _self_dot<1, __m256>(p, p_end); }
template <> inline float _self_dot<4, __m256>(const float *p, const float * const p_end) { return _self_dot<4, __m128>(p, p_end); }

// self_dot_limit
template <size_t N, typename T>
inline float _self_dot_limit(const float *p, const float * const p_end, float limit)
{
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    if (p_end == p) return _0;
    if (p_end - p < N) return _self_dot_limit<N / 2, T>(p, p_end, limit);
    size_t count = (p_end - p) & ~(N - 1);
    T d[PARALLEL], a;
    float result(0), r(0);
    _clear<PARALLEL>(d);
    for (const float *e = p + count; p < e; p += N) {
        __self_dot<PARALLEL>(p, 0, a, d);
        if ((result = _accumulate<PARALLEL>(d)) > limit)
            return MAX;
    }
    if ((p_end - p) == 0) return result;
    if ((r = _self_dot_limit<N / 2, T>(p, p_end, limit)) == MAX) return r;
    return (result += r) > limit ? MAX : result;
}

template <> inline float _self_dot_limit<0, __m128>(const float *p, const float * const p_end, float limit) { return _0; }
template <> inline float _self_dot_limit<0, __m256>(const float *p, const float * const p_end, float limit) { return _0; }
template <> inline float _self_dot_limit<1, __m128>(const float *p, const float * const p_end, float limit) { float r(_ref_self_dot_limit(p, p_end, limit)); return r > limit ? MAX : r; }
template <> inline float _self_dot_limit<1, __m256>(const float *p, const float * const p_end, float limit) { float r(_ref_self_dot_limit(p, p_end, limit)); return r > limit ? MAX : r; }
template <> inline float _self_dot_limit<2, __m128>(const float *p, const float * const p_end, float limit) { return _self_dot_limit<1, __m128>(p, p_end, limit); }
template <> inline float _self_dot_limit<2, __m256>(const float *p, const float * const p_end, float limit) { return _self_dot_limit<1, __m256>(p, p_end, limit); }
template <> inline float _self_dot_limit<4, __m256>(const float *p, const float * const p_end, float limit) { return _self_dot_limit<4, __m128>(p, p_end, limit); }

// dist_sq
template <size_t N, typename T>
inline float _dist_sq(const float *p, const float *q, const float * const p_end)
{
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    if (p_end == p) return _0;
    if (p_end - p < N) return _dist_sq<N / 2, T>(p, q, p_end);
    size_t count = (p_end - p) & ~(N - 1);
    T d[PARALLEL], a, b;
    float result(0);
    _clear<PARALLEL>(d);
    for (const float *e = p + count; p < e; p += N, q += N) {
        __dsq<PARALLEL>(p, q, 0, a, b, d);
        result = _accumulate<PARALLEL>(d);
    }
    if ((p_end - p) == 0) return result;
    return result + _dist_sq<N / 2, T>(p, q, p_end);
}

template <> inline float _dist_sq<0, __m128>(const float *p, const float *q, const float * const p_end) { return _0; }
template <> inline float _dist_sq<0, __m256>(const float *p, const float *q, const float * const p_end) { return _0; }
template <> inline float _dist_sq<1, __m128>(const float *p, const float *q, const float * const p_end) { return _ref_dist_sq(p, q, p_end); }
template <> inline float _dist_sq<1, __m256>(const float *p, const float *q, const float * const p_end) { return _ref_dist_sq(p, q, p_end); }
template <> inline float _dist_sq<2, __m128>(const float *p, const float *q, const float * const p_end) { return _dist_sq<1, __m128>(p, q, p_end); }
template <> inline float _dist_sq<2, __m256>(const float *p, const float *q, const float * const p_end) { return _dist_sq<1, __m256>(p, q, p_end); }
template <> inline float _dist_sq<4, __m256>(const float *p, const float *q, const float * const p_end) { return _dist_sq<4, __m128>(p, q, p_end); }

// dist_sq_limit
template <size_t N, typename T>
inline float _dist_sq_limit(const float *p, const float *q, const float * const p_end, float limit_sq) // about 10% faster than stride32
{
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    if (p_end == p) return _0;
    if (p_end - p < N) return _dist_sq_limit<N / 2, T>(p, q, p_end, limit_sq);
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
    if ((r = _dist_sq_limit<N / 2, T>(p, q, p_end, limit_sq)) == MAX) return r;
    return (result += r) > limit_sq ? MAX : result;
}

template <> inline float _dist_sq_limit<0, __m128>(const float *p, const float *q, const float * const p_end, float limit_sq) { return _0; }
template <> inline float _dist_sq_limit<0, __m256>(const float *p, const float *q, const float * const p_end, float limit_sq) { return _0; }
template <> inline float _dist_sq_limit<1, __m128>(const float *p, const float *q, const float * const p_end, float limit_sq) { float r(_ref_dist_sq(p, q, p_end)); return r > limit_sq ? MAX : r; }
template <> inline float _dist_sq_limit<1, __m256>(const float *p, const float *q, const float * const p_end, float limit_sq) { float r(_ref_dist_sq(p, q, p_end)); return r > limit_sq ? MAX : r; }
template <> inline float _dist_sq_limit<2, __m128>(const float *p, const float *q, const float * const p_end, float limit_sq) { return _dist_sq_limit<1, __m128>(p, q, p_end, limit_sq); }
template <> inline float _dist_sq_limit<2, __m256>(const float *p, const float *q, const float * const p_end, float limit_sq) { return _dist_sq_limit<1, __m256>(p, q, p_end, limit_sq); }
template <> inline float _dist_sq_limit<4, __m256>(const float *p, const float *q, const float * const p_end, float limit_sq) { return _dist_sq_limit<4, __m128>(p, q, p_end, limit_sq); }

// mul1_dist_sq
template <size_t N, typename T>
inline float _mul1_dist_sq(float mul_p, const float *p, const float *q, const float * const p_end)
{
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    if (p_end == p) return _0;
    if (p_end - p < N) return _mul1_dist_sq<N / 2, T>(mul_p, p, q, p_end);
    size_t count = (p_end - p) & ~(N - 1);
    T d[PARALLEL], a, b, ma;
    float result(0);
    __load_const(mul_p, ma);
    _clear<PARALLEL>(d);
    for (const float *e = p + count; p < e; p += N, q += N) {
        __mul1_dsq<PARALLEL>(p, q, 0, ma, a, b, d);
        result = _accumulate<PARALLEL>(d);
    }
    if ((p_end - p) == 0) return result;
    return result + _mul1_dist_sq<N / 2, T>(mul_p, p, q, p_end);
}

template <> inline float _mul1_dist_sq<0, __m128>(float mp, const float *p, const float *q, const float * const p_end) { return _0; }
template <> inline float _mul1_dist_sq<0, __m256>(float mp, const float *p, const float *q, const float * const p_end) { return _0; }
template <> inline float _mul1_dist_sq<1, __m128>(float mp, const float *p, const float *q, const float * const p_end) { return _ref_mul1_dist_sq(mp, p, q, p_end); }
template <> inline float _mul1_dist_sq<1, __m256>(float mp, const float *p, const float *q, const float * const p_end) { return _ref_mul1_dist_sq(mp, p, q, p_end); }
template <> inline float _mul1_dist_sq<2, __m128>(float mp, const float *p, const float *q, const float * const p_end) { return _mul1_dist_sq<1, __m128>(mp, p, q, p_end); }
template <> inline float _mul1_dist_sq<2, __m256>(float mp, const float *p, const float *q, const float * const p_end) { return _mul1_dist_sq<1, __m256>(mp, p, q, p_end); }
template <> inline float _mul1_dist_sq<4, __m256>(float mp, const float *p, const float *q, const float * const p_end) { return _mul1_dist_sq<4, __m128>(mp, p, q, p_end); }

// mul1_dist_sq_limit
template <size_t N, typename T>
inline float _mul1_dist_sq_limit(float mul_p, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    if (p_end == p) return _0;
    if (p_end - p < N) return _mul1_dist_sq_limit<N / 2, T>(mul_p, p, q, p_end, limit_sq);
    size_t count = (p_end - p) & ~(N - 1);
    T d[PARALLEL], a, b, ma;
    float result(0), r(0);
    __load_const(mul_p, ma);
    _clear<PARALLEL>(d);
    for (const float *e = p + count; p < e; p += N, q += N) {
        __mul1_dsq<PARALLEL>(p, q, 0, ma, a, b, d);
        if ((result = _accumulate<PARALLEL>(d)) > limit_sq)
            return MAX;
    }
    if ((p_end - p) == 0) return result;
    if ((r = _mul1_dist_sq_limit<N / 2, T>(mul_p, p, q, p_end, limit_sq)) == MAX) return r;
    return (result += r) > limit_sq ? MAX : result;
}

template <> inline float _mul1_dist_sq_limit<0, __m128>(float mp, const float *p, const float *q, const float * const p_end, float limit_sq) { return _0; }
template <> inline float _mul1_dist_sq_limit<0, __m256>(float mp, const float *p, const float *q, const float * const p_end, float limit_sq) { return _0; }
template <> inline float _mul1_dist_sq_limit<1, __m128>(float mp, const float *p, const float *q, const float * const p_end, float limit_sq) { float r(_ref_mul1_dist_sq(mp, p, q, p_end)); return r > limit_sq ? MAX : r; }
template <> inline float _mul1_dist_sq_limit<1, __m256>(float mp, const float *p, const float *q, const float * const p_end, float limit_sq) { float r(_ref_mul1_dist_sq(mp, p, q, p_end)); return r > limit_sq ? MAX : r; }
template <> inline float _mul1_dist_sq_limit<2, __m128>(float mp, const float *p, const float *q, const float * const p_end, float limit_sq) { return _mul1_dist_sq_limit<1, __m128>(mp, p, q, p_end, limit_sq); }
template <> inline float _mul1_dist_sq_limit<2, __m256>(float mp, const float *p, const float *q, const float * const p_end, float limit_sq) { return _mul1_dist_sq_limit<1, __m256>(mp, p, q, p_end, limit_sq); }
template <> inline float _mul1_dist_sq_limit<4, __m256>(float mp, const float *p, const float *q, const float * const p_end, float limit_sq) { return _mul1_dist_sq_limit<4, __m128>(mp, p, q, p_end, limit_sq); }

// mul2_dist_sq
template <size_t N, typename T>
inline float _mul2_dist_sq(float mul_p, float mul_q, const float *p, const float *q, const float * const p_end)
{
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    if (p_end == p) return _0;
    if (p_end - p < N) return _mul2_dist_sq<N / 2, T>(mul_p, mul_q, p, q, p_end);
    size_t count = (p_end - p) & ~(N - 1);
    T d[PARALLEL], a, b, ma, mb;
    float result(0);
    __load_const(mul_p, ma);
    __load_const(mul_q, mb);
    _clear<PARALLEL>(d);
    for (const float *e = p + count; p < e; p += N, q += N) {
        __mul2_dsq<PARALLEL>(p, q, 0, ma, mb, a, b, d);
        result = _accumulate<PARALLEL>(d);
    }
    if ((p_end - p) == 0) return result;
    return result + _mul2_dist_sq<N / 2, T>(mul_p, mul_q, p, q, p_end);
}

template <> inline float _mul2_dist_sq<0, __m128>(float mp, float mq, const float *p, const float *q, const float * const p_end) { return _0; }
template <> inline float _mul2_dist_sq<0, __m256>(float mp, float mq, const float *p, const float *q, const float * const p_end) { return _0; }
template <> inline float _mul2_dist_sq<1, __m128>(float mp, float mq, const float *p, const float *q, const float * const p_end) { return _ref_mul2_dist_sq(mp, mq, p, q, p_end); }
template <> inline float _mul2_dist_sq<1, __m256>(float mp, float mq, const float *p, const float *q, const float * const p_end) { return _ref_mul2_dist_sq(mp, mq, p, q, p_end); }
template <> inline float _mul2_dist_sq<2, __m128>(float mp, float mq, const float *p, const float *q, const float * const p_end) { return _mul2_dist_sq<1, __m128>(mp, mq, p, q, p_end); }
template <> inline float _mul2_dist_sq<2, __m256>(float mp, float mq, const float *p, const float *q, const float * const p_end) { return _mul2_dist_sq<1, __m256>(mp, mq, p, q, p_end); }
template <> inline float _mul2_dist_sq<4, __m256>(float mp, float mq, const float *p, const float *q, const float * const p_end) { return _mul2_dist_sq<4, __m128>(mp, mq, p, q, p_end); }

// mul2_dist_sq_limit
template <size_t N, typename T>
inline float _mul2_dist_sq_limit(float mul_p, float mul_q, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    static constexpr size_t ELEMS = _mm_traits<T>::count, PARALLEL = N / ELEMS;
    if (p_end == p) return _0;
    if (p_end - p < N) return _mul2_dist_sq_limit<N / 2, T>(mul_p, mul_q, p, q, p_end, limit_sq);
    size_t count = (p_end - p) & ~(N - 1);
    T d[PARALLEL], a, b, ma, mb;
    float result(0), r(0);
    __load_const(mul_p, ma);
    __load_const(mul_q, mb);
    _clear<PARALLEL>(d);
    for (const float *e = p + count; p < e; p += N, q += N) {
        __mul2_dsq<PARALLEL>(p, q, 0, ma, mb, a, b, d);
        if ((result = _accumulate<PARALLEL>(d)) > limit_sq)
            return MAX;
    }
    if ((p_end - p) == 0) return result;
    if ((r = _mul2_dist_sq_limit<N / 2, T>(mul_p, mul_q, p, q, p_end, limit_sq)) == MAX) return r;
    return (result += r) > limit_sq ? MAX : result;
}

template <> inline float _mul2_dist_sq_limit<0, __m128>(float mp, float mq, const float *p, const float *q, const float * const p_end, float limit_sq) { return _0; }
template <> inline float _mul2_dist_sq_limit<0, __m256>(float mp, float mq, const float *p, const float *q, const float * const p_end, float limit_sq) { return _0; }
template <> inline float _mul2_dist_sq_limit<1, __m128>(float mp, float mq, const float *p, const float *q, const float * const p_end, float limit_sq) { float r(_ref_mul2_dist_sq(mp, mq, p, q, p_end)); return r > limit_sq ? MAX : r; }
template <> inline float _mul2_dist_sq_limit<1, __m256>(float mp, float mq, const float *p, const float *q, const float * const p_end, float limit_sq) { float r(_ref_mul2_dist_sq(mp, mq, p, q, p_end)); return r > limit_sq ? MAX : r; }
template <> inline float _mul2_dist_sq_limit<2, __m128>(float mp, float mq, const float *p, const float *q, const float * const p_end, float limit_sq) { return _mul2_dist_sq_limit<1, __m128>(mp, mq, p, q, p_end, limit_sq); }
template <> inline float _mul2_dist_sq_limit<2, __m256>(float mp, float mq, const float *p, const float *q, const float * const p_end, float limit_sq) { return _mul2_dist_sq_limit<1, __m256>(mp, mq, p, q, p_end, limit_sq); }
template <> inline float _mul2_dist_sq_limit<4, __m256>(float mp, float mq, const float *p, const float *q, const float * const p_end, float limit_sq) { return _mul2_dist_sq_limit<4, __m128>(mp, mq, p, q, p_end, limit_sq); }

float dot(const float *p, const float * const p_end)
{
    return _self_dot<64, __m256>(p, p_end);
}

float dot(const float *p, const float * const p_end, float limit)
{
    return limit == MAX ? dot(p, p_end) : _self_dot_limit<64, __m256>(p, p_end, limit);
}

float dot(const float *p, const float *q, const float * const p_end)
{
    return p == q ? dot(p, p_end) : _dot<64, __m256>(p, q, p_end);
}

float dot(const float *p, const float *q, const float * const p_end, float limit)
{
    if (limit == MAX) return dot(p, q, p_end);
    if (p == q) return dot(p, p_end, limit);
    return _dot_limit<64, __m256>(p, q, p_end, limit);
}

float normalize(const float *p, float *out, const float * const p_end)
{
    double norm = sqrt(double(dot(p, p_end))),
    invn = double(1.0) / norm;
    mul(p, float(invn), out, p_end);
    return float(norm);
}

float dist_sq(const float *p, const float *q, const float * const p_end)
{
    return p == q ? _0 : _dist_sq<64, __m256>(p, q, p_end);
}

float dist_sq(const float *p, const float *q, const float * const p_end, float limit_sq)
{
    if (limit_sq == MAX) return dist_sq(p, q, p_end);
    return p == q ? _0 : _dist_sq_limit<64, __m256>(p, q, p_end, limit_sq);
}

float dist_sq(float mul_p, const float *p, const float *q, const float * const p_end)
{
    if (p == q) return _0;
    if (mul_p == _0) return dot(q, q + (p_end - p));
    if (mul_p == _1) return dist_sq(p, q, p_end);
    return _mul1_dist_sq<64, __m256>(mul_p, p, q, p_end);
}

float dist_sq(float mul_p, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    if (limit_sq == MAX) return dist_sq(mul_p, p, q, p_end);
    if (p == q) return _0;
    if (mul_p == _0) return dot(q, q + (p_end - p), limit_sq);
    if (mul_p == _1) return dist_sq(p, q, p_end, limit_sq);
    return _mul1_dist_sq_limit<64, __m256>(mul_p, p, q, p_end, limit_sq);
}

float dist_sq(float mul_p, float mul_q, const float *p, const float *q, const float * const p_end)
{
    if (p == q) return _0;
    bool mp0 = (mul_p == _0), mq0 = (mul_q == _0), mp1 = (mul_p == _1), mq1 = (mul_q == _1);
    if (mp0 || mq0) {
        if (mp0 && mq0) return _0;
        else if (mp0)   return dot(q, q + (p_end - p));
        return dot(p, p_end);
    } else if (mp1 || mq1) {
        if (mp1 && mq1) return dist_sq(p, q, p_end);
        else if (mp1)   return dist_sq(mul_q, q, p, q + (p_end - p));
        return dist_sq(mul_p, p, q, p_end);
    }
    return _mul2_dist_sq<64, __m256>(mul_p, mul_q, p, q, p_end);
}

float dist_sq(float mul_p, float mul_q, const float *p, const float *q, const float * const p_end, float limit_sq)
{
    if (limit_sq == MAX) return dist_sq(mul_p, mul_q, p, q, p_end);
    if (p == q) return _0;
    bool mp0 = (mul_p == _0), mq0 = (mul_q == _0), mp1 = (mul_p == _1), mq1 = (mul_q == _1);
    if (mp0 || mq0) {
        if (mp0 && mq0) return _0;
        else if (mp0)   return dot(q, q + (p_end - p), limit_sq);
        return dot(p, p_end, limit_sq);
    } else if (mp1 || mq1) {
        if (mp1 && mq1) return dist_sq(p, q, p_end, limit_sq);
        else if (mp1)   return dist_sq(mul_q, q, p, q + (p_end - p), limit_sq);
        return dist_sq(mul_p, p, q, p_end, limit_sq);
    }
    return _mul2_dist_sq_limit<64, __m256>(mul_p, mul_q, p, q, p_end, limit_sq);
}

void test()
{
    
}

}
