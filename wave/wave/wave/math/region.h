#pragma once

#include "point2i.h"
#include "size2u.h"
#include "core_math.h"

constexpr point2i operator+(const point2i &p, const size2u &sz) { return point2i(p.x + int(sz.w), p.y + int(sz.h)); }
constexpr size2u  operator+(const size2u &sz, const point2i &p) { return size2u(size_t(std::max(int(sz.w) + p.x, 0)), size_t(std::max(int(sz.h) + p.y, 0)));; }
constexpr point2i operator-(const point2i &p, const size2u &sz) { return point2i(p.x - int(sz.w), p.y - int(sz.h)); }
constexpr size2u  operator-(const size2u &sz, const point2i &p) { return size2u(size_t(std::max(int(sz.w) - p.x, 0)), size_t(std::max(int(sz.h) - p.y, 0))); }

struct region
{
    point2i min, max;
    
    constexpr region()                                       : min(point2i::MAX), max(point2i::MIN) {}
    constexpr region(const point2i &p)                       : min(p), max(p) {}
    constexpr region(const point2i &min, const point2i &max) : min(min), max(max) {}
    constexpr region(const point2i &min, const size2u &sz)   : min(min), max((min + sz) - point2i(1, 1)) {}
    constexpr region(const size2u &sz)                       : min(0, 0), max(point2i(-1, -1) + sz) {}
    constexpr region(const region &other)                    : min(other.min), max(other.max) {}
    
    constexpr region &operator=(const region &o)        { if (this != &o) { min = o.min; max = o.max; } return *this; }
    
    constexpr bool    operator==(const region &o) const { return this == &o || (min == o.min && max == o.max); }
    constexpr bool    operator!=(const region &o) const { return this != &o || min != o.min || max != o.max; }
    
    constexpr region &insert(const point2i &p)          { min = min_mins(p); max = max_maxs(p); return *this; }
    constexpr region &insert(const region &w)           { insert(w.min); insert(w.max); return *this; }
    
    constexpr size2u  size()                      const { return size2u(size_t(max.x - min.x + 1), size_t(max.y - min.y + 1)); }
    constexpr int     width()                     const { return max.x - min.x + 1; }
    constexpr int     height()                    const { return max.y - min.y + 1; }
    constexpr int     area()                      const { return width() * height(); }

    constexpr bool    contains(const point2i &p)  const { return p.x >= min.x && p.x <= max.x && p.y >= min.y && p.y <= max.y; }

    constexpr point2i max_mins(const point2i &p)  const { return point2i(std::max(min.x, p.x), std::max(min.y, p.y)); }
    constexpr point2i max_mins(const region &w)   const { return max_mins(w.min); }
    constexpr point2i max_maxs(const point2i &p)  const { return point2i(std::max(max.x, p.x), std::max(max.y, p.y)); }
    constexpr point2i max_maxs(const region &w)   const { return max_maxs(w.max); }
    constexpr point2i min_maxs(const point2i &p)  const { return point2i(std::min(max.x, p.x), std::min(max.y, p.y)); }
    constexpr point2i min_maxs(const region &w)   const { return min_maxs(w.max); }
    constexpr point2i min_mins(const point2i &p)  const { return point2i(std::min(min.x, p.x), std::min(min.y, p.y)); }
    constexpr point2i min_mins(const region &w)   const { return min_mins(w.min); }

    constexpr region  intersect(const region &w)  const { return valid() && w.valid() ? region(max_mins(w), min_maxs(w)) : region(); }
    constexpr region  combine(const region &w)    const { return region(min_mins(w), max_maxs(w)); }
    
    constexpr size_t  index_at(const point2i &p)  const { return size_t((p.y - min.y) * (max.x - min.x + 1) + (p.x - min.x)); }
    constexpr point2i point_at(size_t i)          const { int w = max.x - min.x + 1; return point2i(int(i % w) + min.x, int(i / w) + min.y); }

    constexpr void    reset()                           { min = point2i::MAX; max = point2i::MIN; }
    constexpr bool    valid()                     const { return max.x >= min.x && max.y >= min.y; }
    constexpr bool    empty()                     const { return max.x <= min.x || max.y <= min.y; }
    
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
