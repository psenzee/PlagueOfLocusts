#include "ranker.h"

#include <iostream>
#include <limits>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstddef>
#include <queue>

#include "simple_timer.h"
#include "utils.h"
#include "generate_vectors.h"

template <size_t N>
void test_ranker_n(size_t dims, size_t count)
{
    typedef float float_t;
    typedef ranker<N, float_t> ranker_t;
    typedef typename ranker_t::rank_t rank_t;
    std::vector<float_t> v;
    std::cout << std::endl << "ranker [" << ranker_t::COUNT << "] " << count << " vectors of dimension " << dims << std::endl;
    ranker_t ranker;
    std::vector<rank_t> ranks;
    simple_timer ta;
    for (size_t i = 0; i < count; i++) {
        generate_vector(dims, v);
        ranks.push_back(ranker.rank(v));
    }
    ta.print_elapsed("  ranker generate + rank vectors ");
    simple_timer tc;
    std::sort(ranks.begin(), ranks.end());
    tc.print_elapsed("  ranker sort ranked vectors ");
    //for (auto r : ranks) std::cout << r << std::endl;
    std::cout << std::dec;
    std::cout << "total count " << ranks.size() << " uniques " << count_unique_on_sorted(ranks.begin(), ranks.end(), std::less<rank_t>()) << std::endl;
}
/*
void test_ranker_1 (size_t dims, size_t count) { test_ranker_n< 1>(dims, count); }
void test_ranker_2 (size_t dims, size_t count) { test_ranker_n< 2>(dims, count); }
void test_ranker_3 (size_t dims, size_t count) { test_ranker_n< 3>(dims, count); }
void test_ranker_4 (size_t dims, size_t count) { test_ranker_n< 4>(dims, count); }
void test_ranker_8 (size_t dims, size_t count) { test_ranker_n< 8>(dims, count); }
void test_ranker_12(size_t dims, size_t count) { test_ranker_n<12>(dims, count); }
void test_ranker_16(size_t dims, size_t count) { test_ranker_n<16>(dims, count); }
void test_ranker_32(size_t dims, size_t count) { test_ranker_n<32>(dims, count); }
*/
