#pragma once

#include <cmath>
#include <limits>
#include <cstddef>
#include <array>
#include <algorithm>
#include <iostream>

#include "simple_bitset.h"
#include "bfloat16.h"

template <size_t N>
struct ranked_vector256
{
    typedef float     full_t;
    typedef bfloat16  value_t;
    typedef int8_t    dim_t;
    typedef uint8_t   nth_t;
    typedef uint64_t  id_t;
    
    struct item_t { dim_t dimension; nth_t nth; value_t value; };
    
    struct compare_t {
        inline bool operator()(const item_t &a, const item_t &b) const
        { full_t _a(abs(bf2f(a.value))), _b(abs(bf2f(b.value))); return (_a > _b) ? true : (_a == _b ? (a.dimension < b.dimension) : false); } };

    typedef std::array<full_t, N> flat_array_t;
    typedef std::array<item_t, N> array_t;
    
    static constexpr full_t _MAX = std::numeric_limits<full_t>::max();
    
    id_t    id;
    full_t  magnitude_sq;
    array_t data;

    inline ranked_vector256() : id(0), magnitude_sq(0) { set_from_flat(flat_array_t({ 0 })); }
    inline ranked_vector256(const ranked_vector256 &v) : id(v.id), magnitude_sq(v.magnitude_sq), data(v.data) {}
    inline ranked_vector256(const flat_array_t &in) : id(0), magnitude_sq(0) { set_from_flat(in); }
    inline ranked_vector256(id_t id, const flat_array_t &in) : id(id), magnitude_sq(0) { set_from_flat(in); }
    
    void set_from_flat(const flat_array_t &in)
    {
        magnitude_sq = full_t(0);
        for (size_t i = 0; i < N; i++) {
            full_t v(in[i]);
            magnitude_sq += v * v;
            data[i] = item_t({ dim_t(i), nth_t(~0), f2bf(v) });
        }
        compare_t compare;
        std::sort(data.begin(), data.end(), compare);
        for (size_t i = 0; i < N; i++) data[data[i].dimension].nth = nth_t(i);
        // negate dimension indices on negative values for sorting whole containers for locality
        for (size_t i = 0; i < N; i++) if (data[i].value < 0) data[i].dimension = -data[i].dimension;
    }
    
    inline flat_array_t &copy_to(flat_array_t &out)                           const { for (size_t i = 0; i < N; i++) out[data[i].dimension] = bf2f(data[i].value); return out; }
    inline flat_array_t  flat()                                               const { flat_array_t out; return copy_to(out); }
    
    inline dim_t         dimension_for_rank(size_t r)                         const { return abs(data[r].dimension); }
    inline nth_t         rank_for_dimension(size_t n)                         const { return data[n].nth; }
    
    inline const item_t &at_rank(size_t r)                                    const { return data[r]; }
    inline const item_t &at_dimension(size_t n)                               const { return data[rank_for_dimension(n)]; }

    inline full_t        diff_at_rank(size_t r, const flat_array_t &u)        const { return bf2f(data[r].value) - u[dimension_for_rank(r)]; }
    inline full_t        diff_at_dimension(size_t n, const flat_array_t &u)   const { return bf2f(at_dimension(n).value) - u[n]; }
    
    inline full_t        diff_at_rank(size_t r, const ranked_vector256 &u)       const { return bf2f(at_rank(r).value) - bf2f(u.at_dimension(dimension_for_rank(r)).value); }
    inline full_t        diff_at_dimension(size_t n, const ranked_vector256 &u)  const { return bf2f(at_dimension(n).value) - bf2f(u.at_dimension(n).value); }

    inline full_t        distance_sq(const flat_array_t &u)                   const { full_t dsq(0); for (size_t r = 0; r < N; r++) dsq += _square(diff_at_rank(r)); return dsq; }
    inline full_t        distance_sq(const flat_array_t &u, full_t max)       const { full_t dsq(0); for (size_t r = 0; r < N && dsq < max; r++) dsq += _square(diff_at_rank(r)); return (dsq < max) ? dsq : _MAX; }
    
    inline full_t distance_sq(const ranked_vector256 &u, full_t max) const
    {
        if (abs(magnitude_sq - u.magnitude_sq) > max) return _MAX;
        full_t dsq(0);
        for (size_t r = 0; r < N; r++) {
            const item_t &ia = data[r], &ib = u.data[u.data[abs(ia.dimension)].nth];
            full_t v = bf2f(ia.value - ib.value);
            dsq += v * v;
            if (dsq >= max) return _MAX;
        }
        return dsq;
    }
    
    inline full_t _dsq(const array_t &a, const array_t &b, size_t rank, simple_bitset<N> &bits) const
    {
        const item_t &u = a[rank];
        size_t dim = abs(u.dimension);
        if (!bits.is_set(dim)) {
            bits.set(dim);
            full_t v = bf2f(u.value - b[b[dim].nth].value);
            return v * v;
        }
        return full_t(0);
    }
    
    inline full_t distance_sq_1(const ranked_vector256 &u, full_t max) const
    {
        if (abs(magnitude_sq - u.magnitude_sq) > max) return _MAX;
        simple_bitset<N> bits;
        full_t dsq(0);
        for (size_t r = 0; r < N; r++) {
            dsq += _dsq(data, u.data, r, bits) + _dsq(u.data, data, r, bits);
            if (dsq >= max) return _MAX;
        }
        return dsq;
    }
};
