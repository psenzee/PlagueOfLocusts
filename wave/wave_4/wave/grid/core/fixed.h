#pragma once

#include <array>

#include "core_grid.h"
#include "utils.h"

namespace grid {

template <typename T, size_t W, size_t H>
struct fixed : public grid<T>
{
    typedef fixed<T, W, H>       self_t;
    typedef std::array<T, W * H> data_t;
    
    data_t _data;
    T      _null;
    
    static const point2i CENTER;
    
    inline fixed(const T null_t = T()) : _data({ null_t }) {}
    inline fixed(const self_t &o) : _null(o._null), _data(o._data) {}
    inline fixed(const readable<T> &o) : _null(o.null_value()) { copy(o, *this); }
    
    inline self_t  &operator= (const self_t &o)                     { if (this == &o) return *this; _null = o._null; _data = o._data; return *this; }
    inline bool     operator==(const self_t &o)               const { if (this == &o) return true; return _data == o._data; }
    
    inline void     set(const point2i &p, const T &value)           { _data[index(p, size2u { W, H })] = value; }
    inline T        get(const point2i &p)                     const { return _data[index(p, size2u { W, H })]; }
    inline T        null_value()                              const { return _null; }
    inline bool     is_referrable()                           const { return true; }
    inline const T &get_ref(const point2i &p)                 const { return _data[index(p, size2u { W, H })]; }
    inline T       &get_ref(const point2i &p)                       { return _data[index(p, size2u { W, H })]; }
    inline size2u   size()                                    const { return size2u { W, H }; }
};

template <typename T, size_t W, size_t H>
const point2i fixed<T, W, H>::CENTER { W / 2, H / 2 };

}
