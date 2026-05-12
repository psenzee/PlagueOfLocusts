#pragma once

#include <iostream>
#include <limits>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstddef>
#include <queue>

#include "stl.h"
#include "rank_id.h"
#include "value_index_t.h"

template <size_t N, typename FloatT, typename SignedIndexT = int16_t>
struct rank
{
    typedef FloatT                          float_t;
    typedef SignedIndexT                    signed_index_t;
    typedef std::pair<FloatT, SignedIndexT> pair_t;
    typedef rank_id<SignedIndexT>           rank_id_t;
    typedef typename rank_id_t::exp_t       exp_t;
    typedef std::vector<pair_t>             keyed_vector_t;
    typedef std::vector<FloatT>             vector_t;
    
    enum { COUNT = N };

    std::array<rank_id_t, N> _data;

    inline rank() : _data({ 0 }) {}
    inline rank(const rank &u) : _data(u._data) {}
    inline rank(const keyed_vector_t &ranked)
    {
        auto b = ranked.begin();
        for (size_t i = 0, sz = std::min(ranked.size(), N); i != sz; i++, ++b)
            _data[i].set(_index(*b), _value(*b));
    }
    
    inline rank_id_t       &at(size_t index)               { return _data[index]; }
    inline const rank_id_t &at(size_t index)         const { return _data[index]; }
    inline const rank_id_t &operator[](size_t index) const { return at(index); }
    inline rank_id_t       &operator[](size_t index)       { return at(index); }
    
    inline rank            &add_offset(size_t index, SignedIndexT d, exp_t e = 0) { at(index).add_offset(d, e); return *this; }
    inline rank            &swap_ranks(size_t index_a, size_t index_b) { if (index_a != index_b) std::swap(_data[index_a], _data[index_b]); return *this; }
    inline rank            &operator=(const rank &u) { if (&u != this) _data = u._data; return *this; }
    
    inline static bool _does_swap_ranks()     { return true; } // a real hack
    inline static int  _has_offset()          { return 2; } // a real hack
    inline static int  _has_iterator_offset() { return 0; } // a real hack
    
    struct compare
    {
        inline bool operator()(const rank &a, const rank &b) const
        {
            typename rank_id_t::compare c;
            for (size_t i = 0; i < N; ++i) {
                const auto &ad = a._data[i], &bd = b._data[i];
                if (c(ad, bd)) return true;
                if (c(bd, ad)) return false;
            }
            return false;
        }
    };

    inline std::ostream &print(std::ostream &os) const { os << "[rank"; for (auto e : _data) os << " " << e; return os << "]"; }
    
    // this is basically our factory
    static rank create(const vector_t &v, keyed_vector_t &buffer)
    {
        if (buffer.size() != v.size()) buffer.resize(v.size());
        size_t i = 0;
        for (auto e : v) { buffer[i] = pair_t(e, i); i++; }
        // do a partial sort, descending by absolute value
        pair_compare_first_greater_abs_second_less<pair_t> cp;
        std::nth_element(buffer.begin(), buffer.begin() + N, buffer.end(), cp);
        std::sort(buffer.begin(), buffer.begin() + N, cp);
        return rank(buffer);
    }
};

template <size_t N, typename FloatT, typename SignedIndexT>
inline std::ostream &operator<<(std::ostream &os, const rank<N, FloatT, SignedIndexT> &value) { return value.print(os); }
