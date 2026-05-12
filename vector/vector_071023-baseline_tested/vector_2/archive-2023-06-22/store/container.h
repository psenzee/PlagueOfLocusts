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
    typedef FloatT                                    value_type;
    typedef std::array<FloatT, M>                     vector_t;
    typedef std::vector<vector_t>                     container_t;
    typedef std::pair<value_type, size_t>             value_index_t;
    typedef std::pair<value_index_t, value_index_t>   range_t;
    typedef vector_generator<float_t>                 generator_t;
    
    static constexpr int  COUNT = M;

    container_t _data;
    
    inline container &add(const vector_t &v) { _data.push_back(v); return *this; }
    inline size_t     size() const { return _data.size(); }
    
    inline container &finish_add() { return *this; }
    
    inline const vector_t &at(size_t i)    const { return _data[i]; }

    inline value_index_t linear_search(const std::vector<value_type> &find) const
    {
        vector_t v;
        std::copy(find.begin(), find.end(), v.begin());
        return linear_search(v);
    }
    
    inline value_type distance_sq(const vector_t &find, size_t index) const { return ::distance_sq(find, _data[index]); }
    inline value_type distance(const vector_t &find, size_t index)    const { return sqrt(distance_sq(find, index)); }
    
    value_index_t linear_search(const vector_t &find) const
    {
        std::pair<value_type, size_t> minimum(std::numeric_limits<value_type>::max(), 0);
        for (size_t i = 0, sz = _data.size(); i != sz; ++i) {
            std::pair<value_type, size_t> current(::distance_sq_limit(find, _data[i], minimum.first), i);
            if (current.first < minimum.first) minimum = current;
        }
        return std::pair<value_type, size_t>(sqrt(minimum.first), minimum.second);
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
        t.print_elapsed("container generate + store vectors");
    }
};

template <int M, typename FloatT>
void create_test_container(container<M, FloatT> &store, size_t count)
{
    vector_generator<FloatT> generator(M);
    store.generate(generator, count);
}
