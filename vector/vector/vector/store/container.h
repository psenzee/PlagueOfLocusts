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
#include <deque>

#include "simple_timer.h"
#include "stl.h"
#include "metric_cache.h"
#include "generators.h"
#include "ranks.h"
#include "ranked_vectors.h"
#include "fixed_vector.h"

template <int M, typename FloatT>
struct container : public fixed_vector::container<M, FloatT>
{
    typedef FloatT                                    value_type;
    typedef ranks<M, 8, FloatT, size_t>               ranks_t;
    typedef std::array<FloatT, M>                     vector_t;
    typedef std::vector<vector_t>                     container_t;
    typedef std::vector<ranks_t>                      ranks_vector_t;
    typedef std::pair<value_type, size_t>             value_index_t;
    typedef std::pair<value_index_t, value_index_t>   range_t;
    typedef metric_cache<vector_t>                    cache_t;
    typedef typename cache_t::result_t                cache_result_t;
    typedef vector_generator<float_t>                 generator_t;
    typedef ranked_vectors<M, FloatT>                 ranked_vectors_t;
    typedef typename ranked_vectors_t::ranked_t       ranked_t;
    typedef typename ranked_vectors_t::const_iterator rank_const_iterator;
    
    static constexpr bool USE_CACHE = false;
    static constexpr int  COUNT     = M;

    container_t      _data;
    ranked_vectors_t _ranked;
    ranks_vector_t   _ranks;
    mutable cache_t  _cache;
    
    inline container &add(const vector_t &v) { _data.push_back(v); _ranks.push_back(ranks_t(v)); _ranked.add(v); return *this; }
    inline size_t     size() const { return _data.size(); }
    
    inline container &finish_add() { _ranked.sort(); return *this; }

    inline       ranked_vectors_t &ranked_vectors()       { return _ranked; }
    inline const ranked_vectors_t &ranked_vectors() const { return _ranked; }
    
    inline const vector_t &at(size_t i)    const { return _data[i]; }
    inline const ranks_t  &ranks(size_t i) const { return _ranks[i]; }
    
    inline value_type distance_sq(size_t index_a, const std::vector<value_type> &b) const
    {
        vector_t bv;
        std::copy(b.begin(), b.end(), bv);
        return distance_sq(index_a, bv);
    }
    inline value_type distance_sq(const std::vector<value_type> &a, uint32_t index_b) const { return distance_sq(index_b, a); }
    inline value_type distance_sq(uint32_t index_a, const vector_t &b)                const
    {
        if (USE_CACHE) {
            cache_result_t r = _cache.find(index_a, b);
            if (r.first) return r.second;
            return _cache.put(index_a, b, ::distance_sq(b, _data[index_a]));
        }
        return ::distance_sq(b, _data[index_a]);
    }
    inline value_type distance_sq(const vector_t &a, uint32_t index_b)                  const { return distance_sq(index_b, a); }
    inline value_type distance_sq(uint32_t index_a, uint32_t index_b)                   const
    {
        if (USE_CACHE) {
            cache_result_t r = _cache.find(index_a, index_b);
            if (r.first) return r.second;
            return _cache.put(index_a, index_b, ::distance_sq(_data[index_b], _data[index_a]));
        }
        return ::distance_sq(_data[index_b], _data[index_a]);
    }
    
    inline value_index_t linear_search(const std::vector<value_type> &find) const
    {
        vector_t v;
        std::copy(find.begin(), find.end(), v.begin());
        return linear_search(v);
    }
    
    value_index_t linear_search(const vector_t &find) const
    {
        std::pair<value_type, size_t> minimum(std::numeric_limits<value_type>::max(), 0);
        for (size_t i = 0, sz = _data.size(); i != sz; ++i) {
            std::pair<value_type, size_t> current(::distance_sq(find, _data[i]), i);
            if (current.first < minimum.first) minimum = current;
        }
        return minimum;
    }
    
    value_index_t linear_search_limit(const vector_t &find) const
    {
        std::pair<value_type, size_t> minimum(std::numeric_limits<value_type>::max(), 0);
        for (size_t i = 0, sz = _data.size(); i != sz; ++i) {
            std::pair<value_type, size_t> current(::distance_sq_limit(find, _data[i], minimum.first), i);
            if (current.first < minimum.first) minimum = current;
        }
        return minimum;
    }
    
    value_index_t linear_search_limit_msq(const vector_t &find) const
    {
        std::pair<value_type, size_t> minimum(std::numeric_limits<value_type>::max(), 0);
        value_type msq = ::magnitude_sq(find);
        for (size_t i = 0, sz = _data.size(); i != sz; ++i) {
            if (abs(_ranks[i].magnitude_sq - msq) < minimum.first) {
                std::pair<value_type, size_t> current(::distance_sq_limit(find, _data[i], minimum.first), i);
                if (current.first < minimum.first) minimum = current;
            }
        }
        return minimum;
    }
    /*
    value_index_t linear_search_ranked(const vector_t &find) const
    {
        ranked_t v(find);
        std::pair<value_type, size_t> minimum(::distance_sq(find, _data[0]), 0);
        size_t ranks_total = 0;
        for (size_t i = 1, sz = _ranked.size(); i != sz; ++i) {
            std::pair<value_type, size_t> current(v.distance_sq(_ranked[i], minimum.first), i);
            if (current.first < minimum.first) {
                minimum = current;
                //std::cout << "_d " << sqrt(minimum.first) << " ranks " << (float(ranks_total) / i) << std::endl;
            }
        }
        return minimum;
    }
    
    value_index_t linear_search_ranked_1(const vector_t &find) const
    {
        ranked_t v(find);
        std::pair<value_type, size_t> minimum(::distance_sq(find, _data[0]), 0);
        for (size_t i = 1, sz = _ranked.size(); i != sz; ++i) {
            std::pair<value_type, size_t> current(v.distance_sq_1(_ranked[i], minimum.first), i);
            if (current.first < minimum.first) minimum = current;
        }
        return minimum;
    }
    */
    
    range_t linear_bounds(const vector_t &find) const
    {
        std::pair<value_type, size_t> minimum(std::numeric_limits<value_type>::max(), 0),
                                      maximum(std::numeric_limits<value_type>::min(), 0);
        for (size_t i = 0, sz = _data.size(); i != sz; ++i) {
            std::pair<value_type, size_t> current(::distance_sq(find, _data[i]), i);
            if (current.first < minimum.first) minimum = current;
            if (current.first > maximum.first) maximum = current;
        }
        return range_t(minimum, maximum);
    }
    
    void generate(generator_t &generator, size_t count)
    {
        vector_t u;
        size_t dims = u.size(); // careful this only works with std::array, std::vector here returns 0
        std::cout << "container [" << count << " vectors of dimension " << dims << "]" << std::endl;
        simple_timer t;
        generator.set_size(dims);
        for (size_t i = 0; i < count; i++) {
            const std::vector<value_type> &v = generator.vector();
            std::copy(v.begin(), v.end(), u.begin());
            add(u);
        }
        ranked_vectors().sort();
        t.print_elapsed("container generate + store vectors");
    }
};

template <int M, typename FloatT>
void create_test_container(container<M, FloatT> &store, size_t count)
{
    vector_generator<FloatT> generator(M);
    store.generate(generator, count);
}
