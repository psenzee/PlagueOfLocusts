#include <iostream>
#include <fstream>
#include <array>
#include <algorithm>
#include <map>

#include "stl.h"
#include "key_traverser.h"
#include "vector_tests.h"
#include "page.h"

template <size_t N, typename FloatT, size_t KeyRanksN>
std::ostream &print_vector(std::ostream &os, const std::array<FloatT, N> &v, int count = -1)
{
    typedef container<N, FloatT>                        container_t;
    typedef typename container_t::traits_t              traits_t;
    key<float, N, KeyRanksN> k(v);
    return os << k;
    //return traits_t::print(os << " ", v, count);
}

template <size_t N, typename FloatT, size_t KeyRanksN>
struct key_statistics
{
    typedef FloatT                            real_t;
    typedef key<real_t, N, KeyRanksN>         key_t;
    typedef std::array<real_t, KeyRanksN>     stats_t;

    stats_t sum, min, max;
    
    key_statistics() : sum({ 0.0 })
    {
        for (size_t i = 0; i < KeyRanksN; i++) {
            min[i] = std::numeric_limits<real_t>::max();
            max[i] = std::numeric_limits<real_t>::min();
        }
    }

    void add(const key_t &k)
    {
        real_t denom = abs(k.values[KeyRanksN - 1]);
        if (denom == 0.0)
            denom = 0.00001;
        real_t value = abs(k.values[0]) / denom;
        sum[0] += value;
        min[0] = std::min(value, min[0]);
        max[0] = std::max(value, max[0]);
        for (size_t i = 1; i < KeyRanksN; i++) {
            real_t denom = abs(k.values[i]);
            if (denom == 0.0) denom = 0.00001;
            real_t value = abs(k.values[0]) / denom;
            sum[i] += value;
            min[i] = std::min(value, min[i]);
            max[i] = std::max(value, max[i]);
        }
    }
    
    std::ostream &print(std::ostream &os, size_t count, const char *type)
    {
        os << std::setprecision(5);
        os << "/- " << type << " " << abs(sum[0] / count) << " min " << min[0] << " max " << max[0] << std::endl;
        for (size_t i = 1; i < KeyRanksN; i++)
            os << "/" << i << " " << type << " " << abs(sum[i] / count) << " min " << min[i] << " max " << max[i] << std::endl;
        return os;
    }
};

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
    
    test_data(size_t count_to_store, size_t count_to_query, unsigned seed)
        : seed(seed), generator(N, generator_t::NORMALIZE_MODE_RANDOM_LENGTH)
    {
        srand(seed);
        std::cout << "[generating test store]" << std::endl;
        simple_timer t;
        store.add(vector_t({ 0 })); // make sure there's a zero vector in there
        store.generate(generator, count_to_store - 1);
        store.sort_by_norm();
        t.print_elapsed(std::cout, "done [generating test store]");
        _generate_test_queries(count_to_query);

        // test page
        std::vector<real_t> dynamic;
        page<vector_t, COUNT_TO_TRY * 2> pagey;
        for (size_t i = 0; i < COUNT_TO_TRY; i++) {
            auto dv = generator.vector();
            vector_t v;
            std::copy(dv.begin(), dv.end(), v.begin());
            pagey.add(i, v);
        }
    }
    
    const vector_t test_vector(size_t id) const { return test_vectors.at(id); }
    
    size_t test_vector_count() const { return test_vectors.size(); }

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

template <size_t N, size_t KeyRanksN>
int test_vectors(const test_data<N> &data, std::ostream &csv_out)
{
    typedef float                                       real_t;
    typedef std::array<real_t, N>                       vector_t;
    typedef key_index<real_t, N, KeyRanksN>             key_index_t;
    typedef key_traverser<real_t, N, KeyRanksN>         key_traverser_t;
    typedef key<real_t, N, KeyRanksN>                   key_t;
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
    //std::array<size_t, 6> order_zero_counts({ 0 }), order_nonzero_counts({ 0 });

    //ki.print_range(std::cout);
    
    key_statistics<N, real_t, KeyRanksN> zeros, not_zeros;

    float_t diff_total = 0, diff_count = 0, magnitude_total = 0, total_dcount = 0.0, total_ccount = 0.0;
    size_t zero_count = 0, non_zero_count = 0, total_us_fast = 0, total_us_slow = 0;
    
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

        if (diff == 0.0) {
            zero_count++;
            zeros.add(k);
        } else {
            non_zero_count++;
            not_zeros.add(k);
        }
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

    //std::cout << std::endl;

    float inv_millions = 1024.0 * 1024.0;
    float av_distance_count = float(total_dcount) / diff_count;
    csv_out << int(N) << "," << int(KeyRanksN) << "," << data.store.size() * inv_millions << ",auto," << data.seed << "," << int(diff_count) << ","
            << (diff_total / diff_count) << "," << (magnitude_total / diff_count) << "," << zero_count << ","
            << av_distance_count << "," << (av_distance_count * inv_millions) << "," << int(vector_generator<float>::MAX_RANDOM)
            << std::endl;
    return 0;
}

template <size_t N>
void test_set(size_t size, std::ostream &out, bool use_control = true)
{
    size_t seed = 2;
    test_data<N> td(size, 1000, seed);
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
}

#include "page.h"

int main(int argc, const char * argv[])
{
    std::ofstream fout;
    fout.open("/Users/paul/Desktop/Projects/vector-results.csv", std::ios_base::app);
    if (!fout) {
        std::cout << "Can't open csv file";
        exit(1);
    }
    //test_set<32>(1024 * 1024, fout, false);
    test_set<1024>(1024 * 1024, fout, false);
    //test_set<8192>(64/*1024*/ * 1024, fout, false);
    fout.close();
}
