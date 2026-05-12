#pragma once

#include <array>

#include "grid.h"
#include "grid_utils.h"

namespace wave {

template <typename T, size_t W, size_t H>
struct fixed_grid : public grid<T>
{
    typedef fixed_grid<T, W, H> self_t;
    typedef std::array<T, W * H> data_t;
    
    data_t _data;
    T      _null;
    
    static const point2i CENTER;
    
    inline fixed_grid(const T null_t = T()) : _data({ null_t }) {}
    inline fixed_grid(const self_t &o) : _null(o._null), _data(o._data) {}
    inline fixed_grid(const readable_grid<T> &o) : _null(o.null_value()) { copy(o, *this); }
    
    inline self_t  &operator= (const readable_grid<T> &o)           { if (this == &o) return *this; copy(o, *this); }
    inline self_t  &operator= (const self_t &o)                     { if (this == &o) return *this; _null = o._null; _data = o._data; return *this; }
    inline bool     operator==(const self_t &o)               const { if (this == &o) return true; return _data == o._data; }
    inline bool     operator==(const readable_grid<T> &o)     const { if (this == &o) return true; return equal(*this, o); }
    
    inline void     set(const point2i &p, const T &value)           { _data[index(p, size())] = value; }
    inline T        get(const point2i &p)                     const { return _data[index(p, size())]; }
    inline T        null_value()                              const { return _null; }
    inline const T &get_ref(const point2i &p)                 const { return _data[index(p, size())]; }
    inline size2u   size()                                    const { return size2u(W, H); }
};

template <typename T, size_t W, size_t H>
const point2i fixed_grid<T, W, H>::CENTER(W / 2, H / 2);

}
