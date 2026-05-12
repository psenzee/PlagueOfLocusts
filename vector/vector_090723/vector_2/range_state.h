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

template <typename FloatT, size_t N, size_t KeyRanksN>
struct range_state
{
    typedef range_state<FloatT, N, KeyRanksN>                           self_t;
    typedef FloatT                                                      real_t;
    typedef key_index<FloatT, N, KeyRanksN>                             key_index_t;
    typedef key<FloatT, N, KeyRanksN>                                   key_t;
    typedef key_id<FloatT, N, KeyRanksN>                                key_id_t;
    typedef typename key_id_t::id_t                                     id_t;
    typedef typename key_t::key_ranks_t                                 key_ranks_t;
    typedef typename key_index_t::const_range_t                         const_range_t;
    typedef typename key_index_t::const_iterator                        const_iterator;
    typedef std::array<FloatT, N>                                       flat_vector_t;
    typedef container<N, FloatT>                                        container_t;
    typedef std::pair<real_t, size_t>                                   value_index_t;
    typedef n_best<real_t, std::less<real_t> >                       queue_t;
    typedef typename key_index_t::norms_t                               norms_t;
    typedef std::vector<id_t>                                           ids_t;
    typedef search_vector<FloatT, N, KeyRanksN>                         search_vector_t;
    
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

    void update()                                           { _from_ids_n(search.key_id); }

    inline const value_index_t &kth_minimum()         const { return queue.kth_best(); }
    
    inline const value_index_t &minimum()             const { return queue.best(); }
    
    inline void set_minimum(const value_index_t &min)       { queue.add(min); }

private:
    
    inline const key_t &_key(id_t id)                                 const { return index._keys[id]; }
    inline const key_t &_key(const typename ids_t::const_iterator &i) const { return index._keys[*i]; }

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
        real_t min = kth_minimum().first;
        while (((top    = (j >= i && _key(j).norm >= std::max(0.f, norm - min))) ||
                (bottom = (k <  e && _key(k).norm <= norm + min)))) {
            if (top)    { _update_minimum(_key(j), *j); --j; }
            if (bottom) { _update_minimum(_key(k), *k); ++k; }
            min = kth_minimum().first;
        }
    }

    void _from_ids_n(const key_id_t &sk)
    {
        const auto &kid = search.key_id;
        const auto &k = kid.key;
        real_t base_ratio = 0.75; // first ratio
        for (size_t i = 0; i < KeyRanksN; i++) {
            _from_ids(index.ids_n(k.ranks[i]), kid);
            real_t ratio(search.ratios[i]);
            if (ratio > base_ratio) break;
            base_ratio = 0.35; // subsequent ratios
        }
    }
    
    real_t _partial_distance(const search_vector_t &search, const key_t &key, real_t limit)
    {
        real_t dsq = 0;
        const auto &sk = search.key_id.key;
        real_t limit_sq = limit * limit, minva(REAL_MAX);
        for (size_t i = 0; i < KeyRanksN; ++i) {
            real_t val(key.values[i]), va(abs(val));
            dsq += _sq(val - search.vector[abs(key.ranks[i])]);
            if (dsq >= limit_sq) return sqrt(dsq);
            if (va < minva) minva = va;
        }
        // TODO we can also incorporate search.key.ranks which weren't in key.ranks
        // match them to the closest value to search's whose absolute value would also be <= the minimum from key.values
        //
        // there *may* an error here with correctly handling negative ranks below:
        for (size_t i = 0; i < KeyRanksN; ++i) {
            real_t u = search.abs_values[i];
            if (find_abs(sk.ranks[i], key.ranks) == -1) { // TODO here use abs_order?? -- is there a faster way here?
                dsq += _sq(u - std::min(u, minva));
                if (dsq >= limit_sq) return sqrt(dsq);
            }
        }
        return sqrt(dsq);
    }
    
    bool _partial_distance_less(const search_vector_t &search, const key_t &key, real_t limit)
    {
        real_t dsq = 0;
        const auto &sk = search.key_id.key;
        real_t limit_sq = limit * limit, minva(REAL_MAX);
        for (size_t i = 0; i < KeyRanksN; ++i) {
            real_t val(key.values[i]), va(abs(val));
            dsq += _sq(val - search.vector[abs(key.ranks[i])]);
            if (dsq >= limit_sq) return false;
            if (va < minva) minva = va;
        }
        // we also incorporate search.key.ranks which weren't in key.ranks
        // match them to the closest value to search's whose absolute value
        // would also be <= the minimum from key.values
        for (size_t i = 0; i < KeyRanksN; ++i) {
            if (find_abs(sk.ranks[i], key.ranks) == -1) { // TODO here use abs_order?? -- is there a faster way here?
                real_t u = search.abs_values[i];
                dsq += _sq(u - std::min(u, minva));
                if (dsq >= limit_sq) return false;
            }
        }
        return dsq < limit_sq;
    }
    
    inline real_t _partial_distance_sq_initial(const search_vector_t &search, const key_t &key, real_t limit_sq)
    {
        real_t dsq = 0;
        for (size_t i = 0; i < KeyRanksN && dsq < limit_sq; ++i) {
            auto r = key.ranks[i];
            //(1 - ((r < 0) << 1)) * r
            dsq += _sq(key.values[i] - search.vector[r < 0 ? -r : r]);
        }
        return dsq;
    }
    
    bool _partial_distance_less_1(const search_vector_t &search, const key_t &key, real_t limit)
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
            real_t va(key.values[i]);
            va = va < 0 ? -va : va;
            //real_t va(abs(key.values[i]));
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
    
    bool _partial_distance_less_2(const search_vector_t &search, const key_t &key, real_t limit)
    {
        real_t dsq = 0;
        const auto &sk = search.key_id.key;
        real_t limit_sq = limit * limit, minva(REAL_MAX);
        std::array<uint64_t, (N / 64) + 1> bits({ 0 });
        for (size_t i = 0; i < KeyRanksN; ++i) {
            real_t val(key.values[i]), va(abs(val));
            size_t abs_rank = abs(key.ranks[i]);
            dsq += _sq(val - search.vector[abs_rank]);
            if (dsq >= limit_sq) return false;
            if (va < minva) minva = va;
            bit_set(bits, abs_rank);
        }
        // we also incorporate search.key.ranks which weren't in key.ranks
        // match them to the closest value to search's whose absolute value
        // would also be <= the minimum from key.values
        for (size_t i = 0; i < KeyRanksN; ++i) {
            if (!bit_test(bits, abs(sk.ranks[i]))) {
                real_t u = search.abs_values[i];
                dsq += _sq(u - std::min(u, minva));
                if (dsq >= limit_sq) return false;
            }
        }
        return dsq < limit_sq;
    }
    
    bool _partial_distance_less_3(const search_vector_t &search, const key_t &key, real_t limit)
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
        std::array<uint64_t, (N / 64) + 1> bits({ 0 });
        for (size_t i = 0; i < KeyRanksN; ++i) {
            real_t va(abs(key.values[i]));
            if (va < minva) minva = va;
            bit_set(bits, abs(key.ranks[i]));
        }
        for (size_t i = 0; i < KeyRanksN; ++i) {
            if (!bit_test(bits, abs(sk.ranks[i]))) {
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
        // compare ratios here?  use it to determine how many ids_n to test?
        if (abs(sk.norm - key.norm) > limit || (BLOOM && examined.isset(id)))
            return false;
        return _partial_distance_less_1(search, key, limit);
    }

    inline void _update_minimum(const key_id_t &kid) { _update_minimum(kid.key, kid.id); }
    
    void _update_minimum(const key_t &key, id_t id)
    {
        compare_count++;
        real_t limit = kth_minimum().first;
        if (!_possibly_less_than_limit(key, id, limit))
            return;
        if (OPTIMIZE && BLOOM) examined.set(id);
        distance_count++;
        real_t limit_sq = _sq(limit);
        real_t dist_sq = _retrieve_and_calculate_distance_sq_limit(key, id, limit_sq);
        if (dist_sq <= limit_sq) set_minimum(value_index_t(sqrt(dist_sq), id));
    }

    real_t _retrieve_and_calculate_distance_sq_limit(const key_t &key, id_t id, real_t limit_sq)
    {
        return arrays::distance_sq_limit(store.at(id), search.vector, limit_sq);
    }
};

//--------


template <typename ContainerT, size_t KeyRanksN>
struct range_state_
{
    typedef ContainerT                                                  container_t;
    
    static constexpr size_t COUNT = container_t::COUNT;
    
    typedef range_state_<ContainerT, KeyRanksN>                         self_t;
    typedef typename container_t::value_type                            real_t;
    typedef key_index_<container_t, KeyRanksN>                          key_index_t;
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

    inline range_state_(const flat_vector_t &vector, const container_t &store, const key_index_t &index)
        : search(vector), store(store), index(index), distance_count(0), compare_count(0), queue(N_CLOSEST) {}

    //void update()                                           { _from_ids_n(search.key_id); }
    void update()                                           { _from_dnids(search); }

    inline const value_index_t &kth_minimum()         const { return queue.kth_best(); }
    
    inline const value_index_t &minimum()             const { return queue.best(); }
    
    inline void set_minimum(const value_index_t &min)       { queue.add(min); }

private:
    
    inline const key_t &_key(id_t id)                                 const { return index._keys[id]; }
    inline const key_t &_key(const typename ids_t::const_iterator &i) const { return index._keys[*i]; }
    inline const key_t &_key(const typename dnids_t::const_iter_t &i) const { return index._keys[(*i).id]; }

    void _from_ids(const ids_t &ids, const key_id_t &sk)
    {
        if (ids.size() == 0) return;
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
        real_t min = kth_minimum().first;
        while (((top    = (j >= i && _key(j).norm >= std::max(0.f, norm - min))) ||
                (bottom = (k <  e && _key(k).norm <= norm + min)))) {
            if (top)    { _update_minimum(_key(j), *j); --j; }
            if (bottom) { _update_minimum(_key(k), *k); ++k; }
            min = kth_minimum().first;
        }
    }

    void _from_dnids(const dnids_const_range_t &range, const search_vector_t &search)
    {
        real_t norm = search.norm();
        auto i = range.first, e = range.second, j = index._dnids.search_norm(i, e, norm), k = j;
        if (j == e)
            return;
        bool top = false, bottom = false;
        real_t min = kth_minimum().first;
        while (((top    = (j >= i && _key(j).norm >= std::max(0.f, norm - min))) ||
                (bottom = (k <  e && _key(k).norm <= norm + min)))) {
            if (top)    { _update_minimum(_key(j), (*j).id); --j; }
            if (bottom) { _update_minimum(_key(k), (*k).id); ++k; }
            min = kth_minimum().first;
        }
    }

    void _from_dnids(const search_vector_t &search)
    {
        const auto &kid = search.key_id;
        const auto &k = kid.key;
        real_t base_ratio = 0.95; // first ratio
        for (size_t i = 0; i < KeyRanksN; i++) {
            auto r = index._dnids.by_dimension(k.ranks[i]);
            if (r.first == r.second) continue;
            _from_dnids(r, search);
            real_t ratio(search.ratios[i]);
            if (ratio > base_ratio) break;
            base_ratio = 0.75; // subsequent ratios
        }
    }

    void _from_ids_n(const key_id_t &sk)
    {
        const auto &kid = search.key_id;
        const auto &k = kid.key;
        real_t base_ratio = 0.95; // first ratio
        for (size_t i = 0; i < KeyRanksN; i++) {
            _from_ids(index.ids_n(k.ranks[i]), kid);
            real_t ratio(search.ratios[i]);
            if (ratio > base_ratio) break;
            base_ratio = 0.95; // subsequent ratios
        }
    }
    
    real_t _partial_distance(const search_vector_t &search, const key_t &key, real_t limit)
    {
        real_t dsq = 0;
        const auto &sk = search.key_id.key;
        real_t limit_sq = limit * limit, minva(REAL_MAX);
        for (size_t i = 0; i < KeyRanksN; ++i) {
            real_t val(key.values[i]), va(abs(val));
            dsq += _sq(val - search.vector[abs(key.ranks[i])]);
            if (dsq >= limit_sq) return sqrt(dsq);
            if (va < minva) minva = va;
        }
        // TODO we can also incorporate search.key.ranks which weren't in key.ranks
        // match them to the closest value to search's whose absolute value would also be <= the minimum from key.values
        //
        // there *may* an error here with correctly handling negative ranks below:
        for (size_t i = 0; i < KeyRanksN; ++i) {
            real_t u = search.abs_values[i];
            if (find_abs(sk.ranks[i], key.ranks) == -1) { // TODO here use abs_order?? -- is there a faster way here?
                dsq += _sq(u - std::min(u, minva));
                if (dsq >= limit_sq) return sqrt(dsq);
            }
        }
        return sqrt(dsq);
    }
    
    bool _partial_distance_less(const search_vector_t &search, const key_t &key, real_t limit)
    {
        real_t dsq = 0;
        const auto &sk = search.key_id.key;
        real_t limit_sq = limit * limit, minva(REAL_MAX);
        for (size_t i = 0; i < KeyRanksN; ++i) {
            real_t val(key.values[i]), va(abs(val));
            dsq += _sq(val - search.vector[abs(key.ranks[i])]);
            if (dsq >= limit_sq) return false;
            if (va < minva) minva = va;
        }
        // we also incorporate search.key.ranks which weren't in key.ranks
        // match them to the closest value to search's whose absolute value
        // would also be <= the minimum from key.values
        for (size_t i = 0; i < KeyRanksN; ++i) {
            if (find_abs(sk.ranks[i], key.ranks) == -1) { // TODO here use abs_order?? -- is there a faster way here?
                real_t u = search.abs_values[i];
                dsq += _sq(u - std::min(u, minva));
                if (dsq >= limit_sq) return false;
            }
        }
        return dsq < limit_sq;
    }

    inline real_t _partial_distance_sq_initial(const search_vector_t &search, const key_t &key, real_t limit_sq)
    {
        real_t dsq = 0;
        for (size_t i = 0; i < KeyRanksN && dsq < limit_sq; ++i)
            dsq += _sq(key.values[i] - search.vector[abs(key.ranks[i])]);
        return dsq;
    }

    bool _partial_distance_less_1(const search_vector_t &search, const key_t &key, real_t limit)
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

    bool _partial_distance_less_2(const search_vector_t &search, const key_t &key, real_t limit)
    {
        real_t dsq = 0;
        const auto &sk = search.key_id.key;
        real_t limit_sq = limit * limit, minva(REAL_MAX);
        std::array<uint64_t, (COUNT / 64) + 1> bits({ 0 });
        for (size_t i = 0; i < KeyRanksN; ++i) {
            real_t val(key.values[i]), va(abs(val));
            size_t abs_rank = abs(key.ranks[i]);
            dsq += _sq(val - search.vector[abs_rank]);
            if (dsq >= limit_sq) return false;
            if (va < minva) minva = va;
            bit_set(bits, abs_rank);
        }
        // we also incorporate search.key.ranks which weren't in key.ranks
        // match them to the closest value to search's whose absolute value
        // would also be <= the minimum from key.values
        for (size_t i = 0; i < KeyRanksN; ++i) {
            if (!bit_test(bits, abs(sk.ranks[i]))) {
                real_t u = search.abs_values[i];
                dsq += _sq(u - std::min(u, minva));
                if (dsq >= limit_sq) return false;
            }
        }
        return dsq < limit_sq;
    }

    bool _partial_distance_less_3(const search_vector_t &search, const key_t &key, real_t limit)
    {
        real_t limit_sq = limit * limit,
               dsq = _partial_distance_sq_initial(search, key, limit_sq);
        if (dsq >= limit_sq)
            return false;
        const auto &sk = search.key_id.key;
        // we also incorporate search.key.ranks which weren't in key.ranks
        // match them to the closest value to search's whose absolute value
        // would also be <= the minimum from key.values
        real_t minva(REAL_MAX);
        std::array<uint64_t, (COUNT / 64) + 1> bits({ 0 });
        for (size_t i = 0; i < KeyRanksN; ++i) {
            real_t va(abs(key.values[i]));
            if (va < minva) minva = va;
            bit_set(bits, abs(key.ranks[i]));
        }
        for (size_t i = 0; i < KeyRanksN; ++i) {
            if (!bit_test(bits, abs(sk.ranks[i]))) {
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
        // compare ratios here?  use it to determine how many ids_n to test?
        if (abs(sk.norm - key.norm) > limit || (BLOOM && examined.isset(id)))
            return false;
        return _partial_distance_less_1(search, key, limit);
    }

    inline void _update_minimum(const key_id_t &kid) { _update_minimum(kid.key, kid.id); }

    void _update_minimum(const key_t &key, id_t id)
    {
        compare_count++;
        real_t limit = kth_minimum().first;
        if (!_possibly_less_than_limit(key, id, limit))
            return;
        if (OPTIMIZE && BLOOM) examined.set(id);
        distance_count++;
        real_t limit_sq = _sq(limit);
        real_t dist_sq = _retrieve_and_calculate_distance_sq_limit(key, id, limit_sq);
        if (dist_sq <= limit_sq) set_minimum(value_index_t(sqrt(dist_sq), id));
    }

    real_t _retrieve_and_calculate_distance_sq_limit(const key_t &key, id_t id, real_t limit_sq)
    {
        return arrays::distance_sq_limit(store.at(id), search.vector, limit_sq);
    }
};

