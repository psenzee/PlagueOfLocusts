#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include "stl.h"
#include "key.h"

template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN>
struct key_index
{
    typedef key_index<FloatT, N, IndexT, KeyRanksN>          self_t;
    typedef key<FloatT, N, IndexT, KeyRanksN>                key_t;
    typedef key_id<FloatT, N, IndexT, KeyRanksN>             key_id_t;
    typedef std::vector<key_id_t>                            data_t;
    typedef typename data_t::const_iterator                  const_iterator;
    typedef std::pair<const_iterator, const_iterator>        const_range_t;
    typedef std::array<FloatT, N>                            vector_t;

    data_t _data;

    inline self_t &add(const key_t &key)  { _data.push_back(key_id_t(key, _data.size())); return *this; }
    inline self_t &add(const vector_t &v) { return add(key_t(v)); }
    
    inline auto    begin() const { return _data.begin(); }
    inline auto    end()   const { return _data.end(); }

    self_t        &index() { typename key_id_t::sort_comparer c; std::sort(_data.begin(), _data.end(), c); return *this; }
};
