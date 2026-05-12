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

#include "simple_timer.h"
#include "stl.h"

template <typename KeyT>
struct key_data
{
    typedef KeyT                          key_t;
    typedef typename KeyT::float_t        float_t;
    typedef typename KeyT::vector_t       vector_t;
    typedef typename KeyT::keyed_vector_t keyed_vector_t;
    
    key_t    key;
    float_t  magsq;
    size_t   index;
    
    inline key_data(const key_t &key, float_t magsq, size_t index) : key(key), magsq(magsq), index(index) {}
    inline key_data(const key_data &u) : key(u.key), magsq(u.magsq), index(u.index) {}
    inline key_data() : magsq(0.0), index(0) {}
    
    inline key_data &operator=(const key_data &u) { if (&u != this) set(u); return *this; }
    
    struct compare { inline bool operator()(const key_data &a, const key_data &b) const { typename key_t::compare c; return c(a.key, b.key); } };

    inline key_data &set(const key_data &u) { key = u.key; magsq = u.magsq; index = u.index; return *this; }
    
    static auto create(const vector_t &v, size_t index, keyed_vector_t &buffer) { return key_data(key_t::create(v, buffer), magnitude_sq(v), index); }
    
    std::ostream &print(std::ostream &os) const { return os << "[key_data " << key << /*" magsq " << magsq << " mag " << sqrt(magsq) <<*/ " index " << index << "]"; }
};

template <typename KeyT>
inline std::ostream &operator<<(std::ostream &os, const key_data<KeyT> &value) { return value.print(os); }

template <typename KeyT>
struct key_index
{
    typedef KeyT                                    key_t;
    typedef typename KeyT::float_t                  float_t;
    typedef key_data<KeyT>                          key_data_t;
    typedef std::vector<key_data_t>                 key_index_t;
    typedef typename key_index_t::const_iterator    const_iterator;
    typedef typename key_t::keyed_vector_t          keyed_vector_t;
    typedef typename key_t::vector_t                vector_t;
    
    key_index_t            _index;
    mutable keyed_vector_t _buffer;
    bool                    _indexed;
    
    inline key_index() : _indexed(false) {}

    inline void add_invalidate(const key_t &key, float_t magnitude_sq, size_t index) { _add(key_data(key, magnitude_sq, index)); }
    inline void add_invalidate(const vector_t &v, size_t index) { _add(create(v, index)); }
    
    void index()
    {
        if (!_indexed) {
            typename key_data_t::compare compare;
            std::sort(_index.begin(), _index.end(), compare);
            _indexed = true;
        }
    }
    
    inline auto begin() const { return _index.begin(); }
    inline auto end()   const { return _index.end(); }
    
    inline auto upper_bound(const key_data_t &search) const { return upper_bound(begin(), end(), search); }
    inline auto lower_bound(const key_data_t &search) const { return lower_bound(begin(), end(), search); }
    inline auto upper_bound(const vector_t &v) const { return upper_bound(begin(), end(), create(v)); }
    inline auto lower_bound(const vector_t &v) const { return lower_bound(begin(), end(), create(v)); }
    inline auto upper_bound(const_iterator begin, const_iterator end, const vector_t &v) const { return upper_bound(begin, end, create(v)); }
    inline auto lower_bound(const_iterator begin, const_iterator end, const vector_t &v) const { return lower_bound(begin, end, create(v)); }
    
    inline auto upper_bound(const_iterator begin, const_iterator end, const key_data_t &search) const { typename key_data_t::compare c; return std::upper_bound(begin, end, search, c); }
    inline auto lower_bound(const_iterator begin, const_iterator end, const key_data_t &search) const { typename key_data_t::compare c; return std::lower_bound(begin, end, search, c); }
    
    auto create(const vector_t &v, size_t index = 0) const { return key_data_t::create(v, index, _buffer); }
    
    std::ostream &print(std::ostream &os) const
    {
        enum { COUNT = 5 };
        for (size_t i = 0, sz = COUNT; i != sz; ++i) os << _index[i] << std::endl;
        os << "..." << std::endl;
        for (size_t i = _index.size() - COUNT, sz = _index.size(); i != sz; ++i) os << _index[i] << std::endl;
        return os;
    }

private:

    inline void _add(const key_data_t &data) { _index.push_back(data); _indexed = false; }
};

template <typename KeyT>
inline std::ostream &operator<<(std::ostream &os, const key_index<KeyT> &value) { return value.print(os); }
