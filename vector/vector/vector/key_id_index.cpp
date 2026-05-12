#include "key_id_index.h"
#include "simple_timer.h"
#include "container.h"
#include "ranks.h"
#include "traverser.h"

template <size_t Dimensions>
struct min_distance_index
{
    typedef std::pair<float, uint64_t>           pair_t;
    typedef container<Dimensions, float>       container_t;
    typedef typename container_t::vector_t     vector_t;
    typedef key_bin<uint64_t, uint64_t>        key_bin_t;
    typedef typename key_bin_t::const_iterator const_iterator;

    pair_t             _minimum;
    size_t             _examined;
    const container_t &_store;
    const vector_t    &_search;
    
    inline min_distance_index(const container_t &store, const vector_t &search) : _store(store), _search(search), _minimum(std::numeric_limits<float>::max(), ~0ull), _examined(0) {}
    inline min_distance_index(const container_t &store, const vector_t &search, const pair_t &minimum) : _store(store), _search(search), _minimum(minimum), _examined(0) {}
    
    inline void evaluate(const_iterator i)
    {
        uint64_t id = _index(*i);
        if (id < _store.size()) {
            const auto &match = _store.at(id);
            auto dsq = ::distance_sq_limit(match, _search, _minimum.first);
            if (dsq < _value(_minimum)) _minimum = pair_t(dsq, id);
            _examined++;
        }
    }
    
    std::ostream &print(std::ostream &os) const
    {
        return os << "TOTAL DISTANCE " << sqrt(_value(_minimum)) << " INDEX " << _index(_minimum) << " EXAMINED " << _examined << std::endl;
    }
};

void test_key_id_index(size_t count)
{
    constexpr size_t COUNT = 1024;
    typedef container<COUNT, float> container_t;
    typedef slices< slice<float, uint32_t> > slices_t;
    typedef key_id_index<uint64_t, uint64_t> index_t;
    typedef vector_generator<float> generator_t;
    typedef traverser<COUNT, float> traverser_t;
    
    generator_t generator(COUNT, generator_t::NORMALIZE_MODE_RANDOM_LENGTH);
    
    simple_timer t;
    index_t ki(20);
    std::cout << std::endl << std::endl << "key_id_index container build" << std::endl;
    container_t store;
    store.generate(generator, count);
    for (size_t i = 0; i < count; i++) {
        auto bits = store.ranks(i).bits();
        for (size_t n = 0; n < bits.size(); n++)
            ki.insert(bits[n], i);
    }
    ki.validate();
    std::cout << "key_id_index ";
    t.print_elapsed(std::cout);
    std::cout << "bin count " << ki.bin_count() << " size " << ki.size() << std::endl;

    // generate search array

    std::vector<float> v;
    generator.vector(v);
    container_t::vector_t array;
    std::copy(v.begin(), v.end(), array.begin());
    
    /*
    std::array<int16_t, COUNT> dimensions({ 0 });
    for (size_t i = 0; i < 32; i++) {
        dimensions[i] = i;
    }
    */
    traverser_t traverse(store.ranked_vectors());
    auto result = traverse.find(array, store);
    std::cout << "traverser TOTAL DISTANCE " << sqrt(_value(result)) << " INDEX " << _index(result) << std::endl;
    
    
    simple_timer tg;
    std::cout << std::endl << "linear search prime:" << std::endl;
    auto vi = store.linear_search(array);
    std::cout << "TOTAL DISTANCE " << sqrt(_value(vi)) << " INDEX " << _index(vi) << std::endl;
    tg.print_elapsed("linear search find complete");
    
    ranks<COUNT, 8, float, uint32_t> rank(array);
    auto bits = rank.bits();
    simple_timer te;
    min_distance_index<COUNT> mdi(store, array);
    for (size_t i = 0; i < bits.size(); i++) {
        auto bin = ki.bin(bits[i]);
        auto b = bin.begin(), e = bin.end(), it = bin.upper_bound(bits[i]);
        if (it == ki.end(bits[i])) {
            std::cout << "[none]" << std::endl;
        } else {
            for (auto i = it; i != e; ++i) {
                mdi.evaluate(i);
            }
            for (auto i = it; i != b; --i) {
                mdi.evaluate(i);
            }
        }
    }
    mdi.print(std::cout);
    te.print_elapsed("key_id_index find complete");
    
    simple_timer td;
    std::cout << std::endl << "linear search:" << std::endl;
    auto vi_ = store.linear_search(array);
    std::cout << "TOTAL DISTANCE " << sqrt(_value(vi_)) << " INDEX " << _index(vi_) << std::endl;
    td.print_elapsed("linear search find complete");
       
    simple_timer tg1;
    std::cout << std::endl << "linear search limit:" << std::endl;
    auto vi0 = store.linear_search_limit(array);
    std::cout << "TOTAL DISTANCE " << sqrt(_value(vi0)) << " INDEX " << _index(vi0) << std::endl;
    tg1.print_elapsed("linear search limit find complete");
    
    simple_timer tg2;
    std::cout << std::endl << "linear search limit msq:" << std::endl;
    auto vi4 = store.linear_search_limit_msq(array);
    std::cout << "TOTAL DISTANCE " << sqrt(_value(vi4)) << " INDEX " << _index(vi4) << std::endl;
    tg2.print_elapsed("linear search limit find complete");
    
    simple_timer th;
    std::cout << std::endl << "linear search ranked prime:" << std::endl;
    auto vi2 = store.ranked_vectors().linear_search_ranked(array);
    std::cout << "TOTAL DISTANCE " << sqrt(_value(vi2)) << " INDEX " << _index(vi2) << std::endl;
    th.print_elapsed("linear search ranked find complete");

    simple_timer th0;
    std::cout << std::endl << "linear search ranked:" << std::endl;
    auto va2 = store.ranked_vectors().linear_search_ranked(array);
    std::cout << "TOTAL DISTANCE " << sqrt(_value(va2)) << " INDEX " << _index(va2) << std::endl;
    th0.print_elapsed("linear search ranked find complete");

    simple_timer tj;
    std::cout << std::endl << "linear search ranked 2:" << std::endl;
    auto vi3 = store.ranked_vectors().linear_search_ranked_1(array);
    std::cout << "TOTAL DISTANCE " << sqrt(_value(vi3)) << " INDEX " << _index(vi3) << std::endl;
    tj.print_elapsed("linear search ranked find complete");
}
