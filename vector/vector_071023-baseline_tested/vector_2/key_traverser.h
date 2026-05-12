#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include "stl.h"
#include "bfloat16.h"
#include "container.h"
#include "simple_timer.h"
#include "key.h"
#include "key_index.h"
#include "utils.h"
#include "range_state.h"

template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN>
struct key_traverser
{
    typedef FloatT                                       real_t;
    typedef key_traverser<FloatT, N, IndexT, KeyRanksN>  self_t;
    typedef key_index<FloatT, N, IndexT, KeyRanksN>      key_index_t;
    typedef key<FloatT, N, IndexT, KeyRanksN>            key_t;
    typedef key_id<FloatT, N, IndexT, KeyRanksN>         key_id_t;
    typedef range_state<FloatT, N, IndexT, KeyRanksN>    range_state_t;
    typedef typename key_t::key_ranks_t                  dimensions_t;
    typedef typename key_index_t::const_range_t          const_range_t;
    typedef typename key_index_t::const_iterator         const_iterator;
    typedef std::array<FloatT, N>                        flat_vector_t;
    typedef container<N, FloatT>                         container_t;
    typedef std::pair<real_t, size_t>                    value_index_t;
    typedef std::vector<uint64_t>                        ids_t;
    
    struct state_t { dimensions_t dimensions; size_t rank; const_range_t range; };
    
    typedef std::deque<state_t>                                 deque_t;

    const key_index_t &_index;
    bool               _breadth_first;
    size_t             _max_ids_per_state;
    size_t             _full_distance_counts;
    
    inline key_traverser(const key_index_t &index) :
         _index(index), _breadth_first(false), _max_ids_per_state(3), _full_distance_counts(0) {}
    
    range_state_t find_ids(const flat_vector_t &search, const container_t &store)
    {
        range_state_t rs(search, store, _index);
        rs.update();
        return rs;
    }

private:
    
    enum { USE_QUEUE = false };
};
