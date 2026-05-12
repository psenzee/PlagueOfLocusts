#pragma once

#include <iostream>
#include <array>
#include <algorithm>

template <size_t N, typename FloatT = float>
struct vector_tests
{
    typedef FloatT                float_t;
    typedef std::array<FloatT, N> vector_t;
    
    inline static vector_t     &set(vector_t &v, float_t value = 0.0, size_t n = N)      { for (size_t i = 0; i < n; i++) v[i] = value; return v; }
    inline static vector_t     &set_asc(vector_t &v, size_t n = N)                       { for (size_t i = 0; i < n; i++) v[i] = float_t(i); return v; }
    
    inline static vector_t     &sort_asc(vector_t &v)                                    { std::sort(v.begin(), v.end(), std::less<float_t>()); return v; }
    inline static vector_t     &sort_desc(vector_t &v)                                   { std::sort(v.begin(), v.end(), std::greater<float_t>()); return v; }
    
    inline static float_t       sum(const vector_t &v)                                   { float_t s(0); for (const auto &f : v) s += f; return s; }
    inline static float_t       sum_abs(const vector_t &v)                               { float_t s(0); for (const auto &f : v) s += ::abs(f); return s; }
    inline static float_t       sum_sq(const vector_t &v)                                { float_t s(0); for (const auto &f : v) s += f * f; return s; }
    inline static float_t       sum_cu(const vector_t &v)                                { float_t s(0); for (const auto &f : v) s += ::abs(f * f * f); return s; }
    inline static vector_t     &abs(vector_t &a)                                         { for (auto &u : a) u = ::abs(u); return a; }
    
    inline static vector_t     &sub(vector_t &a, const vector_t &b)                      { for (size_t i = 0; i < N; i++) a[i] -= b[i]; return a; }
    inline static vector_t     &add(vector_t &a, const vector_t &b)                      { for (size_t i = 0; i < N; i++) a[i] += b[i]; return a; }
    inline static vector_t     &mul(vector_t &a, const vector_t &b)                      { for (size_t i = 0; i < N; i++) a[i] *= b[i]; return a; }
    inline static vector_t     &div(vector_t &a, const vector_t &b)                      { for (size_t i = 0; i < N; i++) a[i] /= b[i]; return a; }

    inline static vector_t     &sub(vector_t &a, float_t b)                              { for (auto &u : a) u -= b; return a; }
    inline static vector_t     &add(vector_t &a, float_t b)                              { for (auto &u : a) u += b; return a; }
    inline static vector_t     &mul(vector_t &a, float_t b)                              { for (auto &u : a) u *= b; return a; }
    inline static vector_t     &div(vector_t &a, float_t b)                              { for (auto &u : a) u /= b; return a; }
    
    inline static float_t       norm(const vector_t &v)                                  { return sqrt(sum_sq(v)); }
    inline static float_t       distance_sq(const vector_t &a, const vector_t &b)        { auto c(a); sub(c, b); return norm(c); }
    inline static float_t       distance(const vector_t &a, const vector_t &b)           { return sqrt(distance_sq(a, b)); }
    
    inline static size_t        index(const vector_t &a, float_t value)                  { return std::upper_bound(a.begin(), a.end(), value) - a.begin(); }
    
    inline static std::ostream &print(std::ostream &os, const vector_t &v)               { os << "[" << std::setprecision(6); for (auto u : v) os << u << " "; return os << "]"; }
    
    inline static std::ostream &print_info(std::ostream &os, const vector_t &v)
    {
        const char *delim = " ";
        float nrm = norm(v);
        os << "unnorm " << nrm << " | norm: ";
        auto u(v);
        mul(u, float_t(1) / nrm);
        //print(os, u);
        os << "[1] " << sum_abs(u) << delim;
//      os << "^2 " << norm(u) << delim;
        os << "[3] " << cbrt(sum_cu(u)) << delim;
        float_t sz(u.size());
        os << "middex "  << index(v, 1 / sqrt(sz));
        return os << std::endl;
    }
};

template <size_t N, typename FloatT = float>
void vector_test(std::ostream &os)
{
    typedef std::array<FloatT, N> vector_t;
    typedef vector_tests<N, FloatT> tests_t;

    vector_t a;
    tests_t::set(a, 0.0);
    tests_t::print_info(os << "0s:           ", a);

    tests_t::set(a, 1.0);
    tests_t::print_info(os << "1s:           ", a);
    
    tests_t::set(a, 0.0, N / 2);
    tests_t::print_info(os << "half 1/0:     ", a);
   // tests_t::print(os, a);
    
    tests_t::set_asc(a);
    tests_t::print_info(os << "asc:          ", a);
    
    tests_t::sort_desc(a);
    tests_t::print_info(os << "desc:         ", a);
    
    size_t kept = size_t(sqrt(N));

    while (kept) {
        tests_t::set_asc(a);
        tests_t::sort_asc(a);
        tests_t::set(a, 0.0, N - kept);
        tests_t::sort_desc(a);
        os << "spike/" << kept << ":";
        tests_t::print_info(os, a);
        kept >>= 1;
    }

    // need random
    // spike
    
    os << std::endl << std::endl;
}
