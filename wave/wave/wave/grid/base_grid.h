#pragma once

#include <vector>

#include "grid.h"
#include "grid_utils.h"

namespace wave {

template <typename T>
struct base_grid : public grid<T>
{
    typedef base_grid<T> self_t;

    std::vector<T> _data;
    size2u         _size;
    T              _null;
    
    inline base_grid(const T null_t = T())                    : _size(0, 0), _null(null_t) {}
    inline base_grid(const size2u &sz, const T null_t = T())  : _size(sz), _null(null_t) { init(sz, null_t); }
    inline base_grid(const self_t &o)                         : _size(o._size), _null(o._null), _data(o._data) {}
    inline base_grid(const readable_grid<T> &o)               : _size(o.size()), _null(T()) { init(o.size(), _null); }
    
    void init(const size2u &sz, const T null_t = T()) { _size = sz; _null = null_t; _data.resize(_size.area()); clear(*this, _null); }
    
    inline self_t &operator= (const grid<T> &o)                 { if (this == &o) return *this; _init(o.size(), o._null); return *this; }
    inline self_t &operator= (const self_t  &o)                 { if (this == &o) return *this; _size = o._size; _null = o._null; _data = o._data; return *this; }
    inline bool    operator==(const self_t  &o)           const { if (this == &o) return true; return _size == o._size && _data == o._data; }
    inline bool    operator==(const grid<T> &o)           const { if (this == &o) return true; return _size == o.size() && equal(*this, o); }
    
    inline void     set(const point2i &p, const T &value)       { _data[index(p, _size)] = value; }
    inline T        get(const point2i &p)                 const { return _data[index(p, _size)]; }
    inline T        null_value()                          const { return _null; }
    inline size2u   size()                                const { return _size; }
};

}
