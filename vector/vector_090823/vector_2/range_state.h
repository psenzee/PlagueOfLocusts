#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include "stl.h"
#include "bfloat16.h"
#include "container.h"
#include "simple_timer.h"
#include "key.h"
#include "key_id.h"
#include "key_index.h"
#include "utils.h"
#include "search_vector.h"
#include "n_best.h"
#include "pairs.h"
#include "bloom.h"
#include "intersect.h"
#include "pairs.h"
#include "arrays.h"
#include "simple_bitset.h"

template <typename ContainerT, size_t KeyRanksN>
struct range_state
{
    typedef ContainerT                                                  container_t;
    
    static constexpr size_t COUNT = container_t::COUNT;
    
    typedef range_state<ContainerT, KeyRanksN>                          self_t;
    typedef typename container_t::value_type                            real_t;
    typedef key_index<container_t, KeyRanksN>                           key_index_t;
    typedef key<real_t, COUNT, KeyRanksN>                               key_t;
    typedef key_id<real_t, COUNT, KeyRanksN>                            key_id_t;
    typedef typename key_id_t::id_t                                     id_t;
    typedef typename key_t::key_ranks_t                                 key_ranks_t;
    typedef typename key_index_t::const_range_t                         const_range_t;
    typedef typename key_index_t::const_iterator                        const_iterator;
    typedef std::array<real_t, COUNT>                                   flat_vector_t;
    typedef std::pair<real_t, size_t>                                   value_index_t;
    typedef n_best<real_t, std::less<real_t> >                          queue_t;
    typedef typename key_index_t::norms_t                               norms_t;
    typedef std::vector<id_t>                                           ids_t;
    typedef search_vector<real_t, COUNT, KeyRanksN>                     search_vector_t;
    typedef typename key_index_t::norm_t                                norm_t;
    typedef typename key_index_t::dnids_t                               dnids_t;
    typedef typename dnids_t::const_iter_t                              dnids_const_iter_t;
    typedef typename dnids_t::const_range_t                             dnids_const_range_t;
    
    static constexpr size_t MAX_RANK_SEARCH = KeyRanksN;
    static constexpr size_t N_CLOSEST       = 8;
    static constexpr size_t BLOOM_BITS      = 128 * 1024 * 8;
    
    static constexpr real_t REAL_MAX        = std::numeric_limits<real_t>::max();
    
    static constexpr bool   OPTIMIZE        = true;
    static constexpr bool   BLOOM           = true;

    const container_t     &store;
    const key_index_t     &index;
    const search_vector_t  search;
    queue_t                queue;
    size_t                 distance_count;
    size_t                 compare_count;
    bloom<BLOOM_BITS>      examined;

    inline range_state(const flat_vector_t &vector, const container_t &store, const key_index_t &index)
        : search(vector), store(store), index(index), distance_count(0), compare_count(0), queue(N_CLOSEST) {}

    void update()                                           { _from_dnids(search); }

    inline const value_index_t &kth_minimum()         const { return queue.kth_best(); }
    
    inline const value_index_t &minimum()             const { return queue.best(); }
    
    inline void set_minimum(const value_index_t &min)       { queue.add(min); }

private:
    
    inline const key_t &_key(id_t id)                                 const { return index._keys[id]; }
    inline const key_t &_key(const typename ids_t::const_iterator &i) const { return index._keys[*i]; }
    inline const key_t &_key(const dnids_const_iter_t &i)             const { return index._keys[(*i).id]; }
    
    inline bool _up(dnids_const_iter_t &i, const dnids_const_iter_t &begin, real_t norm, real_t min)
    {
        if (i < begin) return false;
        compare_count++;
        if ((*i).norm >= norm - min &&
            (*i).norm >= norm - kth_minimum().first) {
            _update_minimum(index._keys[(*i).id], (*i).id);
            --i;
            return true;
        }
        return false;
    }
    
    inline bool _down(dnids_const_iter_t &i, const dnids_const_iter_t &end, real_t norm, real_t min)
    {
        if (i >= end) return false;
        compare_count++;
        if ((*i).norm < norm + min &&
            (*i).norm < norm + kth_minimum().first) {
            _update_minimum(index._keys[(*i).id], (*i).id);
            ++i;
            return true;
        }
        return false;
    }

    inline bool _up(dnids_const_iter_t &i, const dnids_const_iter_t &begin, real_t norm)
    {
        if (i < begin) return false;
        compare_count++;
        if ((*i).norm >= norm - kth_minimum().first) {
            _update_minimum(index._keys[(*i).id], (*i).id);
            --i;
            return true;
        }
        return false;
    }
    
    inline bool _down(dnids_const_iter_t &i, const dnids_const_iter_t &end, real_t norm)
    {
        if (i >= end) return false;
        compare_count++;
        if ((*i).norm < norm + kth_minimum().first) {
            _update_minimum(index._keys[(*i).id], (*i).id);
            ++i;
            return true;
        }
        return false;
    }

    void _from_dnids(const dnids_const_range_t &range, const search_vector_t &search)
    {
        real_t norm = search.norm();
        auto i = range.first, e = range.second, j = index._dnids.lower_norm(i, e, norm), k = j;
        if (j == e) return;
        /*
        real_t min = kth_minimum().first;
        while (_up(j, i, norm, min) && _down(k, e, norm, min)) min = kth_minimum().first;
        while (_up(j, i, norm, min))                           min = kth_minimum().first;
        while (_down(k, e, norm, min))                         min = kth_minimum().first;
         */
        while (_up(j, i, norm) && _down(k, e, norm)) ;
        while (_up(j, i, norm))                      ;
        while (_down(k, e, norm))                    ;
    }

    void _from_dnids(const search_vector_t &search)
    {
        const auto &kid = search.key_id;
        const auto &k = kid.key;
        real_t base_ratio = 0.95; // first ratio
        for (size_t i = 0; i < KeyRanksN; i++) {
            auto r = index._dnids.find_dimension(k.ranks[i]);
            if (r.first == r.second) continue;
            _from_dnids(r, search);
            real_t ratio(search.ratios[i]);
            if (ratio > base_ratio) break;
            base_ratio = 0.75; // subsequent ratios
        }
    }

    inline real_t _partial_distance_sq_initial(const search_vector_t &search, const key_t &key, real_t limit_sq)
    {
        real_t dsq = 0;
        for (size_t i = 0; i < KeyRanksN && dsq < limit_sq; ++i)
            dsq += _sq(key.values[i] - search.vector[abs(key.ranks[i])]);
        return dsq;
    }

    bool _partial_distance_less(const search_vector_t &search, const key_t &key, real_t limit)
    {
        real_t limit_sq = limit * limit;
        real_t dsq = _partial_distance_sq_initial(search, key, limit_sq);
        if (dsq >= limit_sq)
            return false;
        const auto &sk = search.key_id.key;
        // we also incorporate search.key.ranks which weren't in key.ranks
        // match them to the closest value to search's whose absolute value
        // would also be <= the minimum from key.values
        real_t minva(REAL_MAX);
        for (size_t i = 0; i < KeyRanksN; ++i) {
            real_t va(abs(key.values[i]));
            if (va < minva) minva = va;
        }
        for (size_t i = 0; i < KeyRanksN; ++i) {
            if (find_abs(sk.ranks[i], key.ranks) == -1) { // TODO here use abs_order?? -- is there a faster way here?
                real_t u = search.abs_values[i];
                dsq += _sq(u - std::min(u, minva));
                if (dsq >= limit_sq) return false;
            }
        }
        return dsq < limit_sq;
    }

    bool _possibly_less_than_limit(const key_t &key, id_t id, real_t limit)
    {
        if (!OPTIMIZE) return true;
        const auto &sk = search.key_id.key;
        if (abs(sk.norm - key.norm) > limit || (BLOOM && examined.isset(id)))
            return false;
        return _partial_distance_less(search, key, limit);
    }

    inline void _update_minimum(const key_id_t &kid) { _update_minimum(kid.key, kid.id); }

    void _update_minimum(const key_t &key, id_t id)
    {
        real_t limit = kth_minimum().first;
        if (!_possibly_less_than_limit(key, id, limit))
            return;
        if (OPTIMIZE && BLOOM) examined.set(id);
        distance_count++;
        real_t limit_sq = _sq(limit),
               dist_sq  = _retrieve_and_calculate_distance_sq_limit(key, id, limit_sq);
        if (dist_sq <= limit_sq) set_minimum(value_index_t(sqrt(dist_sq), id));
    }

    real_t _retrieve_and_calculate_distance_sq_limit(const key_t &key, id_t id, real_t limit_sq)
    {
        return arrays::distance_sq_limit(store.at(id), search.vector, limit_sq);
    }
};

