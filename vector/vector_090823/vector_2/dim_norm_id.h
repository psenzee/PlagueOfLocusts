#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <tuple>

#include "stl.h"
#include "key.h"
#include "key_id.h"
#include "pairs.h"

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
    
    inline size_t               size()                                                       const { return _data.size(); }

    inline const_range_t        find_dimension(dim_t dimension)                              const { return dimension < 0 ? _negative[-dimension] : _positive[dimension]; }
    inline const_range_t        find(dim_t dimension, real_t norm)                           const { return find_norm(find_dimension(dimension), norm); }

    inline static const_range_t find_dimension(const const_range_t &r, dim_t dimension)            { return find_dimension(r.first, r.second, dimension); }

    inline static const_range_t find_norm(const const_range_t &r, real_t norm)                     { return find_norm(r.first, r.second, norm); }

    inline static const_range_t find(const const_range_t &r, dim_t dimension, real_t norm)         { return find(r.first, r.second, dimension, norm); }
    inline static const_range_t find(const const_range_t &r, const dnid_t &u)                      { return find(r.first, r.second, u); }

    inline static const_range_t find_dimension(const_iter_t b, const_iter_t e, dim_t dimension)    { compare_dimension_t c; return std::equal_range(b, e, dnid_t(dimension, real_t(0)), c); }
    inline static const_range_t find_norm(const_iter_t b, const_iter_t e, real_t norm)             { return b == e ? const_range_t(e, e) : find(b, e, (*b).dimension, norm); }
    
    inline static const_iter_t  lower_norm(const_iter_t b, const_iter_t e, real_t norm)            { compare_norm_t c; return b == e ? e : std::lower_bound(b, e, dnid_t((*b).dimension, norm), c); }
    inline static const_iter_t  upper_norm(const_iter_t b, const_iter_t e, real_t norm)            { compare_norm_t c; return b == e ? e : std::upper_bound(b, e, dnid_t((*b).dimension, norm), c); }

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
    
    self_t &index()
    {
        std::sort(_data.begin(), _data.end());
        dim_t count = std::max(abs(_data[0].dimension), abs(_data.back().dimension));
        _positive.resize(count + 1);
        _negative = _positive;
        auto r(range());
        for (dim_t i = 0; i < count; ++i) {
            _positive[i] = find_dimension(r,  i);
            _negative[i] = find_dimension(r, -i);
        }
        return *this;
    }
};
