#pragma once

#include <vector>
#include <cstddef>

#include "stl.h"
#include "slice.h"
#include "value_index_t.h"

template <typename SliceT>
struct slice_cardinality
{
    typedef SliceT                    slice_type;
    typedef std::pair<size_t, size_t> value_type;

private:

    std::vector<value_type>        _indices;
    std::vector<size_t>            _reverse;
       
public:

    inline slice_cardinality() {}
    inline slice_cardinality(const std::vector<slice_type> &slices) { compute(slices); }
    
    inline size_t size()                        const { return _indices.size(); }
    inline size_t dimension(size_t ordinal)     const { return _index(_indices[ordinal]); }
    inline size_t ordinal(size_t dimension)     const { return _reverse[dimension]; }
    inline size_t cardinality(size_t dimension) const { return _value(_indices[ordinal(dimension)]); }
    
    void compute(const std::vector<slice_type> &slices)
    {
        _indices.resize(slices.size());
        _reverse.resize(slices.size());
        for (size_t i = 0, sz = slices.size(); i != sz; ++i)
            _indices[i] = value_type(slices[i].float_index().cardinality(), i);
        pair_compare_first_greater<value_type> cr;
        std::sort(_indices.begin(), _indices.end(), cr);
        size_t ordinal = 0;
        for (auto e : _indices) _reverse[_index(e)] = ordinal++;
    }
};
