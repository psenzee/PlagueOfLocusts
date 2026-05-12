#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include "stl.h"
#include "key.h"
#include "pairs.h"

template <size_t N>
struct key_histogram
{
    std::array<size_t, N> data;
};

template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN, typename IdT = uint32_t>
struct key_index
{
    typedef key_index<FloatT, N, IndexT, KeyRanksN, IdT>     self_t;
    typedef key<FloatT, N, IndexT, KeyRanksN>                key_t;
    typedef key_id<FloatT, N, IndexT, KeyRanksN, IdT>        key_id_t;
    typedef typename key_id_t::id_t                          id_t;
    typedef std::vector<key_id_t>                            data_t;
    typedef std::vector<key_t>                               keys_t;
    typedef std::pair<FloatT, id_t>                          norm_t;
    typedef std::vector<norm_t>                              norms_t;
    typedef typename data_t::const_iterator                  const_iterator;
    typedef std::pair<const_iterator, const_iterator>        const_range_t;
    typedef std::array<FloatT, N>                            vector_t;

    data_t  _data;
    keys_t  _keys; // keys indexed by internal ids
    // TODO abbreviated keys -- norm + 8 bit ranks only - sorted?
    norms_t _norms;
    norms_t _norms_n[N * 2];
    size_t  _count;
    
    key_index() : _count(0) {}

    inline self_t &add(const key_t &key)  { _add(key, _count++); return *this; }
    inline self_t &add(const vector_t &v) { return add(key_t(v)); }
    
    inline auto    begin() const { return _data.begin(); }
    inline auto    end()   const { return _data.end(); }
    self_t        &index()
    {
        typename key_id_t::sort_comparer ckid;
        std::sort(_data.begin(), _data.end(), ckid);
        pair_compare_first_less<norm_t> cn;
        std::sort(_norms.begin(), _norms.end(), cn);
        for (size_t i = 0; i < N * 2; i++)
            std::sort(_norms_n[i].begin(), _norms_n[i].end(), cn);
        return *this;
    }
    
    inline const auto &norms_n(int n) const { return _norms_n[n < 0 ? -n + N : n]; }
    inline auto       &norms_n(int n)       { return _norms_n[n < 0 ? -n + N : n]; }

    std::ostream  &print_range(std::ostream &os, size_t count = 10, size_t start = 0) const
    {
        if (start > 0) os << "..." << std::endl;
        for (auto i = _data.begin() + start, e = std::min(i + count, _data.end()); i != e; ++i)
            os << (*i) << std::endl;
        if (start + count < _data.size()) os << "..." << std::endl << std::endl;
        return os;
    }

private:

    inline void _add(const key_t &key, size_t id)
    {
        _data.push_back(key_id_t(key, uint32_t(id)));
        _keys.push_back(key);
        norm_t norm(key.norm, uint32_t(id));
        _norms.push_back(norm);
        for (size_t i = 0; i < key.ranks.size(); i++)
            norms_n(key.ranks[i]).push_back(norm);
    }
};
