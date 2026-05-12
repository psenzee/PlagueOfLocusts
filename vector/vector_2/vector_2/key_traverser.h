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

template <typename ContainerT, size_t KeyRanksN>
struct key_traverser
{
    typedef ContainerT                                  container_t;
    typedef typename container_t::value_type            real_t;

    static constexpr size_t COUNT = container_t::COUNT;
    
    typedef key_traverser<container_t, KeyRanksN>       self_t;
    typedef key_index<container_t, KeyRanksN>           key_index_t;
    typedef key<real_t, COUNT, KeyRanksN>               key_t;
    typedef key_id<real_t, COUNT, KeyRanksN>            key_id_t;
    typedef range_state<container_t, KeyRanksN>         range_state_t;
    typedef typename key_t::key_ranks_t                 dimensions_t;
    typedef std::array<real_t, COUNT>                   flat_vector_t;
    typedef std::pair<real_t, size_t>                   value_index_t;

    const key_index_t &_index;
    bool               _breadth_first;
    size_t             _max_ids_per_state;
    size_t             _full_distance_counts;
    
    inline key_traverser(const key_index_t &index) :
         _index(index), _breadth_first(false), _max_ids_per_state(3), _full_distance_counts(0) {}
    
    // TODO -- minimum distance
    // TODO -- exact match/exclude exact match
    range_state_t find_ids(const flat_vector_t &search, const container_t &store)
    {
        range_state_t rs(search, store, _index);
        rs.update();
        return rs;
    }
};
