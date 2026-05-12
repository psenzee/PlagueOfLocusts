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
#include "key_index.h"
#include "utils.h"
#include "search_vector.h"
#include "k_indices_queue.h"
#include "pairs.h"
#include "bloom.h"
#include "intersect.h"
#include "pairs.h"

template <typename FloatT, size_t N, size_t KeyRanksN>
struct range_state
{
    static constexpr size_t MAX_RANK_SEARCH = KeyRanksN;
    static constexpr size_t N_CLOSEST       = 8;
    static constexpr size_t BLOOM_BITS      = 128 * 1024 * 8;
    
    static constexpr bool   OPTIMIZE        = true;
    static constexpr bool   BLOOM           = true;

    typedef FloatT                                                      real_t;
    typedef range_state<FloatT, N, KeyRanksN>                           self_t;
    typedef key_index<FloatT, N, KeyRanksN>                             key_index_t;
    typedef key<FloatT, N, KeyRanksN>                                   key_t;
    typedef key_id<FloatT, N, KeyRanksN>                                key_id_t;
    typedef typename key_id_t::id_t                                     id_t;
    typedef typename key_t::key_ranks_t                                 key_ranks_t;
    typedef typename key_t::traits_t                                    key_traits_t;
    typedef typename key_index_t::const_range_t                         const_range_t;
    typedef typename key_index_t::const_iterator                        const_iterator;
    typedef std::array<FloatT, N>                                       flat_vector_t;
    typedef container<N, FloatT>                                        container_t;
    typedef std::pair<real_t, size_t>                                   value_index_t;
    typedef k_indices_queue<real_t, std::greater<real_t> >              queue_t;
    typedef typename key_index_t::norms_t                               norms_t;
    typedef std::vector<id_t>                                           ids_t;
    typedef search_vector<FloatT, N, KeyRanksN>                         search_vector_t;

    const container_t     &store;
    const key_index_t     &index;
    const search_vector_t  search;
    queue_t                queue;
    const_range_t          range;
    value_index_t          minimum;
    size_t                 distance_count;
    size_t                 compare_count;
    int                    total_top_level;
    bloom<BLOOM_BITS>      examined;

    inline range_state(const flat_vector_t &search, const container_t &store, const key_index_t &index, const const_range_t &range)
        : search(search), store(store), index(index), range(range), minimum(std::numeric_limits<real_t>::max(), 0), distance_count(0), compare_count(0), total_top_level(0), queue(N_CLOSEST) {}

    inline range_state(const flat_vector_t &vector, const container_t &store, const key_index_t &index)
        : search(vector), store(store), index(index), minimum(std::numeric_limits<real_t>::max(), 0), distance_count(0), compare_count(0), total_top_level(0), queue(N_CLOSEST) {}

    void set_base_id_range()
    {
        range = const_range_t(index.begin(), index.end());
    }
    
    inline bool _use_exp_norm_ranks(const key_id_t &sk) const
    {
        return false;
    }

    void update()
    {
        set_base_id_range();
        if (_use_exp_norm_ranks(search.key_id))
            _from_exp_norm_ranks();
        else
            _from_ids_n(search.key_id);
    }

    void _from_exp_norm_ranks()
    {
        std::array<int, 3> offsets({ 0, -1, 1 });
        typename key_id_t::exp_norm_comparer c;
        auto r(range);
        for (auto i : offsets) {
            auto kid(search.key_id);
            kid.key.exp_norm = int8_t(std::max(std::min(i + kid.key.exp_norm, 127), -128));
            r = std::equal_range(range.first, range.second, kid, c);
            if (r.first < r.second) _from_norm_ranks(kid, r);
        }
    }
    
    const_range_t _update_from_range(const const_range_t &r)
    {
        for (auto j = r.first; j != r.second; ++j)
            _update_minimum(*j);
        return r;
    }

    void _from_norm_ranks(const key_id_t &sk, const const_range_t &rn)
    {
        auto i = rn.first, e = rn.second;
        key_id_t k(sk);
        typename key_id_t::exp_norm_comparer c;
        while (i < e) {
            const_range_t r = std::equal_range(i, e, k, c);
            if (r.first == e) break;
            if (r.first < r.second) {
                _from_ranks(sk, r);
                i = r.second;
            }
        }
    }

    void _from_norms(const key_id_t &sk)
    {
        _from_norms(index._norms, sk);
    }

    inline const key_t &_key(id_t id) const
    {
        return index._keys[id];
    }

    inline const key_t &_key(const typename ids_t::const_iterator &i) const
    {
        return index._keys[*i];
    }

    inline real_t _norm_from_id(id_t id) const
    {
        return index._keys[id].norm;
    }

    void _from_ids(const ids_t &ids, const key_id_t &sk)
    {
        if (ids.size() == 0) return;
        typedef typename key_index_t::norm_t norm_t;
        real_t norm = sk.key.norm;
        auto kid(sk);
        kid.id = 0xffffffff;
        auto cn(index.comparer(kid));
        auto i = ids.begin(), e = ids.end();
        auto r = std::equal_range(i, e, kid.id, cn);
        if (r.first == e) return;
        auto mid = r.first + (r.second - r.first) / 2;
        auto j = mid, k = mid + 1;
        bool top = false, bottom = false;
        while ((top    = (j >= i && _key(j).norm >= std::max(0.f, norm - minimum.first))) ||
               (bottom = (k <  e && _key(k).norm <= norm + minimum.first))) {
            if (top)    { _update_minimum(_key(j), *j); --j; }
            if (bottom) { _update_minimum(_key(k), *k); ++k; }
        }
    }

    void _from_ids_n(const key_id_t &sk)
    {
        const auto &kid = search.key_id;
        const auto &k = kid.key;
        real_t base_ratio = 0.95;
        for (size_t i = 0; i < KeyRanksN; i++) {
            _from_ids(index.ids_n(k.ranks[i]), kid);
            real_t ratio(search.ratios[i]);
            if (ratio > base_ratio) break;
            //base_ratio = 0.75; // subsequent ratios
            base_ratio = 0.95;
        }
    }

    bool _from_ranks(const key_id_t &sk, const const_range_t &rn, size_t rank = 0)
    {
        size_t sz          = sk.key.ranks.size(),
               small_batch = get_small_batch_size(rank),
               limit       = std::min(sz, get_rank_limit(rank)),
               max_rank    = std::min(sz, get_max_rank_limit(rank));
        if (rank >= max_rank || (rn.second - rn.first) < small_batch) {
            _update_from_range(rn);
            return true;
        }
        typename key_id_t::rank_comparer c(rank);
        bool any = false;
        auto e = rn.second;
        for (size_t p = rank, psz = std::min(rank + limit, sz); p != psz; ++p) {
            key_id_t k(sk);
            k.key.swap_rank(rank, p);
            auto r(std::equal_range(rn.first, e, k, c));
            if (r.first == e) continue;
            any = false;
            if (rank >= max_rank || r.second - r.first < small_batch) {
                _update_from_range(r);
            } else {
                any = _from_ranks(k, r, rank + 1);
                if (!any) {
                    _update_from_range(r);
                    any = true;
                }
            }
        }
        return any;
    }
    
    inline size_t get_small_batch_size(size_t rank) const { return 8; } // 64 15 1 gives 99+% on 128 for all
    inline size_t get_rank_limit(size_t rank)       const { return 5; }
    inline size_t get_max_rank_limit(size_t rank)   const { return 5; }

private:
    
    real_t _partial_distance(const search_vector_t &search, const key_t &key, real_t limit)
    {
        real_t dsq = 0;
        const auto &sk = search.key_id.key;
        real_t limit_sq = limit * limit, minva = std::numeric_limits<real_t>::max();
        for (size_t i = 0; i < KeyRanksN; ++i) {
            real_t val(key.values[i]), va(abs(val));
            dsq += _sq(val - search.vector[abs(key.ranks[i])]);
            if (dsq > limit_sq) return sqrt(dsq);
            if (va < minva) minva = va;
        }
        // TODO we can also incorporate search.key.ranks which weren't in key.ranks
        // match them to the closest value to search's whose absolute value would also be <= the minimum from key.values
        //
        // there *may* an error here with correctly handling negative ranks below:
        auto maxva = minva;
        for (size_t i = 0; i < KeyRanksN; ++i) {
            real_t u = search.abs_values[i];
            if (find_abs(sk.ranks[i], key.ranks) == -1) { // TODO here use abs_order?? -- is there a faster way here?
                dsq += _sq(u - std::min(u, maxva));
                if (dsq > limit_sq) return sqrt(dsq);
            }
        }
        return sqrt(dsq);
    }
    
    bool _partial_distance_less(const search_vector_t &search, const key_t &key, real_t limit)
    {
        real_t dsq = 0;
        const auto &sk = search.key_id.key;
        real_t limit_sq = limit * limit, minva = std::numeric_limits<real_t>::max();
        for (size_t i = 0; i < KeyRanksN; ++i) {
            real_t val(key.values[i]), va(abs(val));
            dsq += _sq(val - search.vector[abs(key.ranks[i])]);
            if (dsq > limit_sq) return false;
            if (va < minva) minva = va;
        }
        // TODO we can also incorporate search.key.ranks which weren't in key.ranks
        // match them to the closest value to search's whose absolute value would also be <= the minimum from key.values
        //
        // there *may* an error here with correctly handling negative ranks below:
        auto maxva = minva;
        for (size_t i = 0; i < KeyRanksN; ++i) {
            real_t u = search.abs_values[i];
            if (find_abs(sk.ranks[i], key.ranks) == -1) { // TODO here use abs_order?? -- is there a faster way here?
                dsq += _sq(u - std::min(u, maxva));
                if (dsq > limit_sq) return false;
            }
        }
        return dsq < limit_sq;
    }
    
    bool _possibly_less_than_min(const key_t &key, id_t id)
    {
        if (!OPTIMIZE) return true;
        const auto &k0 = search.key_id.key, &k1 = key;
        // compare ratios here?  use it to determine how many ids_n to test?
        if (abs(k0.norm - k1.norm) > minimum.first || (BLOOM && examined.isset(id)))
            return false;
        //return _partial_distance(search, k1, minimum.first) <= minimum.first;
        return _partial_distance_less(search, k1, minimum.first);
    }

    inline void _update_minimum(const key_id_t &kid) { _update_minimum(kid.key, kid.id); }
    
    void _update_minimum(const key_t &key, id_t id)
    {
        compare_count++;
        if (!_possibly_less_than_min(key, id))
            return;
        if (OPTIMIZE && BLOOM) examined.set(id);
        distance_count++;
        real_t min_sq = _sq(minimum.first);
        real_t dist_sq = ::distance_sq_limit(store.at(id), search.vector, min_sq);
        if (dist_sq <= min_sq) {
            minimum = value_index_t(sqrt(dist_sq), id);
            //queue.add(dist, id);
        }
    }
};
