#pragma once

#include <iostream>
#include <iomanip>

#include <limits>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <queue>

#include "stl.h"
#include "value_index_t.h"

template <typename FloatT, typename ValueCompareT>
struct n_best
{
    typedef FloatT                                                      real_t;
    typedef std::pair<real_t, size_t>                                   elem_t;
    typedef ValueCompareT                                               value_compare_t;
    typedef pair_compare_first<elem_t, ValueCompareT>                   compare_t;
    typedef std::priority_queue<elem_t, std::vector<elem_t>, compare_t> priority_queue_t;

    const bool ECHO = false;

private:
    
    priority_queue_t _queue;
    size_t           _max_queue_size;
    size_t           _submitted;
    elem_t           _best;
    value_compare_t  _value_compare;
    
    inline bool _enqueue(const elem_t &e)
    {
        _submitted++;
        //_echo(e);
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

    inline n_best(size_t k) : _max_queue_size(k), _submitted(0)
    {
        real_t initial = _value_compare(0.0, 1.0) ? std::numeric_limits<real_t>::max() : std::numeric_limits<real_t>::min();
        _best = elem_t(initial, ~0);
        _queue.push(_best);
        //_echo(_best);
    }
    
    inline bool          add(real_t value, size_t index) { return add(elem_t(value, index)); }
    inline bool          add(const elem_t &elem)         { return _enqueue(elem); }
    
    inline const elem_t &best()            const { return _best; }
    inline const elem_t &kth_best()        const { return _queue.empty() ? _best : _queue.top(); }
    inline size_t        size()            const { return _queue.size(); }
    inline size_t        submitted_count() const { return _submitted; }
    
    std::ostream &print(std::ostream &os)
    {
        return os << "total submitted " << submitted_count() << " size " << size() <<
            " best " << _value(best()) << " kth best " << _value(kth_best()) << std::endl;
    }
};
