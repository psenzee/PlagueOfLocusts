#pragma once

#include <cstddef>
#include "pow_ui.h"
#include "f01.h"
/*
// clamps a value to a range, by default 0-1
template <typename T> inline constexpr T clamp(const T &a, const T &min, const T &max)       { return a < min ? min : (a > max ? max : a); }
template <typename T> inline constexpr T clamp(const T &a)                                   { return a < T(0) ? T(0) : (a > T(1) ? T(1) : a); }

// maps a value in the range min-max to a value in the range 0-1
template <typename T> inline constexpr T norm(const T &t, const T &min, const T &max)        { return (t - min) / (max - min); }

// maps a value in the range 0-1 to a value in the range min-max
template <typename T> inline constexpr T denorm(const T &t, const T &min, const T &max)      { return t * (max - min) + min; }

// Performs linear interpolation between two values.
//
// @param n0 The first value.
// @param n1 The second value.
// @param a The alpha value.
//
// @returns The interpolated value.
//
// The alpha value should range from 0.0 to 1.0.  If the alpha value is
// 0.0, this function returns @a n0.  If the alpha value is 1.0, this
// function returns @a n1.
//
template <typename T, typename U> inline T lerp(const T &a, const T &b, const U &t)               { return a * (U(1) - t) + b * t; }
*/
template <typename T, typename U> inline T linear_interpolate(const T &a, const T &b, const U &t) { return a * (U(1) - t) + b * t; }

// Performs cubic interpolation between two values bound between two other
// values.
//
// @param n0 The value before the first value.
// @param n1 The first value.
// @param n2 The second value.
// @param n3 The value after the second value.
// @param a The alpha value.
//
// @returns The interpolated value.
//
// The alpha value should range from 0.0 to 1.0.  If the alpha value is
// 0.0, this function returns @a n1.  If the alpha value is 1.0, this
// function returns @a n2.
//
template <typename T, typename U> inline T cubic_interpolate(const T &n0, const T &n1, const T &n2, const T &n3, const U &t)
{
    T p = (n3 - n2) - (n0 - n1), q = (n0 - n1) - p, r = n2 - n0, s = n1;
    U tt = t * t, ttt = tt * t;
    return p * ttt + q * tt + r * t + s;
}

// Maps a value onto a cubic S-curve.
//
// @param a The value to map onto a cubic S-curve.
//
// @returns The mapped value.
//
// @a a should range from 0.0 to 1.0.
//
// The derivitive of a cubic S-curve is zero at @a a = 0.0 and @a a =
// 1.0
//
template <typename T> inline T s_curve_cubic(const T &a) { return (a * a * (T(3) - T(2) * a)); }
template <typename T> inline T smooth_step(const T &a, const T &b, const T &t) { return s_curve_cubic(clamp((t - a) / (b - a))); }

// Maps a value onto a quintic S-curve.
//
// @param a The value to map onto a quintic S-curve.
//
// @returns The mapped value.
//
// @a a should range from 0.0 to 1.0.
//
// The first derivitive of a quintic S-curve is zero at @a a = 0.0 and
// @a a = 1.0
//
// The second derivitive of a quintic S-curve is zero at @a a = 0.0 and
// @a a = 1.0
//
template <typename T> inline T s_curve_quintic(const T &a) { T a3 = a * a * a, a4 = a3 * a, a5 = a4 * a; return (T(6) * a5) - (T(15) * a4) + (T(10) * a3); }
template <typename T> inline T smoother_step(const T &a, const T &b, const T &t) { return s_curve_quintic(clamp((t - a) / (b - a))); } // this actually isn't quite right, we need to rescale back from 0, 1

// Returns binomial coefficient without explicit use of factorials,
// which can't be used with negative integers
inline constexpr int pascal_triangle_procedural(int a, unsigned N)
{
    int r(1);
    for (unsigned i = 0; i < N; ++i)
        r *= (a - i) / (i + 1);
    return r;
}

template <unsigned N> inline constexpr int pascal_triangle   (int a) { return ((a - (N - 1)) / N) * pascal_triangle<N - 1>(a); }
template <>           inline constexpr int pascal_triangle<0>(int a) { return 1; }
template <>           inline constexpr int pascal_triangle<1>(int a) { return a; }

// Generalized smoothstep
template <typename T> inline T general_smooth_step(int N, T x)
{
    x = clamp(x); // x must be equal to or between 0 and 1
    T result = T(0);
    for (int n = 0; n <= N; ++n)
        result += pascal_triangle_procedural(-N - 1, n) *
                  pascal_triangle_procedural(2 * N + 1, N - n) *
                  pow(x, N + n + 1);
    return result;
}

// tests

template <unsigned N> void test_pascal_triangle()
{
    for (int i = -10; i < 10; i++) {
        int a = pascal_triangle_procedural(i, N), b = pascal_triangle<N>(i);
        if (a != b)
            std::cout << "error [" << i << " " << N << "] " << a << " != " << b << std::endl;
    }
}

/*
void tests_interpolate()
{
    test_pascal_triangle<1>();
    test_pascal_triangle<2>();
    test_pascal_triangle<3>();
    test_pascal_triangle<4>();
}
*/
