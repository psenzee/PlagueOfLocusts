#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>
#include "stl.h"

static constexpr size_t _lg(size_t n) { return n <= 1 ? 0 : (1 + _lg(n >> 1)); }

template <typename T> inline int _sq(T val)                         { return val * val; }
template <typename T> inline int _sign(T val)                       { return (T(0) < val) - (val < T(0)); }
template <typename T> inline int _compare(T a, T b)                 { return _sign(a - b); }
template <typename T> inline int _compare_uint(T a, T b)            { return _sign(int(a) - int(b)); }
template <typename T> inline int _compare_exp(T a, T b, int o = 0)  { int xa, xb; frexp(a, &xa); frexp(b, &xb); return _sign(xa - xb + o); }

typedef std::array<size_t, 4> partition_set_t;

template <size_t N, size_t KeyRanksN>
inline uint8_t get_type_from_partition(const partition_set_t &partitions)
{
    return 1;
    size_t sz = partitions.size();
    if (partitions[0] == 0)
        return 0;
    for (size_t i = sz; i != 0; --i)
        if (partitions[i - 1] <= KeyRanksN + 1)
            return ((sz - (i - 1)) >> 2) + 1;
    return _lg(partitions[0]) + sz;
}

template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN>
struct key
{
    typedef FloatT                                                 real_t;
    typedef key<FloatT, N, IndexT, KeyRanksN>                      self_t;
    typedef std::array<IndexT, KeyRanksN>                          key_ranks_t;
    typedef std::array<FloatT, KeyRanksN>                          key_values_t;
    typedef std::array<FloatT, N>                                  vector_t;
    typedef std::pair<FloatT, IndexT>                              value_index_t;
    typedef std::array<value_index_t, N>                           indexed_vector_t;

    uint16_t     base_id;
    int8_t       exp_norm;
    uint8_t      type;
    real_t       norm;
    key_ranks_t  ranks;
    key_values_t values;

    inline key(uint16_t base_id = 0) : base_id(base_id), norm(0), exp_norm(0), ranks({ 0 }), values({ 0 }) {}
    inline key(const vector_t &v, uint16_t base_id = 0) : base_id(base_id), norm(0), exp_norm(0), ranks({ 0 }), values({ 0 }) { _set(v); }
    inline key(const std::vector<real_t> &v, uint16_t base_id = 0) : base_id(base_id), norm(0), exp_norm(0), ranks({ 0 }), values({ 0 })
        { vector_t u; std::copy(v.begin(), v.end(), u.begin()); _set(u); }

    inline static int compare__type(const self_t &a, const self_t &b)                  { return _compare_uint(a.type, b.type); }
    inline static int compare__base_id(const self_t &a, const self_t &b)               { return _compare_uint(a.base_id, b.base_id); }
    inline static int compare__exp_norm(const self_t &a, const self_t &b)              { return _compare(a.exp_norm, b.exp_norm); }
    inline static int compare__norm(const self_t &a, const self_t &b)                  { return _compare(a.norm, b.norm); }
    inline static int compare__rank(size_t r, const self_t &a, const self_t &b)        { return _compare(a.ranks[r], b.ranks[r]); }
    inline static int compare__value(size_t r, const self_t &a, const self_t &b)       { return _compare(a.values[r], b.values[r]); }
    inline static int compare__exp_value(size_t r, const self_t &a, const self_t &b)   { return _compare_exp(a.values[r], b.values[r]); }
    inline static int compare__ranks(const self_t &a, const self_t &b)
    {
        int r = 0;
        for (size_t i = 0; i != KeyRanksN; i++)
            if ((r = compare__rank(i, a, b)) != 0)
                return r;
        return 0;
    }
    inline static int compare__type_exp_norm(const self_t &a, const self_t &b)         { int s = compare__type(a, b);    return s ? s : compare__exp_norm(a, b); }
    inline static int compare__base_id_type_exp_norm(const self_t &a, const self_t &b) { int s = compare__base_id(a, b); return s ? s : compare__type_exp_norm(a, b); }
    inline static int compare__initial(const self_t &a, const self_t &b)               { return compare__base_id_type_exp_norm(a, b); }

    inline static int compare(const self_t &a, const self_t &b)
    {
        int r = compare__initial(a, b), rs = 0, rev = 0;
        return r ? r : (((rs = compare__ranks(a, b)) != 0) ? rs : ((rev = compare__exp_value(0, a, b)) != 0 ? rev : compare__norm(a, b)));
    }

    inline void swap_rank(size_t ra, size_t rb) { if (ra != rb) std::swap(ranks[ra], ranks[rb]); }
    
    std::ostream &print(std::ostream &os) const
    {
        os << "[size " << sizeof(*this) << " type " << size_t(type) << " norm " << std::setprecision(2) << norm << " r";
        for (size_t i = 0; i < ranks.size(); i++) os << " " << int(ranks[i]);
        return os << "]";
    }

private:

    struct _rank_order_comparer {
        inline bool operator()(const value_index_t &a, const value_index_t &b) const {
            FloatT aa(abs(a.first)), ab(abs(b.first));
            return aa != ab ? aa > ab : a.second < b.second;
        }
    };
    
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
        real_t inv_norm = real_t(1) / norm;

        // calculate exp_norm
        int xp = 0;
        frexp(norm, &xp);
        exp_norm = xp;

        // calculate ranks
        _rank_order_comparer c;
        size_t half = N / 2; // this is RanksN for the old way -- could be sqrt(N) instead; could also be KeyRanksN for faster if we don't need type/partiion info
        auto b = u.begin(), mid = b + half;
        std::nth_element(b, mid, u.end(), c);
        std::sort(b, mid, c);
        for (size_t i = 0; i != KeyRanksN; i++) {
            ranks[i] = (u[i].first < 0 ? -1 : 1) * u[i].second;
            values[i] = u[i].first/* * inv_norm*/;
        }

        // calculate type
        real_t sum_sq(0);
        std::array<real_t, 4> thresholds({ 0.5, 0.75, 0.90, 0.98 });
        partition_set_t partitions({ 0 });
        size_t k = 0, ksz = thresholds.size();
        for (size_t i = 0; i < half && u[i].first != 0 && k < ksz; ++i) {
            sum_sq += u[i].first * u[i].first;
            real_t sum = sqrt(sum_sq);
            if (sum >= norm * thresholds[k])
                partitions[k++] = i + 1;
        }
        type = get_type_from_partition<N, KeyRanksN>(partitions);
        //if (k) std::cout << ">>> PARTITIONS";
        //for (size_t i = 0; i != k; ++i) std::cout << " " << partitions[i];
        //if (k) std::cout << std::endl;
    }
};

template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN>
inline std::ostream &operator<<(std::ostream &os, const key<FloatT, N, IndexT, KeyRanksN> &key) { return key.print(os); }

/// -----------

template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN, typename IdT = uint64_t>
struct key_id
{
    typedef FloatT                               real_t;
    typedef key<FloatT, N, IndexT, KeyRanksN>    key_t;
    typedef key_id<FloatT, N, IndexT, KeyRanksN> self_t;
    typedef IdT                                  id_t;

    key_t  key;
    id_t   id;

    inline key_id(const key_t &key, id_t id) : key(key), id(id) {}
    inline key_id() : key(), id(0) {}

    struct sort_comparer     { inline bool operator()(const self_t &a, const self_t &b) const { int r = key_t::compare(a.key, b.key); return r ? r < 0 : a.id < b.id; } };
    struct exp_norm_comparer { inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__exp_norm(a.key, b.key) < 0; } };
    struct norm_comparer     { inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__norm(a.key, b.key) < 0; } };
    struct base_id_comparer  { inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__base_id(a.key, b.key) < 0; } };
    struct initial_comparer  { inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__initial(a.key, b.key) < 0; } };
    
    struct rank_comparer {
        size_t _rank;
        inline rank_comparer(size_t r) : _rank(r) {}
        inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__rank(_rank, a.key, b.key) < 0; }
    };
    
    struct value_comparer {
        size_t _rank;
        inline value_comparer(size_t r) : _rank(r) {}
        inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__value(_rank, a.key, b.key) < 0; }
    };
    
    struct exp_value_comparer {
        size_t _rank;
        inline exp_value_comparer(size_t r) : _rank(r) {}
        inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__exp_value(_rank, a.key, b.key) < 0; }
    };
};

template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN>
inline std::ostream &operator<<(std::ostream &os, const key_id<FloatT, N, IndexT, KeyRanksN> &key) { return os << key.key << ":" << key.id; }
