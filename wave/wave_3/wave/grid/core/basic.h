#pragma once

#include <vector>

#include "core_grid.h"
#include "utils.h"
#include "serialize.h"

namespace grid {

template <typename T>
struct basic : public grid<T>, serializable
{
    typedef basic<T>     self_t;
    typedef readable<T>  readable_t;

    std::vector<T> _data;
    size2u         _size;
    T              _null;

    inline basic(const T null_t = T())                    : _size(0, 0), _null(null_t) {}
    inline basic(const size2u &sz, const T null_t = T())  : _size(sz), _null(null_t) { _init(sz, null_t); }
    inline basic(const self_t &o)                         : _size(o._size), _null(o._null), _data(o._data) {}
    inline basic(const readable_t &o)                     : _size(o.size()), _null(T()) { _init(o.size(), _null); }

    inline self_t &operator= (const self_t &o)                  { if (this == &o) return *this; _size = o._size; _null = o._null; _data = o._data; return *this; }
    inline bool    operator==(const self_t &o)            const { if (this == &o) return true; return _size == o._size && _data == o._data; }

    inline void     set(const point2i &p, const T &v)           { _data[index(p, _size)] = v; }
    inline T        get(const point2i &p)                 const { return _data[index(p, _size)]; }
    inline bool     is_referrable()                       const { return true; }
    inline const T &get_ref(const point2i &p)             const { return _data[index(p, _size)]; }
    inline T       &get_ref(const point2i &p)                   { return _data[index(p, _size)]; }
    inline T        null_value()                          const { return _null; }
    inline size2u   size()                                const { return _size; }

    std::size_t     data_write(binary_out_stream &s) const
    {
        std::size_t bytes = s.write(_size) + ::data_write(s, _null);
        bytes += ::data_write(s, _data);
        return bytes;
    }

    std::size_t     data_read(binary_in_stream &s)
    {
        std::size_t bytes = s.read(_size);
        bytes += ::data_read(s, _null);
        _data.clear();
        return bytes + ::data_read(s, _data);
    }

private:

    void   _init(const size2u &sz, T null_t = T())              { _size = sz; _null = null_t; _data.resize(_size.area()); clear(*this, _null); }
};

}
