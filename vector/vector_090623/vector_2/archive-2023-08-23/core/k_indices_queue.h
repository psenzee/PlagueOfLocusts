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

#include "simple_timer.h"
#include "btree_set.h"
#include "stl.h"

template <typename FloatT, typename ValueCompareT>
struct k_indices_queue
{
    typedef FloatT                                                      value_type;
    typedef std::vector<value_type>                                     vector_t;
    typedef std::pair<value_type, size_t>                               elem_t;
    typedef ValueCompareT                                               value_compare_t;
    typedef compare_0<elem_t, ValueCompareT>                            compare_t;
    typedef std::priority_queue<elem_t, std::vector<elem_t>, compare_t> priority_queue_t;
    typedef btree::btree_set<size_t>                                    index_set_t;
    
    const bool ECHO = false;
    
private:
    
    priority_queue_t _queue;
    size_t           _max_queue_size;
    size_t           _submitted;
    elem_t           _best;
    index_set_t      _seen;
    value_compare_t  _value_compare;
    
    bool _enqueue(const elem_t &e)
    {
        _submitted++;
        if (_seen.find(_index(e)) != _seen.end())
            return false;
        _seen.insert(_index(e));
        _best = _value_compare(_value(e), _value(_best)) ? e : _best;
        if (_queue.size() < _max_queue_size) {
            _queue.push(_echo(e));
        } else if (_value_compare(_value(e), _value(_queue.top()))) {
            _queue.push(_echo(e));
            _queue.pop();
        }
        return true;
    }
    
    inline const elem_t &_echo(const elem_t &e) { if (ECHO) std::cout << "Q " << _value(e) << " @" << _index(e) << std::endl; return e; }
    
public:

    inline k_indices_queue(size_t k) : _max_queue_size(k), _submitted(0)
    {
        value_type initial = _value_compare(0.0, 1.0) ? std::numeric_limits<value_type>::max() : std::numeric_limits<value_type>::min();
        _best = elem_t(initial, ~0);
    }
    
    inline bool add(value_type value, size_t index) { return _enqueue(elem_t(value, index)); }
    
    inline const elem_t &best()            const { return _best; }
    inline const elem_t &kth_best()        const { return _queue.empty() ? _best : _queue.top(); }
    inline size_t        queue_size()      const { return _queue.size(); }
    inline size_t        size()            const { return _seen.size(); }
    inline size_t        submitted_count() const { return _submitted; }
    
    std::ostream &print(std::ostream &os)
    {
        return os << "total submitted " << submitted_count() << " distinct " << size() <<
            " best " << _value(best()) << " kth best " << _value(kth_best()) << std::endl;
    }
};
