#pragma once

#include "core_grid.h"
#include "utils.h"
#include "serialize.h"
#include <unordered_map>

namespace grid {

// TODO

template <typename T>
struct sparse : public grid<T>, serializable
{
    typedef sparse<T>    self_t;
    typedef readable<T>  readable_t;

    std::unordered_map<point2i, T> _data;
    region                         _region;
    T                              _null;

    inline sparse(const T null_t = T())                    : _region(size2u(0, 0)), _null(null_t) {}
    inline sparse(const size2u &sz, const T null_t = T())  : _region(sz), _null(null_t) { _init(region(sz), null_t); }
    inline sparse(const region &r, const T null_t = T())   : _region(r), _null(null_t) { _init(r, null_t); }
    inline sparse(const self_t &o)                         : _region(o._region), _null(o._null), _data(o._data) {}
    inline sparse(const readable_t &o)                     : _region(o.size()), _null(T()) { _init(region(o.size()), _null); }

    inline self_t &operator= (const self_t &o)                  { if (this == &o) return *this; _region = o._region; _null = o._null; _data = o._data; return *this; }
    inline bool    operator==(const self_t &o)            const { if (this == &o) return true; return _region == o._region && _data == o._data; }

    inline void     set(const point2i &p, const T &v)           { if (v != _null) _data[p] = v; else _data.erase(p); }
    inline T        get(const point2i &p)                 const { return _data.contains(p) ? _data.at(p) : _null; }
    inline bool     is_referrable()                       const { return false; }
    inline T        null_value()                          const { return _null; }
    inline size2u   size()                                const { return _region.size(); }
    
    inline region   get_region()                          const { return _region; }
    
    inline void     clear()                                     { _data.clear(); _region.reset(); }
    
    std::size_t     data_write(binary_out_stream &s) const
    {
        std::size_t bytes = s.write(_region) + ::data_write(s, _null);
        bytes += ::data_write(s, _data);
        return bytes;
    }

    std::size_t     data_read(binary_in_stream &s)
    {
        std::size_t bytes = s.read(_region) + ::data_read(s, _null);
        _data.clear();
        bytes += ::data_read(s, _data);
        return bytes;
    }


private:

    void _init(const struct region &r = region(), T null_t = T()) { _region = r; _null = null_t; clear(); }
};

}
