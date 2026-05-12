#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>
#include "stl.h"
#include "expand_bitset.h"

template <typename T> inline int _sign(T val)               { return (T(0) < val) - (val < T(0)); }
template <typename T> inline int _compare(T a, T b)         { return _sign(a - b); }
template <typename T> inline int _compare_uint(T a, T b)    { return _sign(int(a) - int(b)); }
template <typename T> inline int _compare_exp(T a, T b)     { int xa, xb; frexp(a, &xa); frexp(b, &xb); return _sign(xa - xb); }

template <typename FloatT, size_t N, typename IndexT, size_t RanksN, size_t KeyRanksN>
struct key_
{
    typedef FloatT                                     real_t;
    typedef key_<FloatT, N, IndexT, RanksN, KeyRanksN> self_t;
    typedef std::array<IndexT, KeyRanksN>              key_ranks_t;
    typedef std::array<FloatT, KeyRanksN>              key_values_t;
    typedef std::array<FloatT, N>                      vector_t;
    typedef std::pair<FloatT, IndexT>                  value_index_t;
    typedef std::array<value_index_t, N>               indexed_vector_t;

    real_t       norm;
    uint16_t     base_id;
    key_ranks_t  ranks;
    //key_values_t values;

    inline key_() : base_id(0), norm(0), ranks({ 0 }) /*, values({ 0.0 })*/ {}
    inline key_(const vector_t &v) : base_id(0), norm(0), ranks({ 0 }) /*, values({ 0.0 })*/ { _set(v); }
    inline key_(const std::vector<real_t> &v) : base_id(0), norm(0), ranks({ 0 }) /*, values({ 0.0 })*/ { vector_t u; std::copy(v.begin(), v.end(), u.begin()); _set(u); }

    inline static int compare__base_id(const self_t &a, const self_t &b)                  { return _compare_uint(a.base_id, b.base_id); }
    inline static int compare__exp_norm(const self_t &a, const self_t &b)                 { return _compare_exp(a.norm, b.norm); }
    inline static int compare__norm(const self_t &a, const self_t &b)                     { return _compare(a.norm, b.norm); }
    inline static int compare__base_id_exp_norm(const self_t &a, const self_t &b)         { int s = _compare_uint(a.base_id, b.base_id); return s ? s : _compare_exp(a.norm, b.norm); }

    inline static int compare__rank(size_t r, const self_t &a, const self_t &b)           { return _compare(a.ranks[r], b.ranks[r]); }
    //inline static int compare__exp_value(size_t r, const self_t &a, const self_t &b)      { return _compare_exp(a.values[r], b.values[r]); }
    //inline static int compare__rank_exp_value(size_t r, const self_t &a, const self_t &b) { int s = compare__rank(r, a, b); return s ? s : compare__exp_value(r, a, b); }
    //inline static int compare__value(size_t r, const self_t &a, const self_t &b)          { return _compare(a.values[r], b.values[r]); }

    inline static int compare(const self_t &a, const self_t &b)
    {
        int r = compare__base_id_exp_norm(a, b);
        if (r) return r;
        //for (size_t i = 0; i != KeyRanksN; i++) if ((r = compare__rank_exp_value(i, a, b)) != 0) return r;
        for (size_t i = 0; i != KeyRanksN; i++) if ((r = compare__rank(i, a, b)) != 0) return r;
        return compare__norm(a, b);
    }

    inline void swap_rank(size_t ra, size_t rb)
    {
        if (ra != rb) {
            std::swap(ranks[ra], ranks[rb]);
            //std::swap(values[ra], values[rb]);
        }
    }
    
    std::ostream &print(std::ostream &os) const
    {
        os << "[size " << sizeof(*this) << " so(index) " << sizeof(IndexT) << " norm " << std::setprecision(2) << norm << " r";
        for (size_t i = 0; i < ranks.size(); i++) os << " " << int(ranks[i]);
        return os << "]";
    }

private:

    struct _rank_order_comparer {
        const FloatT _eps = FloatT(0.10); // if within .. of each other
        inline bool operator()(const value_index_t &a, const value_index_t &b) const {
            FloatT aa(abs(a.first)), ab(abs(b.first));
            return aa != ab ? aa > ab : a.second < b.second;
            //FloatT ab(abs(a.first)), aa(abs(b.first)), max(std::max(aa, ab)), d(ab - aa), v(d == 0 ? 0 : (d / max));
            //return v > _eps ? true : (v < -_eps ? false : a.second > b.second);
        }
    };

    void _set(const vector_t &v)
    {
        // calculate ranks & norm
        indexed_vector_t u;
        real_t msq(0);
        for (size_t i = 0; i != N; i++) {
            u[i] = value_index_t(v[i], int16_t(i));
            msq += v[i] * v[i];
        }
        norm = sqrt(msq);
        _rank_order_comparer c;
        auto b = u.begin(), mid = b + RanksN;
        std::nth_element(b, mid, u.end(), c);
        std::sort(b, mid, c);
        for (size_t i = 0; i != KeyRanksN; i++)  {
            ranks[i] = (u[i].first < 0 ? -1 : 1) * u[i].second;
            //values[i] = u[i].first / norm;
        }
        //expand_bitset<N, KeyRanksN, IndexT> eb(ranks);
    }
};

template <typename FloatT, size_t N, typename IndexT, size_t RanksN, size_t KeyRanksN>
inline std::ostream &operator<<(std::ostream &os, const key_<FloatT, N, IndexT, RanksN, KeyRanksN> &key) { return key.print(os); }

template <typename FloatT, size_t N, typename IndexT, size_t RanksN, size_t KeyRanksN>
struct key_id_
{
    typedef FloatT                                        real_t;
    typedef key_<FloatT, N, IndexT, RanksN, KeyRanksN>    key_t;
    typedef key_id_<FloatT, N, IndexT, RanksN, KeyRanksN> self_t;
    typedef uint64_t                                      id_t;

    key_t  key;
    id_t   id;

    inline key_id_(const key_t &key, id_t id) : key(key), id(id) {}
    inline key_id_() : key(), id(0) {}

    struct sort_comparer             { inline bool operator()(const self_t &a, const self_t &b) const { int r = key_t::compare(a.key, b.key); return r ? r < 0 : a.id < b.id; } };

    struct base_id_comparer          { inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__base_id(a.key, b.key) < 0; } };
    struct exp_norm_comparer         { inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__exp_norm(a.key, b.key) < 0; } };

    struct base_id_exp_norm_comparer { inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__base_id_exp_norm(a.key, b.key) < 0; } };
    
    struct rank_comparer {
        size_t _rank;
        inline rank_comparer(size_t r) : _rank(r) {}
        inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__rank(_rank, a.key, b.key) < 0; }
    };
    
    struct exp_value_comparer {
        size_t _rank;
        inline exp_value_comparer(size_t r) : _rank(r) {}
        inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__exp_value(_rank, a.key, b.key) < 0; }
    };
};

template <typename FloatT, size_t N, typename IndexT, size_t RanksN, size_t KeyRanksN>
inline std::ostream &operator<<(std::ostream &os, const key_id_<FloatT, N, IndexT, RanksN, KeyRanksN> &key) { return os << key.key << ":" << key.id; }
