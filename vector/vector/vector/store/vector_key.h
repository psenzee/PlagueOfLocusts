#pragma once

#include <limits>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>
#include <cstddef>

#include "stl.h"

template <typename VectorT, int RanksN>
struct vector_key
{
    typedef VectorT                        vector_t;
    typedef typename VectorT::FloatT       value_type;
    typedef std::pair<value_type, int16_t> value_index_t;
    typedef std::array<uint16_t, RanksN>   ranks_t;
       
    uint64_t   id;
    value_type slope,
               intercept,
               norm;
    ranks_t    ranks;
    
    // size is:
    // full id (8) + slope (4) + intercept (4) + norm (4) + ranks (2 * RanksN) = 20 + 2 * RanksN
    // no id                                                                   = 12 + 2 * RanksN
    // no id, bfloat16 for slope, intercept and norm                           =  6 + 2 * RanksN
    // no id, frexp for slope, intercept and norm                              =  3 + 2 * RanksN
    
    vector_key(const vector_t &in) : id(0), slope(0), intercept(0), norm(0)
    {
        std::vector<value_index_t> &r = _buffer;
        //std::vector<value_index_t> r; // use this instead to make thread-safe without TLS, tho marginally slower
        size_t n = in.size;
        r.resize(n);
        value_type sum_a(0), sum_b(0), sum_ab(0), sum_aa(0), sum_sq(0), val;
        // calculate regression & norm
        for (size_t i = 0; i != n; ++i) {
            value_type val(in[i]), ai(i), bi(val);
            r[i] = value_index_t(val, (val < 0 ? -1 : 1) * int16_t(i));
            sum_a += ai; sum_b += bi; sum_sq += bi * bi;
            sum_ab += ai * bi; sum_aa += ai * ai;
        }
        norm = sqrt(sum_sq);
        slope = (n * sum_ab - sum_a * sum_b) / (n * sum_aa - sum_a * sum_a);
        intercept = (sum_b - slope * sum_a) / n;
        pair_compare_first_greater_abs_second_less<value_index_t> c;
        std::nth_element(r.begin(), r.begin() + RanksN, r.end(), c);
        std::sort(r.begin(), r.begin() + RanksN, c);
        for (size_t i = 0; i != RanksN; ++i) ranks[i] = r[i].second;
    }

private:
    static std::vector<value_index_t> _buffer; // use TLS to make thread-safe
};
