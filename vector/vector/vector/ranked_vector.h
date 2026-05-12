#pragma once

#include <cmath>
#include <limits>
#include <cstddef>
#include <array>
#include <algorithm>
#include <iostream>

#include "simple_bitset.h"

template <size_t N, typename FloatT, typename DimT, typename NthT>
struct _rank_item
{
    typedef _rank_item<N, FloatT, DimT, NthT> self_t;
    typedef FloatT                            value_t;
    typedef DimT                              dim_t;
    typedef NthT                              nth_t;
    
    dim_t   dimension;
    nth_t   nth;
    value_t value;
    
    struct compare_t {
        inline bool operator()(const self_t &a, const self_t &b) const
            { value_t _a(abs(a.value)), _b(abs(b.value)); return (_a > _b) ? true : (_a == _b ? (a.dimension < b.dimension) : false); } };
};

template <size_t N, typename FloatT = float, typename DimT = int16_t, typename NthT = uint16_t>
struct ranked_vector
{
    typedef ranked_vector<N, FloatT, DimT, NthT> self_t;
    typedef _rank_item<N, FloatT, DimT, NthT>    item_t;
    typedef typename item_t::compare_t           ranking_compare_t;
    typedef FloatT                               value_t;
    typedef DimT                                 dim_t;
    typedef NthT                                 nth_t;
    typedef uint64_t                             id_t;

    struct sort_compare_t
    {
        inline bool operator()(const ranked_vector &a, const ranked_vector &b) const
        {
            auto i = a.data.begin();
            for (const auto &bi : b.data) {
                const auto &ai = *i;
                if      (ai.dimension < bi.dimension) return true;
                else if (ai.dimension > bi.dimension) return false;
                i++;
            }
            return false;
        }
    };
    
    struct find_compare_t
    {
        size_t _rank;
        inline find_compare_t(size_t rank) : _rank(rank) {}
        inline bool operator()(const ranked_vector &a, const ranked_vector &b) const
            { return a.data[_rank].dimension < b.data[_rank].dimension; }
    };

    typedef std::array<value_t, N> flat_array_t;
    typedef std::array<item_t,  N> array_t;
    
    static constexpr value_t _MAX = std::numeric_limits<value_t>::max();
    
    id_t    id;
    value_t magnitude_sq;
    array_t data;

    inline ranked_vector() : id(0), magnitude_sq(0) { set_from_flat(flat_array_t({ 0 })); }
    inline ranked_vector(const ranked_vector &v) : id(v.id), magnitude_sq(v.magnitude_sq), data(v.data) {}
    inline ranked_vector(const flat_array_t &in) : id(0), magnitude_sq(0) { set_from_flat(in); }
    inline ranked_vector(id_t id, const flat_array_t &in) : id(id), magnitude_sq(0) { set_from_flat(in); }
    
    inline auto   begin() const { return data.begin(); }
    inline auto   end()   const { return data.end(); }
    inline size_t size()  const { return data.size(); }
    
    void set_from_flat(const flat_array_t &in)
    {
        magnitude_sq = value_t(0);
        for (size_t i = 0; i < N; i++) {
            value_t v(in[i]);
            magnitude_sq += v * v;
            data[i] = item_t({ dim_t(i), nth_t(~0), v });
        }
        ranking_compare_t compare;
        std::sort(data.begin(), data.end(), compare);
        for (size_t i = 0; i < N; i++) data[data[i].dimension].nth = nth_t(i);
        // negate dimension indices on negative values for sorting whole containers for locality
        for (size_t i = 0; i < N; i++) if (data[i].value < 0) data[i].dimension = -data[i].dimension;
    }
    
    inline self_t       &explicit_set(size_t rank, const item_t &item)              { data[rank] = item; return *this; }
    
    inline flat_array_t &copy_to(flat_array_t &out)                           const { for (size_t i = 0; i < N; i++) out[data[i].dimension] = data[i].value; return out; }
    inline flat_array_t  flat()                                               const { flat_array_t out; return copy_to(out); }
    
    inline dim_t         dimension_for_rank(size_t r)                         const { return abs(data[r].dimension); }
    inline nth_t         rank_for_dimension(size_t n)                         const { return data[n].nth; }
    
    inline const item_t &at_rank(size_t r)                                    const { return data[r]; }
    inline const item_t &at_dimension(size_t n)                               const { return data[rank_for_dimension(n)]; }

    inline value_t       diff_at_rank(size_t r, const flat_array_t &u)        const { return data[r].value - u[dimension_for_rank(r)]; }
    inline value_t       diff_at_dimension(size_t n, const flat_array_t &u)   const { return at_dimension(n).value - u[n]; }
    
    inline value_t       diff_at_rank(size_t r, const ranked_vector &u)       const { return at_rank(r).value - u.at_dimension(dimension_for_rank(r)).value; }
    inline value_t       diff_at_dimension(size_t n, const ranked_vector &u)  const { return at_dimension(n).value - u.at_dimension(n).value; }

    inline value_t       distance_sq(const flat_array_t &u)                   const { value_t dsq(0); for (size_t r = 0; r < N; r++) dsq += _square(diff_at_rank(r)); return dsq; }
    inline value_t       distance_sq(const flat_array_t &u, value_t max)      const { value_t dsq(0); for (size_t r = 0; r < N && dsq < max; r++) dsq += _square(diff_at_rank(r)); return (dsq < max) ? dsq : _MAX; }
    
    inline value_t distance_sq(const ranked_vector &u, value_t max) const
    {
        if (abs(magnitude_sq - u.magnitude_sq) > max) return _MAX;
        value_t dsq(0);
        for (size_t r = 0; r < N; r++) {
            const item_t &ia = data[r], &ib = u.data[u.data[abs(ia.dimension)].nth];
            value_t v = ia.value - ib.value;
            dsq += v * v;
            if (dsq >= max) return _MAX;
        }
        return dsq;
    }

    inline value_t distance_sq_1(const ranked_vector &u, value_t max) const
    {
        if (abs(magnitude_sq - u.magnitude_sq) > max) return _MAX;
        simple_bitset<N> bits;
        value_t dsq(0);
        for (size_t r = 0; r < N; r++) {
            dsq += _dsq(data, u.data, r, bits) + _dsq(u.data, data, r, bits);
            if (dsq >= max) return _MAX;
        }
        return dsq;
    }

private:

    inline value_t _dsq(const array_t &a, const array_t &b, size_t rank, simple_bitset<N> &bits) const
    {
        const item_t &u = a[rank];
        size_t dim = abs(u.dimension);
        if (!bits.is_set(dim)) { bits.set(dim); value_t v = u.value - b[b[dim].nth].value; return v * v; }
        return value_t(0);
    }
};
