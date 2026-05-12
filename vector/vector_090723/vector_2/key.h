#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>
#include "stl.h"
#include "bfloat16.h"
#include "int_t_for_max.h"

//static constexpr size_t _lg(size_t n) { return n <= 1 ? 0 : (1 + _lg(n >> 1)); }

template <typename T> inline uint8_t _m7(T u)                           { return f2bf(float(u)) & 0x07f; }
template <typename T> inline uint8_t _m4(T u)                           { return (f2bf(float(u)) & 0x7f) >> 3; }
template <typename T> inline T       _sq(T u)                           { return u * u; }
template <typename T> inline int     _sign(T u)                         { return (T(0) < u) - (u < T(0)); }
template <typename T> inline int     _compare(T a, T b)                 { return _sign(a - b); }
template <typename T> inline int     _compare_uint(T a, T b)            { return _sign(int(a) - int(b)); }
template <typename T> inline int     _compare_exp(T a, T b, int o = 0)  { int xa, xb; frexp(a, &xa); frexp(b, &xb); return _sign(xa - xb + o); }

template <typename FloatT, size_t N, size_t KeyRanksN>
struct key
{
    typedef FloatT                                                 real_t;
    typedef typename int_t_for_max<N>::int_t                       index_t;
    typedef key<FloatT, N, KeyRanksN>                              self_t;
    typedef std::array<index_t, KeyRanksN>                         key_ranks_t;
    typedef std::array<FloatT, KeyRanksN>                          key_values_t;
    typedef std::array<FloatT, N>                                  vector_t;
    typedef std::array<uint8_t, KeyRanksN>                         order_t;
    typedef std::vector<FloatT>                                    dynamic_t;
    typedef std::pair<FloatT, index_t>                             value_index_t;
    typedef std::array<value_index_t, N>                           indexed_vector_t;

    int8_t       exp_norm;
    real_t       norm;
    key_ranks_t  ranks;
    key_values_t values;

    inline key()                   : exp_norm(0), ranks({ 0 }), values({ 0 }), norm(0) {}
    inline key(const vector_t &v)  : exp_norm(0), ranks({ 0 }), values({ 0 }), norm(0) { _set(v); }
    inline key(const dynamic_t &v) : exp_norm(0), ranks({ 0 }), values({ 0 }), norm(0) { _set(v); }

    inline static int compare__exp_norm(const self_t &a, const self_t &b)            { return _compare(a.exp_norm, b.exp_norm); }
    inline static int compare__norm(const self_t &a, const self_t &b)                { return _compare(a.norm, b.norm); }
    inline static int compare__rank(size_t r, const self_t &a, const self_t &b)      { return _compare(a.ranks[r], b.ranks[r]); }
    inline static int compare__value(size_t r, const self_t &a, const self_t &b)     { return _compare(a.values[r], b.values[r]); }
    inline static int compare__exp_value(size_t r, const self_t &a, const self_t &b) { return _compare_exp(a.values[r], b.values[r]); }
    inline static int compare__initial(const self_t &a, const self_t &b)             { return compare__exp_norm(a, b); }
    
    inline static int compare__ranks(const self_t &a, const self_t &b)
    {
        int r(0);
        for (size_t i = 0; i != KeyRanksN; i++)
            if ((r = compare__rank(i, a, b)) != 0)
                return r;
        return 0;
    }

    inline static int compare(const self_t &a, const self_t &b)
    {
        int r = compare__initial(a, b), rs = 0;
        return r ? r : (((rs = compare__ranks(a, b)) != 0) ? rs : compare__norm(a, b));
    }

    inline void swap_rank(size_t ra, size_t rb) { if (ra != rb) { std::swap(ranks[ra], ranks[rb]); std::swap(values[ra], values[rb]); } }

    std::ostream &print(std::ostream &os) const
    {
        os << "[size " << sizeof(*this) << " norm " << std::setprecision(2) << norm << " r";
        for (size_t i = 0; i < ranks.size(); i++) os << " " << int(ranks[i]) << ":" << values[i];
        return os << " most/least " << abs(values[0] / values[KeyRanksN - 1]) << "]\t";
    }

private:

    struct _rank_order_comparer {
        inline bool operator()(const value_index_t &a, const value_index_t &b) const {
            FloatT aa(abs(a.first)), ab(abs(b.first));
            return aa != ab ? aa > ab : a.second < b.second;
        }
    };
    
    inline static int _int_sign(real_t v) { return (fpclassify(v) == FP_ZERO || !signbit(v)) ? 1 : -1;  }

    void _set(const dynamic_t &v) { vector_t u; std::copy(v.begin(), v.end(), u.begin()); _set(u); }

    void _set(const vector_t &v)
    {
        // calculate norm
        indexed_vector_t u;
        real_t msq(0);
        for (size_t i = 0; i != N; i++) {
            u[i] = value_index_t(v[i], int16_t(i));
            msq += v[i] * v[i];
        }
        norm = sqrt(msq);

        // calculate exp_norm
        int xp = 0;
        frexp(norm, &xp);
        exp_norm = xp;

        // calculate ranks
        _rank_order_comparer c;
        auto b = u.begin(), mid = b + KeyRanksN;
        std::nth_element(b, mid, u.end(), c);
        std::sort(b, mid, c);
        for (size_t i = 0; i != KeyRanksN; i++) {
            real_t v = u[i].first;
            ranks[i] = _int_sign(v) * u[i].second;
            values[i] = v;
        }
    }
};

template <typename FloatT, size_t N, size_t KeyRanksN>
inline std::ostream &operator<<(std::ostream &os, const key<FloatT, N, KeyRanksN> &key) { return key.print(os); }
