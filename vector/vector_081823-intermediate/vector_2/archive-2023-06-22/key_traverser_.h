#include <iostream>
#include <array>
#include <algorithm>
#include "stl.h"
#include "bfloat16.h"
#include "container.h"
#include "simple_timer.h"
#include "key_.h"
#include "ranges.h"
#include "expand_bitset.h"

template <typename FloatT, size_t N, typename IndexT, size_t RanksN, size_t KeyRanksN>
struct key_index_
{
    typedef key_index_<FloatT, N, IndexT, RanksN, KeyRanksN> self_t;
    typedef key_<FloatT, N, IndexT, RanksN, KeyRanksN>       key_t;
    typedef key_id_<FloatT, N, IndexT, RanksN, KeyRanksN>    key_id_t;
    typedef std::vector<key_id_t>                            data_t;
    typedef typename data_t::const_iterator                  const_iterator;
    typedef std::pair<const_iterator, const_iterator>        const_range_t;
    typedef std::array<FloatT, N>                            vector_t;

    data_t _data;

    inline self_t &add(const key_t &key)  { _data.push_back(key_id_t(key, _data.size())); return *this; }
    inline self_t &add(const vector_t &v) { return add(key_t(v)); }
    
    inline auto    begin() const { return _data.begin(); }
    inline auto    end()   const { return _data.end(); }

    self_t        &index() { typename key_id_t::sort_comparer c; std::sort(_data.begin(), _data.end(), c); return *this; }
};

template <typename FloatT, size_t N, typename IndexT, size_t RanksN, size_t KeyRanksN>
struct key_traverser_
{
    typedef FloatT                                               real_t;
    typedef key_traverser_<FloatT, N, IndexT, RanksN, KeyRanksN> self_t;
    typedef key_index_<FloatT, N, IndexT, RanksN, KeyRanksN>     key_index_t;
    typedef key_<FloatT, N, IndexT, RanksN, KeyRanksN>           key_t;
    typedef key_<FloatT, N, IndexT, RanksN, RanksN>              long_key_t;
    typedef key_id_<FloatT, N, IndexT, RanksN, KeyRanksN>        key_id_t;
    typedef typename key_t::key_ranks_t                          dimensions_t;
    typedef typename key_index_t::const_range_t                  const_range_t;
    typedef typename key_index_t::const_iterator                 const_iterator;
    typedef std::array<FloatT, N>                                flat_vector_t;
    typedef container<N, FloatT>                                 container_t;
    typedef std::pair<real_t, size_t>                            value_index_t;
    typedef std::vector<uint64_t>                                ids_t;
    
    struct state_t { dimensions_t dimensions; size_t rank; const_range_t range; };
    
    typedef std::deque<state_t>                                 deque_t;

    const key_index_t &_index;
    deque_t            _deque;
    ids_t              _ids;
    bool               _breadth_first;
    size_t             _max_ids_per_state;
    value_index_t      _minimum;
    const_range_t      _outer_range;
    flat_vector_t      _search;
    key_id_t           _search_key;
    size_t             _full_distance_counts;
    
    static const int MAX_RANK_SEARCH = KeyRanksN;
    
    inline key_traverser_(const key_index_t &index) :
         _index(index), _breadth_first(false), _max_ids_per_state(3), _full_distance_counts(0) {}
    
    struct range_state
    {
        typedef std::array<uint64_t, (N >> 6)> key_bits_t;
        
        const flat_vector_t &search;
        const container_t   &store;
        const key_index_t   &index;
        key_id_t             skey;
        long_key_t           lkey;
        key_bits_t           skey_bits,
                             lkey_bits;
        const_range_t        range;
        value_index_t        minimum;
        size_t               distance_count;
        size_t               max_common_dimensions;
        int                  total_top_level;
        
        inline range_state(const flat_vector_t &search, const container_t &store, const key_index_t &index, const const_range_t &range, const key_t &key)
            : search(search), store(store), index(index), skey(key, 0), lkey(search), range(range), minimum(std::numeric_limits<real_t>::max(), 0), distance_count(0), total_top_level(0), max_common_dimensions(0), skey_bits({ 0 }), lkey_bits({ 0 })
        {
            encode(key.ranks, skey_bits);
            encode(key.ranks, lkey_bits);
        }
        
        inline range_state(const flat_vector_t &search, const container_t &store, const key_index_t &index, const key_t &key_)
            : search(search), store(store), index(index), skey(key_, 0), lkey(search), minimum(std::numeric_limits<real_t>::max(), 0), distance_count(0), total_top_level(0), max_common_dimensions(0), skey_bits({ 0 }), lkey_bits({ 0 })
        {
            encode(key_.ranks, skey_bits);
            encode(key_.ranks, lkey_bits);
            // set base range
            typename key_id_t::base_id_comparer c;
            range = const_range_t(index.begin(), index.end());
            range = std::equal_range(range.first, range.second, skey, c);
        }
        
        static std::ostream &print_range(std::ostream &os, const const_range_t &r, const char *label = nullptr)
        {
            return os << "range interval " << (label ? label : "") << " " << (r.second - r.first) << std::endl;
        }

        inline std::ostream &print_range(std::ostream &os, const char *label = nullptr) const
        {
            return print_range(os, range, label);
        }

        void update()
        {
            typename key_id_t::exp_norm_comparer c;
            auto i = std::lower_bound(range.first, range.second, skey, c);
            if (i != range.second) _update_range((*i).id);
            _from_norm_ranks(skey, range);
        }

        void _update_range(size_t id)
        {
            _update_minimum(id);
            _update_range_from_distance(minimum.first);
        }

        void _update_from_range(const const_range_t &r)
        {
            real_t initial = minimum.first;
            for (auto j = r.first; j != r.second; ++j) {
                _update_minimum((*j).id);
                std::cout << *j << " limit " << minimum.first << " count " << distance_count << std::endl;
            }
            if (minimum.first < initial)
                _update_range_from_distance(minimum.first);
        }
        
        void _get_best_ids_from_range_limit(const const_range_t &r, real_t search_norm, std::vector<id_t> &ids)
        {
            real_t limit = minimum.first;
            auto i = r.first;
            for (auto j = r.first; j != r.second; ++j) {
                real_t diff = abs((*j).key.norm - search_norm);
                if (diff < limit) { i = j; limit = diff; ids.push_back((*i).id); } // TODO here store off id, norm_diff and depth
            }
        }
        
        // by distance
        void _update_from_range_limit(const const_range_t &r, real_t search_norm)
        {
            real_t limit = minimum.first;
            auto i = r.first;
            for (auto j = r.first; j != r.second; ++j) {
                real_t diff = abs((*j).key.norm - search_norm);
                if (diff < limit) {
                    i = j;
                    limit = diff;
                    _update_minimum((*i).id);
                }
            }
            _update_minimum((*i).id);
            std::cout << *i << " limit " << minimum.first << " diff-limit " << limit << " count " << distance_count << std::endl;
        }
        
        // by common dimensions
        void _update_from_range_common_dimensions(const const_range_t &r, real_t search_norm, const key_t &search_key)
        {
            size_t max_common_dimensions = 0;
            auto i = r.first;
            std::array<uint64_t, (N >> 6)> key_data({ 0 });
            encode(search_key.ranks, key_data);
            for (auto j = r.first; j != r.second; ++j) {
                size_t intersections = count_intersection(key_data, (*j).key.ranks);
                if (intersections > max_common_dimensions) {
                    i = j;
                    max_common_dimensions = intersections;
                    _update_minimum((*i).id);
                }
            }
            _update_minimum((*i).id);
            std::cout << *i << " limit " << minimum.first << " intersections " << max_common_dimensions << " count " << distance_count << std::endl;
        }
        
        // by distance and common dimensions
        void _update_from_range_limit(const const_range_t &r, const key_t &search_key)
        {
            real_t limit = minimum.first, search_norm = search_key.norm;
            auto min_diff_i = r.first, max_common_dims_i = r.first;
            size_t updated = 0;
            for (auto j = r.first; j != r.second; ++j) {
                bool update = false;
                real_t diff = abs((*j).key.norm - search_norm);
                size_t intersections = count_intersection(lkey_bits, (*j).key.ranks);
                if (diff < limit) {
                    min_diff_i = j;
                    limit = diff;
                    update = true;
                }
                if (intersections > max_common_dimensions) {
                    max_common_dims_i = j;
                    max_common_dimensions = intersections;
                    update = true;
                }
                if (update) {
                    _update_minimum((*j).id);
                    updated++;
                }
            }
            if (updated) {
                if (min_diff_i == max_common_dims_i) {
                    std::cout << *min_diff_i << " minimum " << minimum.first << " diff-limit " << limit
                              << " intersections " << max_common_dimensions << " count " << distance_count << std::endl;
                } else {
                    std::cout << *min_diff_i << " minimum " << minimum.first << " diff-limit " << limit << std::endl
                              << *max_common_dims_i << " intersections " << max_common_dimensions << " count " << distance_count << std::endl;
                }
            }
        }

        void _from_norm_ranks(const key_id_t &sk, const const_range_t &rn)
        {
            auto i = std::max(rn.first, range.first), e = std::min(rn.second, range.second);
            key_id_t k(sk);
            typename key_id_t::exp_norm_comparer c;
            while (i < e) {
                k.key.norm = (*i).key.norm;
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
            size_t count = 0;
            bool result = false;
            while (i < e) {
                const_range_t r = std::equal_range(i, e, k, c);
                if (r.first == e) {
                    return false;
                }
                if (r.first >= r.second) {
                    //_update_from_range(const_range_t(r.first, r.second + 1));
                    return false;
                }
                std::cout << "equal range size " << (r.second - r.first) << std::endl;
                std::cout << "depth " << rank + 1 << std::endl;
                bool any = _from_n_ranks(sk, r, rank + 1);
                if (!any) {
                    //_update_from_range(r);
                    _update_from_range_limit(r, k.key);
                    return true;
                }
                i = r.second;
                std::cout << " so(index) " << sizeof(IndexT) << " ";
                std::cout << "top level count " << count++ << " total " << total_top_level++ << std::endl;
            }
            return result;
        }
/*
        void _from_rank_values(const key_id_t &sk, const const_range_t &rn, size_t rank = 0, size_t search_rank = 0)
        {
            if (rank >= MAX_RANK_SEARCH) {
                _update_from_range(rn);
                return;
            }
            auto i = std::max(rn.first, range.first), e = std::min(rn.second, range.second);
            key_id_t k(sk);
            float value = k.key.values[search_rank];
            typename key_id_t::exp_value_comparer c(rank);
            size_t count = 0, rr = 1;
            while (i < e) {
                //k.key.ranks[rank] = (*i).key.ranks[rank];
                const_range_t r = std::equal_range(i, e, k, c);
                if (r.first == e) break;
                std::cout << "equal range size " << (r.second - r.first) << std::endl;
                if (r.first >= r.second) {
                    std::cout << "=r.second rank " << rank << " rank dim before " << k.key.ranks[rank] << " after " << sk.key.ranks[rank + rr] << " rr " << rr << std::endl;
                    k.key.ranks[rank] = sk.key.ranks[rank + rr];
                    //if (rr++ >= 4) break;
                    //continue;
                    break;
                }
                if (dim == k.key.ranks[rank])
                    _from_ranks(sk, r, rank + 1, search_rank + 1);
                else if (rank > 0)
                    _update_from_range(r);
                if (i == r.second) {
                    std::cout << "ERROR! no progress!" << std::endl;
                }
                i = r.second;
                std::cout << "top level count " << count++ << " total " << total_top_level++ << std::endl;
            }
        }*/
        
    private:

        real_t _update_minimum(size_t id, real_t limit = std::numeric_limits<real_t>::max())
        {
            distance_count++;
            limit = std::min(limit, minimum.first);
            real_t sq = ::distance_sq_limit(store.at(id), search, limit * limit), dist = sqrt(sq);
            if (dist < limit) {
                minimum = value_index_t(dist, id);
                std::cout <<  "min distance " << dist << " id " << id << std::endl;
            }
            return dist;
        }
        
        range_state &_update_range_from_distance(real_t distance)
        {
            if (distance == std::numeric_limits<real_t>::max())
                return *this;
            typename key_id_t::base_id_exp_norm_comparer c;
            if (distance <= 0) {
                range = std::equal_range(range.first, range.second, skey, c);
                return *this;
            }
            key_id_t lo(skey), hi(skey);
            lo.key.norm -= distance;
            hi.key.norm += distance;
            auto e = index.end();
            if (range.first  != e && !c(lo, *(range.first)))  range.first  = std::lower_bound(range.first, range.second, lo, c);
            if (range.second != e &&  c(hi, *(range.second))) range.second = std::upper_bound(range.first, range.second, hi, c);
            print_range(std::cout);
            return *this;
        }
    };
    
    void find_ids(const flat_vector_t &search, const container_t &store)
    {
        key_t search_key(search);
        range_state rs(search, store, _index, search_key);
        rs.print_range(std::cout, "A");

        // get an initial distance
        rs.update();
        rs.print_range(std::cout, "B");
    }

private:
    
    enum { USE_QUEUE = false };
    
    inline std::ostream &_print_range(std::ostream &os, const const_range_t &range) const
    {
        return os << "range " << (range.first - _index.begin()) << "..." << (range.second - _index.begin());
    }

    inline std::ostream &_print_state(std::ostream &os, const state_t &state) const
    {
        _print_range(os, state.range) << " rank " << state.rank << " dimensions ";
        return _stream_out(os, state.dimensions, 16) << std::endl;
    }
};
