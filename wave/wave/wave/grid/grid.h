#pragma once

#include <algorithm>

#include "point2i.h"
#include "size2u.h"
#include "region.h"

namespace {
template <typename T, typename U> inline T _mod(U value, T div)          { return (value % div + div) % div; } // correctly handle negatives for this purpose
template <typename T>             inline T _clamp(T value, T min, T max) { return std::max(std::min(value, max), min); }
}

namespace wave {

inline size_t  index_clamp(const point2i &p, const size2u &sz)  { return _clamp(p.x, 0, int(sz.w) - 1) + _clamp(p.y, 0, int(sz.h) - 1) * sz.w; }
inline size_t  index_wrap(const point2i &p, const size2u &sz)   { return _mod(p.x, sz.w) + _mod(p.y, sz.h) * sz.w; }
inline size_t  index(const point2i &p, const size2u &sz)        { return index_clamp(p, sz); }
inline point2i normalize(const point2i &p, const size2u &sz)    { return point2i(int(_mod(p.x, sz.w)), int(_mod(p.y, sz.h))); }

template <typename T>
struct readable_grid
{
    virtual ~readable_grid();
    
    virtual T           get(const point2i &p)      const = 0;
    virtual T           null_value()               const = 0;
    virtual size2u      size()                     const = 0;
    
    inline T            operator()(const point2i &p) const { return get(p); }
};

template <typename T> readable_grid<T>::~readable_grid() {}

template <typename T>
struct grid : public readable_grid<T>
{
    virtual void set(const point2i &p, const T &value) = 0;
};

template <typename T>
struct readable_contiguous_row_grid : public readable_grid<T>
{
    virtual const T *row(size_t y) const = 0;
};

template <typename T>
struct contiguous_row_grid : public grid<T>, public readable_contiguous_row_grid<T>
{
    virtual T *row(size_t y) = 0;
};

template <typename T>
region valid_region(const readable_grid<T> &source, const region &area)
{
    region main(point2i(0, 0), source.size());
    return area.valid() ? main.intersect(area) : main;
}

template <typename SourceT, typename TargetT>
struct null_operator
{
    inline void operator()(const readable_grid<SourceT> &source, const point2i &p, grid<TargetT> &target) const {}
};

template <typename SourceT, typename TargetT>
struct copy_operator
{
    inline void operator()(const readable_grid<SourceT> &source, const point2i &p, grid<TargetT> &target) const { target.set(p, source.get(p)); }
};

template <typename SourceT, typename TargetT, typename Operator>
void transform(const readable_grid<SourceT> &source, grid<TargetT> &target, Operator &fn, region area = region())
{
    region main(point2i(0, 0), source.size());
    main = area.valid() ? main.intersect(area) : main;
    if (!main.empty()) {
        for (point2i p(main.min); p.y <= main.max.y; p.y++) {
            for (p.x = main.min.x; p.x <= main.max.x; p.x++) {
                fn(source, p, target);
            }
        }
    }
}

}
