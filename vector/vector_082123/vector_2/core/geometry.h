#pragma once

#include <cmath>
#include <algorithm>
#include <numeric>

#include "base.h"

/*
// this is slow -- it is not fully vectorizable by the compiler
template <typename T0, typename T1>
inline typename T0::value_type dot(const T0 &a, const T1 &b)
{
    typedef typename T0::value_type value_type_0;
    typedef typename T1::value_type value_type_1;
    return accumulate_combine(a, b, op_multiply<value_type_0, value_type_1>(), value_type_0(0), op_add<value_type_0, value_type_1>());
}
*/

template <typename ContainerT0, typename ContainerT1>
inline typename ContainerT0::value_type dot(const ContainerT0 &a, const ContainerT1 &b)
{
    typedef typename ContainerT0::value_type val_0_t;
    typedef typename ContainerT1::value_type val_1_t;
    val_0_t sum(0);
    auto j = b.begin();
    for (auto i = a.begin(), e = a.end(); i != e; ++i, ++j) sum += *i * *j;
    return sum;
}

template <typename ContainerT>
inline typename ContainerT::value_type magnitude_sq(const ContainerT &u)
{
    typedef typename ContainerT::value_type val_t;
    val_t sum(0);
    for (const auto &v : u) sum += v * v; return sum;
    return sum;
}
/*
template <typename T, size_t N>
inline T dot(const std::array<T, N> &a, const std::array<T, N> &b)
    { T sum(0); for (auto i = a.begin(), j = b.begin(), e = a.end(); i != e; ++i, ++j) sum += *i * *j; return sum; }

template <typename T>
inline T dot(const std::vector<T> &a, const std::vector<T> &b)
    { T sum(0); for (auto i = a.begin(), j = b.begin(), e = a.end(); i != e; ++i, ++j) sum += *i * *j; return sum; }
*/
inline float  dot(float a,  float b)  { return a * b; }
inline double dot(double a, double b) { return a * b; }

inline float  magnitude_sq(float a)  { return a * a; }
inline double magnitude_sq(double a) { return a * a; }

namespace {

template <typename T>
struct _powers
{
    template <size_t P> inline static T root(T v) { return ::pow(v, T(1) / P); }

    template <> inline static T root<0>(T v) { return T(1); }
    template <> inline static T root<1>(T v) { return v; }
    template <> inline static T root<2>(T v) { return sqrt(v); }
    template <> inline static T root<3>(T v) { return cbrt(v); }

    template <size_t P> inline static T pow(T v) { return pow<P - 1>(v) * v; }
    
    template <> inline static T pow<0>(T v) { return T(1); }
    template <> inline static T pow<1>(T v) { return v; }
    template <> inline static T pow<2>(T v) { return v * v; }
};

}

template <size_t P, typename T0, typename T1>
inline typename T0::value_type lp_distance_power(const T0 &a, const T1 &b)
{
    typedef typename T0::value_type value_type;
    auto j = b.begin(), je = b.end();
    value_type sum(0);
    for (auto i = a.begin(), ie = a.end(); j != je && i != ie; i++, j++)
        sum += abs(_powers<value_type>::template pow<P>(*i - *j));
    return sum;
}

template <size_t P, typename T0, typename T1>
inline typename T0::value_type lp_distance(const T0 &a, const T1 &b)
{
    typedef typename T0::value_type value_type;
    //value_type value = lp_distance_power<P>(a, b);
    return _powers<value_type>::template root<P>(lp_distance_power<P>(a, b));
}

//template <typename T0, typename T1> inline typename T0::value_type distance_sq(const T0 &a, const T1 &b) { return lp_distance_power<2>(a, b); }
//template <typename T0, typename T1> inline typename T0::value_type distance   (const T0 &a, const T1 &b) { return lp_distance<2>(a, b); }

template <typename ContainerT0, typename ContainerT1>
inline typename ContainerT0::value_type distance_sq(const ContainerT0 &a, const ContainerT1 &b)
{
    typedef typename ContainerT0::value_type value_t;
    value_t sum(0); for (auto i = a.begin(), j = b.begin(), e = a.end(); i != e; ++i, ++j) { value_t d = (*i - *j); sum += d * d; } return sum;
}

template <typename ContainerT0, typename ContainerT1>
inline typename ContainerT0::value_type distance(const ContainerT0 &a, const ContainerT1 &b) { return sqrt(distance_sq(a, b)); }

template <typename ContainerT0, typename ContainerT1>
inline typename ContainerT0::value_type distance_sq_limit(const ContainerT0 &a, const ContainerT1 &b, typename ContainerT0::value_type limit_sq)
{
    typedef typename ContainerT0::value_type val_t;
    val_t sum(0);
    for (auto i = a.begin(), j = b.begin(), e = a.end(); i != e; ++i, ++j) {
        val_t d = (*i - *j); sum += d * d;
        if (sum > limit_sq) return std::numeric_limits<val_t>::max();
    }
    return sum;
}
