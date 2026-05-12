#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <tuple>

#include "stl.h"
#include "key.h"
#include "key_id.h"
#include "pairs.h"

template <typename FloatT, size_t N, size_t KeyRanksN, typename IdT = uint32_t>
struct key_index
{
    typedef FloatT                                    real_t;
    typedef key_index<FloatT, N, KeyRanksN, IdT>      self_t;
    typedef key<FloatT, N, KeyRanksN>                 key_t;
    typedef key_id<FloatT, N, KeyRanksN, IdT>         key_id_t;
    typedef typename key_id_t::id_t                   id_t;
    typedef std::vector<key_id_t>                     data_t;
    typedef std::vector<key_t>                        keys_t;
    typedef std::pair<FloatT, id_t>                   norm_t;
    typedef std::vector<norm_t>                       norms_t;
    typedef std::vector<id_t>                         norm_ids_t;
    typedef typename data_t::const_iterator           const_iterator;
    typedef std::pair<const_iterator, const_iterator> const_range_t;
    typedef std::array<FloatT, N>                     vector_t;

    data_t     _data;
    keys_t     _keys; // keys indexed by internal ids
    norm_ids_t _ids_n[N * 2];
    size_t     _count;

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
        auto cn_ids(comparer());
        for (size_t i = 0; i < N * 2; i++)
            std::sort(_ids_n[i].begin(), _ids_n[i].end(), cn_ids);
        return *this;
    }

    inline const auto &ids_n(int n) const { return _ids_n[n < 0 ? N - n : n]; }
    inline auto       &ids_n(int n)       { return _ids_n[n < 0 ? N - n : n]; }

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
        key_id_t kid(key, id_t(id));
        _data.push_back(kid);
        _keys.push_back(key);
        real_t sum_sq(0), base_ratio = 0.99;
        for (size_t i = 0; i < key.ranks.size(); i++) {
            ids_n(key.ranks[i]).push_back(kid.id);
            sum_sq += _sq(key.values[i]);
            real_t ratio = sqrt(sum_sq) / key.norm;
            if (ratio > base_ratio) break;
            base_ratio = 0.95; // subsequent ratios
        }
    }
};

template <typename FloatT, typename IdT, typename DimensionT = int32_t>
struct dim_norm_id
{
    typedef FloatT                               real_t;
    typedef IdT                                  id_t;
    typedef DimensionT                           dim_t;
    typedef dim_norm_id<FloatT, IdT, DimensionT> self_t;

    dim_t   dimension;
    real_t  norm;
    id_t    id;

    inline dim_norm_id(dim_t dimension, real_t norm, id_t id = ~id_t(0)) : dimension(dimension), norm(norm), id(id) {}
    inline dim_norm_id(const self_t &u) : dimension(u.dimension), norm(u.norm), id(u.id) {}
    
    inline bool operator<(const self_t &u) const { return dimension > u.dimension ? false : (dimension < u.dimension || norm < u.norm); }
    
    struct compare_dimension { inline bool operator()(const self_t &a, const self_t &b) const { return a.dimension < b.dimension; } };
    struct compare_norm      { inline bool operator()(const self_t &a, const self_t &b) const { return a.norm < b.norm; } };
    struct compare           { inline bool operator()(const self_t &a, const self_t &b) const { return a < b; } };
};

template <typename FloatT, typename IdT, typename DimensionT = int32_t>
std::ostream &operator<<(std::ostream &os, const dim_norm_id<FloatT, IdT, DimensionT> &u) { return os << "[d " << u.dimension << std::setprecision(2) << " n " << u.norm << " id " << u.id << "]"; }

template <typename FloatT, typename IdT, typename DimensionT = int32_t>
struct dim_norm_id_list
{
    typedef FloatT                                real_t;
    typedef IdT                                   id_t;
    typedef DimensionT                            dim_t;
    typedef dim_norm_id_list<real_t, id_t, dim_t> self_t;
    typedef dim_norm_id<dim_t, id_t, real_t>      dnid_t;
    typedef std::vector<dnid_t>                   data_t;
    typedef typename data_t::const_iterator       const_iter_t;
    typedef std::pair<const_iter_t, const_iter_t> const_range_t;
    typedef std::vector<const_range_t>            index_t;
    typedef typename dnid_t::compare_dimension    compare_dimension_t;
    typedef typename dnid_t::compare_norm         compare_norm_t;

    data_t  _data;
    index_t _positive, _negative;
    
    inline self_t              &add(dim_t dimension, real_t norm, id_t id)                         { _data.push_back(dnid_t(dimension, norm, id)); return *this; }
    
    inline const_iter_t         begin()                                                      const { return _data.begin(); }
    inline const_iter_t         end()                                                        const { return _data.end(); }
    inline const_range_t        range()                                                      const { return const_range_t(_data.begin(), _data.end()); }
    
    inline self_t              &index()
    {
        std::sort(_data.begin(), _data.end());
        dim_t count = std::max(abs(_data[0].dimension), abs(_data.back().dimension));
        _positive.clear();
        _negative.clear();
        _positive.resize(count + 1);
        _negative.resize(count + 1);
        auto base_range = range();
        for (dim_t i = 0; i < count; ++i) {
            _positive[i] = find_dimension(base_range,  i);
            _negative[i] = find_dimension(base_range, -i);
        }
        return *this;
    }
    
    inline const_range_t        find_dimension(dim_t dimension)                              const { return find_dimension(range(), dimension); }
    
    inline const_range_t        by_dimension(dim_t dimension)                                const { return dimension < 0 ? _negative[-dimension] : _positive[dimension]; }
    inline const_range_t        find(dim_t dimension, real_t norm)                           const { return find(range(), dimension, norm); }

    inline static const_range_t find_dimension(const const_range_t &r, dim_t dimension)            { return find_dimension(r.first, r.second, dimension); }
    inline static const_range_t find_norm(const const_range_t &r, real_t norm)                     { return find_norm(r.first, r.second, norm); }
    inline static const_range_t find(const const_range_t &r, dim_t dimension, real_t norm)         { return find(r.first, r.second, dimension, norm); }
    inline static const_range_t find(const const_range_t &r, const dnid_t &u)                      { return find(r.first, r.second, u); }
    
    inline static const_range_t find_dimension(const_iter_t b, const_iter_t e, dim_t dimension)    { compare_dimension_t c; return std::equal_range(b, e, dnid_t(dimension, real_t(0)), c); }
    inline static const_range_t find_norm(const_iter_t b, const_iter_t e, real_t norm)             { return b == e ? const_range_t(e, e) : find(b, e, (*b).dimension, norm); }
    
    inline static const_iter_t  search_norm(const_iter_t b, const_iter_t e, real_t norm)           { compare_norm_t c; return b == e ? e : std::lower_bound(b, e, dnid_t((*b).dimension, norm), c); }
    
    inline static const_range_t find(const_iter_t b, const_iter_t e, dim_t dimension, real_t norm) { return find(b, e, dnid_t(dimension, norm)); }
    inline static const_range_t find(const_iter_t b, const_iter_t e, const dnid_t &u)              { return std::equal_range(b, e, u); }
    
    std::ostream &print(std::ostream &os, size_t count = 0) const
    {
        count = count == 0 ? _data.size() : std::min(count, _data.size());
        for (size_t i = 0; i < count; i++) {
            if (i != 0) os << std::endl;
            os << _data[i];
        }
        return os << (count < _data.size() ? "..." : "") << std::endl;
    }
};

template <typename ContainerT, size_t KeyRanksN>
struct key_index_
{
    typedef ContainerT                                container_t;
    
    static constexpr size_t COUNT = container_t::COUNT;
    
    typedef typename container_t::value_type          real_t;
    typedef typename container_t::traits_t            traits_t;
    typedef key_index_<container_t, KeyRanksN>        self_t;
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
    norm_ids_t _ids_n[COUNT * 2];
    size_t     _count;
    
    dnids_t    _dnids;

    key_index_() : _count(0) {}

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
        // ids_n
        auto cn_ids(comparer());
        for (size_t i = 0; i < COUNT * 2; i++)
            std::sort(_ids_n[i].begin(), _ids_n[i].end(), cn_ids);
        // dnids
        _dnids.index();
        return *this;
    }

    inline const auto &ids_n(int n) const { return _ids_n[n < 0 ? COUNT - n : n]; }
    inline auto       &ids_n(int n)       { return _ids_n[n < 0 ? COUNT - n : n]; }

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
        size_t sum = 0;
        for (size_t i = 0; i < COUNT * 2; ++i)
            sum += _ids_n[i].size();
        return sum;
    }
    
    std::ostream  &print_info(std::ostream &os) const
    {
        size_t sum = 0;
        for (size_t i = 0; i < COUNT * 2; ++i) {
            sum += _ids_n[i].size();
            if (_ids_n[i].size() > 0) {
                int bucket = int(i);
                if (bucket >= COUNT) bucket = int(COUNT) - bucket;
                os << "#" << bucket << ":" << _ids_n[i].size() << std::endl;
            }
        }
        std::cout << "total " << sum << std::endl << "------" << std::endl;
        return os;
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
            ids_n(key.ranks[i]).push_back(id);
            _dnids.add(key.ranks[i], key.norm, id);
            sum_sq += _sq(key.values[i]);
            real_t ratio = sqrt(sum_sq) / key.norm;
            if (ratio > base_ratio) break;
            base_ratio = 0.5; // subsequent ratios
        }
    }
};
