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

#include "simple_timer.h"
#include "btree_map.h"
#include "stl.h"
//#include "slices.h"
#include "generators.h"

template <typename VectorT>
struct metric_cache
{
    typedef VectorT                                vector_t;
    typedef typename VectorT::value_type           value_type;
    typedef btree::btree_map<uint64_t, value_type> map_t;
    typedef typename map_t::const_iterator         map_iterator_t;
    typedef std::vector<VectorT>                   container_t;
    typedef std::pair<bool, value_type>            result_t;
    
    enum { QUERIED = 1ull << 31 };

    container_t _queried;
    map_t       _map;
    
    result_t find(uint32_t index_a, uint32_t index_b)
    {
        uint64_t key = _key(index_a, index_b);
        bool found = false;
        value_type value = value_type(0);
        map_iterator_t i = _map.find(key);
        if (i != _map.end()) {
            found = true;
            value = (*i).second;
            std::cout << "found in cache" << std::endl;
        }
        return result_t(found, value);
    }
    
    inline result_t find(uint32_t index_a, const VectorT &b)
    {
        uint32_t ib = _find_in_queried(b);
        return find(index_a, ib | QUERIED);
    }
    
    inline result_t find(const VectorT &a, uint32_t index_b) { return find(index_b, a); }
    
    inline value_type put(uint32_t index_a, uint32_t index_b, value_type metric)
    {
        uint64_t key = _key(index_a, index_b);
        _map[key] = metric;
        return metric;
    }

    inline value_type put(uint32_t index_a, const VectorT &b, value_type metric)
    {
        uint32_t ib = _find_in_queried(b);
        return put(index_a, ib | QUERIED, metric);
    }
    
    inline value_type put(const VectorT &a, uint32_t index_b, value_type metric) { return put(index_b, a, distance); }
    
private:
    
    uint32_t _find_in_queried(const VectorT &v)
    {
        for (uint32_t i = 0, sz = uint32_t(_queried.size()); i != sz; ++i)
            if (_queried[i] == v) return i;
        return _add_to_queried(v);
    }
    
    uint32_t _add_to_queried(const VectorT &v)
    {
        uint32_t id(_queried.size());
        _queried.push_back(v);
        return id;
    }
    
    inline uint64_t _key(uint32_t index_a, uint32_t index_b) const
    {
        uint64_t u = std::min(index_a, index_b), v = std::max(index_a, index_b);
        return (u << 32 | v);
    }
};
