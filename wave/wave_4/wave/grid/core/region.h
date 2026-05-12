#pragma once

#include "point2i.h"
#include "size2u.h"
#include "std_array.h"

constexpr point2i  operator+(const point2i &p, const size2u &sz) { return point2i { p[0] + int(sz[0]), p[1] + int(sz[1]) }; }
constexpr size2u   operator+(const size2u &sz, const point2i &p) { return size2u { size_t(std::max(int(sz[0]) + p[0], 0)), size_t(std::max(int(sz[1]) + p[1], 0)) }; }
constexpr point2i  operator-(const point2i &p, const size2u &sz) { return point2i { p[0] - int(sz[0]), p[1] - int(sz[1]) }; }
constexpr size2u   operator-(const size2u &sz, const point2i &p) { return size2u { size_t(std::max(int(sz[0]) - p[0], 0)), size_t(std::max(int(sz[1]) - p[1], 0)) }; }

struct region
{
    point2i min, max;
    
    constexpr region()                                       : min(std_array::max<int, 2>()), max(std_array::lowest<int, 2>()) {}
    constexpr region(const point2i &p)                       : min(p), max(p) {}
    constexpr region(const point2i &min, const point2i &max) : min(min), max(max) {}
    constexpr region(const point2i &min, const size2u &sz)   : min(min), max((min + sz) - point2i { 1, 1 }) {}
    constexpr region(const size2u &sz)                       : min { 0, 0 }, max(point2i { -1, -1 } + sz) {}
    constexpr region(const region &other)                    : min(other.min), max(other.max) {}
    
    constexpr region &operator=(const region &o)        { if (this != &o) { min = o.min; max = o.max; } return *this; }
    
    constexpr bool    operator==(const region &o) const { return this == &o || (min == o.min && max == o.max); }
    constexpr bool    operator!=(const region &o) const { return this != &o || min != o.min || max != o.max; }
    
    constexpr region &insert(const point2i &p)          { min = min_mins(p); max = max_maxs(p); return *this; }
    constexpr region &insert(const region &w)           { insert(w.min); insert(w.max); return *this; }
    
    constexpr size2u  size()                      const { return size2u { size_t(max[0] - min[0] + 1), size_t(max[1] - min[1] + 1) }; }
    constexpr int     width()                     const { return max[0] - min[0] + 1; }
    constexpr int     height()                    const { return max[1] - min[1] + 1; }
    constexpr int     area()                      const { return width() * height(); }

    constexpr bool    contains(const point2i &p)  const { return p[0] >= min[0] && p[0] <= max[0] && p[1] >= min[1] && p[1] <= max[1]; }

    constexpr point2i max_mins(const point2i &p)  const { return point2i { std::max(min[0], p[0]), std::max(min[1], p[1]) }; }
    constexpr point2i max_mins(const region &w)   const { return max_mins(w.min); }
    constexpr point2i max_maxs(const point2i &p)  const { return point2i { std::max(max[0], p[0]), std::max(max[1], p[1]) }; }
    constexpr point2i max_maxs(const region &w)   const { return max_maxs(w.max); }
    constexpr point2i min_maxs(const point2i &p)  const { return point2i { std::min(max[0], p[0]), std::min(max[1], p[1]) }; }
    constexpr point2i min_maxs(const region &w)   const { return min_maxs(w.max); }
    constexpr point2i min_mins(const point2i &p)  const { return point2i { std::min(min[0], p[0]), std::min(min[1], p[1]) }; }
    constexpr point2i min_mins(const region &w)   const { return min_mins(w.min); }

    constexpr region  intersect(const region &w)  const { return valid() && w.valid() ? region(max_mins(w), min_maxs(w)) : region(); }
    constexpr region  combine(const region &w)    const { return region(min_mins(w), max_maxs(w)); }
    
    constexpr size_t  index_at(const point2i &p)  const { return size_t((p[1] - min[1]) * (max[0] - min[0] + 1) + (p[0] - min[0])); }
    constexpr point2i point_at(size_t i)          const { int w = max[0] - min[0] + 1; return point2i { int(i % w) + min[0], int(i / w) + min[1] }; }

    constexpr void    reset()                           { min = std_array::max<int, 2>(); max = std_array::lowest<int, 2>(); }
    constexpr bool    valid()                     const { return max[0] >= min[0] && max[1] >= min[1]; }
    constexpr bool    empty()                     const { return max[0] <= min[0] || max[1] <= min[1]; }
    
    struct const_iterator
    {
        typedef const_iterator self_t;

        const region *_r;
        size_t _ri, _rarea;

        constexpr const_iterator(const region &r, size_t i = 0)     : _r(&r), _rarea(r.area()), _ri(i)             {}
        constexpr const_iterator(const region &r, const point2i &p) : _r(&r), _rarea(r.area()), _ri(r.index_at(p)) {}
        constexpr const_iterator(const const_iterator &o)           : _r(o._r), _rarea(o._rarea), _ri(o._ri)       {}

        constexpr self_t  &operator=(const self_t &o)        { if (&o != this) { _r = o._r; _rarea = o._rarea; _ri = o._ri; } return *this; }

        constexpr bool     operator==(const self_t &o) const { return _ri == o._ri && _r == o._r; }
        constexpr bool     operator!=(const self_t &o) const { return _ri != o._ri || _r != o._r; }
        
        constexpr self_t  &operator++()                      { _ri++; return *this; }
        constexpr self_t  &operator++(int)                   { _ri++; return *this; }
        constexpr point2i  operator*()                 const { return _r->point_at(_ri); }

        constexpr bool     valid()                     const { return _ri < _rarea; }
    };

    constexpr const_iterator begin()              const { return const_iterator(*this, 0); }
    constexpr const_iterator end()                const { return const_iterator(*this, area()); }
};
