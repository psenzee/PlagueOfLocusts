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
#include "search_vector.h"
#include "k_indices_queue.h"
#include "pairs.h"

template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN, size_t LongKeyRanksN = 16>
struct range_state
{
    static const int MAX_RANK_SEARCH = KeyRanksN;
    static const int N_CLOSEST       = 8;

    typedef FloatT                                                      real_t;
    typedef range_state<FloatT, N, IndexT, KeyRanksN>                   self_t;
    typedef key_index<FloatT, N, IndexT, KeyRanksN>                     key_index_t;
    typedef key<FloatT, N, IndexT, KeyRanksN>                           key_t;
    typedef key<FloatT, N, IndexT, LongKeyRanksN>                       long_key_t;
    typedef key_id<FloatT, N, IndexT, KeyRanksN>                        key_id_t;
    typedef typename key_t::key_ranks_t                                 dimensions_t;
    typedef typename key_index_t::const_range_t                         const_range_t;
    typedef typename key_index_t::const_iterator                        const_iterator;
    typedef std::array<FloatT, N>                                       flat_vector_t;
    typedef container<N, FloatT>                                        container_t;
    typedef std::pair<real_t, size_t>                                   value_index_t;
    typedef k_indices_queue<real_t, std::greater<real_t> >              queue_t;
    typedef std::vector<uint64_t>                                       ids_t;
    typedef search_vector<FloatT, N, IndexT, KeyRanksN, LongKeyRanksN>  search_vector_t;

    const container_t     &store;
    const key_index_t     &index;
    const search_vector_t  search;
    queue_t                queue;
    const_range_t          range;
    value_index_t          minimum;
    size_t                 distance_count;
    size_t                 max_common_dimensions;
    int                    total_top_level;
    
    inline range_state(const flat_vector_t &search, const container_t &store, const key_index_t &index, const const_range_t &range)
        : search(search), store(store), index(index), range(range), minimum(std::numeric_limits<real_t>::max(), 0), distance_count(0), total_top_level(0), max_common_dimensions(0), queue(N_CLOSEST) {}
    
    inline range_state(const flat_vector_t &vector, const container_t &store, const key_index_t &index)
        : search(vector), store(store), index(index), minimum(std::numeric_limits<real_t>::max(), 0),
          distance_count(0), total_top_level(0), max_common_dimensions(0), queue(N_CLOSEST)
    {
        // set base id range
        typename key_id_t::base_id_comparer c;
        range = const_range_t(index.begin(), index.end());
        range = std::equal_range(range.first, range.second, search.key_id, c);
    }
    
    static std::ostream &print_range(std::ostream &os, const const_range_t &r, const char *label = nullptr)
    {
        return os << "range interval " << (label ? label : "") << " " << (r.second - r.first) << std::endl;
    }

    inline std::ostream &print_range(std::ostream &os, const char *label = nullptr) const
    {
        //return print_range(os, range, label);
        return _print_range(os, range);
    }
    
    void update()
    {
        _from_exp_norm();
    }

    void _from_exp_norm()
    {
        std::array<int, 3> offsets({ 0, -1, 1 /*, -2, 2, -3, 3, -4, 4, -5, 5*/ });
        typename key_id_t::exp_norm_comparer c;
        auto r(range);
        for (auto i : offsets) {
            auto key_id(search.key_id);
            key_id.key.exp_norm = int8_t(std::max(std::min(i + key_id.key.exp_norm, 127), -128));
            r = std::equal_range(range.first, range.second, key_id, c);
            if (r.second - r.first > 0) {
                //_print_range(std::cout << "offset " << i << " " , r);
                if (r.first < r.second)
                    _from_norm_ranks(key_id, r);
            }
        }
    }

    void _update_range(size_t id)
    {
        _update_minimum(id);
        _update_range_from_distance(minimum.first);
    }

    void _update_from_range(const const_range_t &r)
    {
        real_t initial = minimum.first;
        for (auto j = r.first; j != r.second; ++j)
            _update_minimum((*j).id);
        if (minimum.first < initial)
            _update_range_from_distance(minimum.first);
    }
    
    inline static real_t norm_diff(const key_t &a, const key_t &b) { return abs(a.norm - b.norm); }

    const_range_t _update_from_range_limit(const const_range_t &r, const key_t &search_key)
    {
        auto b = r.first, e = r.second;
        while (b < e) {
            if (norm_diff((*b).key, search_key) <= minimum.first)
                break;
            b++;
        }
        while (b < e) {
            if (norm_diff((*(e - 1)).key, search_key) <= minimum.first)
                break;
            e--;
        }
        for (auto j = b; j < e; ++j)
            if (norm_diff((*j).key, search_key) <= minimum.first)
                _update_minimum((*j).id);
        return const_range_t(b, e);
    }

    void _from_norm_ranks(const key_id_t &sk, const const_range_t &rn)
    {
        auto i = std::max(rn.first, range.first), e = std::min(rn.second, range.second);
        key_id_t k(sk);
        typename key_id_t::exp_norm_comparer c;
        while (i < e) {
            k.key.exp_norm = (*i).key.exp_norm;
            const_range_t r = std::equal_range(i, e, k, c);
            if (r.first == e) break;
            if (r.first >= r.second) continue;
            _from_n_ranks(sk, r);
            i = r.second;
        }
    }
    
    bool _from_n_ranks(const key_id_t &sk, const const_range_t &rn, size_t rank = 0)
    {
        if (rank >= MAX_RANK_SEARCH) {
            _update_from_range(rn);
            return true;
        }
        bool any = false;
        for (size_t i = rank; i < MAX_RANK_SEARCH; i++) {
            key_id_t k(sk);
            k.key.swap_rank(rank, i);
            any |= _from_ranks(k, rn, rank);
        }
        return any;
    }

    bool _from_ranks(const key_id_t &sk, const const_range_t &rn, size_t rank = 0)
    {
        if (rank >= MAX_RANK_SEARCH) {
            _update_from_range(rn);
            return true;
        }
        const_range_t r(range_intersect(rn, range));
        auto i = r.first, e = r.second;
        key_id_t k(sk);
        typename key_id_t::rank_comparer c(rank);
        while (i < e) {
            const_range_t r = std::equal_range(i, e, k, c);
            if (r.first == e)
                return false;
            if (r.first == r.second) {
                _update_from_range(r);
                return false;
            }
            bool any = _from_n_ranks(sk, r, rank + 1);
            if (!any) {
                r = _update_from_range_limit(r, k.key);
                return true;
            }
            i = r.second;
        }
        return false;
    }
    
private:

    real_t _update_minimum(size_t id, real_t limit = std::numeric_limits<real_t>::max())
    {
        distance_count++;
        limit = std::min(limit, minimum.first);
        real_t sq = ::distance_sq_limit(store.at(id), search.vector, limit * limit), dist = sqrt(sq);
        if (dist <= limit) {
            minimum = value_index_t(dist, id);
            queue.add(dist, id);
        }
        return dist;
    }
    
    range_state &_update_range_from_distance(real_t distance)
    {
        if (distance == std::numeric_limits<real_t>::max())
            return *this;
        typename key_id_t::initial_comparer c;
        if (distance <= 0) {
            range = std::equal_range(range.first, range.second, search.key_id, c);
            return *this;
        }
        key_id_t lo(search.key_id), hi(search.key_id);
        lo.key.norm -= distance;
        hi.key.norm += distance;
        auto e = index.end();
        if (range.first  != e && !c(lo, *(range.first)))  range.first  = std::lower_bound(range.first, range.second, lo, c);
        if (range.second != e &&  c(hi, *(range.second))) range.second = std::upper_bound(range.first, range.second, hi, c);
        //print_range(std::cout);
        return *this;
    }
    
    inline std::ostream &_print_range(std::ostream &os, const const_range_t &range) const
    {
        return os << "range (" << (range.first - index.begin()) << "..." << (range.second - index.begin()) << ") total " << (range.second - range.first) << std::endl;
    }

    inline std::ostream &_print_state(std::ostream &os, const self_t &state) const
    {
        _print_range(os, state.range) << " rank " << state.rank << " dimensions ";
        return _stream_out(os, state.dimensions, 16) << std::endl;
    }
};
