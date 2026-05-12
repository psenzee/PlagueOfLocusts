#pragma once

#include <iostream>
#include <limits>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>

#include "simple_timer.h"
#include "btree_map.h"
#include "double_double.h"
#include "exponent_histogram.h"
#include "element.h"

template <typename FloatT, typename IndexT>
class sorter
{
    typedef element<FloatT, IndexT> element_type;

public:
    
    enum { PRECOUNT = 1 };

    // this is about 40%-50% slower than native_sort at multiple n :/
    void sort(const std::vector<element_type> &in, std::vector<element_type> &out)
    {
        out.resize(0);
        exponent_histogram<FloatT, IndexT> counts;
        if (PRECOUNT) {
            std::cout << "precount ";
            simple_timer st;
            counts.clear_to(0.0);
            for (auto i = in.begin(), e = in.end(); i != e; ++i)
                counts.bucket((*i).value)++;
            st.print_elapsed();
        }
        exponent_histogram<FloatT, std::vector<element_type> > buckets;
        std::cout << "bucketing ";
        simple_timer st0;
        for (auto i = in.begin(), e = in.end(); i != e; ++i) {
            const FloatT &v = (*i).value;
            std::vector<element_type> &ve = buckets.bucket(v);
            if (PRECOUNT) {
                if (ve.size() == 0)
                    ve.reserve(counts.bucket(v));
            }
            ve.push_back(*i);
        }
        st0.print_elapsed();
        std::cout << "reserve/resize ";
        simple_timer st1;
        //out.reserve(in.size());
        out.resize(in.size());
        st1.print_elapsed();
        typename element_type::compare comparator;
        size_t count = 0;
        size_t n = 0;
        simple_timer st2;
        for (auto i = buckets.data.begin(), e = buckets.data.end(); i != e; ++i) {
            std::sort((*i).begin(), (*i).end(), comparator);
            if ((*i).size() > 0) {
                std::cout << "size[" << count << "] " << (*i).size() << std::endl;
            }
            // todo use inserter
            for (auto k = (*i).begin(), ke = (*i).end(); k != ke; ++k)
                out[n++] = *k;
                //out.push_back(*k);
            count++;
        }
        std::cout << "refill ";
        st2.print_elapsed();
    }
    
    void native_sort(const std::vector<element_type> &in, std::vector<element_type> &out)
    {
        out = in;
        typename element_type::compare_reverse comparator;
        std::sort(out.begin(), out.end(), comparator);
    }
    
    void native_abs_sort(const std::vector<element_type> &in, std::vector<element_type> &out)
    {
        out = in;
        typename element_type::compare_abs comparator;
        std::sort(out.begin(), out.end(), comparator);
    }
};

int test_sorter()
{
    typedef double float_t;
    typedef element<float_t, uint32_t> element_type;
    
    auto st = sorter<float_t, uint32_t>();
    std::vector<element_type> in, out;
    //float_t factor = float_t(0.5/*0125*/);
    for (uint32_t i = 0; i < 10000000; i++) {
        float_t factor = pow(2, (rand() % 16) - (rand() % 16));
        element_type e(i, ((rand() % 2) == 0 ? -1 : 1) * float_t(rand()) / (RAND_MAX * factor));
        in.push_back(e);
    }
    
    //std::cout << in << std::endl;
    out.clear();
    simple_timer a;
    st.sort(in, out);
    a.print_elapsed();
    out.clear();
    simple_timer b;
    st.native_sort(in, out);
    b.print_elapsed();
    //std::cout << out << std::endl;
    
    typename element_type::compare comparator;
    std::cout << (std::is_sorted(out.begin(), out.end(), comparator) ? "sorted" : "not sorted") << std::endl;

    // ---- add unsorted vs sorted, watch for precision drift
    double unsorted_sum = 0.0, abs_sorted_sum = 0.0, sorted_sum = 0.0;
    for (auto i = in.begin(), e = in.end(); i != e; ++i)  {
        unsorted_sum += (*i).value;
    }
    for (auto i = out.begin(), e = out.end(); i != e; ++i)  {
        sorted_sum += (*i).value;
    }
    out.clear();
    st.native_abs_sort(in, out);
    for (auto i = out.begin(), e = out.end(); i != e; ++i)  {
        abs_sorted_sum += (*i).value;
    }
    std::cout << "unsorted sum " << std::setprecision(15) << unsorted_sum << " | sorted sum " << sorted_sum << " | abs sorted sum " << abs_sorted_sum << std::endl;
    return 0;
}
