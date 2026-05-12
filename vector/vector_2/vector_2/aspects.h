#pragma once

#include <iostream>
#include <array>
#include <algorithm>

#include "stl.h"
#include "key.h"
#include "key_id.h"
#include "pairs.h"
#include "aspect.h"

template <typename FloatT, size_t N, typename IdT, bool SortShapeBool>
struct aspects
{
    typedef FloatT                                         real_t;
    typedef IdT                                            id_t;
    typedef aspect<FloatT, N, IdT, SortShapeBool>          aspect_t;
    typedef typename aspect_t::dimension_t                 dimension_t;
    typedef typename aspect_t::shape_t                     shape_t;
    typedef aspects<real_t, N, id_t, SortShapeBool>        self_t;
    typedef std::vector<aspect_t>                          data_t;
    typedef typename data_t::const_iterator                const_iter_t;
    typedef std::pair<const_iter_t, const_iter_t>          const_range_t;
    typedef std::vector<const_range_t>                     index_t;
    typedef typename aspect_t::compare_norm                compare_norm_t;
    typedef typename aspect_t::compare_dimension           compare_dimension_t;
    typedef typename aspect_t::compare_shape_norm          compare_shape_norm_t;
    typedef typename aspect_t::compare_dimension_shape     compare_dimension_shape_t;
       
    data_t  _data;

    inline self_t              &add(dimension_t dimension, shape_t shape, real_t norm, id_t id)                            { _data.push_back(aspect_t(dimension, shape, norm, id)); return *this; }
    
    inline const_iter_t         begin()                                                                              const { return _data.begin(); }
    inline const_iter_t         end()                                                                                const { return _data.end(); }
    inline const_range_t        range()                                                                              const { return _r(_data.begin(), _data.end()); }
    inline size_t               size()                                                                               const { return _data.size(); }

    inline const_range_t        find_dimension(dimension_t dimension)                                                const { return _find_dimension(dimension); }
    inline const_range_t        find_dimension_shape(dimension_t dimension, shape_t shape)                           const { return aspect_t::SORT_SHAPE ? _find_dimension_shape(dimension, shape) : _find_dimension(dimension); }
    inline const_range_t        find(dimension_t dimension, shape_t shape, real_t norm)                              const { return find_norm(find_dimension_shape(dimension, shape), norm); }

    inline static const_range_t find_dimension(const const_range_t &r, dimension_t dimension)                              { return find_dimension(r.first, r.second, dimension); }
    inline static const_range_t find_dimension_shape(const const_range_t &r, dimension_t dimension, shape_t shape)         { return find_dimension_shape(r.first, r.second, dimension, shape); }

    inline static const_range_t find_shape_norm(const const_range_t &r, shape_t shape, real_t norm)                        { return find_shape_norm(r.first, r.second, shape, norm); }

    inline static const_range_t find(const const_range_t &r, dimension_t dimension, shape_t shape, real_t norm)            { return find(r.first, r.second, dimension, norm); }
    inline static const_range_t find(const const_range_t &r, const aspect_t &u)                                            { return find(r.first, r.second, u); }

    inline static const_range_t find_dimension(const_iter_t b, const_iter_t e, dimension_t dimension)                      { compare_dimension_t c; return std::equal_range(b, e, aspect_t(dimension, 0, 0), c); }
    inline static const_range_t find_dimension_shape(const_iter_t b, const_iter_t e, dimension_t dimension, shape_t shape) { compare_dimension_shape_t c; return std::equal_range(b, e, aspect_t(dimension, shape, 0), c); }

    inline static const_range_t find_shape_norm(const_iter_t b, const_iter_t e, shape_t shape, real_t norm)                { compare_shape_norm_t c; return b == e ? _r(e, e) : c; return std::equal_range(b, e, _from_shape_norm(b, shape, norm), c); }
    inline static const_iter_t  lower_shape_norm(const_iter_t b, const_iter_t e, shape_t shape, real_t norm)               { compare_shape_norm_t c; return b == e ? e : std::lower_bound(b, e, _from_shape_norm(b, shape, norm), c); }
    inline static const_iter_t  upper_shape_norm(const_iter_t b, const_iter_t e, shape_t shape, real_t norm)               { compare_shape_norm_t c; return b == e ? e : std::upper_bound(b, e, _from_shape_norm(b, shape, norm), c); }
    
    inline static const_iter_t  lower_norm(const_iter_t b, const_iter_t e, real_t norm)                                    { compare_norm_t c; return b == e ? e : std::lower_bound(b, e, _from_norm(b, norm), c); }
    inline static const_iter_t  upper_norm(const_iter_t b, const_iter_t e, real_t norm)                                    { compare_norm_t c; return b == e ? e : std::upper_bound(b, e, _from_norm(b, norm), c); }

    inline static const_range_t find(const_iter_t b, const_iter_t e, dimension_t dimension, shape_t shape, real_t norm)    { return find(b, e, aspect_t(dimension, shape, norm)); }
    inline static const_range_t find(const_iter_t b, const_iter_t e, const aspect_t &u)                                    { return std::equal_range(b, e, u); }

    std::ostream &print(std::ostream &os, size_t count = 0) const
    {
        count = count == 0 ? _data.size() : std::min(count, _data.size());
        for (size_t i = 0; i < count; i++) { if (i != 0) os << std::endl; os << _data[i]; }
        return os << (count < _data.size() ? "..." : "") << std::endl;
    }

    self_t &index()
    {
        std::sort(_data.begin(), _data.end());
        _index_dimension();
        _index_dimension_shape();
        return *this;
    }

private:
    
    index_t _pos_d, _neg_d, _pos_ds, _neg_ds;
    
    inline static size_t        _dimension_shape_id(dimension_t dimension, shape_t shape)          { return (size_t(abs(dimension)) << aspect_t::SHAPE_BITS) + shape; }
    inline static aspect_t      _from_norm(const_iter_t i, real_t norm)                            { return aspect_t((*i).dimension, (*i).shape, norm); }
    inline static aspect_t      _from_shape_norm(const_iter_t i, shape_t shape, real_t norm)       { return aspect_t((*i).dimension, shape, norm); }
    inline static const_range_t _r(const_iter_t b, const_iter_t e)                                 { return const_range_t(b, e); }
    
    inline const_range_t        _find_dimension(dimension_t dimension)                       const { return dimension < 0 ? _neg_d[-dimension]  : _pos_d[dimension]; }
    inline const_range_t        _find_dimension_shape(dimension_t dimension, shape_t shape)  const { size_t i = _dimension_shape_id(dimension, shape); return dimension < 0 ? _neg_ds[i] : _pos_ds[i]; }
    
    void _index_dimension_shape()
    {
        if (!aspect_t::SORT_SHAPE)
            return;
        static constexpr size_t count = (size_t(aspect_t::DIMENSION_COUNT) << aspect_t::SHAPE_BITS) + aspect_t::SHAPE_COUNT;
        _pos_ds.resize(count);
        _neg_ds.resize(count);
        auto r(range());
        for (dimension_t i = 0; i < aspect_t::DIMENSION_COUNT; ++i) {
            for (shape_t k = 0; k < aspect_t::SHAPE_COUNT; ++k) {
                size_t index = _dimension_shape_id(i, k);
                _pos_ds[index] = find_dimension_shape(r,  i, k);
                _neg_ds[index] = find_dimension_shape(r, -i, k);
            }
        }
    }
    
    void _index_dimension()
    {
        static constexpr size_t count = aspect_t::DIMENSION_COUNT;
        _pos_d.resize(count);
        _neg_d.resize(count);
        auto r(range());
        for (dimension_t i = 0; i < count; ++i) {
            _pos_d[i] = find_dimension(r,  i);
            _neg_d[i] = find_dimension(r, -i);
        }
    }
};
