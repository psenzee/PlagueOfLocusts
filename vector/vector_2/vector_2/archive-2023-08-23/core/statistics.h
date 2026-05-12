#pragma once

#include <cmath>
#include <algorithm>
#include <numeric>
#include "stl.h"

template <typename VectorT>
inline typename VectorT::value_type sum(const VectorT &values)
{
    typedef typename VectorT::value_type value_type;
    return std::accumulate(values.begin(), values.end(), value_type(0));
}

template <typename VectorT>
inline typename VectorT::value_type mean(const VectorT &values)
{
    return sum(values) / values.size();
}

template <typename VectorT>
inline typename VectorT::value_type variance(const VectorT &values)
{
    typedef typename VectorT::value_type value_type;
    value_type m(mean(values)), sum(0);
    for (auto v : values) {
        value_type u(v - m);
        sum += u * u;
    }
    return sum / (values.size() - 1);
}

template <typename VectorT>
inline typename VectorT::value_type skewness(const VectorT &values)
{
    typedef typename VectorT::value_type value_type;
    value_type n(values.size()), factor = n / ((n - 1.0) * (n - 2.0)),
               sum(0), sd = sqrt(variance(values)),
               m(mean(values));
    for (auto v : values) {
        value_type u = (v - m) / sd;
        sum += u * u * u;
    }
    return sum * factor;
}

template <typename VectorT>
inline typename VectorT::value_type kurtosis(const VectorT &values)
{
    typedef typename VectorT::value_type value_type;
    value_type n(values.size()), factor = n * (n + 1) / ((n - 1.0) * (n - 2.0) * (n - 3.0)),
               sum(0), sd = sqrt(variance(values)),
               m(mean(values));
    for (auto v : values) {
        value_type u = (v - m) / sd;
        sum += u * u * u * u;
    }
    return sum * factor - (3.0 * (n - 1.0) * (n - 1.0)) / ((n - 2.0) * (n - 3.0));
}

template <typename VectorT>
inline std::pair<typename VectorT::value_type, typename VectorT::value_type> least_squares(const VectorT &a, const VectorT &b)
{
    typedef typename VectorT::value_type value_type;
    value_type sum_a(0), sum_b(0), sum_ab(0), sum_aa(0);
    size_t n = a.size();
    for (size_t i = 0; i != n; ++i) {
        value_type ai(a[i]), bi(b[i]);
        sum_a += ai; sum_b += bi;
        sum_ab += ai * bi; sum_aa += ai * ai;
    }
    value_type m = (n * sum_ab - sum_a * sum_b) / (n * sum_aa - sum_a * sum_a);
    return std::pair<value_type, value_type>(m, (sum_b - m * sum_a) / n);
}

template <typename PairValueT0, typename PairValueT1>
inline std::pair<PairValueT0, PairValueT0> least_squares(const std::vector< std::pair<PairValueT0, PairValueT1> >  &a)
{
    typedef std::pair<PairValueT0, PairValueT0> pair_type;
    typedef typename pair_type::first_type value_type;
    value_type sum_a(0), sum_b(0), sum_ab(0), sum_aa(0);
    size_t n = a.size();
    for (size_t i = 0; i != n; ++i) {
        value_type ai(a[i].first), bi(a[i].second);
        sum_a += ai; sum_b += bi;
        sum_ab += ai * bi; sum_aa += ai * ai;
    }
    value_type m = (n * sum_ab - sum_a * sum_b) / (n * sum_aa - sum_a * sum_a);
    return std::pair<value_type, value_type>(m, (sum_b - m * sum_a) / n);
}

template <typename PairValueT0, typename PairValueT1, int N>
inline std::pair<PairValueT0, PairValueT0> least_squares(const std::array< std::pair<PairValueT0, PairValueT1>, N > &a)
{
    typedef std::pair<PairValueT0, PairValueT0> pair_type;
    typedef typename pair_type::first_type value_type;
    value_type sum_a(0), sum_b(0), sum_ab(0), sum_aa(0);
    size_t n = a.size();
    for (size_t i = 0; i != n; ++i) {
        value_type ai(a[i].first), bi(a[i].second);
        sum_a += ai; sum_b += bi;
        sum_ab += ai * bi; sum_aa += ai * ai;
    }
    value_type m = (n * sum_ab - sum_a * sum_b) / (n * sum_aa - sum_a * sum_a);
    return std::pair<value_type, value_type>(m, (sum_b - m * sum_a) / n);
}

template <typename PairValueT0, typename PairValueT1, int N>
inline std::pair<PairValueT0, PairValueT0> least_squares_reversed(const std::array< std::pair<PairValueT0, PairValueT1>, N > &a)
{
    typedef std::pair<PairValueT0, PairValueT0> pair_type;
    typedef typename pair_type::first_type value_type;
    value_type sum_a(0), sum_b(0), sum_ab(0), sum_aa(0);
    size_t n = a.size();
    for (size_t i = 0; i != n; ++i) {
        value_type ai(a[i].second), bi(a[i].first);
        sum_a += ai; sum_b += bi;
        sum_ab += ai * bi; sum_aa += ai * ai;
    }
    value_type m = (n * sum_ab - sum_a * sum_b) / (n * sum_aa - sum_a * sum_a);
    return std::pair<value_type, value_type>(m, (sum_b - m * sum_a) / n);
}

template <typename PairValueT0, typename PairValueT1>
inline std::pair<PairValueT0, PairValueT0> least_squares_reversed(const std::vector< std::pair<PairValueT0, PairValueT1> >  &a)
{
    typedef std::pair<PairValueT0, PairValueT0> pair_type;
    typedef typename pair_type::first_type value_type;
    value_type sum_a(0), sum_b(0), sum_ab(0), sum_aa(0);
    size_t n = a.size();
    for (size_t i = 0; i != n; ++i) {
        value_type ai(a[i].second), bi(a[i].first);
        sum_a += ai; sum_b += bi;
        sum_ab += ai * bi; sum_aa += ai * ai;
    }
    value_type m = (n * sum_ab - sum_a * sum_b) / (n * sum_aa - sum_a * sum_a);
    return std::pair<value_type, value_type>(m, (sum_b - m * sum_a) / n);
}

template <typename VectorT>
inline std::pair<typename VectorT::value_type, typename VectorT::value_type> least_squares_with_index(const VectorT &b)
{
    typedef typename VectorT::value_type value_type;
    value_type sum_i(0), sum_b(0), sum_ib(0), sum_ii(0);
    size_t n = b.size();
    for (size_t i = 0; i != n; ++i) {
        value_type ii(i), bi(b[i]);
        sum_i += ii; sum_b += bi;
        sum_ib += ii * bi; sum_ii += ii * ii;
    }
    value_type m = (n * sum_ib - sum_i * sum_b) / (n * sum_ii - sum_i * sum_i);
    return std::pair<value_type, value_type>(m, (sum_b - m * sum_i) / n);
}

