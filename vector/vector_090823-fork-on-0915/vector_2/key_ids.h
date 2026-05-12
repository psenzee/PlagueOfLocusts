#pragma once

#include <array>
#include <vector>
#include <algorithm>

#include "stl.h"
#include "key.h"
#include "key_id.h"
#include "pairs.h"
#include "aspects.h"

template <typename ContainerT, size_t KeyRanksN>
struct key_ids
{
    typedef ContainerT                                          container_t;
    typedef typename container_t::value_type                    real_t;
    typedef key_ids<container_t, KeyRanksN>                     self_t;
    typedef key<real_t, container_t::COUNT, KeyRanksN>          key_t;
    typedef key_id<real_t, container_t::COUNT, KeyRanksN, id_t> key_id_t;
    typedef typename key_id_t::id_t                             id_t;
    typedef std::vector<key_id_t>                               data_t;
    typedef typename data_t::const_iterator                     const_iterator;
    typedef std::pair<const_iterator, const_iterator>           const_range_t;
    typedef typename container_t::vector_t                      vector_t;
    typedef typename key_id_t::sort_comparer                    sort_comparer_t;

    inline key_ids() {}

    inline self_t     &add(const key_id_t &kid)                 { _data.push_back(kid); return *this; }
    inline self_t     &add(id_t id, const key_t &key)           { return add(key_id_t(key, id)); return *this; }
    inline self_t     &add(id_t id, const vector_t &v)          { return add(id, key_t(v)); }

    inline auto        begin()                            const { return _data.begin(); }
    inline auto        end()                              const { return _data.end(); }
    inline auto        begin()                                  { return _data.begin(); }
    inline auto        end()                                    { return _data.end(); }
    inline size_t      size()                             const { return _data.size(); }
    self_t            &index()                                  { sort_comparer_t c; std::sort(_data.begin(), _data.end(), c); return *this; }

private:

    data_t _data;
};
