#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>
#include "stl.h"
#include "arrays.h"
#include "int_types.h"
#include "compile_time.h"

template <typename FloatT, size_t N, size_t KeyRanksN>
struct key
{
    static constexpr size_t COUNT      = N,
                            SQRT_COUNT = compile_time::isqrt(COUNT);
    
    typedef FloatT                                                 real_t;
    typedef typename int_for_max_value<N>::signed_t                index_t;
    typedef typename int_for_max_value<SQRT_COUNT + 1>::unsigned_t shape_t;
    typedef key<FloatT, N, KeyRanksN>                              self_t;
    typedef std::array<index_t, KeyRanksN>                         key_ranks_t;
    typedef std::array<FloatT, KeyRanksN>                          key_values_t;
    typedef std::array<FloatT, N>                                  vector_t;
    typedef std::array<uint8_t, KeyRanksN>                         order_t;
    typedef std::vector<FloatT>                                    dynamic_t;
    typedef std::pair<FloatT, index_t>                             value_index_t;
    typedef std::array<value_index_t, N>                           indexed_vector_t;

    int8_t       exp_norm;
    shape_t      shape;
    real_t       norm;
    key_ranks_t  ranks;
    key_values_t values;

    inline key()                   : exp_norm(0), ranks({ 0 }), values({ 0 }), norm(0), shape(~shape_t(0)) {}
    inline key(const vector_t &v)  : exp_norm(0), ranks({ 0 }), values({ 0 }), norm(0), shape(~shape_t(0)) { _set(v); }
    inline key(const dynamic_t &v) : exp_norm(0), ranks({ 0 }), values({ 0 }), norm(0), shape(~shape_t(0)) { _set(v); }

    inline static int compare__exp_norm(const self_t &a, const self_t &b)            { return _compare_signed(a.exp_norm, b.exp_norm); }
    inline static int compare__norm(const self_t &a, const self_t &b)                { return _compare_signed(a.norm, b.norm); }
    inline static int compare__rank(size_t r, const self_t &a, const self_t &b)      { return _compare_signed(a.ranks[r], b.ranks[r]); }
    inline static int compare__value(size_t r, const self_t &a, const self_t &b)     { return _compare_signed(a.values[r], b.values[r]); }
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

    inline void _set(const dynamic_t &v) { vector_t u; std::copy(v.begin(), v.end(), u.begin()); _set(u); }

    void _set(const vector_t &v)
    {
        // calculate norm
        indexed_vector_t u;
        real_t msq(0);
        for (size_t i = 0; i != N; i++) {
            u[i] = value_index_t(v[i], index_t(i));
            msq += v[i] * v[i];
        }
        norm = sqrt(msq);
        
        // calculate shape ( uint(floor(sum(abs(v) / norm))) )
        shape = 0;
        if (norm != real_t(0))
            shape = static_cast<shape_t>(arrays::sum_normalized_abs(v) + 0.5);

        // calculate exp_norm
        int xp = 0;
        frexp(norm, &xp);
        exp_norm = xp;

        // calculate ranks
        _rank_order_comparer c;
        auto b = u.begin(), mid = b + KeyRanksN;
        std::nth_element(b, mid, u.end(), c);
        std::sort(b, mid, c);
        for (size_t i = 0; i != KeyRanksN; i++)
            ranks[i] = _int_sign_0_is_pos((values[i] = u[i].first)) * u[i].second;
    }
};

template <typename FloatT, size_t N, size_t KeyRanksN>
inline std::ostream &operator<<(std::ostream &os, const key<FloatT, N, KeyRanksN> &key) { return key.print(os); }
