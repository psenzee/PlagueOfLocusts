#pragma once

#include <iostream>
#include <limits>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstddef>

#include "simple_timer.h"
#include "btree_map.h"
#include "double_double.h"
#include "exponent_histogram.h"
#include "element.h"
#include "sorter.h"

//  vector<element> is the naive type for an index of one single dimension

template <typename FloatT, int N>
class vector_index
{
public:

    typedef double                                   float_t;
    typedef element<float_t, uint32_t>               index_element;
    typedef std::array<FloatT, N>                    fixed_vector;
    typedef btree::btree_map<float_t, index_element> btree;

private:

    std::array<btree, N>      _indices;
    std::vector<fixed_vector> _data;

public:

    inline void add(const fixed_vector &v)
    {
        size_t index = v.size(), dim = 0;
        _data.push_back(v);
        for (auto i = v.begin(), e = v.end(); i != e; ++i) {
            index_element ne = { index, *i };
            /*
            ne.index = index;
            ne.value = *i;
            */
            btree &bt = _indices[dim++];
            bt[*i] = ne;
        }
    }
    
    inline float_t dot(const fixed_vector &a, const fixed_vector &b)
    {
        float_t sum = 0;
        for (size_t i = 0; i < N; i++) {
            float_t v = b[i] - a[i];
            sum += v * v;
        }
        return sum;
    }
    
    inline float_t distance_euclidean(const fixed_vector &a, const fixed_vector &b)
    {
        return sqrt(dot(a, b));
    }
    
    inline void find_range(const fixed_vector &fv, size_t dimension, const std::pair<float_t, float_t> &range = std::pair<float_t, float_t>(0.0, 0.0))
    {
        btree &bt = _indices[dimension];
        float_t v = fv[dimension];
        auto    lo   = bt.lower_bound(v + range.first),
                hi   = bt.upper_bound(v + range.second),
                mini = bt.end();
        if (lo == bt.end()) {
            return; //...
        }
        float_t min  = std::numeric_limits<float_t>::max();
        for (; lo != hi && lo != mini; ++lo) {
            float_t dist = distance_euclidean(fv, *lo);
            if (dist < min) {
                mini = lo;
                min = dist;
            }
        }
    }
};
