#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>
#include "stl.h"
#include "bfloat16.h"

struct val_index_8_8
{
    typedef val_index_8_8 self_t;

    int8_t  _index;
    uint8_t _value;
    
    inline self_t &set_value(float value) { int fr; frexp(value, &fr); _value = int8_t(fr); return *this; }
    inline self_t &set_index(int index)   { _index = int8_t(index); return *this; }
};

template <size_t N, typename IndexT, typename ValueT>
struct ranks_values
{
    std::array<IndexT, N> _indices;
    std::array<ValueT, N> _values;
};

//

template <typename FloatT, size_t N, typename IndexT = int16_t, size_t RanksN = 32, size_t KeyRanksN = 4>
struct key
{
    typedef FloatT                                    value_type;
    typedef key<FloatT, N, IndexT, RanksN, KeyRanksN> self_t;
    typedef std::array<IndexT, KeyRanksN>             key_ranks_t;
    typedef std::array<FloatT, N>                     vector_t;
    typedef std::pair<FloatT, IndexT>                 value_index_t;
    typedef std::array<value_index_t, N>              indexed_vector_t;

    uint16_t    base_vector_id;
    bfloat16    norm;
    uint16_t    slope_key;
    key_ranks_t ranks;

    inline key() : base_vector_id(0), norm(0), slope_key(0), ranks({ 0 }) {}
    inline key(const vector_t &v) : base_vector_id(0), norm(0), slope_key(0), ranks({ 0 }) { _set(v); }
    inline key(const std::vector<value_type> &v) : base_vector_id(0), norm(0), slope_key(0), ranks({ 0 })
        { vector_t u; std::copy(v.begin(), v.end(), u.begin()); _set(u); }
    
    inline value_type get_norm()             const { return bf2f(norm); }
    inline self_t    &set_norm(value_type v)       { norm = f2bf(v); return *this; }
    
    struct base_id_comparator {
        inline bool operator()(const self_t &a, const self_t &b) const
            { return a.base_vector_id < b.base_vector_id; }
    };

    struct norm_comparator {
        inline bool operator()(const self_t &a, const self_t &b) const
            { return bf2f(a.norm) < bf2f(b.norm); }
    };
    
    struct slope_comparator {
        inline bool operator()(const self_t &a, const self_t &b) const
            { return a.slope_key < b.slope_key; }
    };
    
    struct base_id_norm_comparator {
        inline bool operator()(const self_t &a, const self_t &b) const {
            if (a.base_vector_id == b.base_vector_id) return bf2f(a.norm) < bf2f(b.norm);
            return a.base_vector_id < b.base_vector_id;
        }
    };
    
    struct rank_comparator {
        uint16_t _rank;
        inline rank_comparator(uint16_t r) : _rank(r) {}
        inline bool operator()(const self_t &a, const self_t &b) const
            { return a.ranks[_rank] < b.ranks[_rank]; }
    };

    std::ostream &print(std::ostream &os) const
    {
        os << "[size " << sizeof(*this) << " norm " << std::setprecision(2) << bf2f(norm)
           << " slope " << slope_key << " r";
        for (size_t i = 0; i < ranks.size(); i++) os << " " << ranks[i];
        return os << "]";
    }

private:

    struct _rank_comparator {
        const FloatT _eps = 0.10; // if within .. of each other
        inline bool operator()(const value_index_t &a, const value_index_t &b) const {
            FloatT ab(abs(a.first)), aa(abs(b.first)), max(std::max(aa, ab)), d(ab - aa), v(d == 0 ? 0 : (d / max));
            return v > _eps ? true : (v < -_eps ? false : a.second > b.second);
        }
    };

    void _set(const vector_t &v)
    {
        // calculate ranks & norm
        indexed_vector_t u;
        value_type msq(0);
        for (int16_t i = 0; i != N; i++) {
            u[i] = value_index_t(v[i], int16_t(i));
            msq += v[i] * v[i];
        }
        float magnitude = sqrt(msq);
        norm = f2bf(magnitude);
        _rank_comparator c;
        auto b = u.begin(), mid = b + RanksN;
        std::nth_element(b, mid, u.end(), c);
        std::sort(b, mid, c);
        // calculate 4 bits of slope
        float sa = abs((*b).first), sb = abs((*mid).first);
        uint32_t slope((sa - sb) * 16.0 / magnitude);
        slope_key = slope;
        for (size_t i = 0; i != KeyRanksN; i++)
            ranks[i] = (u[i].first < 0 ? -1 : 1) * u[i].second;
    }
};

template <typename FloatT, size_t N, typename IndexT, size_t RanksN, size_t KeyRanksN>
inline std::ostream &operator<<(std::ostream &os, const key<FloatT, N, IndexT, RanksN, KeyRanksN> &key) { return key.print(os); }

template <typename FloatT, size_t N, typename IndexT = int16_t, size_t RanksN = 32, size_t KeyRanksN = 4>
struct key_id
{
    typedef FloatT                                       value_type;
    typedef key<FloatT, N, IndexT, RanksN, KeyRanksN>    key_t;
    typedef key_id<FloatT, N, IndexT, RanksN, KeyRanksN> self_t;
    typedef uint64_t                                     id_t;
    
    key_t  key;
    id_t   id;
    
    inline key_id(const key_t &key, id_t id) : key(key), id(id) {}
    inline key_id() : key(), id(0) {}
    
    struct sort_comparator {
        inline bool operator()(const self_t &a, const self_t &b) const {
            const key_t &ak = a.key, &bk = b.key;
            id_t aid = ak.base_vector_id, bid = bk.base_vector_id;
            if (aid != bid) return aid < bid;
            if (ak.norm != bk.norm) return bf2f(ak.norm) < bf2f(bk.norm);
            uint16_t ask = ak.slope_key, bsk = bk.slope_key;
            if (ask != bsk) return ask < bsk;
            for (size_t i = 0; i != KeyRanksN; i++) {
                IndexT ar = ak.ranks[i], br = bk.ranks[i];
                if (ar != br) return ar < br;
            }
            return a.id < b.id;
        }
    };
    
    struct base_id_comparator {
        inline bool operator()(const self_t &a, const self_t &b) const
            { return a.key.base_vector_id < b.key.base_vector_id; }
    };

    struct norm_comparator {
        inline bool operator()(const self_t &a, const self_t &b) const
            { return bf2f(a.key.norm) < bf2f(b.key.norm); }
    };
    
    struct slope_comparator {
        inline bool operator()(const self_t &a, const self_t &b) const
            { return a.key.slope_key < b.key.slope_key; }
    };
    
    struct base_id_norm_comparator {
        inline bool operator()(const self_t &a, const self_t &b) const {
            if (a.key.base_vector_id == b.key.base_vector_id) return bf2f(a.key.norm) < bf2f(b.key.norm);
            return a.key.base_vector_id < b.key.base_vector_id;
        }
    };
    
    struct rank_comparator {
        uint16_t _rank;
        inline rank_comparator(uint16_t r) : _rank(r) {}
        inline bool operator()(const self_t &a, const self_t &b) const
            { return a.key.ranks[_rank] < b.key.ranks[_rank]; }
    };
};

template <typename FloatT, size_t N, typename IndexT, size_t RanksN, size_t KeyRanksN>
inline std::ostream &operator<<(std::ostream &os, const key_id<FloatT, N, IndexT, RanksN, KeyRanksN> &key) { return os << key.key << ":" << key.id; }
