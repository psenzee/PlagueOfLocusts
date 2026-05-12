#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>
#include "stl.h"
#include "bfloat16.h"

static constexpr size_t _lg(size_t n) { return n <= 1 ? 0 : (1 + _lg(n >> 1)); }

template <typename T> inline uint8_t _m7(T u)                           { return f2bf(float(u)) & 0x07f; }
template <typename T> inline uint8_t _m4(T u)                           { return (f2bf(float(u)) & 0x7f) >> 3; }
template <typename T> inline T       _sq(T u)                           { return u * u; }
template <typename T> inline int     _sign(T u)                         { return (T(0) < u) - (u < T(0)); }
template <typename T> inline int     _compare(T a, T b)                 { return _sign(a - b); }
template <typename T> inline int     _compare_uint(T a, T b)            { return _sign(int(a) - int(b)); }
template <typename T> inline int     _compare_exp(T a, T b, int o = 0)  { int xa, xb; frexp(a, &xa); frexp(b, &xb); return _sign(xa - xb + o); }

template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN>
struct key
{
    typedef FloatT                                                 real_t;
    typedef key<FloatT, N, IndexT, KeyRanksN>                      self_t;
    typedef std::array<IndexT, KeyRanksN>                          key_ranks_t;
    typedef std::array<FloatT, KeyRanksN>                          key_values_t;
    typedef std::array<FloatT, N>                                  vector_t;
    typedef std::array<uint8_t, KeyRanksN>                         order_t;
    typedef std::vector<FloatT>                                    dynamic_t;
    typedef std::pair<FloatT, IndexT>                              value_index_t;
    typedef std::array<value_index_t, N>                           indexed_vector_t;

    uint16_t     base_id;
    int8_t       exp_norm;
    uint8_t      _spacer;
    real_t       norm;
    key_ranks_t  ranks;
    key_values_t values;
    order_t      abs_order;

    inline key(uint16_t base_id = 0) : base_id(base_id), norm(0), exp_norm(0), ranks({ 0 }), values({ 0 }) {}
    inline key(const vector_t &v, uint16_t base_id = 0)  : base_id(base_id), norm(0), exp_norm(0), ranks({ 0 }), values({ 0 }) { _set(v); }
    inline key(const dynamic_t &v, uint16_t base_id = 0) : base_id(base_id), norm(0), exp_norm(0), ranks({ 0 }), values({ 0 }) { _set(v); }

    inline static int compare__base_id(const self_t &a, const self_t &b)             { return _compare_uint(a.base_id, b.base_id); }
    inline static int compare__exp_norm(const self_t &a, const self_t &b)            { return _compare(a.exp_norm, b.exp_norm); }
    inline static int compare__norm(const self_t &a, const self_t &b)                { return _compare(a.norm, b.norm); }
    inline static int compare__rank(size_t r, const self_t &a, const self_t &b)      { return _compare(a.ranks[r], b.ranks[r]); }
    inline static int compare__value(size_t r, const self_t &a, const self_t &b)     { return _compare(a.values[r], b.values[r]); }
    inline static int compare__exp_value(size_t r, const self_t &a, const self_t &b) { return _compare_exp(a.values[r], b.values[r]); }
    inline static int compare__base_id_exp_norm(const self_t &a, const self_t &b)    { int s = compare__base_id(a, b); return s ? s : compare__exp_norm(a, b); }
    inline static int compare__initial(const self_t &a, const self_t &b)             { return compare__base_id_exp_norm(a, b); }
    
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
        int r = compare__initial(a, b), rs = 0, rev = 0;
        return r ? r : (((rs = compare__ranks(a, b)) != 0) ? rs : ((/*rev = compare__exp_value(0, a, b)*/0) != 0 ? rev : compare__norm(a, b)));
    }

    inline void swap_rank(size_t ra, size_t rb) { if (ra != rb) { std::swap(ranks[ra], ranks[rb]); std::swap(values[ra], values[rb]); } }
    
    inline real_t values_min_abs() const { real_t min(0); for (const auto v : values) min = (abs(v) < abs(min)) ? v : min; return min; }

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
            ranks[i] = ((fpclassify(v) == FP_ZERO || !signbit(v)) ? 1 : -1) * u[i].second;
            //ranks[i] = (u[i].first < 0 ? -1 : 1) * u[i].second;
            values[i] = v;
        }
        
        // calculate order
        typedef std::pair<IndexT, uint8_t> pair_ri_t;
        
        std::array<pair_ri_t, KeyRanksN> order;
        for (size_t i = 0; i != KeyRanksN; i++)
            order[i] = pair_ri_t(ranks[i], i);
        pair_compare_first_less_abs<pair_ri_t> pc;
        std::sort(order.begin(), order.end(), pc);
        for (size_t i = 0; i != KeyRanksN; i++)
            abs_order[order[i].second] = i;
    }
};

template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN>
inline std::ostream &operator<<(std::ostream &os, const key<FloatT, N, IndexT, KeyRanksN> &key) { return key.print(os); }

/// -----------

template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN, typename IdT = uint32_t>
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

    struct sort_comparer       { inline bool operator()(const self_t &a, const self_t &b) const { int r = key_t::compare(a.key, b.key); return r ? r < 0 : a.id < b.id; } };
    struct exp_norm_comparer   { inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__exp_norm(a.key, b.key) < 0; } };
    struct norm_comparer       { inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__norm(a.key, b.key) < 0; } };
    struct base_id_comparer    { inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__base_id(a.key, b.key) < 0; } };
    struct initial_comparer    { inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__initial(a.key, b.key) < 0; } };
    
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

template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN, typename IdT>
inline std::ostream &operator<<(std::ostream &os, const key_id<FloatT, N, IndexT, KeyRanksN, IdT> &key) { return os << key.key << ":" << key.id; }


template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN, typename IdT = uint32_t>
struct key_abbrev
{
    typedef FloatT                                                 real_t;
    typedef IdT                                                    id_t;
    typedef key_abbrev<FloatT, N, IndexT, KeyRanksN>               self_t;
    typedef key<FloatT, N, IndexT, KeyRanksN>                      key_t;
    typedef key_id<FloatT, N, IndexT, KeyRanksN, IdT>              key_id_t;
    typedef std::array<int8_t, KeyRanksN>                          key_ranks8_t;

    real_t       norm;
    key_ranks8_t ranks;
    id_t         id;

    inline key_abbrev(const key_t &key, id_t id) : norm(key.norm), id(id) { _set(key); }
    inline key_abbrev(const key_id_t &key_id) : norm(key_id.key.norm), id(key_id.id) { _set(key_id.key); }

    inline static int compare(const self_t &a, const self_t &b)
    {
        int r = compare__norm(a, b);
        return r ? r : compare__ranks(a, b);
    }
    
    inline bool intersects(const std::array<int8_t, KeyRanksN> &a) const
    {
        for (auto u : a) {
            for (auto v : ranks) {
                if (v == u) return true;
                if (v > u) break; // this must be true because 'ranks' is sorted
            }
        }
        return false;
    }
    
    inline bool intersects_sorted(const std::array<int8_t, KeyRanksN> &sorted) const
    {
        return has_intersect_sorted(ranks, sorted);
    }

    std::ostream &print(std::ostream &os) const
    {
        os << "[key_abbrev size " << sizeof(*this) << " norm " << std::setprecision(2) << norm << " r";
        for (size_t i = 0; i < ranks.size(); i++) os << " " << int(ranks[i]);
        return os << "]\t";
    }

private:

    inline static int compare__norm(const self_t &a, const self_t &b)                { return _compare(a.norm, b.norm); }
    inline static int compare__rank(size_t r, const self_t &a, const self_t &b)      { return _compare(a.ranks[r], b.ranks[r]); }
    inline static int compare__ranks(const self_t &a, const self_t &b)
    {
        int r(0);
        for (size_t i = 0; i != KeyRanksN; i++)
            if ((r = compare__rank(i, a, b)) != 0)
                return r;
        return 0;
    }
    
    void _set(const key_t &key)
    {
        for (size_t i = 0, sz = ranks.size(); i < sz; i++) {
            auto r = key.ranks[i];
            ranks[i] = int8_t(abs(r) & 0x7f) * (r < 0 ? int8_t(-1) : int8_t(1));
        }
        std::sort(ranks.begin(), ranks.end());
    }
};

template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN, typename IdT>
inline std::ostream &operator<<(std::ostream &os, const key_abbrev<FloatT, N, IndexT, KeyRanksN, IdT> &key) { return os << key << ":" << key.id; }
