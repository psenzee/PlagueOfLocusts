#pragma once

#include <iostream>
#include <limits>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstddef>

#include "stl.h"
#include "value_index_t.h"

template <typename FloatT = float, typename IndexT = uint32_t>
struct float_index
{
    typedef FloatT                                            real_type;
    typedef IndexT                                            index_type;
    typedef std::pair<FloatT, IndexT>                         value_type;
    typedef std::vector<value_type>                           container_type;
    typedef compare_nth<value_type, 0, std::less<FloatT> >    compare_up;
    typedef compare_nth<value_type, 0, std::greater<FloatT> > compare_down;
    typedef typename container_type::const_iterator           const_iterator;

    inline float_index() : _sum(0.0), _minimum(_fmax()), _maximum(_fmin()) { invalidate(); }
   
    inline real_type mean()               const { return _sum / _data.size(); }
    inline real_type minimum()            const { return _minimum; }
    inline real_type maximum()            const { return _maximum; }
    inline real_type range()              const { return _maximum - _minimum; }
    inline real_type midrange()           const { return range() * 0.5; }
    inline real_type variance()           const { return (isnan(_variance)) ? _calculate_variance() : _variance; }
    inline real_type median()             const { return (isnan(_median) && _indexed) ? _calculate_median() : _median; }
    inline int       cardinality()        const { return (_cardinality == -1 && _indexed) ? _calculate_cardinality() : _cardinality; }
    inline real_type standard_deviation() const { return sqrt(variance()); }
    inline bool      is_indexed()         const { return _indexed; }
    inline auto      begin()              const { return _data.begin(); }
    inline auto      end()                const { return _data.end(); }
    inline void      invalidate()               { _indexed = false; _variance = _median = _fqnan(); _cardinality = -1; }
    inline auto      &data()              const { return _data; }
    inline auto      &data()                    { return _data; }
    
    void inflections(std::vector<int> &v) const
    {
        std::vector<float_t> deltas, results;
        deltas.resize(_data.size());
        results.resize(_data.size());
        deltas[0] = float_t(0.0);
        for (size_t i = 1, sz = _data.size(); i != sz; ++i)
            deltas[i] = _value(_data[i]) - _value(_data[i - 1]);
        // here we want some kind of guassian? currently just eyeballing it.  I should probably use the formula tho
        std::array<float_t, 9> window({ 0.1, 0.3, 0.6, 0.8, 1.0, 0.8, 0.6, 0.3, 0.1 });
        // normalize - make it sum to 1.0
        float_t sum(0.0);
        for (auto v : window) sum += v;
        for (size_t k = 0, ksz = window.size(); k != ksz; ++k) window[k] /= sum;
        size_t half = window.size() / 2;
        for (size_t i = half, sz = deltas.size() - half; i != sz; ++i) {
            float_t sum = float_t(0.0);
            for (size_t k = 0, ksz = window.size(); k != ksz; ++k)
                sum += deltas[i + k - half] * window[k];
            results[i] = sum;
        }
        // now find the peaks/troughs; inflection points
    }

    inline void add_invalidate(real_type value)
    {
        _data.push_back(value_type(value, _data.size()));
        _minimum = std::min(_minimum, value);
        _maximum = std::max(_maximum, value);
        _sum += value;
        invalidate(); // reset everything that needs it
    }
    
    void index()
    {
        if (!_indexed) {
            invalidate(); // reset everything that needs it
            std::sort(_data.begin(), _data.end(), compare_up());
            _indexed = true;
        }
    }

    std::ostream &print(std::ostream &os) const
    {
        os << "index";
        if (!_indexed) os << " [unindexed]";
        return os << " min " << minimum() << " max " << maximum() << " range " << range()
                  << " mean " << mean() << " midrange " << midrange() << " median " << median()
                  << " stddev " << standard_deviation() << " cardinality " << cardinality() << std::endl;
    }
    
    void reverse_index(std::vector<IndexT> &reverse)
    {
        reverse.resize(_data.size());
        for (index_type i = 0, sz = index_type(_data.size()); i != sz; i++) {
            assert(_data[i].second < sz);
            reverse[_data[i].second] = i;
        }
    }
    
private:

    container_type _data;
    real_type _minimum, _maximum, _sum;
    mutable real_type _variance, _median;
    mutable int _cardinality;
    bool _indexed;
    
    inline static FloatT _fqnan() { return std::numeric_limits<FloatT>::quiet_NaN(); }
    inline static FloatT _fmax()  { return std::numeric_limits<FloatT>::max(); }
    inline static FloatT _fmin()  { return std::numeric_limits<FloatT>::min(); }
    
    real_type _calculate_variance() const
    {
        if (_data.size() <= 1) {
            return real_type(0);
        }
        real_type mn = mean(), sumsq = real_type(0);
        for (auto i = _data.begin(), e = _data.end(); i != e; ++i) {
            real_type d(_value(*i) - mn);
            sumsq += d * d;
        }
        return _variance = sumsq / (_data.size() - 1);
    }
    
    real_type _calculate_median() const
    {
        // requires that this list be sorted/indexed
        size_t sz = _data.size(), half = sz / 2;
        real_type m = _value(_data[half]);
        if (sz == half + half) {
            // average the two if it's even
            m = (m + _value(_data[half - 1])) * FloatT(0.5);
        }
        return _median = m;
    }
    
    int _calculate_cardinality() const
    {
        _cardinality = int(count_unique_on_sorted(_data.begin(), _data.end(), compare_up()));
        return _cardinality;
    }
    
    // todo: calculate other statistical moments & other info
};

template <typename FloatT = float, typename IndexT = uint32_t>
inline std::ostream &operator<<(std::ostream &os, const float_index<FloatT, IndexT> &index) { return index.print(os); }

//std::accumulate( mpg.begin(), mpg.end(), 0.0 )/ mpg.size()
