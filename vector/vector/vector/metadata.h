#pragma once

#include <iostream>
#include <limits>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>

#include "stl.h"
#include "dct.h"
#include "value_index_t.h"
#include "stat_key.h"

template <typename Value> inline std::ostream &print_array(std::ostream &os, const std::vector<Value> &v, size_t count) { return _stream_out(os, v, count); }
template <typename Value, int N> inline std::ostream &print_array(std::ostream &os, const std::array<Value, N> &v, size_t count) { return _stream_out(os, v, count); }
template <typename Value> inline std::ostream &print_array(std::ostream &os, const valid_t<Value> &v, size_t count) { if (v.valid) return _stream_out(os, *v, count); return os << v; }

template <typename VectorT>
struct metadata
{
    typedef VectorT                       vector_t;
    typedef typename VectorT::value_type  value_type;
    typedef std::pair<value_type, size_t> value_index_t;
    typedef std::vector<value_index_t>    index_t;
    typedef std::array<value_index_t, 2>  median_pair_t;
    
    enum { KEY_LENGTH = 8 };

    vector_t                        _data;
    valid_t<vector_t>               _dct, _idct;
    valid_t<index_t>                _ranked;
    valid_t<index_t>                _indices;
    valid_t<value_type>             _sum, _mean, _maximum, _minimum,
                                    _range, _midrange,
                                    _variance, _skewness, _kurtosis,
                                    _slope, _intercept,
                                    _magnitude_sq, _magnitude,
                                    _flatness, _density, _peaked;
    valid_t<value_index_t>          _mode;
    valid_t<median_pair_t>          _median;
    valid_t<stat_key>               _key;
    bool                            _indexed;

    inline metadata(const vector_t &data) : _data(data), _indexed(false) { calculate(); }

    void calculate()
    {
        if (!_indexed) {
            _calculate_index();
            _calculate_regression();
            _calculate_statistics();
            _calculate_geometrics();
            _calculate_dct();
            _calculate_idct();
            _calculate_mode();
            _calculate_flatness();
            _calculate_density();
            if (_density.valid && _peaked.valid && _flatness.valid && _ranked.valid) {
                const auto &r = (*_ranked)[0];
                (*_key)
                    .set_type(stat_key::TYPE_RDPFM)
                    .set_rank(_index(r), _value(r))
                    .set_density(*_density)
                    .set_peaked(*_peaked)
                    .set_flatness(*_flatness)
                    .set_magnitude(*_magnitude);
                _key.valid = true;
            }
            _indexed = true;
        }
    }
    
    std::ostream &print(std::ostream &os)
    {
        os << std::fixed;
        if (!_indexed)
            return _stream_out(os << "data: [unindexed] ", _data,   8) << std::endl;

        valid_t<value_type> range, midrange, stddev, magnitude, median, nonmode;
        if (_maximum.valid && _minimum.valid) {
            range = *_maximum - *_minimum;
            midrange = *range * 0.5;
        }
        if (_magnitude_sq.valid)
            magnitude = sqrt(*_magnitude_sq);
        if (_variance.valid)
            stddev = sqrt(*_variance);
        if (_median.valid)
            median = ((*_median)[0].first + (*_median)[1].first) * 0.5;
        if (_mode.valid)
            nonmode = _data.size() - _index(*_mode);

        os << std::fixed << std::endl << std::setprecision(3)
           << "data:             "; print_array(os, _data,  8)   << std::endl
           << "  count:          " << _data.size()               << std::endl
           << "  mean:           " << _mean                      << std::endl
           << "  median val:     " << median                     << std::endl
           << "  sum:            " << _sum                       << std::endl
           << "  minimum:        " << _minimum                   << std::endl
           << "  maximum:        " << _maximum                   << std::endl
           << "  range:          " << range                      << std::endl
           << "  midrange:       " << midrange                   << std::endl
           << "  variance:       " << _variance                  << std::endl
           << "  std dev:        " << stddev                     << std::endl
           << "  magnitude:      " << magnitude                  << std::endl
           << "  skewness:       " << _skewness                  << std::endl
           << "  kurtosis:       " << _kurtosis                  << std::endl
           << "  slope:          " << _slope                     << std::endl
           << "  intercept:      " << _intercept                 << std::endl
           << "  mode:           " << _mode                      << std::endl
           << "  nonmode:        " << nonmode                    << std::endl
           << "  flatness:       " << _flatness                  << std::endl
           << "  density:        " << _density                   << std::endl
           << "  peaked:         " << _peaked                    << std::endl
           << "  key:            " << _key                       << std::endl
           << "  dct:            "; print_array(os, _dct,     8) << std::endl
           << "  idct:           "; print_array(os, _idct,    8) << std::endl
           << "  index:          "; print_array(os, _indices, 6) << std::endl
           << "  ranked:         "; print_array(os, _ranked,  6) << std::endl;
        return os;
    }

private:
    
    std::ostream &_print_key(std::ostream &os, const std::array<uint8_t, KEY_LENGTH> &key) const
    {
        os << std::hex << std::setfill('0');
        for (auto v : _key) os << std::setw(2) << uint32_t(v) << " ";
        return os << std::dec;
    }
    
    void _calculate_index()
    {
        // calc _index
        size_t sz = _data.size();
        index_t &index = _indices.data;
        index.resize(sz);
        for (size_t i = 0; i != sz; ++i) index[i] = value_index_t(_data[i], i);
        std::sort(index.begin(), index.end(), pair_compare_first_greater_second_less<value_index_t>());
        _indices.valid = true;

        // calc _median
        _median = median_pair_t({ index[sz / 2 - ((sz & 1) ? 0 : 1)], index[sz / 2] });

        // calc _ranked
        _ranked = _indices;
        pair_compare_first_greater_abs_second_less<value_index_t> compare;
        std::sort((*_ranked).begin(), (*_ranked).end(), compare);
    }
    
    void _calculate_regression()
    {
        if (_indices.valid) {
            std::pair<value_type, value_type> ls = least_squares_reversed<value_type, size_t>(*_indices);
            _slope = ls.first;
            _intercept = ls.second;
        }
    }
    
    void _calculate_geometrics()
    {
        value_type mag_sq = magnitude_sq(_data), mag = sqrt(mag_sq);
        _magnitude_sq = mag_sq;
        _magnitude = mag;
        if (_ranked.valid)
            _peaked = _calculate_peaked(mag, *_ranked);
    }
    
    void _calculate_dct()
    {
        compute_dct(_data, *_dct);
        _dct.valid = true;
    }
    
    void _calculate_idct()
    {
        if (_dct.valid) {
            compute_idct(*_dct, *_idct);
            _idct.valid = true;
        }
    }
    
    void _calculate_statistics()
    {
        value_type sum(0), max(std::numeric_limits<value_type>::min()), min(std::numeric_limits<value_type>::max());
        for (auto v : _data) { max = std::max(v, max); min = std::min(v, min); sum += v; }
        _sum      = sum;
        _minimum  = min;
        _maximum  = max;
        _range    = max - min;
        _midrange = (max - min) * 0.5;
        _mean     = sum / _data.size();
        _variance = variance(_data);
        _skewness = skewness(_data);
        _kurtosis = kurtosis(_data);
    }
    
    void _calculate_flatness()
    {
        if (_indices.valid && _magnitude_sq.valid) {
            const index_t &r = *_ranked;
            value_type msq = *_magnitude_sq;
            size_t sz = r.size(), i = 0;
            for (; i != sz; ++i)
                if (_value(r[i]) * _value(r[i]) * r.size() <= msq)
                    break;
            value_type flat = std::min(i / (sz * 0.5), 1.0), min_flat = 1.0 / (sz * 0.5), max_flat = 1.0;
            flat -= min_flat;
            flat /= max_flat - min_flat;
            _flatness = flat;
        }
    }
    
    void _calculate_mode()
    {
        if (_indices.valid) {
            value_index_t run = value_index_t(_value((*_indices)[0]), 0), max_run(run);
            for (auto p : *_indices) {
                if (_value(p) == _value(run))
                    run.second++;
                else
                    run = value_index_t(_value(p), 1);
                if (_index(run) > _index(max_run))
                    max_run = run;
            }
            _mode = (_index(max_run) == 0) ? value_index_t(value_type(0), 0) : max_run;
        }
    }
    
    void _calculate_density()
    {
        if (_indices.valid && _mode.valid) {
            size_t sz = (*_indices).size();
            _density = (sz - _index(*_mode)) / value_type(sz - 1);
        }
    }
    
    value_type _calculate_peaked(value_type magnitude, const index_t &ranked)
    {
        const value_type z(0), n(ranked.size());
        if (magnitude == z) return z;
        value_type val(_value(ranked[0]));
        val /= magnitude; // normalize
        return abs(val) - (1 / sqrt(n));
    }
};
