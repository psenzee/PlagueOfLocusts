#include <iostream>
#include <fstream>
#include <array>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <cstdio>

#include "stl.h"
#include "key_traverser.h"
#include "vector_tests.h"

template <size_t N, typename FloatT, size_t KeyRanksN>
std::ostream &print_vector(std::ostream &os, const std::array<FloatT, N> &v, int count = -1)
{
    typedef container<N, FloatT>                        container_t;
    typedef typename container_t::traits_t              traits_t;
    key<float, N, KeyRanksN> k(v);
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
    container_t         test_vectors;
    unsigned            seed;
    bool                get_linear_search_times;
    
    test_data(size_t count_to_store, size_t count_to_query, unsigned seed, bool get_linear_search_times = false)
        : seed(seed), generator(N, generator_t::NORMALIZE_MODE_RANDOM_LENGTH), get_linear_search_times(get_linear_search_times)
    {
        srand(seed);
        std::cout << "[generating test store]" << std::endl;
        simple_timer t;
        store.add(vector_t({ 0 })); // make sure there's a zero vector in there
        store.generate(generator, count_to_store - 1);
        store.sort_by_norm();
        t.print_elapsed(std::cout, "done [generating test store]");
        _generate_test_queries(count_to_query);
    }
    
    const vector_t test_vector(size_t id) const { return test_vectors.at(id); }
    
    size_t test_vector_count() const { return test_vectors.size(); }
    
    int save_store(const char *path) const
    {
        return store.save(path);
    }

    query_info_t linear_search(size_t id) const
    {
        auto i = query_map.find(id);
        if (i != query_map.end())
            return (*i).second;
        const auto &va = test_vectors.at(id);
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
        std::cout << "[generating test query vectors] ";
        simple_timer t;
        test_vectors.add(vector_t({ 0 })); // make sure there's a zero vector in there
        test_vectors.generate(generator, count_to_query - 1);
        t.print_elapsed(std::cout, "done [generating test query vectors]");
    }
};


template <size_t N>
struct test_data_saved
{
    typedef float                               real_t;
    typedef std::array<real_t, N>               vector_t;
    typedef file_map_container<N, real_t>       container_t;
    typedef container<N, real_t>                query_container_t;
    typedef vector_generator<real_t>            generator_t;
    typedef typename container_t::traits_t      traits_t;
    typedef std::pair<real_t, size_t>           value_index_t;
    typedef std::pair<value_index_t, size_t>    query_info_t;
    typedef std::map<size_t, query_info_t>      query_map_t;
    typedef std::vector<vector_t>               queries_t;
    
    enum { COUNT_TO_TRY = 1000 };
    
    container_t          store;
    generator_t          generator;
    mutable query_map_t  query_map;
    query_container_t    test_vectors;
    unsigned             seed;
    
    test_data_saved(const char *filename, size_t count_to_query, unsigned seed)
        : seed(seed), generator(N, generator_t::NORMALIZE_MODE_RANDOM_LENGTH)
    {
        srand(seed);
        std::cout << "[loading test store]" << std::endl;
        simple_timer t;
        store.load(filename);
        t.print_elapsed(std::cout, "done [loading test store]");
        _generate_test_queries(count_to_query);
    }
    
    const vector_t test_vector(size_t id) const { return test_vectors.at(id); }
    
    size_t test_vector_count() const { return test_vectors.size(); }
    
    int save_store(const char *path) const
    {
        return 0;
    }

    query_info_t linear_search(size_t id) const
    {
        auto i = query_map.find(id);
        if (i != query_map.end())
            return (*i).second;
        const auto &va = test_vectors.at(id);
        simple_timer t;
        //auto dist_id = value_index_t(0, 0);//store.linear_search(va);
        auto dist_id = store.linear_search(va);
        size_t elapsed = t.us_elapsed();
        query_info_t qi(dist_id, elapsed);
        //std::cout << "linear search dist " << dist_id.first << " id " << dist_id.second << std::endl;
        query_map[id] = qi;
        return qi;
    }

private:

    void _generate_test_queries(size_t count_to_query)
    {
        std::cout << "[generating test query vectors] ";
        simple_timer t;
        test_vectors.add(vector_t({ 0 })); // make sure there's a zero vector in there
        test_vectors.generate(generator, count_to_query - 1);
        t.print_elapsed(std::cout, "done [generating test query vectors]");
    }
};

template <size_t N, size_t KeyRanksN>
int test_vectors(const test_data<N> &data, std::ostream &csv_out)
{
    typedef typename test_data<N>::real_t               real_t;
    typedef typename test_data<N>::container_t          container_t;
    typedef std::array<real_t, N>                       vector_t;
    typedef key_index<container_t, KeyRanksN>           key_index_t;
    typedef key_traverser<container_t, KeyRanksN>       key_traverser_t;
    typedef key<real_t, N, KeyRanksN>                   key_t;
    typedef vector_generator<real_t>                    generator_t;
    
    key_index_t ki;
    simple_timer t0;
    for (size_t i = 0; i < data.store.size(); i++) ki.add(data.store.at(i));
    simple_timer t1;
    ki.index();

    float_t diff_total = 0, diff_count = 0, magnitude_total = 0, total_dcount = 0.0, total_ccount = 0.0;
    size_t zero_count = 0, total_us_fast = 0, total_us_slow = 0;
    
    for (size_t i = 0; i < data.test_vector_count(); i++) {
        vector_t va(data.test_vector(i));
        key_t k(va);
        key_traverser_t kt(ki);
        simple_timer st0;
        auto rs = kt.find_ids(va, data.store);
        total_us_fast += st0.us_elapsed();
        auto found_id = rs.minimum();
        auto linear = data.linear_search(i);
        auto dist_id = linear.first;
        total_us_slow += linear.second;

        float found_norm = ki._data[found_id.second].key.norm, dist_norm = ki._data[dist_id.second].key.norm;
        float_t diff = abs(found_id.first - dist_id.first), diff_fast(abs(found_norm - k.norm)), diff_slow(abs(dist_norm - k.norm));//, order = 1.0;
        diff_total += diff;
        diff_count += 1.0;

        if (diff == 0.0) zero_count++;

        magnitude_total += dist_id.first;

        if (false && diff > 0) {
            std::cout << "*** RESULT linear search id " << dist_id.second << " distance " << dist_id.first
            << " | found id " << found_id.second << " distance " << found_id.first
            << " \u0394 " << diff << " distance count " << rs.distance_count;
            if (diff > 0)
                std::cout << " | \u0394 fast " << diff_fast << " slow " << diff_slow << " : \u0394 " << diff_fast - diff_slow;
            const int DISPLAY_COUNT = 16;
            print_vector<N, real_t, KeyRanksN>(std::cout << std::endl << "linear: ", data.store.at(dist_id.second),  DISPLAY_COUNT);
            print_vector<N, real_t, KeyRanksN>(std::cout << std::endl << "search: ", va,                             DISPLAY_COUNT);
            print_vector<N, real_t, KeyRanksN>(std::cout << std::endl << "found:  ", data.store.at(found_id.second), DISPLAY_COUNT);
            std::cout << std::endl << std::endl;
        }
        total_dcount += rs.distance_count;
        total_ccount += rs.compare_count;
    }
    
    std::cout << "[" << KeyRanksN << ":" << N << "] times -- fast ";
    simple_timer::write(std::cout, total_us_fast / data.test_vector_count()) << " slow ";
    simple_timer::write(std::cout, total_us_slow / data.test_vector_count());
    std::cout << std::endl << "[" << KeyRanksN << ":" << N << "] "
              << "average \u0394 " << diff_total / diff_count << " magnitude " << magnitude_total / diff_count
              << " 0s " << zero_count << "/" << int(diff_count) << " dcount " << total_dcount / diff_count
              << " ccount " << total_ccount / diff_count << std::endl;

    float inv_millions = 1024.0 * 1024.0;
    float av_distance_count = float(total_dcount) / diff_count;
    csv_out << int(N) << "," << int(KeyRanksN) << "," << data.store.size() * inv_millions << ",auto," << data.seed << "," << int(diff_count) << ","
            << (diff_total / diff_count) << "," << (magnitude_total / diff_count) << "," << zero_count << ","
            << av_distance_count << "," << (av_distance_count * inv_millions) << "," << int(vector_generator<float>::MAX_RANDOM)
            << std::endl;
    return 0;
}

template <size_t N, size_t KeyRanksN>
int test_vectors_saved(const test_data_saved<N> &data, std::ostream &csv_out)
{
    typedef typename test_data_saved<N>::real_t         real_t;
    typedef typename test_data_saved<N>::container_t    container_t;
    typedef std::array<real_t, N>                       vector_t;
    typedef key_index<container_t, KeyRanksN>           key_index_t;
    typedef key_traverser<container_t, KeyRanksN>       key_traverser_t;
    typedef key<real_t, N, KeyRanksN>                   key_t;
    typedef vector_generator<real_t>                    generator_t;
    
    key_index_t ki;
    simple_timer t0;
    for (size_t i = 0; i < data.store.size(); i++) ki.add(data.store.at(i));
    //ki.print_info(std::cout);
    std::cout << "id_count " << ki.id_count() << std::endl;
    simple_timer t1;
    ki.index();

    float_t diff_total = 0, diff_count = 0, magnitude_total = 0, total_dcount = 0.0, total_ccount = 0.0;
    size_t zero_count = 0, total_us_fast = 0, total_us_slow = 0;
    
    for (size_t i = 0; i < data.test_vector_count(); i++) {
        vector_t va(data.test_vector(i));
        key_t k(va);
        key_traverser_t kt(ki);
        simple_timer st0;
        auto rs = kt.find_ids(va, data.store);
        total_us_fast += st0.us_elapsed();
        auto found_id = rs.minimum();
        auto linear = data.linear_search(i);
        auto dist_id = linear.first;
        total_us_slow += linear.second;

        float found_norm = ki._data[found_id.second].key.norm, dist_norm = ki._data[dist_id.second].key.norm;
        float_t diff = abs(found_id.first - dist_id.first), diff_fast(abs(found_norm - k.norm)), diff_slow(abs(dist_norm - k.norm));//, order = 1.0;
        diff_total += diff;
        diff_count += 1.0;

        if (diff == 0.0) zero_count++;

        magnitude_total += dist_id.first;

        if (false && diff > 0) {
            std::cout << "*** RESULT linear search id " << dist_id.second << " distance " << dist_id.first
            << " | found id " << found_id.second << " distance " << found_id.first
            << " \u0394 " << diff << " distance count " << rs.distance_count;
            if (diff > 0)
                std::cout << " | \u0394 fast " << diff_fast << " slow " << diff_slow << " : \u0394 " << diff_fast - diff_slow;
            const int DISPLAY_COUNT = 16;
            print_vector<N, real_t, KeyRanksN>(std::cout << std::endl << "linear: ", data.store.at(dist_id.second),  DISPLAY_COUNT);
            print_vector<N, real_t, KeyRanksN>(std::cout << std::endl << "search: ", va,                             DISPLAY_COUNT);
            print_vector<N, real_t, KeyRanksN>(std::cout << std::endl << "found:  ", data.store.at(found_id.second), DISPLAY_COUNT);
            std::cout << std::endl << std::endl;
        }
        total_dcount += rs.distance_count;
        total_ccount += rs.compare_count;
    }
    
    std::cout << "[" << KeyRanksN << ":" << N << "] times -- fast ";
    simple_timer::write(std::cout, total_us_fast / data.test_vector_count()) << " slow ";
    simple_timer::write(std::cout, total_us_slow / data.test_vector_count());
    std::cout << std::endl << "[" << KeyRanksN << ":" << N << "] "
              << "average \u0394 " << diff_total / diff_count << " magnitude " << magnitude_total / diff_count
              << " 0s " << zero_count << "/" << int(diff_count) << " dcount " << total_dcount / diff_count
              << " ccount " << total_ccount / diff_count << std::endl;

    float inv_millions = 1024.0 * 1024.0;
    float av_distance_count = float(total_dcount) / diff_count;
    csv_out << int(N) << "," << int(KeyRanksN) << "," << data.store.size() * inv_millions << ",auto," << data.seed << "," << int(diff_count) << ","
            << (diff_total / diff_count) << "," << (magnitude_total / diff_count) << "," << zero_count << ","
            << av_distance_count << "," << (av_distance_count * inv_millions) << "," << int(vector_generator<float>::MAX_RANDOM)
            << std::endl;
    return 0;
}

template <size_t N>
void test_set(size_t size, std::ostream &out, const char *path)
{
    size_t seed = 2;
    test_data<N> td(size, 1000, seed);
    td.save_store(path);
    test_vectors<N,  1>(td, out);
    test_vectors<N,  2>(td, out);
    test_vectors<N,  3>(td, out);
    test_vectors<N,  4>(td, out);
    test_vectors<N,  5>(td, out);
    test_vectors<N,  6>(td, out);
    test_vectors<N,  7>(td, out);
    test_vectors<N,  8>(td, out);
    test_vectors<N,  9>(td, out);
    test_vectors<N, 10>(td, out);
    test_vectors<N, 11>(td, out);
    test_vectors<N, 12>(td, out);
    test_vectors<N, 13>(td, out);
    test_vectors<N, 14>(td, out);
    test_vectors<N, 15>(td, out);
    test_vectors<N, 16>(td, out);
    test_vectors<N, 17>(td, out);
    test_vectors<N, 18>(td, out);
    test_vectors<N, 19>(td, out);
    test_vectors<N, 20>(td, out);
    test_vectors<N, 21>(td, out);
    test_vectors<N, 22>(td, out);
    test_vectors<N, 23>(td, out);
    test_vectors<N, 24>(td, out);
    test_vectors<N, 25>(td, out);
    test_vectors<N, 26>(td, out);
    test_vectors<N, 27>(td, out);
    test_vectors<N, 28>(td, out);
    test_vectors<N, 29>(td, out);
    test_vectors<N, 30>(td, out);
    test_vectors<N, 31>(td, out);
    test_vectors<N, 32>(td, out);
}

template <size_t N>
void test_set_saved(std::ostream &out, const char *path)
{
    size_t seed = 17;
    test_data_saved<N> td(path, 1000, seed);
    test_vectors_saved<N,  1>(td, out);
    test_vectors_saved<N,  2>(td, out);
    test_vectors_saved<N,  3>(td, out);
    test_vectors_saved<N,  4>(td, out);
    test_vectors_saved<N,  5>(td, out);
    test_vectors_saved<N,  6>(td, out);
    test_vectors_saved<N,  7>(td, out);
    test_vectors_saved<N,  8>(td, out);
    test_vectors_saved<N,  9>(td, out);
    test_vectors_saved<N, 10>(td, out);
    test_vectors_saved<N, 11>(td, out);
    test_vectors_saved<N, 12>(td, out);
    test_vectors_saved<N, 13>(td, out);
    test_vectors_saved<N, 14>(td, out);
    test_vectors_saved<N, 15>(td, out);
    test_vectors_saved<N, 16>(td, out);
    test_vectors_saved<N, 17>(td, out);
    test_vectors_saved<N, 18>(td, out);
    test_vectors_saved<N, 19>(td, out);
    test_vectors_saved<N, 20>(td, out);
    test_vectors_saved<N, 21>(td, out);
    test_vectors_saved<N, 22>(td, out);
    test_vectors_saved<N, 23>(td, out);
    test_vectors_saved<N, 24>(td, out);
    test_vectors_saved<N, 25>(td, out);
    test_vectors_saved<N, 26>(td, out);
    test_vectors_saved<N, 27>(td, out);
    test_vectors_saved<N, 28>(td, out);
    test_vectors_saved<N, 29>(td, out);
    test_vectors_saved<N, 30>(td, out);
    test_vectors_saved<N, 31>(td, out);
    test_vectors_saved<N, 32>(td, out);
    
    test_vectors_saved<N, 33>(td, out);
    test_vectors_saved<N, 34>(td, out);
    test_vectors_saved<N, 35>(td, out);
    test_vectors_saved<N, 36>(td, out);
    test_vectors_saved<N, 37>(td, out);
    test_vectors_saved<N, 38>(td, out);
    test_vectors_saved<N, 39>(td, out);
    test_vectors_saved<N, 40>(td, out);
    test_vectors_saved<N, 41>(td, out);
    test_vectors_saved<N, 42>(td, out);
    test_vectors_saved<N, 43>(td, out);
    test_vectors_saved<N, 44>(td, out);
    test_vectors_saved<N, 45>(td, out);
    test_vectors_saved<N, 46>(td, out);
    test_vectors_saved<N, 47>(td, out);
    test_vectors_saved<N, 48>(td, out);
    test_vectors_saved<N, 49>(td, out);
    test_vectors_saved<N, 50>(td, out);
    test_vectors_saved<N, 51>(td, out);
    test_vectors_saved<N, 52>(td, out);
    test_vectors_saved<N, 53>(td, out);
    test_vectors_saved<N, 54>(td, out);
    test_vectors_saved<N, 55>(td, out);
    test_vectors_saved<N, 56>(td, out);
    test_vectors_saved<N, 57>(td, out);
    test_vectors_saved<N, 58>(td, out);
    test_vectors_saved<N, 59>(td, out);
    test_vectors_saved<N, 60>(td, out);
    test_vectors_saved<N, 61>(td, out);
    test_vectors_saved<N, 62>(td, out);
    test_vectors_saved<N, 63>(td, out);
    test_vectors_saved<N, 64>(td, out);
}

int main(int argc, const char * argv[])
{
    std::cout << "starting..." << std::endl;

    vector_test<1024>(std::cout);
    
    char path[8192] = { 0 };
    const char *directory = "/Users/paul/Desktop/Projects";
    snprintf(path, sizeof(path), "%s/%s", directory, "vector-results.csv");
    
    std::ofstream fout;
    fout.open(path, std::ios_base::app);
    if (!fout) {
        std::cout << "Can't open csv file";
        exit(1);
    }
    snprintf(path, sizeof(path), "%s/%s", directory, "vectors_1024.vdb");
    //test_set<1024>(1024 * 1024, fout, path);
    test_set_saved<1024>(fout, path);
    fout.close();

    std::cout << "done" << std::endl;
}
