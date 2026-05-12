#pragma once

#include "core_grid.h"
#include "utils.h"

namespace grid {

template <typename T>
struct const_data : public readable<T>
{
    typedef const_data<T> self_t;
    
    const T *_data;
    size2u   _size;
    
    inline const_data(const size2u &sz = size2u(0, 0), const T *data = nullptr) : _size(sz), _data(data) {}
    inline const_data(const self_t &o) : _size(o._size), _data(o._data) {}
    
    inline bool    operator==(const self_t &o)           const { if (this == &o) return true; return _data == o._data || equal(*this, o); }
    
    bool            is_read_only()                       const { return true; }
    
    inline T        get(const point2i &p)                const { return _data[index(p, _size)]; }
    inline T        null_value()                         const { return T(0); }
    inline bool     is_referrable()                      const { return true; }
    inline const T &get_ref(const point2i &p)            const { return _data[index(p, _size)]; }
    inline bool     is_row_accessible(std::size_t y)     const { return true; }
    inline const T *row(int y)                           const { return &_data[index(point2i(0, y), _size)]; }
    inline size2u   size()                               const { return _size; }
};

}
