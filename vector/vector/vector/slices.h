#pragma once

#include <iostream>
#include <limits>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstddef>

#include "stl.h"
#include "slice.h"
#include "slice_cardinality.h"

template <typename SliceT>
struct slices
{
    typedef SliceT                         slice_type;
    typedef typename SliceT::value_type    scalar_type;
    typedef std::pair<scalar_type, size_t> elem_type;

private:
    
    SliceT                            _magnitudes;
    std::vector<SliceT>               _slices;
    slice_cardinality<SliceT>         _cardinality;
    size_t                            _dimensions;
    bool                              _indexed;

    mutable std::vector<scalar_type>  _median;
    mutable std::vector<scalar_type>  _mean;
    mutable std::vector<scalar_type>  _zeros;
    mutable std::vector<scalar_type>  _minimum;
    mutable std::vector<scalar_type>  _maximum;
    
    std::ostream &print(std::ostream &os, const std::vector<scalar_type> &v) const
    {
        os << "[";
        for (auto e : v) os << " " << e;
        return os << "]";
    }

public:

    slices(size_t dimensions) : _dimensions(dimensions), _indexed(false)
    {
        _slices.resize(dimensions);
    }
    
    void add_invalidate(const std::vector<scalar_type> &v)
    {
        _magnitudes.add_invalidate(sqrt(magnitude_sq(v)));
        for (size_t i = 0, sz = _slices.size(); i != sz; ++i)
            _slices[i].add_invalidate(v[i]);
        _indexed = false;
    }
    
    const std::vector<scalar_type> &median() const
    {
        if (_median.size() < size()) _median.resize(size());
        for (size_t i = 0, sz = size(); i < sz; i++)
            _median[i] = _slices[i].float_index().median();
        return _median;
    }
    
    const std::vector<scalar_type> &mean() const
    {
        if (_mean.size() < size()) _mean.resize(size());
        for (size_t i = 0, sz = size(); i < sz; i++)
            _mean[i] = _slices[i].float_index().mean();
        return _mean;
    }
    
    const std::vector<scalar_type> &minimum() const
    {
        if (_minimum.size() < size()) _minimum.resize(size());
        for (size_t i = 0, sz = size(); i < sz; i++)
            _minimum[i] = _slices[i].float_index().minimum();
        return _minimum;
    }
    
    const std::vector<scalar_type> &maximum() const
    {
        if (_maximum.size() < size()) _maximum.resize(size());
        for (size_t i = 0, sz = size(); i < sz; i++)
            _maximum[i] = _slices[i].float_index().maximum();
        return _maximum;
    }
    
    const std::vector<scalar_type> &zeros() const
    {
        if (_zeros.size() < size()) _zeros.resize(size());
        for (size_t i = 0, sz = size(); i < sz; i++)
            _zeros[i] = 0.0;
        return _zeros;
    }

    inline scalar_type distance(size_t index, const std::vector<scalar_type> &r, std::vector<scalar_type> &v) const
    {
        get(index, v);
        subtract(v, r);
        return sqrt(magnitude_sq(v));
    }

    inline scalar_type distance(size_t index, const std::vector<scalar_type> &r) const
    {
        std::vector<scalar_type> v;
        return distance(index, r, v);
    }

    bool is_indexed() const
    {
        if (!_indexed) return false;
        bool valid = true;
        for (size_t i = 0, sz = _dimensions; i != sz; ++i) {
            if (!_slices[i].is_indexed()) {
                valid = false;
                break;
            }
        }
        return valid;
    }

    inline size_t        size()          const { return _slices.size(); }
    inline const SliceT &magnitudes()    const { return _magnitudes; }
    inline const SliceT &slice(size_t i) const { return _slices[i]; }
    
    void get(size_t index, std::vector<scalar_type> &into) const
    {
        if (into.size() < size())
            into.resize(size());
        for (size_t i = 0, sz = size(); i != sz; ++i)
            into[i] = _slices[i].at(index);
    }
    
    std::ostream &print(std::ostream &os) const
    {
        os << std::endl << "----" << std::endl;
        os << "min    "; print(os, minimum()) << std::endl;
        os << "mean   "; print(os, mean()) << std::endl;
        os << "median "; print(os, median()) << std::endl;
        os << "max    "; print(os, maximum()) << std::endl;
        _magnitudes.print(os, "magn");
        char buffer[128];
        int pad = 3;
        int dsz = int(size());
        for (size_t i = 0; i < dsz; i++) {
            if (dsz <= 8 || i < pad || i >= dsz - pad) {
                sprintf(buffer, "%4lu", i);
                _slices[i].print(os, buffer);
            }
            if (dsz > 8 && i == pad) std::cout << "..." << std::endl;
        }
        return os << "----" << std::endl;
    }

    void index()
    {
        if (is_indexed())
            return;
        for (size_t i = 0, sz = size(); i != sz; ++i)
            _slices[i].index();
        _magnitudes.index();
        _cardinality.compute(_slices);
        _indexed = true;
    }
};

void test_slices(size_t dims, size_t count);
