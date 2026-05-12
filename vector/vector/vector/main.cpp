#include <iostream>
#include <limits>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstddef>
#include <queue>
#include <thread>

#include "simple_timer.h"
#include "btree_map.h"
#include "stl.h"
#include "slices.h"
#include "generators.h"
#include "k_indices_queue.h"
#include "container.h"
#include "key_index.h"
#include "pattern.h"
#include "sorts.h"
#include "mdct.h"
#include "dim_exp.h"
#include "dct.h"
#include "rank.h"
#include "key_finder.h"
#include "metadata.h"
#include "key_id_index.h"
#include "slices_finder.h"
#include "ranked_vector.h"

#include "mysql.h"
#include "hilbert.h"

template <typename SliceT>
class finder
{
    typedef typename SliceT::value_type scalar_type;

    std::vector<scalar_type>                                                      _reference;
    const slices<SliceT>                                                         &_indices;
    k_indices_queue<scalar_type, std::less<scalar_type> >                         _best_q;
    
public:

    typedef SliceT                   slice_type;
    typedef slices<SliceT>           indices_type;
    typedef std::vector<scalar_type> vector_type;

    inline finder(const indices_type &indices, size_t k_to_find = 8) : _indices(indices), _best_q(k_to_find) { _reference.resize(indices.size()); }
    
    inline void start(const vector_type &search)
    {
        if (search.size() != _indices.size()) {
            std::cerr << "expected " << _indices.size() << " dimensions, but received " << search.size() << std::endl;
            // error ...
            return;
        }
        _reference = search;
        auto m = _find_range_in_magnitudes();
    }
    
    // get a range estimate from items of similar magnitude
    // if they are all in a thin shell at approximately the same distance from the center
    // this won't work well, so we need to check the variance/stddev
    scalar_type _find_range_in_magnitudes()
    {
        auto magnitude = sqrt(magnitude_sq(_reference));

        std::cout << "magnitude " << magnitude << std::endl;
        _indices.print(std::cout);
        simple_timer t;
        auto &mags = _indices.magnitudes();
        auto i = mags.lower_bound(magnitude);
        std::vector<scalar_type> v;
        auto range = _indices.distance(_index(*i), _reference, v);
        _best_q.add(range, _index(*i));
        t.print_elapsed("find range");

        //range = sqrt(range);

        std::cout << "distance " << range << std::endl;
        // idea: precalc highest dimension index for each vector
        auto mina = std::min(scalar_type(0.0), magnitude - range), maxa = fabs(magnitude + range);
        std::cout << "[" << mina << "," << maxa << "]" << std::endl;

        //std::upper_bound(mags.index_begin(), mags.index_end(), slice_type::compare_type());
        auto lo_iter = mags.upper_bound(mina), hi_iter = mags.lower_bound(maxa);
        size_t count = 0;
        for (auto j = i; j != hi_iter; ++j) {
            auto ix = _index(*j);
            _best_q.add(_indices.distance(ix, _reference, v), ix);
            count++;
        }
        for (auto j = i; j != lo_iter; --j) {
            auto ix = _index(*j);
            _best_q.add(_indices.distance(ix, _reference, v), ix);
            count++;
        }
        _best_q.print(std::cout);
        return _value(_best_q.kth_best());
    }
};

void test_slices_and_finder(size_t dims, size_t count)
{
    typedef float float_t;
    typedef slices< slice<float_t, uint32_t> > indices_t;
    simple_timer t;
    indices_t index(dims);
    std::vector<float_t> v;
    std::cout << std::endl << "index/finder generating " << count << " vectors of dimension " << dims << std::endl;
    simple_timer ta;
    float_vector_generator generator(dims, float_vector_generator::NORMALIZE_MODE_RANDOM_LENGTH);
    for (size_t i = 0; i < count; i++) {
        generator.vector(v);
        index.add_invalidate(v);
    }
    ta.print_elapsed("  index generate + invalidating add");
    simple_timer tc;
    index.index();
    tc.print_elapsed("  index index");
    t.print_elapsed("index total");
    
    // now generate search vector
    simple_timer td;
    generator.vector(v);
    finder< slice<float_t, uint32_t> > find(index, 8);
    find.start(v);
    td.print_elapsed("  finder start");
}

template <typename FloatT>
void normalize(std::vector<FloatT> &v)
{
    typedef FloatT value_type;
    value_type msq = sqrt(magnitude_sq(v));
    if (isnan(msq) || msq <= value_type(0)) {
        // do nothing
        return;
    }
    for (auto &u : v) u /= msq;
}

void print_metadata(std::vector<float_t> &in)
{
    std::cout << std::endl << "unnormalized";
    metadata< std::vector<float_t> > md(in);
    md.print(std::cout);
    normalize(in);
    metadata< std::vector<float_t> > md2(in);
    std::cout << std::endl << "normalized";
    md2.print(std::cout);
}

void test_metadata()
{
    std::vector<float_t> in, out;
    vector_generator<float_t> reals(1024, vector_generator<float_t>::NORMALIZE_MODE_NONE/*RANDOM_LENGTH*/);
    for (size_t i = 0; i < 5; i++) {
        in = reals.vector();
        print_metadata(in);
    }
    vector_generator<float_t> reals1(1024, vector_generator<float_t>::NORMALIZE_MODE_RANDOM_LENGTH);
    for (size_t i = 0; i < 5; i++) {
        in = reals1.vector();
        print_metadata(in);
    }

    for (size_t i = 0; i < 1024; i++)
        in[i] = 2.3;
    print_metadata(in);

    in[234] = 10000.0;
    print_metadata(in);
    
    for (size_t i = 0; i < 1024; i++)
        in[i] = 42.3 * ((i % 2) ? -1.0 : 1.0);
    print_metadata(in);
    for (size_t i = 0; i < 1024; i++)
        in[i] = 89.6 * ((i % 2) ? 1.0 : -1.0);
    print_metadata(in);
    for (size_t i = 0; i < 1024; i++)
        in[i] = 0.f;
    print_metadata(in);
}

#include "file_mapper_.h"

int main(int argc, const char * argv[])
{
    test_file_mapper();
    
    hilbert<5, 3, uint64_t> h;
    h.print_stats(std::cout);
//  h.test_bin_to_gray_to_bin();
    h.test_coding();
    
//  hilbert<1024, 8, uint64_t> h2;
//  h2.test_bin_to_gray_to_bin();
//  h2.test_coding();
    
    hilbert<1024, 4, uint64_t> h3;
    h3.print_stats(std::cout);
//  h3.test_bin_to_gray_to_bin();
    h3.test_coding();
    /*
    hilbert<1024, 3, uint64_t> h4;
    h4.print_stats(std::cout);
//  h4.test_bin_to_gray_to_bin();
    h4.test_coding();
    */
    /*
    Database *db = new Database;
    db->execute();
    delete db;
    */
    
    test_key_id_index(1024 * 1024);
    return 0;
    //test_slices(1024, 128);
    /*
    test_slices_finder(1024 * 128);

    std::cout << std::endl << std::endl;
    return 0;
    test_metadata();
    */
    /*
    compute_dct(in, out);
    std::cout << in  << std::endl;
    std::cout << out << std::endl;
    size_t count = 0;
    for (auto i = out.rbegin(), e = out.rend(); i != e && *i == float_t(0); ++i)
        count++;
    std::cout << "active items " << count << std::endl;
     */
    /*
    mdct_lookup mdct;
    mdct_init(&mdct, 1024);
    float in[1024] = { 0.f }, out[1024] = { 0.f };
    mdct_forward(&mdct, in, out);
    mdct_clear(&mdct);
    */
    
    //test_radix_sort();
    //test_float_index_bucket_sort();
    
    //test_slice(10000000);
    //test_slices(1024, 1024 * 128);
    //test_slices_and_finder(1024, 1024 * 128);
    //test_ranker_4(1024, 1024 * 1024);
    //test_rank_index_n_parallel<4, 1024>(1024 * 1024, 0);
    
    //test_rank_index_n<8, 1024>(1024 * 1024, 0);
    //test_key_index_n<8, 1024, rank<8, float_t> >(1024 * 1024, 0);
    typedef container<1024, float_t> container_t;
    container_t store;
    vector_generator<float_t> generator(1024);
    store.generate(generator, 1024 * 1024);
    test_key_index_n<8, container_t, rank<8, float_t> >(store, generator);
    
    test_key_index_n<8, container_t, pattern<8, float_t> >(store, generator);
    
    //auto vi = vector_index<float_t, 1024>();
    
    //auto map = btree::btree_map<float_t, element_type>();

    return 0;
}
