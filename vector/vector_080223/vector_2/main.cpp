#include <iostream>
#include <fstream>
#include <array>
#include <algorithm>

#include "stl.h"
#include "key_traverser.h"
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

template <size_t N, typename FloatT, typename IndexT, size_t KeyRanksN>
std::ostream &print_vector(std::ostream &os, const std::array<FloatT, N> &v, int count = -1)
{
    typedef container<N, FloatT>                        container_t;
    typedef typename container_t::traits_t              traits_t;
    key<float, N, IndexT, KeyRanksN> k(v);
    return traits_t::print(os << k << " ", v, count);
}

template <size_t N, typename IndexT, size_t KeyRanksN>
int test_dimensions(size_t to_generate, unsigned seed, std::ostream &csv_out)
{
    typedef float                                       real_t;
    typedef std::array<real_t, N>                       vector_t;
    typedef key_index<real_t, N, IndexT, KeyRanksN>     key_index_t;
    typedef key_traverser<real_t, N, IndexT, KeyRanksN> key_traverser_t;
    typedef key<real_t, N, IndexT, KeyRanksN>           key_t;
    typedef container<N, real_t>                        container_t;
    typedef vector_generator<real_t>                    generator_t;
    typedef typename container_t::traits_t              traits_t;

    srand(seed);
    
    std::cout << "starting..." << std::endl;
    container_t store;
    generator_t generator(N, generator_t::NORMALIZE_MODE_RANDOM_LENGTH);
    store.generate(generator, to_generate);
    key_index_t ki;
    
    simple_timer t0;
    for (const auto &v : store._data) ki.add(v);
    t0.print_elapsed(std::cout, "adding to index");
    simple_timer t1;
    ki.index();
    t1.print_elapsed(std::cout, "indexing");
    std::array<size_t, 6> order_zero_counts({ 0 }), order_nonzero_counts({ 0 });

    ki.print_range(std::cout);

    enum { COUNT_TO_TRY = 1000 };
    float_t diff_total = 0, diff_count = 0, magnitude_total = 0;
    size_t zero_count = 0;
    float_t total_dcount = 0.0;
    for (size_t i = 0; i < COUNT_TO_TRY; i++) {
        auto v(generator.vector());
        vector_t va;
        std::copy(v.begin(), v.end(), va.begin());
        key<float, N, IndexT, KeyRanksN> k(v);
        key_traverser_t kt(ki);
        auto rs = kt.find_ids(va, store);
        auto found_id = rs.minimum;
        auto dist_id = store.linear_search(va);

        float found_norm = ki._data[found_id.second].key.norm, dist_norm = ki._data[dist_id.second].key.norm;
        float diff = abs(found_id.first - dist_id.first), diff_fast(abs(found_norm - k.norm)), diff_slow(abs(dist_norm - k.norm)), order = 1.0; // 
        diff_total += diff;
        diff_count += 1.0;
        if (diff == 0.0) {
            zero_count++;
            for (size_t j = 0; j < order_zero_counts.size(); j++) {
                if (dist_id.first < order) {
                    order_zero_counts[j]++;
                    break;
                }
                order *= 10.0f;
            }
        } else {
            for (size_t j = 0; j < order_nonzero_counts.size(); j++) {
                if (dist_id.first < order) {
                    order_nonzero_counts[j]++;
                    break;
                }
                order *= 10.0f;
            }
        }
        magnitude_total += dist_id.first;
        if (/*false &&*/ diff > 0) {
            std::cout << "*** RESULT linear search id " << dist_id.second << " distance " << dist_id.first
            << " | found id " << found_id.second << " distance " << found_id.first
            << " diff " << diff << " distance count " << rs.distance_count;
            if (diff > 0)
                std::cout << " | diff fast " << diff_fast << " slow " << diff_slow << " : delta " << diff_fast - diff_slow;
            const int DISPLAY_COUNT = 16;
            print_vector<N, real_t, IndexT, KeyRanksN>(std::cout << std::endl << "linear: ", store.at(dist_id.second),  DISPLAY_COUNT);
            print_vector<N, real_t, IndexT, KeyRanksN>(std::cout << std::endl << "search: ", va,                        DISPLAY_COUNT);
            print_vector<N, real_t, IndexT, KeyRanksN>(std::cout << std::endl << "found:  ", store.at(found_id.second), DISPLAY_COUNT);
            std::cout << std::endl << std::endl;
        }
        total_dcount += rs.distance_count;
    }
    
    std::cout << std::endl << "[" << KeyRanksN << "] "
              << "average diff " << diff_total / diff_count << " magnitude " << magnitude_total / diff_count
              << " zeros " << zero_count << "/" << int(diff_count) << " dcount " << total_dcount / diff_count
              << " | zcount per ";
    float order = 1.f;
    for (size_t i = 0; i < order_zero_counts.size(); i++) {
        std::cout << order << ":" << order_zero_counts[i] << "/" << order_nonzero_counts[i] << " ";
        order *= 10.f;
    }
    std::cout << std::endl;
    
    size_t millions = (to_generate >> 20);
    float av_distance_count = float(total_dcount) / diff_count;

    csv_out << int(N) << "," << int(KeyRanksN) << "," << millions << "," << sizeof(IndexT) << "," << seed << "," << int(diff_count) << ","
            << (diff_total / diff_count) << "," << (magnitude_total / diff_count) << "," << zero_count << ","
            << av_distance_count << "," << (av_distance_count / millions) << "," << int(vector_generator<float>::MAX_RANDOM)
            << std::endl;
    return 0;
}

template <size_t N, typename IndexT = int16_t>
void test_set(size_t millions, std::ostream &out)
{
    size_t size = millions * 1024 * 1024, seed = 2;
    /*
    try { test_dimensions<N, IndexT,  1>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT,  2>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT,  3>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT,  4>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT,  5>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT,  6>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT,  7>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT,  8>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT,  9>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 10>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 11>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 12>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 13>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 14>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 15>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 16>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    */
    try { test_dimensions<N, IndexT, 17>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 18>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 19>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 20>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 21>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 22>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 23>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 24>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 25>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 26>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 27>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 28>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 29>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 30>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 31>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
    try { test_dimensions<N, IndexT, 32>(size, seed, out); } catch (...) { std::cout << "segfault" << std::endl; }
}

#include "bloom.h"

int main(int argc, const char * argv[])
{
    enum { K64 = 1024 * 64, K64_BITS = K64 * 8, BITS = K64_BITS * 2 };
    test_bloom<BITS>(1000);
    test_bloom<BITS>(10000);
    test_bloom<BITS>(100000);
    test_bloom<BITS>(500000);
    test_bloom<BITS>(1000000);
    //exit(0);
    std::ofstream fout;
    fout.open("/Users/paul/Desktop/Projects/vector-results.csv", std::ios_base::app);
    if (!fout) {
        std::cout << "Can't open csv file";
        exit(1);
    }
    test_set<1024, int16_t>(1, fout);
    fout.close();
}
