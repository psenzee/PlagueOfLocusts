#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <tuple>

#include "stl.h"
#include "key.h"
#include "key_id.h"
#include "pairs.h"
#include "dim_norm_id.h"

template <typename ContainerT, size_t KeyRanksN>
struct key_index
{
    typedef ContainerT                                container_t;
    
    static constexpr size_t COUNT = container_t::COUNT;
    
    typedef typename container_t::value_type          real_t;
    typedef typename container_t::traits_t            traits_t;
    typedef key_index<container_t, KeyRanksN>         self_t;
    typedef key<real_t, COUNT, KeyRanksN>             key_t;
    typedef key_id<real_t, COUNT, KeyRanksN, id_t>    key_id_t;
    typedef typename key_id_t::id_t                   id_t;
    typedef std::vector<key_id_t>                     data_t;
    typedef std::vector<key_t>                        keys_t;
    typedef std::pair<real_t, id_t>                   norm_t;
    typedef std::vector<norm_t>                       norms_t;
    typedef std::vector<id_t>                         norm_ids_t;
    typedef typename data_t::const_iterator           const_iterator;
    typedef std::pair<const_iterator, const_iterator> const_range_t;
    typedef std::array<real_t, COUNT>                 vector_t;
    typedef dim_norm_id<int32_t, id_t, real_t>        dnid_t;
    typedef dim_norm_id_list<int32_t, id_t, real_t>   dnids_t;

    data_t     _data;
    keys_t     _keys; // keys indexed by internal ids
    size_t     _count;
    
    dnids_t    _dnids;

    key_index() : _count(0) {}

    struct compare_id_norms
    {
        const keys_t &keys;
        inline compare_id_norms(const keys_t &keys) : keys(keys) {}
        inline bool operator()(id_t a, id_t b) const { return keys[a].norm < keys[b].norm; }
    };

    struct compare_id_norms_fixed_key
    {
        const keys_t &keys;
        const key_id_t &kid;
        inline compare_id_norms_fixed_key(const keys_t &keys, const key_id_t &kid) : keys(keys), kid(kid) {}
        inline bool operator()(id_t a, id_t b) const { return (a == kid.id ? kid.key : keys[a]).norm < (b == kid.id ? kid.key : keys[b]).norm; }
    };

    inline auto    comparer() const { return compare_id_norms(_keys); }
    inline auto    comparer(const key_id_t &kid) const { return compare_id_norms_fixed_key(_keys, kid); }

    inline self_t &add(const key_t &key)  { _add(key, _count++); return *this; }
    inline self_t &add(const vector_t &v) { return add(key_t(v)); }

    inline auto    begin()    const { return _data.begin(); }
    inline auto    end()      const { return _data.end(); }
    self_t        &index()
    {
        typename key_id_t::sort_comparer ckid;
        std::sort(_data.begin(), _data.end(), ckid);
        // dnids
        _dnids.index();
        return *this;
    }


    std::ostream  &print_range(std::ostream &os, size_t count = 10, size_t start = 0) const
    {
        if (start > 0) os << "..." << std::endl;
        for (auto i = _data.begin() + start, e = std::min(i + count, _data.end()); i != e; ++i)
            os << (*i) << std::endl;
        if (start + count < _data.size()) os << "..." << std::endl << std::endl;
        return os;
    }
    
    size_t id_count() const
    {
        return _dnids.size();
    }

private:

    inline void _add(const key_t &key, size_t id)
    {
        // on KeyRanksN=16, 0.85/0.75 yields about 4x the id count (as opposed to 16x for 1.0/1.0
        key_id_t kid(key, id_t(id));
        _data.push_back(kid);
        _keys.push_back(key);
        real_t sum_sq(0), base_ratio = 0.85;
        for (size_t i = 0; i < key.ranks.size(); i++) {
            _dnids.add(key.ranks[i], key.norm, id);
            //sum_sq += _sq(key.values[i]);
            //real_t ratio = sqrt(sum_sq) / key.norm;
            sum_sq += _sq(key.values[i] / key.norm);
            real_t ratio = sqrt(sum_sq)/* / key.norm*/;
            if (ratio > base_ratio) break;
            base_ratio = 0.5; // subsequent ratios
        }
    }
};
