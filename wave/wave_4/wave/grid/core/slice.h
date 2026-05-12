#pragma once

#include <vector>

#include "grid.h"

namespace grid {

template <typename T, std::size_t N>
struct slice : public grid<T>
{
    typedef slice<T, N>      self_t;
    typedef std::array<T, N> value_t;
    typedef grid<value_t>    source_grid_t;
    typedef readable<T>      readable_t;

    source_grid_t &_source;
    std::size_t    _index;
    
    inline slice(source_grid_t &source, std::size_t index) : _source(source), _index(index) {}
    
    inline source_grid_t &get_source()                                { return _source; }
    inline self_t        &set_source(source_grid_t &source)           { _source = source; return *this; }

    inline self_t        &operator= (const readable_t &o)             { if (this == &o) return *this; copy(o, *this); return *this; }
    inline bool           operator==(const readable_t &o)       const { if (this == &o) return true; return size() == o.size() && equal(*this, o); }
    
    inline void           set(const point2i &p, const T &value)       { auto v = _source.get(p); v[_index] = value; _source.set(p, v); }
    inline T              get(const point2i &p)                 const { return _source.get(p)[_index]; }
    inline bool           is_referrable()                       const { return _source.is_referrable(); }
    inline const T       &get_ref(const point2i &p)             const { return _source.get_ref(p)[_index]; }
    inline T             &get_ref(const point2i &p)                   { return _source.get_ref(p)[_index]; }
    inline T              null_value()                          const { return _source.null_value()[_index]; }
    inline size2u         size()                                const { return _source.size(); }
};

}
