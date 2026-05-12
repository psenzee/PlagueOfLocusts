#pragma once

#include <vector>

#include "core_grid.h"
#include "utils.h"

namespace grid {

template <typename T>
struct view : public grid<T>
{
    typedef view<T>     self_t;
    typedef readable<T> readable_t;
    typedef grid<T>     source_grid_t;

    source_grid_t &_source;
    region         _region;
    
    constexpr view(source_grid_t &source, const region &region) : _source(source), _region(region) {}
    constexpr view(self_t &o) : _source(o._source), _region(o._region) {}

    inline self_t  &operator= (const self_t &o)                 { if (this == &o) return *this; _source = o._source; _region = o._region; return *this; }

    inline void     set(const point2i &p, const T &value)       { _source.set(p + _region.min, value); }
    inline T        get(const point2i &p)                 const { return _source.get(p + _region.min); }
    inline bool     is_referrable()                       const { return _source.is_referrable(); }
    inline const T &get_ref(const point2i &p)             const { return _source.get_ref(p + _region.min); }
    inline T       &get_ref(const point2i &p)                   { return _source.get_ref(p + _region.min); }
    inline T        null_value()                          const { return _source.null_value(); }
    inline size2u   size()                                const { return _region.size(); }
};

}
