#pragma once

#include <iostream>
#include <limits>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>

template <typename FloatT, typename ValueT>
struct exponent_histogram
{
    enum
    {
        MIN_EXP = std::numeric_limits<FloatT>::min_exponent,
        MAX_EXP = std::numeric_limits<FloatT>::max_exponent,
        COUNT   = (MAX_EXP - MIN_EXP) + 3,
    };

    std::array<ValueT, COUNT * 2> data;
    
    inline size_t        size() const                  { return data.size(); }
    inline void          clear_to(const ValueT &v)     { data = { v }; }
    inline size_t        index(const FloatT &v)  const { int e; frexp(v, &e); e += -MIN_EXP; return COUNT + (v < 0 ? -e : e); }
    
    inline ValueT       &bucket(const FloatT &v)       { return data[index(v)]; }
    inline const ValueT &bucket(const FloatT &v) const { return data[index(v)]; }
    
    inline std::ostream &print(std::ostream &os) const
    {
        return os << "[exponent_histogram min " << MIN_EXP << " max " << MAX_EXP << " count " << COUNT << "]";
    }
};


template <typename FloatT, typename IndexT>
inline std::ostream &operator<<(std::ostream &os, const exponent_histogram<FloatT, IndexT> &eh) { return eh.print(os); }
