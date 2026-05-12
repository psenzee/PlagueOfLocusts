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
struct real_index
{
    typedef FloatT                                            real_type;
    typedef IndexT                                            index_type;
    typedef std::pair<FloatT, IndexT>                         value_type;
    typedef std::vector<value_type>                           container_type;
    typedef compare_nth<value_type, 0, std::less<FloatT> >    compare_up;
    typedef compare_nth<value_type, 0, std::greater<FloatT> > compare_down;
    typedef typename container_type::const_iterator           const_iterator;
    typedef std::vector<float_type>                           in_vector_type;

    inline real_index() : _sum(0.0), _minimum(_fmax()), _maximum(_fmin()) { invalidate(); }
    inline real_index(const in_vector_type &v) : _sum(0.0), _minimum(_fmax()), _maximum(_fmin()) { invalidate(); }
   
    inline real_type  mean()               const { return _sum / _data.size(); }
    inline real_type  minimum()            const { return _minimum; }
    inline real_type  maximum()            const { return _maximum; }
    inline real_type  range()              const { return _maximum - _minimum; }
    inline real_type  midrange()           const { return range() * 0.5; }
    inline real_type  variance()           const { return (isnan(_variance)) ? _calculate_variance() : _variance; }
    inline real_type  standard_deviation() const { return sqrt(variance()); }
    inline real_type  median()             const { return (isnan(_median) && _indexed) ? _calculate_median() : _median; }
    inline int        cardinality()        const { return (_cardinality == -1 && _indexed) ? _calculate_cardinality() : _cardinality; }
    inline bool       is_sorted()          const { return _sorted; }
    inline auto       begin()              const { return _data.begin(); }
    inline auto       end()                const { return _data.end(); }
    inline void       invalidate()               { _sorted = false; _variance = _median = _fqnan(); _cardinality = -1; }
    inline auto      &data()               const { return _data; }
    inline auto      &data()                     { return _data; }
    inline auto       clear()                    { invalidate(); _data.clear(); }

    void insert(const in_vector_type &v, in_vector_type::const_iterator b, in_vector_type::const_iterator e)
    {
        invalidate();
        size_t count = b - v.begin();
        for (; b != e; ++b) _data.push_back(value_type(*b, count++));
        sort();
    }
    
    inline void add(const real_type &value, index_type index)
    {
        _data.push_back(value_type(value, index));
        _minimum = std::min(_minimum, value);
        _maximum = std::max(_maximum, value);
        _sum += value;
        invalidate(); // reset everything that needs it
    }
    
    void sort()
    {
        if (!_sorted) {
            invalidate();
            std::sort(_data.begin(), _data.end(), compare_up());
            _sorted = true;
        }
    }

    std::ostream &print(std::ostream &os) const
    {
        os << "index";
        if (!_sorted) os << " [unindexed]";
        return os << " min " << minimum() << " max " << maximum() << " range " << range()
                  << " mean " << mean() << " midrange " << midrange() << " median " << median()
                  << " std dev " << standard_deviation() << " cardinality " << cardinality() << std::endl;
    }
    
private:

    container_type _data;
    real_type _minimum, _maximum, _sum;
    mutable real_type _variance, _median;
    mutable int _cardinality;
    bool _sorted;
    
    inline static FloatT _fqnan() { return std::numeric_limits<FloatT>::quiet_NaN(); }
    inline static FloatT _fmax()  { return std::numeric_limits<FloatT>::max(); }
    inline static FloatT _fmin()  { return std::numeric_limits<FloatT>::min(); }
    
    real_type _calculate_variance() const
    {
        if (_data.size() <= 1)
            return real_type(0);
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
inline std::ostream &operator<<(std::ostream &os, const real_index<FloatT, IndexT> &index) { return index.print(os); }

