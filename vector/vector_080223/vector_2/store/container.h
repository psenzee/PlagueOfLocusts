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
#include "generators.h"
#include "fixed_vector.h"

template <int M, typename FloatT>
struct container : public fixed_vector::container<M, FloatT>
{
    typedef typename fixed_vector::traits<M, FloatT>  traits_t;
    typedef FloatT                                    value_type;
    typedef std::array<FloatT, M>                     vector_t;
    typedef std::vector<value_type>                   dynamic_t;
    typedef std::vector<vector_t>                     container_t;
    typedef std::pair<value_type, size_t>             value_index_t;
    typedef std::pair<value_index_t, value_index_t>   range_t;
    typedef vector_generator<float_t>                 generator_t;
    typedef container<M, value_type>                  self_t;

    static constexpr int COUNT = M;

    container_t _data;
    
    inline container      &add(const vector_t &v)                                                                { _data.push_back(v); return *this; }
    inline container      &add(const dynamic_t &v)                                                               { return add(traits_t::fixed(v)); }
    inline size_t          size()                                                                          const { return _data.size(); }
    inline container      &finish_add()                                                                          { return *this; }
    inline const vector_t &at(size_t i)                                                                    const { return _data[i]; }
    
//  inline static std::ostream &print(std::ostream &os, const vector_t &v)                                       { return traits_t::print(os, v); }
//  inline static std::ostream &print(std::ostream &os, const dynamic_t &v)                                      { return traits_t::print(os, v); }
    inline std::ostream   &print(std::ostream &os, size_t index, int count = -1)                           const { return traits_t::print(os, _data[index], count); }
     
    inline value_type      distance_sq(const vector_t &find, size_t index)                                 const { return ::distance_sq(find, _data[index]); }
    inline value_type      distance_sq(const dynamic_t &find, size_t index)                                const { return ::distance_sq(find, _data[index]); }
    inline value_type      distance_sq(size_t index_a, size_t index_b)                                     const { return ::distance_sq(_data[index_a], _data[index_b]); }
    
    inline value_type      distance_sq_limit(const vector_t &find, size_t index, value_type limit_sq)      const { return ::distance_sq_limit(find, _data[index], limit_sq); }
    inline value_type      distance_sq_limit(const dynamic_t &find, size_t index, value_type limit_sq)     const { return ::distance_sq_limit(find, _data[index], limit_sq); }
    inline value_type      distance_sq_limit(size_t index_a, size_t index_b, value_type limit_sq)          const { return ::distance_sq_limit(_data[index_a], _data[index_b], limit_sq); }

    inline value_type      distance(const vector_t &find, size_t index)                                    const { return sqrt(distance_sq(find, index)); }
    inline value_type      distance(const dynamic_t &find, size_t index)                                   const { return sqrt(distance_sq(find, index)); }
    inline value_type      distance(size_t index_a, size_t index_b)                                        const { return sqrt(distance_sq(index_a, index_b)); }

    value_index_t linear_search(const vector_t &find) const
    {
        value_index_t minimum_sq(std::numeric_limits<value_type>::max(), 0);
        for (size_t i = 0, sz = _data.size(); i != sz; ++i) {
            value_index_t current(distance_sq_limit(find, i, minimum_sq.first), i);
            if (current.first < minimum_sq.first) minimum_sq = current;
        }
        return std::pair<value_type, size_t>(sqrt(minimum_sq.first), minimum_sq.second);
    }

    inline value_index_t linear_search(const dynamic_t &find) const { return linear_search(traits_t::fixed(find)); }

    void generate(generator_t &generator, size_t count)
    {
        std::cout << "container [" << count << " vectors of dimension " << COUNT << "]" << std::endl;
        simple_timer t;
        generator.set_size(COUNT);
        for (size_t i = 0; i < count; i++) add(generator.vector());
        t.print_elapsed("container generate + store vectors");
    }
};

template <int M, typename FloatT>
void create_test_container(container<M, FloatT> &store, size_t count)
{
    vector_generator<FloatT> generator(M);
    store.generate(generator, count);
}
