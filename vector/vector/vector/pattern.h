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

template <size_t N, typename FloatT, typename SignedIndexT = int16_t>
struct pattern
{
    typedef FloatT                          float_t;
    typedef SignedIndexT                    signed_index_t;
    typedef std::pair<FloatT, SignedIndexT> pair_t;
    typedef std::vector<pair_t>             keyed_vector_t;
    typedef std::vector<FloatT>             vector_t;
    
    enum { COUNT = N };

    std::array<SignedIndexT, N> _data;

    inline pattern() : _data({ 0 }) {}
    inline pattern(const pattern &u) : _data(u._data) {}
    inline pattern(const keyed_vector_t &value)
    {
        auto b = value.begin();
        for (size_t i = 0, sz = std::min(value.size(), N); i != sz; i++, ++b)
            _data[i] = _index(*b);
    }
    
    inline pattern            &add_offset(size_t index, SignedIndexT d, SignedIndexT e = 0) { return *this; }
    inline pattern            &swap_ranks(size_t index_a, size_t index_b) { return *this; }
    
    inline signed_index_t       &at(size_t index)               { return _data[index]; }
    inline const signed_index_t &at(size_t index)         const { return _data[index]; }
    inline const signed_index_t &operator[](size_t index) const { return at(index); }
    inline signed_index_t       &operator[](size_t index)       { return at(index); }

    inline pattern              &operator=(const pattern &u) { if (&u != this) _data = u._data; return *this; }
    
    inline static bool _does_swap_ranks()     { return false; } // a real hack
    inline static int  _has_offset()          { return 0; } // a real hack
    inline static int  _has_iterator_offset() { return 150; } // a real hack
    
    struct compare
    {
        inline bool operator()(const pattern &a, const pattern &b) const
        {
            for (size_t i = 0; i < N; ++i) {
                SignedIndexT ad = a._data[i], bd = b._data[i];
                if (ad < bd) return true; else if (ad > bd) return false;
            }
            return false;
        }
    };

    inline std::ostream &print(std::ostream &os) const
    {
        os << "[pattern" << std::hex;
        for (auto e : _data) os << " " << std::setw(4) << std::setfill('0') << e;
        return os << "]" << std::dec;
    }
    
    // this is basically our factory
    static pattern create(const vector_t &v, keyed_vector_t &buffer)
    {
        if (buffer.size() != v.size()) buffer.resize(v.size());
        size_t i = 0;
        for (auto e : v) { buffer[i] = pair_t(e, i); i++; }
        // do a partial sort, descending by absolute value
        std::nth_element(buffer.begin(), buffer.begin() + N, buffer.end(), compare_0ad_1u<pair_t>());
        std::sort(buffer.begin(), buffer.begin() + N, compare_1u<pair_t>());
        return pattern(buffer);
    }
};

template <size_t N, typename FloatT, typename SignedIndexT>
inline std::ostream &operator<<(std::ostream &os, const pattern<N, FloatT, SignedIndexT> &value) { return value.print(os); }
