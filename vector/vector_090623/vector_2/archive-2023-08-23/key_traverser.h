#include <iostream>
#include <array>
#include <algorithm>
#include "stl.h"
#include "bfloat16.h"
#include "container.h"
#include "simple_timer.h"
#include "key.h"

template <typename FloatT, size_t N, typename IndexT = int16_t, size_t RanksN = 32, size_t KeyRanksN = 5>
struct key_index
{
    typedef key_index<FloatT, N, IndexT, RanksN, KeyRanksN> self_t;
    typedef key<FloatT, N, IndexT, RanksN, KeyRanksN>       key_t;
    typedef key_id<FloatT, N, IndexT, RanksN, KeyRanksN>    key_id_t;
    typedef std::vector<key_id_t>                           data_t;
    typedef typename data_t::const_iterator                 const_iterator;
    typedef std::pair<const_iterator, const_iterator>       const_range_t;
    typedef std::array<FloatT, N>                           vector_t;

    data_t _data;

    inline self_t &add(const key_t &key)  { _data.push_back(key_id_t(key, _data.size())); return *this; }
    inline self_t &add(const vector_t &v) { return add(key_t(v)); }
    
    inline auto    begin() const { return _data.begin(); }
    inline auto    end()   const { return _data.end(); }

    self_t        &index() { typename key_id_t::sort_comparator c; std::sort(_data.begin(), _data.end(), c); return *this; }
};

template <typename FloatT, size_t N, typename IndexT = int16_t, size_t RanksN = 32, size_t KeyRanksN = 5>
struct key_traverser
{
    typedef FloatT                                              real_t;
    typedef key_traverser<FloatT, N, IndexT, RanksN, KeyRanksN> self_t;
    typedef key_index<FloatT, N, IndexT, RanksN, KeyRanksN>     key_index_t;
    typedef key<FloatT, N, IndexT, RanksN, KeyRanksN>           key_t;
    typedef key_id<FloatT, N, IndexT, RanksN, KeyRanksN>        key_id_t;
    typedef typename key_t::key_ranks_t                         dimensions_t;
    typedef typename key_index_t::const_range_t                 const_range_t;
    typedef std::array<FloatT, N>                               flat_vector_t;
    typedef container<N, FloatT>                                container_t;
    typedef std::pair<real_t, size_t>                           value_index_t;
    typedef std::vector<uint64_t>                               ids_t;
    
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
    
    inline key_traverser(const key_index_t &index) :
         _index(index), _breadth_first(false), _max_ids_per_state(3), _full_distance_counts(0) {}
    
    struct range_state
    {
        const flat_vector_t &search;
        const container_t   &store;
        const key_index_t   &index;
        key_id_t             skey;
        const_range_t        range;
        value_index_t        minimum;
        size_t               distance_count;
        
        inline range_state(const flat_vector_t &search, const container_t &store, const key_index_t &index, const const_range_t &range, const key_t &key)
            : search(search), store(store), index(index), skey(key, 0), range(range), minimum(std::numeric_limits<real_t>::max(), 0), distance_count(0) {}
        
        inline range_state(const flat_vector_t &search, const container_t &store, const key_index_t &index, const key_t &key_)
            : search(search), store(store), index(index), skey(key_, 0), minimum(std::numeric_limits<real_t>::max(), 0), distance_count(0)
        {
            // set base range
            typename key_id_t::base_id_comparator c;
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
            typename key_id_t::base_id_norm_comparator c;
            auto i = std::lower_bound(range.first, range.second, skey, c);
            if (i != range.second) _update_range((*i).id);
            _from_norm_slope_ranks(skey, range);
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
        
        void _from_norm_slope_ranks(const key_id_t &sk, const const_range_t &rn)
        {
            auto i = std::max(rn.first, range.first), e = std::min(rn.second, range.second);
            key_id_t k(sk);
            typename key_id_t::norm_comparator c;
            while (i < e) {
                k.key.norm = (*i).key.norm;
                const_range_t r = std::equal_range(i, e, k, c);
                if (r.first == e) break;
                if (r.first >= r.second) continue;
                _from_slope_ranks(sk, r);
                i = r.second;
            }
        }
        
        void _from_slope_ranks(const key_id_t &sk, const const_range_t &rn)
        {
            auto i = std::max(rn.first, range.first), e = std::min(rn.second, range.second);
            key_id_t k(sk);
            size_t slope_key = k.key.slope_key;
            typename key_id_t::slope_comparator c;
            while (i < e) {
                k.key.slope_key = (*i).key.slope_key;
                const_range_t r = std::equal_range(i, e, k, c);
                if (r.first == e) break;
                if (r.first >= r.second) continue;
                // determine what to do by slope, we need to process low slopes differently
                if (abs(int(slope_key) - int(k.key.slope_key)) <= 2) {
                    if (slope_key <= 3) _from_low_slope(sk, r);
                    else                _from_ranks(sk, r);
                }
                i = r.second;
            }
        }
        
        void _from_low_slope(const key_id_t &sk, const const_range_t &rn, size_t rank = 0)
        {
            _update_from_range(rn);
        }
        
        void _from_ranks(const key_id_t &sk, const const_range_t &rn, size_t rank = 0)
        {
            if (rank >= 4) {
                _update_from_range(rn);
                return;
            }
            auto i = std::max(rn.first, range.first), e = std::min(rn.second, range.second);
            key_id_t k(sk);
            size_t dim = k.key.ranks[rank];
            typename key_id_t::rank_comparator c(rank);
            while (i < e) {
                k.key.ranks[rank] = (*i).key.ranks[rank];
                const_range_t r = std::equal_range(i, e, k, c);
                if (r.first == e) break;
                if (r.first >= r.second) continue;
                if (dim == k.key.ranks[rank])
                    _from_ranks(sk, r, rank + 1);
                else if (rank > 0)
                    _update_from_range(r);
                i = r.second;
            }
        }

    private:

        real_t _update_minimum(size_t id, real_t limit = std::numeric_limits<real_t>::max())
        {
            distance_count++;
            limit = std::min(limit, minimum.first);
            real_t sq = ::distance_sq_limit(store.at(id), search, limit * limit), dist = sqrt(sq);
            if (dist < limit) {
                minimum.first  = dist;
                minimum.second = id;
                std::cout <<  "min distance " << dist << " id " << id << std::endl;
            }
            return dist;
        }
        
        range_state &_update_range_from_distance(real_t distance)
        {
            if (distance == std::numeric_limits<real_t>::max())
                return *this;
            typename key_id_t::base_id_norm_comparator c;
            if (distance <= 0) {
                range = std::equal_range(range.first, range.second, skey, c);
                return *this;
            }
            real_t norm(skey.key.get_norm());
            key_id_t lo(skey), hi(skey);
            lo.key.set_norm(norm - distance);
            hi.key.set_norm(norm + distance);
            if (!c(lo, *(range.first)))  range.first  = std::lower_bound(range.first, range.second, lo, c);
            if ( c(hi, *(range.second))) range.second = std::upper_bound(range.first, range.second, hi, c);
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
