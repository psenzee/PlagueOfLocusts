#pragma once

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
#include "stl.h"
#include "k_indices_queue.h"
#include "generators.h"
#include "key_index.h"

template <int D, typename KeyT>
struct key_finder
{
    typedef KeyT                       key_t;
    typedef typename KeyT::float_t     float_t;
    typedef key_index<key_t>           indices_t;
    typedef key_data<key_t>            key_data_t;
    typedef std::vector<float_t>       vector_t;
    typedef std::array<float_t, D>     vec_t;
    typedef container<D, float_t>      container_t;

    vector_t                                       _reference;
    key_data_t                                     _reference_key;
    const indices_t                               &_indices;
    const container_t                             &_container;
    k_indices_queue<float_t, std::less<float_t> >  _best_q;
    size_t                                         _thread_id;

    const bool LOG_EXTRA = true;
    
public:

    inline key_finder(const indices_t &indices, const container_t &container, size_t k_to_find = 8, size_t thread_id = 0) :
         _indices(indices), _container(container), _best_q(k_to_find), _thread_id(thread_id) {}
    
    inline const vector_t &reference() { return _reference; }
    
    inline void start(const vector_t &search)
    {
        _reference = search;
        _reference_key = _indices.create(_reference);
        std::cout << "reference key " << _reference_key << std::endl;
        auto m = find_range();
    }

    inline void enqueue(size_t index)
    {
        typename container_t::vector_t r;
        std::copy(_reference.begin(), _reference.end(), r.begin());
        auto distsq = _container.distance_sq(size_t(index), r);
        _best_q.add(distsq, index);
    }
    
    size_t find_range_one(const key_data_t &reference)
    {
        simple_timer t;
        vector_t v;
        key_data_t prev(reference), next(reference);
        int off = key_t::_has_offset();
        if (off) {
            prev.key.add_offset(0, 0, -off);
            next.key.add_offset(0, 0,  off);
        }
        off = key_t::_has_iterator_offset();
        auto lb  = _indices.lower_bound(prev) - off;
        auto ub  = _indices.upper_bound(lb, _indices.end(), next) + off;
        auto mid = _indices.lower_bound(lb, ub, reference);
        size_t count = 1;
        enqueue((*ub).index);
        enqueue((*lb).index);
        for (auto i = lb; i != ub; ++i) {
            enqueue((*i).index);
            count++;
        }
        t.print_elapsed("find range");
        if (LOG_EXTRA) {
            std::cout << "reference   " << reference << std::endl;
            std::cout << "examined    " << count << std::endl;
            std::cout << "lower bound " << *lb << std::endl;
            std::cout << "midpoint    " << *mid << std::endl;
            std::cout << "upper bound " << *ub << std::endl;
        }
        std::cout << "distance best " << sqrt(_value(_best_q.best())) << " kth-best " << sqrt(_value(_best_q.kth_best())) << std::endl;
        return count;
    }

    float_t find_range()
    {
        std::cout << "------------------------------------" << std::endl;
        key_data_t ref(_reference_key);
        size_t count = 0;
        simple_timer t;
        if (key_t::_does_swap_ranks()) {
            for (size_t i = 1, sz = key_data_t::key_t::COUNT - 1; i < sz; i++) {
                count += find_range_one(ref);
                ref.key.swap_ranks(0, i);
            }
        } else {
            count = find_range_one(ref);
        }
        t.print_elapsed("search total time");
        std::cout << "total examined " << count << std::endl;

        return 0;
    }
    
    void linear_search()
    {
        std::cout << "------------------------------------" << std::endl;
        std::cout << "linear search " << std::endl;
        simple_timer ts;
        typename container_t::value_index_t vi = _container.linear_search(_reference);
        std::cout << "distance sq " << _value(vi) << " index " << "" << _index(vi) << std::endl;
        std::cout << "distance best " << sqrt(_value(vi)) << " index " << "" << _index(vi) << std::endl;
        ts.print_elapsed("linear search total time");
    }
};
/*
template <int D, typename KeyT>
struct key_finder_parallel
{
    typedef KeyT                      key_t;
    typedef typename KeyT::float_t    float_t;
    typedef key_index<key_t>         indices_t;
    typedef key_data<key_t>          key_data_t;
    typedef std::vector<float_t>       vector_t;
    typedef std::array<float_t, D>     vec_t;
    typedef container<vec_t>           container_t;

    vector_t                         _reference;
    key_data_t                       _reference_key;
    const indices_t                 &_indices;
    const container_t               &_container;
    min_k_indices_queue<float_t>     _best_q;
    size_t                           _swap_rank_id;
    
public:


    inline key_finder_parallel(const indices_t &indices, const container_t &container, size_t k_to_find = 8, size_t swap_rank_id = 0) :
         _indices(indices), _container(container), _best_q(k_to_find), _swap_rank_id(swap_rank_id) {}
    
    inline void start(const vector_t &search)
    {
        _reference = search;
        _reference_key = _indices.create(_reference);
        std::cout << "reference key " << _reference_key << std::endl;
        auto m = _find_range();
    }

    inline void _enqueue(size_t index)
    {
        typename container_t::vector_t r;
        std::copy(_reference.begin(), _reference.end(), r.begin());
        auto distsq = _container.distance_sq(size_t(index), r);
        _best_q.add(distsq, index);
    }
    
    size_t _find_range_one(const key_data_t &reference)
    {
        simple_timer t;
        vector_t v;
        key_data_t prev(reference), next(reference);
        prev.key.add_offset(0, -1);
        next.key.add_offset(0,  1);
        auto lb  = _indices.lower_bound(prev);
        auto ub  = _indices.upper_bound(lb, _indices.end(), next);
        auto mid = _indices.lower_bound(lb, ub, reference);
        _enqueue((*ub).index);
        _enqueue((*lb).index);
        for (auto i = lb; i != ub; ++i)
            _enqueue((*i).index);
        t.print_elapsed("find range");
        std::cout << "reference   " << reference << std::endl;
        std::cout << "examined    " << _best_q.submitted_count() << std::endl;
        std::cout << "lower bound " << *lb << std::endl;
        std::cout << "midpoint    " << *mid << std::endl;
        std::cout << "upper bound " << *ub << std::endl;
        std::cout << "distance best " << sqrt(_value(_best_q.best())) << " kth-best " << sqrt(_value(_best_q.kth_best())) << std::endl;
        return _best_q.submitted_count();
    }

    float_t _find_range()
    {
        key_data_t ref(_reference_key);
        if (_swap_rank_id != 0)
            ref.key.swap_ranks(0, _swap_rank_id);
        size_t count = _find_range_one(ref);
        return 0;
    }
};
*/
template <size_t N, typename ContainerT, typename KeyT>
void test_key_index_n(const ContainerT &store, vector_generator<typename ContainerT::value_type> &generator)
{
    typedef typename ContainerT::value_type  float_t;
    typedef KeyT                             key_t;
    typedef typename ContainerT::vector_t    vec_t;
    typedef ContainerT                       container_t;
    typedef key_finder<ContainerT::COUNT, key_t> finder_t;

    typename container_t::vector_t u;
    size_t dims = u.size();
    std::vector<float_t> v;
    v.resize(dims);
    key_index<key_t> keys;
    simple_timer tb;
    size_t index = 0;
    for (const auto &u : store._data) {
        std::copy(u.begin(), u.end(), v.begin());
        keys.add_invalidate(v, index++);
    }
    tb.print_elapsed("  key_index store vector keys");
    simple_timer tc;
    keys.index();
    tc.print_elapsed("  key_index index keyed vectors");
    std::cout << keys;
    std::cout << std::dec;
//  std::cout << "total count " << keys.size() << " uniques " << count_unique_on_sorted(keys.begin(), keys.end(), std::less<key_t>()) << std::endl;
    simple_timer td;
    auto finder = finder_t(keys, store);
    v = generator.vector();
    finder.start(v);
    td.print_elapsed("  key_finder start");
    finder.linear_search();
}
/*
template <size_t N, size_t M, typename KeyT>
void test_key_index_n_parallel(size_t count, int normalize_mode)
{
    typedef float                         float_t;
    typedef KeyT                          key_t;
    typedef std::array<float_t, M>        vec_t;
    typedef container<vec_t>              container_t;
    typedef key_finder_parallel<M, key_t> finder_t;

    size_t dims = M;
    std::vector<float_t> v;
    std::cout << std::endl << "key_index [" << key_t::COUNT << "] " << count << " vectors of dimension " << dims << std::endl;
    key_index<key_t> keys;
    simple_timer ta;
    container_t container;
    for (size_t i = 0; i < count; i++) {
        generate_vector(dims, v);
        process_vector(v, normalize_mode);
        keys.add_invalidate(v, container.size());
        vec_t u;
        std::copy(v.begin(), v.end(), u.begin());
        container.add(u);
    }
    ta.print_elapsed("  key_index generate + key vectors");
    simple_timer tc;
    keys.index();
    tc.print_elapsed("  key_index index keyed vectors");
    std::cout << keys;
    //for (auto r : keys) std::cout << r << std::endl;
    std::cout << std::dec;
    //std::cout << "total count " << keys.size() << " uniques " << count_unique_on_sorted(keys.begin(), keys.end(), std::less<key_t>()) << std::endl;

    generate_vector(dims, v);
    process_vector(v, normalize_mode);

    unsigned int num_threads = std::thread::hardware_concurrency();
    std::cout << "number of threads " << num_threads << std::endl;
    simple_timer td;
    enum { K_CLOSEST = 8, RANKS = 8 };
    std::vector<std::thread> threads;
    std::vector<finder_t *> finders;
    for (size_t i = 0; i < std::min(size_t(num_threads), size_t(RANKS - 2)); i++) {
        finder_t *f = new finder_t(keys, container, K_CLOSEST, i + 1);
        finders.push_back(f);
        threads.push_back(std::thread(
            [i](std::vector<float_t> v, const key_index<key_t> &keys, const container_t &container) -> void {
                finder_t finder(keys, container, K_CLOSEST, i + 1); finder.start(v); },
              v, std::ref(keys), std::ref(container)));
    }
    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    td.print_elapsed("  finder start parallel");
}
*/
