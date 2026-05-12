#pragma once

#include <array>
#include <vector>
#include <algorithm>

#include "key.h"

template <typename ContainerT, size_t KeyRanksN>
struct keys
{
    typedef ContainerT                                    container_t;
    typedef typename container_t::value_type              real_t;
    typedef keys<container_t, KeyRanksN>                  self_t;
    typedef key<real_t, container_t::COUNT, KeyRanksN>    key_t;
    typedef std::vector<key_t>                            data_t;
    typedef typename data_t::const_iterator               const_iterator;
    typedef std::pair<const_iterator, const_iterator>     const_range_t;

    inline keys() {}

    inline self_t     &add(const key_t &key)                  { _keys.push_back(key); return *this; }
    inline self_t     &set(size_t i, const key_t &key)        { if (_keys.size() <= i) _keys.resize(i + 1); _keys[i] = key; return *this; }
    inline key_t      &at(size_t i)                           { return _keys[i]; }
    inline key_t      &at(size_t i)                     const { return _keys[i]; }
    inline size_t      size()                           const { return _keys.size(); }

    inline auto        begin()                          const { return _keys.begin(); }
    inline auto        end()                            const { return _keys.end(); }
    inline auto        begin()                                { return _keys.begin(); }
    inline auto        end()                                  { return _keys.end(); }
    
    inline auto       &operator[](size_t i)                   { return _keys[i]; }
    inline const auto &operator[](size_t i)             const { return _keys[i]; }

private:

    data_t _keys; // keys indexed by internal ids
};
