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
#include "float_index.h"

template <typename FloatT = float, typename IndexT = uint32_t>
struct slice
{
    typedef FloatT                           value_type;
    typedef std::vector<FloatT>              data_t;
    typedef float_index<FloatT, IndexT>      index_t;
    typedef std::vector<IndexT>              reverse_t;
    typedef typename index_t::const_iterator index_iter_t;
    typedef typename index_t::value_type     index_value_type;
    typedef typename index_t::compare_up     compare_type;

private:

    data_t    _data;
    index_t   _index;
    reverse_t _reverse;
    bool      _indexed;

public:
    
    inline slice() : _indexed(false) {}

    inline void add_invalidate(FloatT value) { _index.add_invalidate(value); _data.push_back(value); _indexed = false; }
    
    inline value_type at(size_t index) const { return _data[index]; }
    
    inline auto float_index()    const { return _index; }
    inline auto index_begin()    const { return _index.begin(); }
    inline auto index_end()      const { return _index.end(); }
    inline auto begin()          const { return _data.begin(); }
    inline auto end()            const { return _data.end(); }

    inline auto upper_bound(index_iter_t begin, index_iter_t end, const index_value_type &value) const { return std::upper_bound(begin, end, value, compare_type()); }
    inline auto lower_bound(index_iter_t begin, index_iter_t end, const index_value_type &value) const { return std::upper_bound(begin, end, value, compare_type()); }
    
    inline auto upper_bound(const index_value_type &value) const { return upper_bound(index_begin(), index_end(), value); }
    inline auto lower_bound(const index_value_type &value) const { return lower_bound(index_begin(), index_end(), value); }
    
    inline auto upper_bound(index_iter_t begin, index_iter_t end, value_type value) const { return std::upper_bound(begin, end, index_value_type(value, 0), compare_type()); }
    inline auto lower_bound(index_iter_t begin, index_iter_t end, value_type value) const { return std::upper_bound(begin, end, index_value_type(value, 0), compare_type()); }
    
    inline auto upper_bound(value_type value) const { return upper_bound(index_begin(), index_end(), value); }
    inline auto lower_bound(value_type value) const { return lower_bound(index_begin(), index_end(), value); }
    
    // todo find 'centroids'
    
    std::ostream &print(std::ostream &os, const char *tag = 0) const { if (tag) os << tag << " "; return os << _index; }

    inline bool is_indexed() const { return _indexed && _index.is_indexed(); }
    inline void index()            { if (is_indexed()) return; _index.index(); _index.reverse_index(_reverse); _indexed = true; }
};

void test_slice(size_t count);
