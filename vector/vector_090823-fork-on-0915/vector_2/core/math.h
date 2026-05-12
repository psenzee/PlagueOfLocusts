#pragma once

#include <cmath>
#include <cstdlib>
#include <algorithm>

namespace math
{

enum Sign { NEGATIVE = -1, ZERO = 0, POSITIVE = 1 };

constexpr double PI      = 3.1415926535897931;
constexpr double TWOPI   = PI * 2.0;
constexpr double FOURPI  = PI * 4.0;
constexpr double HALFPI  = PI / 2.0;
constexpr double EPSILON = 0.0000076;

inline uint32_t                       to_bits(float a)                                     { return *(uint32_t *)((void *)&a); }
inline uint64_t                       to_bits(double a)                                    { return *(uint64_t *)((void *)&a); }
inline uint32_t                      *to_bits(float *a)                                    { return  (uint32_t *)((void *)a); }
inline uint64_t                      *to_bits(double *a)                                   { return  (uint64_t *)((void *)a); }

inline float                          to_real(uint32_t a)                                  { return *(float   *)((void *)&a); }
inline double                         to_real(uint64_t a)                                  { return *(double  *)((void *)&a); }
inline float                         *to_real(uint32_t *a)                                 { return  (float   *)((void *)a); }
inline double                        *to_real(uint64_t *a)                                 { return  (double  *)((void *)a); }

template <typename T> inline T        sq(T u)                                              { return u * u; }

template <typename T> inline T        random(T lo, T hi)                                   { T r = T(rand()) / T(RAND_MAX); return (hi - lo) * r + lo; }
inline int                            rand_sign()                                          { return rand() <= (RAND_MAX / 2) ? -1 : 1; }
template <typename T> inline T        eps(T a)                                             { const T aa(abs(a) + 1); return isinf(aa) ? T(EPSILON) : T(EPSILON) * aa; }
template <typename T> inline bool     approx_eq(T a, T b)                                  { return (a == b) || (abs(a - b) <= eps(a)); }
template <typename T> inline Sign     sign(T a)                                            { return (a < -eps(a)) ? NEGATIVE : (a > eps(a) ? POSITIVE : ZERO); }
template <typename T> inline bool     eq(T a, T b)                                         { return approx_eq(a, b); }
template <typename T> inline bool     lt(T a, T b)                                         { return a - b < -eps(a); }
template <typename T> inline bool     gt(T a, T b)                                         { return a - b >  eps(a); }
template <typename T> inline bool     lte(T a, T b)                                        { return a - b <  eps(a); }
template <typename T> inline bool     gte(T a, T b)                                        { return a - b > -eps(a); }
template <typename T> inline bool     gt_zero(T a)                                         { return a > eps(a); }
template <typename T> inline bool     lt_zero(T a)                                         { return a < -eps(a); }
template <typename T> inline bool     near_zero(T a)                                       { return abs(a) <= eps(a); }
template <typename T> inline bool     sign_eq(const T &a, const T &b)                      { return sign(a) == sign(b); }
template <typename T> inline T        clamp(const T &a, const T &mn, const T &mx)          { return a < mn ? mn : (a > mx ? mx : a); }
template <typename T> inline void     sort2(T &a, T &b)                                    { if (a > b) std::swap(a, b); }
template <typename T> inline void     sort3(T &a, T &b, T &c)                              { sort2(a, b); sort2(b, c); sort2(a, b); }

template <typename T> inline uint32_t hash(T v)                                            { if (v == T(0)) return 0; int ex; T ma(frexp(v, &ex)); return (uint32_t)((2 * fabs(ma) - 1) * ~0u); }

template <typename T> inline T        det2x2(T a, T b, T c, T d)                           { return a * d - b * c; }
template <typename T> inline T        det3x3(T a, T b, T c, T d, T e, T f, T g, T h, T i)  { return a * (e * i - f * h) + b * (f * g - d * i) + c * (d * h - e * g); }

template <typename T> inline T        det4x4(T a, T b, T c, T d, T e, T f, T g, T h,
                                             T i, T j, T k, T l, T m, T n, T o, T p)
{
    return a * det3x3(f, g, h, j, k, l, n, o, p) +
		   b * det3x3(g, h, e, k, l, i, o, p, m) +
		   c * det3x3(h, e, f, l, i, j, p, m, n) +
		   d * det3x3(e, f, g, i, j, k, m, n, o);
}


};
