#pragma once

#include <iostream>
#include <limits>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstddef>

#include "stl.h"
#include "slices.h"
#include "simple_timer.h"

struct _vector_state
{
    size_t id;
    int    rank;
    float  distance_sq;
    struct compare { inline bool operator()(const _vector_state &a, const _vector_state &b) const
        { return (a.distance_sq > b.distance_sq) ? true : (a.rank > b.rank); } };

    inline void set(size_t index, int r, float dsq) { id = index; rank = r; distance_sq = dsq; }
};

template <typename ContainerT, typename SlicesT>
struct slices_finder
{
    typedef SlicesT                                                        slices_t;
    typedef typename SlicesT::slice_type                                   slice_t;
    typedef _vector_state                                                  state_t;
    typedef ContainerT                                                     container_t;
    typedef typename ContainerT::vector_t                                  array_t;
    typedef std::vector<state_t>                                           state_vector_t;
    typedef std::pair<float, size_t>                                       rank_pair_t;
    typedef std::array<rank_pair_t, container_t::COUNT>                    ranks_t;
    typedef std::priority_queue<state_t, state_vector_t, state_t::compare> priority_queue_t;

private:
    
    slices_t         &_slices;
    container_t      &_container;
    priority_queue_t  _queue;
    array_t           _search;
    ranks_t           _ranks;
    
    enum { RANKED = 8 };
    
    inline void _push(size_t id, int rank = -1, float distance_sq = std::numeric_limits<float>::max())
    {
        _vector_state vs;
        vs.set(id, rank, distance_sq);
        _queue.push(vs);
    }
    
    inline std::ostream &_print(std::ostream &s, const _vector_state &vs) const
    {
        return std::cout << "["  << vs.id <<  " " << vs.rank << " " << vs.distance_sq << " (" << sqrt(vs.distance_sq) << ")]" << std::endl;
    }
    
    inline void _push_iter(typename slice_t::index_iter_t iter, float search, size_t rank)
    {
        auto id = _index(*iter);
        auto target = _value(*iter);

        // push state
        _vector_state vs;
        target -= search;
        vs.set(id, int(rank), target * target);
        _queue.push(vs);

        _print(std::cout, vs);
    }
    
    inline void _push_new_state(size_t rank)
    {
        simple_timer t;
        
        _vector_state vs;
        const auto &pair = _ranks[rank];

        auto dimension = _index(pair);
        auto search = _value(pair);

        const slice_t &s = _slices.slice(dimension);
        auto iter = s.upper_bound(search);

        for (size_t i = 0; i < 5; i++) {
            _push_iter(iter + i, search, rank);
            if (i != 0)
                _push_iter(iter - i, search, rank);
        }

        t.print_elapsed("slices_finder._push_new_state");
        _print(std::cout, vs);
    }
    
    inline void _advance_top_state()
    {
        simple_timer t;

        auto vs = _queue.top();
        ++vs.rank;
        const auto &pair = _ranks[vs.rank];

        auto dimension = _index(pair);
        auto search = _value(pair);
        
        const slice_t &s = _slices.slice(dimension);
        auto target = s.at(vs.id);

        // pop top
        _queue.pop();

        // push state
        target -= search;
        vs.distance_sq += target * target;
        _queue.push(vs);
        
        t.print_elapsed("slices_finder._advance_top_state");
        _print(std::cout, vs);
    }

    void _initialize_ranks()
    {
        simple_timer t;
        size_t sz = _search.size();
        for (size_t i = 0; i != sz; ++i)
            _ranks[i] = rank_pair_t(_search[i], i);
        pair_compare_first_greater_abs< std::pair<float, size_t> > c;
        auto begin = _ranks.begin(), mid = begin + RANKED, end = _ranks.end();
        std::nth_element(begin, mid, end, c);
        std::sort(begin, mid, c);
        t.print_elapsed("slices_finder._initialize_ranks");
    }

public:
    
    inline slices_finder(container_t &container, slices_t &slices) : _slices(slices), _container(container) {}
    
    void set_search(const array_t &a)
    {
        simple_timer t;
        _search = a;
        _initialize_ranks();
        _push_new_state(0);
        t.print_elapsed("slices_finder.set_search");
    }

    void process()
    {
        _advance_top_state();
    }
};

void test_slices_finder(size_t count);
