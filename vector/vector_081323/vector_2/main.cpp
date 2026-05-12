#include <iostream>
#include <fstream>
#include <array>
#include <algorithm>
#include <map>

#include "stl.h"
#include "key_traverser.h"
#include "vector_tests.h"

template <size_t N, typename FloatT, typename IndexT, size_t KeyRanksN>
std::ostream &print_vector(std::ostream &os, const std::array<FloatT, N> &v, int count = -1)
{
    typedef container<N, FloatT>                        container_t;
    typedef typename container_t::traits_t              traits_t;
    key<float, N, IndexT, KeyRanksN> k(v);
    return os << k;
    //return traits_t::print(os << " ", v, count);
}

template <size_t N>
struct test_data
{
    typedef float                               real_t;
    typedef std::array<real_t, N>               vector_t;
    typedef container<N, real_t>                container_t;
    typedef vector_generator<real_t>            generator_t;
    typedef typename container_t::traits_t      traits_t;
    typedef std::pair<real_t, size_t>           value_index_t;
    typedef std::pair<value_index_t, size_t>    query_info_t;
    typedef std::map<size_t, query_info_t>      query_map_t;
    typedef std::vector<vector_t>               queries_t;
    
    enum { COUNT_TO_TRY = 1000 };
    
    container_t         store;
    generator_t         generator;
    mutable query_map_t query_map;
    queries_t           test_vectors;
    unsigned            seed;
    
    test_data(size_t count_to_store, size_t count_to_query, unsigned seed)
        : seed(seed), generator(N, generator_t::NORMALIZE_MODE_RANDOM_LENGTH)
    {
        srand(seed);
        std::cout << "generating test store..." << std::endl;
        store.generate(generator, count_to_store);
        _generate_test_queries(count_to_query);
    }
    
    const vector_t test_vector(size_t id) const { return test_vectors[id]; }
    
    size_t test_vector_count() const { return test_vectors.size(); }

    query_info_t linear_search(size_t id) const
    {
        auto i = query_map.find(id);
        if (i != query_map.end())
            return (*i).second;
        const auto &va = test_vectors[id];
        simple_timer t;
        auto dist_id = store.linear_search(va);
        size_t elapsed = t.us_elapsed();
        query_info_t qi(dist_id, elapsed);
        query_map[id] = qi;
        return qi;
    }

private:

    void _generate_test_queries(size_t count_to_query)
    {
        std::cout << "generating test queries..." << std::endl;
        for (size_t i = 0; i < count_to_query; i++) {
            auto v(generator.vector());
            vector_t va;
            std::copy(v.begin(), v.end(), va.begin());
            test_vectors.push_back(va);
        }
        std::cout << "done generating test data" << std::endl;
    }
};

template <size_t N, typename IndexT, size_t KeyRanksN>
int test_vectors(const test_data<N> &data, std::ostream &csv_out)
{
    typedef float                                       real_t;
    typedef std::array<real_t, N>                       vector_t;
    typedef key_index<real_t, N, IndexT, KeyRanksN>     key_index_t;
    typedef key_traverser<real_t, N, IndexT, KeyRanksN> key_traverser_t;
    typedef key<real_t, N, IndexT, KeyRanksN>           key_t;
    typedef container<N, real_t>                        container_t;
    typedef vector_generator<real_t>                    generator_t;
    typedef typename container_t::traits_t              traits_t;
    
    key_index_t ki;
    simple_timer t0;
    for (const auto &v : data.store._data) ki.add(v);
    //t0.print_elapsed(std::cout, "added to index");
    simple_timer t1;
    ki.index();
    //t1.print_elapsed(std::cout, "indexed ");
    std::array<size_t, 6> order_zero_counts({ 0 }), order_nonzero_counts({ 0 });

    //ki.print_range(std::cout);

    float_t diff_total = 0, diff_count = 0, magnitude_total = 0, total_dcount = 0.0;
    size_t zero_count = 0, total_us_fast = 0, total_us_slow = 0;
    for (size_t i = 0; i < data.test_vector_count(); i++) {
        vector_t va(data.test_vector(i));
        key<float, N, IndexT, KeyRanksN> k(va);
        key_traverser_t kt(ki);
        simple_timer st0;
        auto rs = kt.find_ids(va, data.store);
        total_us_fast += st0.us_elapsed();
        auto found_id = rs.minimum;
        auto linear = data.linear_search(i);
        auto dist_id = linear.first;
        total_us_slow += linear.second;

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

        if (false && diff > 0) {
            std::cout << "*** RESULT linear search id " << dist_id.second << " distance " << dist_id.first
            << " | found id " << found_id.second << " distance " << found_id.first
            << " \u0394 " << diff << " distance count " << rs.distance_count;
            if (diff > 0)
                std::cout << " | \u0394 fast " << diff_fast << " slow " << diff_slow << " : \u0394 " << diff_fast - diff_slow;
            const int DISPLAY_COUNT = 16;
            print_vector<N, real_t, IndexT, KeyRanksN>(std::cout << std::endl << "linear: ", data.store.at(dist_id.second),  DISPLAY_COUNT);
            print_vector<N, real_t, IndexT, KeyRanksN>(std::cout << std::endl << "search: ", va,                        DISPLAY_COUNT);
            print_vector<N, real_t, IndexT, KeyRanksN>(std::cout << std::endl << "found:  ", data.store.at(found_id.second), DISPLAY_COUNT);
            std::cout << std::endl << std::endl;
        }
        total_dcount += rs.distance_count;
    }
    
    std::cout << std::endl << "[" << KeyRanksN << ":" << N << "] times -- fast ";
    simple_timer::write(std::cout, total_us_fast / data.test_vector_count()) << " slow ";
    simple_timer::write(std::cout, total_us_slow / data.test_vector_count());
    std::cout << std::endl << "[" << KeyRanksN << ":" << N << "] "
              << "average \u0394 " << diff_total / diff_count << " magnitude " << magnitude_total / diff_count
              << " 0s " << zero_count << "/" << int(diff_count) << " dcount " << total_dcount / diff_count;
    /*
              << " | zcount per ";
    float order = 1.f;
    for (size_t i = 0; i < order_zero_counts.size(); i++) {
        std::cout << order << ":" << order_zero_counts[i] << "/" << order_nonzero_counts[i] << " ";
        order *= 10.f;
    }
    */
    std::cout << std::endl;
    
    float inv_millions = 1024.0 * 1024.0;
    float av_distance_count = float(total_dcount) / diff_count;
    csv_out << int(N) << "," << int(KeyRanksN) << "," << data.store.size() * inv_millions << "," << sizeof(IndexT) << "," << data.seed << "," << int(diff_count) << ","
            << (diff_total / diff_count) << "," << (magnitude_total / diff_count) << "," << zero_count << ","
            << av_distance_count << "," << (av_distance_count * inv_millions) << "," << int(vector_generator<float>::MAX_RANDOM)
            << std::endl;
    return 0;
}

template <size_t N, typename IndexT = int16_t>
void test_set(size_t size, std::ostream &out, bool use_control = true)
{
    size_t seed = 2;
    test_data<N> td(size, 1000, seed);
//    test_vectors<N, IndexT,  1>(td, out);
//    test_vectors<N, IndexT,  2>(td, out);
    test_vectors<N, IndexT,  3>(td, out);
    test_vectors<N, IndexT,  4>(td, out);
    test_vectors<N, IndexT,  5>(td, out);
    test_vectors<N, IndexT,  6>(td, out);
    test_vectors<N, IndexT,  7>(td, out);
    test_vectors<N, IndexT,  8>(td, out);
    test_vectors<N, IndexT,  9>(td, out);
    test_vectors<N, IndexT, 10>(td, out);
    test_vectors<N, IndexT, 11>(td, out);
    test_vectors<N, IndexT, 12>(td, out);
    test_vectors<N, IndexT, 13>(td, out);
    test_vectors<N, IndexT, 14>(td, out);
    test_vectors<N, IndexT, 15>(td, out);
    test_vectors<N, IndexT, 16>(td, out);
/*  test_vectors<N, IndexT, 17>(td, out);
    test_vectors<N, IndexT, 18>(td, out);
    test_vectors<N, IndexT, 19>(td, out);
    test_vectors<N, IndexT, 20>(td, out);
    test_vectors<N, IndexT, 21>(td, out);
    test_vectors<N, IndexT, 22>(td, out);
    test_vectors<N, IndexT, 23>(td, out);
    test_vectors<N, IndexT, 24>(td, out);
    test_vectors<N, IndexT, 25>(td, out);
    test_vectors<N, IndexT, 26>(td, out);
    test_vectors<N, IndexT, 27>(td, out);
    test_vectors<N, IndexT, 28>(td, out);
    test_vectors<N, IndexT, 29>(td, out);
    test_vectors<N, IndexT, 30>(td, out);
    test_vectors<N, IndexT, 31>(td, out);
    test_vectors<N, IndexT, 32>(td, out);*/
}

#include "bloom.h"

int main(int argc, const char * argv[])
{
    //test_intersect_timing_cross_over<16>();
    //test_intersect_timing_cross_over<32>();
    //test_intersect_timing_cross_over<64>();
    std::ofstream fout;
    fout.open("/Users/paul/Desktop/Projects/vector-results.csv", std::ios_base::app);
    if (!fout) {
        std::cout << "Can't open csv file";
        exit(1);
    }
    test_set<32, int16_t>(1024 * 1024, fout, false);
    fout.close();
}
