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
#include "range_set.h"
#include "pairs.h"

template <typename IndexT, size_t KeyRanksN>
struct ordered_ranks
{
    typedef std::pair<IndexT, size_t>           rank_index_t;
    typedef std::array<IndexT, KeyRanksN>       ranks_t;
    typedef std::array<rank_index_t, KeyRanksN> ranks_sorted_t;
    typedef std::array<size_t, KeyRanksN>       ranks_reverse_t;
    
    ranks_sorted_t  _sorted;
    ranks_reverse_t _reverse;
    
    inline ordered_ranks(const ranks_t &ranks)
    {
        for (size_t i = 0; i < KeyRanksN; i++)
            _sorted[i] = rank_index_t(ranks[i], i);
        pair_compare_first_less<rank_index_t> c;
        std::sort(_sorted.begin(), _sorted.end(), c);
        for (size_t i = 0; i < KeyRanksN; i++)
            _reverse[_sorted[i].second] = i;
    }
};

template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN>
struct rank_range_matches
{
    typedef key_index<FloatT, N, IndexT, KeyRanksN>  key_index_t;
    typedef key<FloatT, N, IndexT, KeyRanksN>        key_t;
    typedef key_id<FloatT, N, IndexT, KeyRanksN>     key_id_t;
    typedef typename key_t::key_ranks_t              key_ranks_t;
    typedef typename key_index_t::const_range_t      const_range_t;
    typedef typename key_index_t::const_iterator     const_iterator;
    typedef std::array<const_range_t, KeyRanksN>     matches_t;
    typedef std::array<const_range_t, KeyRanksN + 1> unmatches_t;
    typedef ordered_ranks<IndexT, KeyRanksN>         ordered_ranks_t;
    
    matches_t              _matches;
    unmatches_t            _unmatches;
    
    rank_range_matches(const key_id_t &sk, const const_range_t &rn, size_t rank, const ordered_ranks_t &ordered)
    {
        typename key_id_t::rank_comparer c(rank);
        auto i = rn.first, e = rn.second + 1;
        const_range_t er(e, e);
        _matches = { er };
        _unmatches = { er };
        size_t umc = 0;
        for (size_t p = 0; p != KeyRanksN && i != e; ++p) {
            key_id_t k(sk);
            typename ordered_ranks_t::rank_index_t ri(ordered._sorted[p]);
            k.key.ranks[rank] = ri.first;
            auto r(std::equal_range(i, e, k, c));
            if (r.first == e) continue;
            _matches[ri.second] = r;
            _unmatches[umc++] = const_range_t(i, r.first);
            i = r.second + 1;
        }
        if (i != e) _unmatches[umc++] = const_range_t(i, e);
    }
};

template <typename FloatT, size_t N, typename IndexT, size_t KeyRanksN, size_t LongKeyRanksN = 16>
struct range_state
{
    static constexpr size_t MAX_RANK_SEARCH = KeyRanksN;
    static constexpr size_t N_CLOSEST       = 8;
    static constexpr size_t BLOOM_BITS      = 128 * 1024 * 8;

    typedef FloatT                                                      real_t;
    typedef range_state<FloatT, N, IndexT, KeyRanksN>                   self_t;
    typedef key_index<FloatT, N, IndexT, KeyRanksN>                     key_index_t;
    typedef key<FloatT, N, IndexT, KeyRanksN>                           key_t;
    typedef key<FloatT, N, IndexT, LongKeyRanksN>                       long_key_t;
    typedef key_id<FloatT, N, IndexT, KeyRanksN>                        key_id_t;
    typedef typename key_t::key_ranks_t                                 key_ranks_t;
    typedef typename key_index_t::const_range_t                         const_range_t;
    typedef typename key_index_t::const_iterator                        const_iterator;
    typedef std::array<FloatT, N>                                       flat_vector_t;
    typedef container<N, FloatT>                                        container_t;
    typedef std::pair<real_t, size_t>                                   value_index_t;
    typedef k_indices_queue<real_t, std::greater<real_t> >              queue_t;
    typedef std::vector<uint64_t>                                       ids_t;
    typedef search_vector<FloatT, N, IndexT, KeyRanksN, LongKeyRanksN>  search_vector_t;
    typedef ordered_ranks<IndexT, KeyRanksN>                            ordered_ranks_t;
    typedef rank_range_matches<FloatT, N, IndexT, KeyRanksN>            rank_range_matches_t;

    const container_t     &store;
    const key_index_t     &index;
    const search_vector_t  search;
    queue_t                queue;
    const_range_t          range;
    value_index_t          minimum;
    size_t                 distance_count;
    int                    total_top_level;
    bloom<BLOOM_BITS>      examined;

    inline range_state(const flat_vector_t &search, const container_t &store, const key_index_t &index, const const_range_t &range)
        : search(search), store(store), index(index), range(range), minimum(std::numeric_limits<real_t>::max(), 0), distance_count(0), total_top_level(0), queue(N_CLOSEST) {}

    inline range_state(const flat_vector_t &vector, const container_t &store, const key_index_t &index)
        : search(vector), store(store), index(index), minimum(std::numeric_limits<real_t>::max(), 0), distance_count(0), total_top_level(0), queue(N_CLOSEST) {}

    void set_base_id_range()
    {
        // set base id range
        typename key_id_t::base_id_comparer c;
        range = const_range_t(index.begin(), index.end());
        range = std::equal_range(range.first, range.second, search.key_id, c);
    }

    void update()
    {
        set_base_id_range();
        _from_exp_m_norm(false);
    }

    void _from_exp_m_norm(bool whole_range = false)
    {/*
        std::array<int, 3> offsets({ 0, -1, 1 });
        typename key_id_t::exp_m_norm_comparer c;
        auto r(range);
        for (auto i : offsets) {
            auto kid(search.key_id);
            kid.key.exp_norm = int8_t(std::max(std::min(i + kid.key.exp_norm, 127), -128));
            r = std::equal_range(range.first, range.second, kid, c);
            if (r.first < r.second)
                _from_norm_ranks(kid, r, whole_range);
        }*/
        _from_norms(search.key_id);
    }

    void _from_exp_m_norm_2()
    {
        std::array<int, 3> offsets({ 0, -1, 1 });
        typename key_id_t::exp_m_norm_comparer c;
        auto r(range);
        for (auto i : offsets) {
            auto kid(search.key_id);
            kid.key.exp_norm = int8_t(std::max(std::min(i + kid.key.exp_norm, 127), -128));
            kid.key.m_norm = 0;
            r = std::equal_range(range.first, range.second, kid, c);
            if (r.first < r.second)
                _update_from_range(r);
        }
    }
    
    const_range_t _update_from_range(const const_range_t &r)
    {
        for (auto j = r.first; j != r.second + 1; ++j)
            _update_minimum(*j);
        return r;
    }

    void _from_norm_ranks(const key_id_t &sk, const const_range_t &rn, bool whole_range = false)
    {
        auto i = rn.first, e = rn.second;
        key_id_t k(sk);
        typename key_id_t::exp_m_norm_comparer c;
        while (i < e) {
            const_range_t r = std::equal_range(i, e, k, c);
            if (r.first == e) break;
            if (r.first < r.second) {
                _from_ranks_1(sk, r, whole_range);
                i = r.second;
            }
        }
    }

    void _from_norms_(const key_id_t &sk, real_t distance)
    {
        real_t norm = sk.key.norm;
        pair_compare_first_less<typename key_index_t::norm_t> cn;
        typename key_index_t::norm_t r0(norm - distance, 0), r1(norm + distance, 0);
        auto i = index._norms.begin(), e = index._norms.end();
        i = std::lower_bound(i, e, r0, cn);
        e = std::upper_bound(i, e, r1, cn);
        for (; i != e; ++i) {
            const key_t &ik = index._keys[(*i).second];
            if (count_intersect(ik.ranks, sk.key.ranks) > 0) {
                key_id_t k(ik, (*i).second);
                _update_minimum(k);
            }
        }
    }

    void _update_from_norms_iter(const key_id_t &sk, typename key_index_t::norms_t::const_iterator i)
    {
        id_t id = (*i).second;
        const key_t &key = index._keys[id];
        _update_minimum(key_id_t(key, id));
    }

    void _from_norms(const key_id_t &sk)
    {
        typedef typename key_index_t::norm_t norm_t;
        real_t norm = sk.key.norm;
        pair_compare_first_less<typename key_index_t::norm_t> cn;
        auto i = index._norms.begin(), e = index._norms.end();
        auto r = std::equal_range(i, e, norm_t(norm, 0), cn);
        auto j = r.first, k = r.second;
        bool top = false, bottom = false;
        while ((top    = (j > i && (*j).first >= norm - minimum.first)) ||
               (bottom = (k < e && (*k).first <= norm + minimum.first))) {
            if (top)    _update_from_norms_iter(sk, j--);
            if (bottom) _update_from_norms_iter(sk, k++);
        }
    }

    bool _from_ranks_1(const key_id_t &sk, const const_range_t &rn, bool whole_range = false, size_t rank = 0)
    {
        enum { SMALL_BATCH = 8 };
        size_t sz = sk.key.ranks.size();
        if (rank >= sz) {
            _update_from_range(rn);
            return true;
        }
        typename key_id_t::rank_comparer c(rank);
        bool any = false;
        auto e = rn.second + 1;
        for (size_t p = rank; p != sz; ++p) {
            key_id_t k(sk);
            k.key.swap_rank(rank, p);
            auto r(std::equal_range(rn.first, e, k, c));
            if (r.first == e) continue;
            any = false;
            if (r.second - r.first < SMALL_BATCH) {
                _update_from_range(r);
            } else {
                any = _from_ranks_1(k, r, whole_range, rank + 1);
                if (!any && whole_range) {
                    _update_from_range(r); // this is redundant, but it doesn't work well without it.  There are some improvements to be made here
                    any = true;
                }
            }
        }
        return any;
    }
    /*
    void _ranks_sorted(const key_ranks_t &ranks, ranks_sorted_t &sorted, key_ranks_t &lookup)
    {
        for (size_t i = 0; i < KeyRanksN; i++) sorted[i] = rank_index_t(ranks[i], i);
        pair_compare_first_less<rank_index_t> c;
        std::sort(sorted.begin(), sorted.end(), c);
        for (size_t i = 0; i < KeyRanksN; i++) lookup[sorted[i].second] = i;
    }
    
    bool _from_ranks_1a(const key_id_t &sk, const const_range_t &rn, size_t rank = 0)
    {
        ranks_sorted_t sorted;
        key_ranks_t lookup;
        _ranks_sorted(sk.key.ranks, sorted, lookup);
        return __from_ranks_1a(sk, rn, rank, sorted, lookup);
    }*/
    
    bool _from_ranks_1a(const key_id_t &sk, const const_range_t &rn, size_t rank = 0)
    {
        ordered_ranks_t ordered(sk.key.ranks);
        return __from_ranks_1a(sk, rn, rank, ordered);
    }
    /*
    // fill *match* with matching ranges in order
    size_t _find_match_ranges(const key_id_t &sk,
                             const const_range_t &rn,
                             size_t rank,
                             const ranks_sorted_t &sorted,
                             const key_ranks_t &lookup,
                             matches_t &matched,
                             unmatches_t &unmatched)
    {
        size_t sz = KeyRanksN;
        typename key_id_t::rank_comparer c(rank);
        auto i = rn.first, e = rn.second + 1;
        const_range_t er(e, e);
        matched = matches_t({ er });
        unmatched = unmatches_t({ er });
        size_t umc = 0;
        for (size_t p = 0; p != sz && i != e; ++p) {
            key_id_t k(sk);
            k.key.ranks[rank] = sorted[p].first;
            auto r = matched[lookup[p]] = (std::equal_range(i, e, k, c));
            if (r.first == e) continue;
            unmatched[umc++] = const_range_t(i, r.first);
            i = r.second + 1;
        }
        if (i != e)
            unmatched[umc++] = const_range_t(i, e);
        return umc;
    }
*/
    bool __from_ranks_1a(const key_id_t &sk,
                         const const_range_t &rn,
                         size_t rank,
                         const ordered_ranks_t &ordered)
    {
        enum { SMALL_BATCH = 8 };
        size_t sz = KeyRanksN;
        if (rank >= sz) {
            _update_from_range(rn);
            return true;
        }
        typename key_id_t::rank_comparer c(rank);
        //rank_range_matches_t matches(sk, rn, rank, ordered);

        bool any = false;
        auto i = rn.first, e = rn.second + 1;
        for (size_t p = 0; p != sz; ++p) {
            key_id_t k(sk);
            k.key.swap_rank(rank, p);
            //auto r(matches._matches[p]);
            auto r(std::equal_range(rn.first, e, k, c));
            if (r.first == e) continue;
            any = false;
            if (r.second - r.first < SMALL_BATCH) {
                _update_from_range(r);
            } else {
                any = __from_ranks_1a(k, r, rank + 1, ordered);
                if (!any) {
                    _update_from_range(r); // this seems redundant, but it doesn't work well without it
                    any = true;
                }
            }
        }
        /*
        // TODO now here do unmatched.. (they must be segmented by rank)
        for (size_t p = 0; p != sz; ++p) {
            key_id_t k(sk);
            k.key.swap_rank(rank, p);
            auto r(matches._matches[p]);
            if (r.first == e) continue;
            any = false;
            if (r.second - r.first < SMALL_BATCH) {
                _update_from_range(r);
            } else {
                any = __from_ranks_1a(k, r, rank + 1, ordered);
                if (!any) {
                    _update_from_range(r); // this seems redundant, but it doesn't work well without it
                    any = true;
                }
            }
        }
         */

        return any;
    }
    
    void _fill_ranges(const key_id_t &sk, const const_range_t &rn, size_t rank, std::vector<const_range_t> &included /* max KeyRanksN */, std::vector<const_range_t> &unincluded /* max KeyRanksN + 1 */)
    {
        auto ranks = sk.key.ranks;
        size_t sz = ranks.size();
        if (rank >= sz) return;
        std::sort(ranks.begin(), ranks.end());
        typename key_id_t::rank_comparer c(rank);
        const_range_t q(rn.first, rn.second);
        key_id_t k(sk);
        for (auto rr : ranks) {
            k.key.ranks[rank] = rr;
            auto e = rn.second + 1;
            auto r(std::equal_range(q.first, e, k, c));
            if (r.first == e) continue;
            included.push_back(r);
            q.second = r.first;
            unincluded.push_back(q);
            q.first = r.second + 1;
            q.second = e;
            if (q.first >= e) break;
        }
        if (q.first < q.second)
            unincluded.push_back(q);
    }

    bool _from_ranks_2(const key_id_t &sk, const const_range_t &rn, size_t rank = 0, size_t index = 0)
    {
        auto ranks = sk.key.ranks;
        size_t sz = ranks.size();
        if (rank >= sz) {
            if (index > 0) _update_from_range(rn);
            return true;
        }
        const_range_t rb(range_intersect(rn, range));
        typename key_id_t::rank_comparer c(rank);
        bool any = false;
        auto i = rb.first, e = rb.second;
        while (i < e) {
            int16_t ri = (*i).key.ranks[rank];
            key_id_t k(sk);
            //k.key.ranks[rank] = ri;
            int pos = find(ri, ranks);
            if (pos >= 0 && pos != rank) {
                k.key.swap_rank(pos, rank);
            } else {
                k.key.ranks[rank] = ri;
            }
            size_t new_index = index + (pos >= 0);
            const_range_t r(i, e);
            r.second = std::upper_bound(i, e, k, c);
            if (r.second == e) {
                //if (new_index)
                //    _update_from_range(r);
                key_id_t k(sk);
                if (pos >= 0) {
               //     std::cout << "here" << std::endl;
                //    k.key.swap_rank(pos, new_index);
                } else {
                    any |= _from_ranks_2(k, r, rank + 1, new_index);}
                break;
            }
            if (r.first == r.second) {
                _update_minimum(*r.first);
            } else {
                key_id_t k(sk);
                if (pos >= 0) {
               //     std::cout << "here" << std::endl;
                   // k.key.swap_rank(pos, new_index);
                } else {
                    any |= _from_ranks_2(k, r, rank + 1, new_index);}
                /*
                if (!any) {
                    _update_from_range(r);
                    any = true;
                }*/
            }
            r.second++;
            //...
            i = r.second;
        }
        return any;
    }

private:

    inline real_t _partial_distance(const search_vector_t &search, const key_t &key)
    {
        real_t dsq = 0;
        const auto &sk = search.key_id.key;
        for (size_t i = 0; i < KeyRanksN; ++i)
            dsq += _sq(key.values[i] - search.vector[abs(key.ranks[i])]);
        // TODO we can also incorporate search.key.ranks which weren't in key.ranks
        // match them to the closest value to search's whose absolute value would also be <= the minimum from key.values
        //
        // I think there is an error here with correctly handling negative ranks below:
        auto maxv = abs(key.values_min_abs());
        for (size_t i = 0; i < KeyRanksN; ++i) {
            real_t u = abs(sk.values[i]);
            if (find_abs(sk.ranks[i], key.ranks) == -1)
                dsq += _sq(u - std::min(u, maxv));
        }
        return sqrt(dsq);
    }
    
    inline bool _possibly_less_than_minimum(const key_id_t &kid)
    {
        auto &k0 = search.key_id.key, &k1 = kid.key;
        if (abs(k0.norm - k1.norm) > minimum.first || !has_intersect(k0.ranks, k1.ranks) || examined.isset(kid.id))
            return false;
        return _partial_distance(search, k1) <= minimum.first;
    }

    void _update_minimum(const key_id_t &kid)
    {
        if (!_possibly_less_than_minimum(kid))
            return;
        examined.set(kid.id);
        distance_count++;
        real_t dist = sqrt(::distance_sq_limit(store.at(kid.id), search.vector, _sq(minimum.first)));
        if (dist <= minimum.first) {
            minimum = value_index_t(dist, kid.id);
            queue.add(dist, kid.id);
        }
    }
};
