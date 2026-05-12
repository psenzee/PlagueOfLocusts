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
#include <thread>
#include <deque>

#include "stl.h"
#include "simple_timer.h"
#include "ranked_vector.h"
#include "ranked_vector256.h"

template <int M, typename FloatT>
struct ranked_vectors
{
    typedef FloatT                                    value_type;
    typedef ranked_vectors<M, FloatT>                 self_t;
    typedef std::pair<value_type, size_t>             value_index_t;
    typedef ranked_vector<M>                          ranked_t;
    typedef std::array<FloatT, M>                     flat_vector_t;
    typedef std::vector<ranked_t>                     vectors_t;
    typedef std::vector<uint64_t>                     indices_t;
    typedef typename ranked_t::item_t                 item_t;
    typedef typename vectors_t::const_iterator        const_iterator;
    typedef std::pair<const_iterator, const_iterator> const_range_t;
    
    static constexpr int COUNT = M;

    vectors_t _ranked;
    indices_t _indices;

    inline self_t &add(const ranked_t &r) { _ranked.push_back(r); return *this; }
    inline self_t &add(const flat_vector_t &v) { return add(ranked_t(_ranked.size(), v)); }
    inline size_t  size()  const { return _ranked.size(); }
    
    inline auto    begin()       { return _ranked.begin(); }
    inline auto    end()         { return _ranked.end(); }
    inline auto    begin() const { return _ranked.begin(); }
    inline auto    end()   const { return _ranked.end(); }
    
    void sort()
    {
        typename ranked_t::sort_compare_t compare;
        std::sort(_ranked.begin(), _ranked.end(), compare);
        _indices.resize(_ranked.size());
        for (size_t i = 0, sz = _ranked.size(); i != sz; ++i) {
            uint64_t r = _ranked[i].id;
            _indices[r] = i;
        }
    }
    
    inline const ranked_t &at(size_t i)    const { return _ranked[i]; }
    
    inline auto equal_range(const_iterator b, const_iterator e, size_t rank, const ranked_t &find) const
    {
        typename ranked_t::find_compare_t c(rank);
        return std::equal_range(b, e, find, c);
    }
    
    inline auto equal_range(const_iterator b, const_iterator e, size_t rank, int16_t dimension) const
    {
        ranked_t find;
        find.explicit_set(rank, item_t({ dimension, 0, 0 }));
        typename ranked_t::find_compare_t c(rank);
        return std::equal_range(b, e, find, c);
    }
    inline auto equal_range(const const_range_t &range, size_t rank, int16_t dimension) const { return equal_range(range.first, range.last, rank, dimension); }
    inline auto equal_range(size_t rank, int16_t dimension) const { return equal_range(_ranked.begin(), _ranked.end(), rank, dimension); }
    
    value_index_t linear_search_ranked(const flat_vector_t &find, value_type max = std::numeric_limits<value_type>::max()) const
    {
        ranked_t v(find);
        std::pair<value_type, size_t> minimum(max, 0);
        for (size_t i = 0, sz = _ranked.size(); i != sz; ++i) {
            std::pair<value_type, size_t> current(v.distance_sq(_ranked[i], minimum.first), i);
            if (current.first < minimum.first) {
                minimum = current;
                //std::cout << "_d " << sqrt(minimum.first) << " ranks " << (float(ranks_total) / i) << std::endl;
            }
        }
        return minimum;
    }
    
    value_index_t linear_search_ranked_1(const flat_vector_t &find, value_type max = std::numeric_limits<value_type>::max()) const
    {
        ranked_t v(find);
        std::pair<value_type, size_t> minimum(max, 0);
        for (size_t i = 0, sz = _ranked.size(); i != sz; ++i) {
            std::pair<value_type, size_t> current(v.distance_sq_1(_ranked[i], minimum.first), i);
            if (current.first < minimum.first) minimum = current;
        }
        return minimum;
    }
};
