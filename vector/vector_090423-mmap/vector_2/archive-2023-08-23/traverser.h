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
#include <deque>

#include "stl.h"
#include "simple_timer.h"
#include "ranked_vector.h"
#include "ranked_vectors.h"
#include "container.h"

template <int M, typename FloatT>
struct traverser
{
    typedef FloatT                                        real_t;
    typedef ranked_vectors<M, FloatT>                     vectors_t;
    typedef typename vectors_t::ranked_t                  ranked_t;
    typedef typename vectors_t::const_iterator            const_iterator;
    typedef typename vectors_t::const_range_t             range_t;
    typedef std::array<int16_t, M>                        dimensions_t;
    typedef std::array<FloatT, M>                         flat_vector_t;
    typedef container<M, FloatT>                          container_t;
    typedef std::pair<real_t, size_t>                     value_index_t;
    
    struct state_t { dimensions_t dimensions; size_t rank; range_t range; };
    
    typedef std::deque<state_t>                           deque_t;
    typedef std::vector<uint64_t>                         ids_t;
    
    const vectors_t &_vectors;
    deque_t          _deque;
    ids_t            _ids;
    bool             _breadth_first;
    size_t           _max_ids_per_state;
    value_index_t    _minimum;
    
    inline traverser(const vectors_t &vectors) : _vectors(vectors), _breadth_first(false), _max_ids_per_state(3) {}
    
    inline std::ostream &_print_range(std::ostream &os, const range_t &range) const
    {
        return os << "range " << (range.first - _vectors.begin()) << "..." << (range.second - _vectors.begin());
    }
    
    inline std::ostream &_print_state(std::ostream &os, const state_t &state) const
    {
        _print_range(os, state.range) << " rank " << state.rank << " dimensions ";
        return _stream_out(os, state.dimensions, 16) << std::endl;
    }
    
    inline const ids_t &find_ids(const flat_vector_t &search) { return find_ids(ranked_t(search)); }

    const ids_t &find_ids(const ranked_t &search)
    {
        simple_timer t;
        dimensions_t dim;
        for (size_t i = 0, sz = search.size(); i != sz; ++i)
             dim[i] = search.data[i].dimension;
        state_t s({ dim, 0, range_t(_vectors.begin(), _vectors.end()) });
      //_print_state(std::cout << "start ", s) << std::endl;
        _push_next(s);
        size_t max_size = 0;
        while (_deque.size()) {
            _process();
            if (_deque.size() > max_size) {
                std::cout << "largest size " << max_size << std::endl;
                max_size = _deque.size();
            }
        }
        std::cout << "total ids " << _ids.size() << std::endl;
        t.print_elapsed(std::cout);
        return _ids;
    }
    
    const value_index_t &find(const flat_vector_t &search, const container_t &container)
    {
        _minimum = value_index_t(std::numeric_limits<real_t>::max(), 0);
        value_index_t current(_minimum);
        for (auto id : find_ids(search)) {
            current.first = ::distance_sq_limit(search, container.at(id), _minimum.first);
            current.second = id;
            if (current.first < _minimum.first) _minimum = current;
        }
        return _minimum;
    }

private:
    
    enum { USE_QUEUE = false };
    
    void _push_next(const state_t &s)
    {
        enum { SWAP_COUNT = 16 };
        for (size_t i = 0, sz = SWAP_COUNT - s.rank; i < sz; i++) {
            dimensions_t dim(s.dimensions);
            size_t t = dim[i]; dim[i] = dim[s.rank]; dim[s.rank] = t;
            auto range = _vectors.equal_range(s.range.first, s.range.second, s.rank, dim[s.rank]);
            if (_queue_ids(range, false)) continue;
            state_t ns({ dim, s.rank + 1, range });
            if (USE_QUEUE) {
                //_print_state(std::cout << "push ", ns) << std::endl;
                _deque.push_front(ns);
            } else {
                _push_next(ns);
            }
        }
    }

    inline bool _queue_ids(const range_t &range, bool force)
    {
        if (range.first == _vectors.end())
            return true;
        size_t count = range.second - range.first;
        if (!force && count >= _max_ids_per_state) return false;
        for (auto i = range.first; i != range.second; ++i)
            _ids.push_back((*i).id);
        return true;
    }
    
    void _process()
    {
        state_t s;
        if (_breadth_first) {
            s = _deque.back();
            _deque.pop_back();
        } else {
            s = _deque.front();
            _deque.pop_front();
        }
        //_print_state(std::cout << "pop ", s) << std::endl;
        if (s.rank < 8) {
            _push_next(s);
            return;
        }
        _queue_ids(s.range, true);
    }
};
