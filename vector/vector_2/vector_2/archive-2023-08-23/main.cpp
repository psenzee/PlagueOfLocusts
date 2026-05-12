#include <iostream>
#include <array>
#include <algorithm>
#include "stl.h"
#include "key_traverser_.h"
#include "vector_tests.h"

void test_isqrt()
{
    vector_test<1024>(std::cout);
    vector_test<16 * 1024>(std::cout);

    for (size_t i = 0; i < 16384; i++) {
        //std::cout << "x " << i << ":" << _lg_plus1_uint(i) << "|" << (log2(float(i))+1) << " " << std::endl;
    }
    for (size_t i = 0; i < 16384; i++) {
        std::cout << " sqrt pow2 " << i << ":" << _approx_sqrt_pow2_uint(i) << ":" << sqrt(i) << "  lg+1 " << _lg_plus1_uint(i) << "|" << floor(log2(float(i))+1) << std::endl;
    }
    std::cout << std::setprecision(2);
}

template <size_t N, typename IndexT = int16_t, size_t RanksN, size_t KeyRanksN>
int test_dimensions(size_t to_generate)
{
    typedef std::array<float, N> vector_t;

    std::cout << "starting..." << std::endl;
    container<N, float> store;
    vector_generator<float> generator(N, vector_generator<float>::NORMALIZE_MODE_RANDOM_LENGTH);
    store.generate(generator, to_generate);
    key_index_<float, N, IndexT, RanksN, KeyRanksN> ki;
    
    simple_timer t0;
    for (const auto &v : store._data) ki.add(v);
    t0.print_elapsed(std::cout, "adding to index");
    simple_timer t1;
    ki.index();
    t1.print_elapsed(std::cout, "indexing");

    size_t count = 100;
    for (const auto &v : ki._data) {
        std::cout << v << std::endl;
        if (count-- == 0) {
            std::cout << "..." << std::endl;
            break;
        }
    }

    auto v(generator.vector());
    vector_t va;
    std::copy(v.begin(), v.end(), va.begin());
    key_<float, N, IndexT, RanksN, KeyRanksN> k(v);
    key_traverser_<float, N, IndexT, RanksN, KeyRanksN> kt(ki);
    kt.find_ids(va, store);

    auto dist_id = store.linear_search(va);
    std::cout << "linear search id " << dist_id.second << " distance " << dist_id.first << std::endl;

    // insert code here...
    std::cout << k << std::endl << std::endl;
    return 0;
}

int main(int argc, const char * argv[])
{
    test_dimensions<128, int8_t, 32, 3>(1024 * 1024 * 2/* * 16*/);
}
