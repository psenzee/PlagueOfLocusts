#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>
#include "stl.h"
#include "int_types.h"
#include "compile_time.h"
#include "arrays.h"

namespace {
template <typename T> inline T _sq(T u) { return u * u; }
}

template <typename FloatT, size_t N>
struct ordered_vector
{
    typedef FloatT                                                 real_t;

    static constexpr size_t COUNT      = N,
                            SQRT_COUNT = compile_time::isqrt(COUNT);
    static constexpr real_t REAL_MAX   = std::numeric_limits<real_t>::max();
    
    typedef ordered_vector<FloatT, N>                              self_t;
    typedef typename int_for_max_value<COUNT>::unsigned_t          index_t;
    typedef std::array<index_t, COUNT>                             ranks_t;
    typedef std::array<real_t, COUNT>                              vector_t;
    typedef std::vector<real_t>                                    dynamic_t;

    real_t   norm, norm_sum;
    ranks_t  ranks;
    vector_t values;

    inline ordered_vector()                   : norm(0), norm_sum(0), ranks({ 0 }), values({ 0 }) {}
    inline ordered_vector(const vector_t &v)  : norm(0), norm_sum(0), ranks({ 0 }), values({ 0 }) { _set(v); }
    inline ordered_vector(const dynamic_t &v) : norm(0), norm_sum(0), ranks({ 0 }), values({ 0 }) { _set(v); }
    inline ordered_vector(const self_t &u)    : norm(u.norm), norm_sum(u.norm_sum), ranks(u.ranks), values(u.values) {}

    inline real_t distance_sq(const self_t    &u) const { return arrays::distance_sq(values, u.values); }
    inline real_t distance_sq(const vector_t  &u) const { return arrays::distance_sq(values, u); }
    inline real_t distance_sq(const dynamic_t &u) const { return arrays::distance_sq(values, u); }
    
    real_t distance_sq_limit(const self_t &u, real_t limit_sq = REAL_MAX) const
    {
        if (limit_sq == REAL_MAX)
            return distance_sq(u);
        if (abs(norm - u.norm) > limit_sq)
            return REAL_MAX;
        real_t sum_a(0), sum_b(0);
        for (size_t i = 0, sz = ranks.size(); i != sz; ++i) {
            auto ra = ranks[i], rb = u.ranks[i];
            sum_a += arrays::sq(values[ra] - u.values[ra]);
            if (sum_a > limit_sq) return REAL_MAX;
            sum_b += arrays::sq(values[rb] - u.values[rb]);
            if (sum_b > limit_sq) return REAL_MAX;
        }
        return sum_a;
    }
    
    real_t distance_sq_limit(const vector_t &u, real_t limit_sq = REAL_MAX) const
    {
        if (limit_sq == REAL_MAX)
            return distance_sq(u);
        real_t sum(0);
        for (auto r : ranks) {
            sum += arrays::sq(values[r] - u[r]);
            if (sum > limit_sq) return REAL_MAX;
        }
        return sum;
    }
    
    real_t distance_sq_limit(const dynamic_t &u, real_t limit_sq = REAL_MAX) const
    {
        if (limit_sq == REAL_MAX)
            return distance_sq(u);
        real_t sum(0);
        for (auto r : ranks) {
            sum += arrays::sq(values[r] - u[r]);
            if (sum > limit_sq) return REAL_MAX;
        }
        return sum;
    }
    
    inline bool operator<(const elem_t &u) const
    {
        if (norm != u.norm) return norm < u.norm;
      //if (norm_sum != u.norm_sum) return norm_sum < u.norm_sum;
        for (size_t i = 0, sz = ranks.size(); i != sz; ++i)
            if (ranks[i] != u.ranks[i])
                return ranks[i] < u.ranks[i];
        return norm_sum < u.norm_sum;
    }
    
private:

    typedef std::pair<real_t, index_t> elem_t;
    typedef std::array<elem_t, COUNT>  elems_t;
    
    struct _rank_comparer {
        inline bool operator()(const elem_t &a, const elem_t &b) const
            { real_t u(abs(a.first)), v(abs(b.first)); return u != v ? u > v : a.second < b.second; }
    };

    inline void _set(const dynamic_t &v) { vector_t u; std::copy(v.begin(), v.end(), u.begin()); _set(u); }

    void _set(const vector_t &v)
    {
        values = v; // values remain in original order
        norm = sqrt(arrays::dot(v, v));
        real_t _0(0), _1(1), sum(_0), invn(_1 / norm);

        // calculate norm_sum = sum(abs(v[..]) / norm)
        if (norm != _0) { for (auto u : v) sum += abs(u) * invn; norm_sum = sum; }

        // sort ranks
        _rank_comparer c;
        elems_t u;
        for (index_t i = 0; i != N; i++) u[i] = elem_t(v[i], i);
        std::sort(u.begin(), u.end(), c);
        for (size_t i = 0; i != N; ++i) ranks[i] = u[i].second;
    }
};


template <typename FloatT, size_t N>
struct ordered_vectors
{
    typedef FloatT                            real_t;
    typedef ordered_vector<FloatT, N>         elem_t;
    typedef std::vector<elem_t>               data_t;
    typedef typename data_t::const_iterator_t const_iterator_t;
    typedef std::pair<real_t, size_t>         pair_t;
    
    static constexpr size_t COUNT    = N;
    static constexpr real_t REAL_MAX = std::numeric_limits<real_t>::max();

    data_t _data;
    
    inline self_t         &add(const vector_t &v)       { return add(elem_t(v)); }
    inline self_t         &add(const elem_t &r)         { _data.push_back(r); return *this; }
    inline self_t         &add(const dynamic_t &v)      { return add(elem_t(v)); }
    inline size_t          size()                 const { return _data.size(); }
    inline self_t         &finish_add()                 { return *this; }
    inline const elem_t   &at(size_t i)           const { return _data[i]; }
    inline const elem_t   *data()                 const { return &_data; }

    inline real_t          distance_sq(const elem_t &find, size_t index)                                   const { return _data[index].distance_sq(find); }
    inline real_t          distance_sq(const vector_t &find, size_t index)                                 const { return _data[index].distance_sq(find); }
    inline real_t          distance_sq(const dynamic_t &find, size_t index)                                const { return _data[index].distance_sq(find); }
    inline real_t          distance_sq(size_t index_a, size_t index_b)                                     const { return _data[index_a].distance_sq(_data[index_b]); }

    inline real_t          distance_sq_limit(const elem_t &find, size_t index, value_type limit_sq)        const { return _data[index].distance_sq_limit(find, limit_sq);}
    inline real_t          distance_sq_limit(const vector_t &find, size_t index, value_type limit_sq)      const { return _data[index].distance_sq_limit(find, limit_sq); }
    inline real_t          distance_sq_limit(const dynamic_t &find, size_t index, value_type limit_sq)     const { return _data[index].distance_sq_limit(find, limit_sq); }
    inline real_t          distance_sq_limit(size_t index_a, size_t index_b, value_type limit_sq)          const { return _data[index_a].distance_sq_limit(_data[index_b], limit_sq); }

    inline real_t          distance(const elem_t &find, size_t index)                                      const { return sqrt(distance_sq(find, index)); }
    inline real_t          distance(const vector_t &find, size_t index)                                    const { return sqrt(distance_sq(find, index)); }
    inline real_t          distance(const dynamic_t &find, size_t index)                                   const { return sqrt(distance_sq(find, index)); }
    inline real_t          distance(size_t index_a, size_t index_b)                                        const { return sqrt(distance_sq(index_a, index_b)); }

    pair_t linear_search(const elem_t &find) const
    {
        pair_t minimum_sq(REAL_MAX, 0);
        for (size_t i = 0, sz = _data.size(); i != sz; ++i) {
            pair_t current(distance_sq_limit(find, i, minimum_sq.first), i);
            if (current.first < minimum_sq.first) minimum_sq = current;
        }
        return pair_t(sqrt(minimum_sq.first), minimum_sq.second);
    }
    
    inline pair_t linear_search(const vector_t &find)  const { return linear_search(elem_t(find)); }
    inline pair_t linear_search(const dynamic_t &find) const { return linear_search(traits_t::fixed(find)); }
    
    inline void _update(const elem_t &find, const_iterator_t i, size_t offset, pair_t &min)
    {
        auto dsq = find.distance_sq_limit(*i);
        if (dsq < arrays::sq(min.first)) {
            min.first = sqrt(dsq);
            min.second = offset;
        }
    }
    
    inline pair_t sorted_search(const vector_t &find)  const { return sorted_search(elem_t(find)); }
    inline pair_t sorted_search(const dynamic_t &find) const { return sorted_search(traits_t::fixed(find)); }
    
    pair_t sorted_search(const elem_t &find) const
    {
        auto b = _data.begin(), e = _data.end(), i = std::lower_bound(b, e, find), j = i + 1;
        pair_t min(REAL_MAX, 0);
        bool top = false, bottom = false;
        while ((top    = (i >= b && (*i).norm >= find.norm - min.first)) ||
               (bottom = (j < e  && (*j).norm <  find.norm + min.first))) {
            if (top)    { _update(find, i, i - b, min); i--; }
            if (bottom) { _update(find, j, j - b, min); j++; }
        }
    }
    
    void sort()
    {
        std::cout << "[sort by norm/ranks] ... ";
        simple_timer t;
        std::sort(_data.begin(), _data.end());
        t.print_elapsed("done [sort by norm/ranks]");
    }
};
