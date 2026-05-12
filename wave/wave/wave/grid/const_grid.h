#pragma once

#include "grid.h"

namespace wave {

template <typename T, size_t W, size_t H>
struct const_grid : public grid<T>
{
    typedef const_grid<T, W, H> self_t;
    
    const T *_data;
    size     _size;
    
    inline const_grid() : _size(0, 0), _data(nullptr) {}
    inline const_grid(const self_t &other) : _size(other._size), _data(other._data) {}
    
    inline self_t &operator= (const self_t &other)                 { if (this == &other) return *this; _size = other._size, _data = other._data; return *this; }
    inline bool    operator==(const self_t &other)           const { if (this == &other) return true; return _data == other._data || grid_utils<T>::equal(*this, other); }
    inline bool    operator==(const readable_grid<T> &other) const { if (this == &other) return true; return grid_utils<T>::equal(*this, other); }
    
    inline void     set(const point2i &p, const T &value)          { }
    inline T        get(const point2i &p)                    const { return _data[index(p)]; }
    inline T        null_value()                             const { return T(0); }
    inline const T &get_ref(const point2i &p)                const { return _data[index(p)]; }
    inline size     size()                                   const { return _size; }
};

}
