#pragma once

#include <iostream>
#include <iomanip>
#include "grid.h"
#include "size2u.h"

namespace wave {

template <typename T>
bool copy(const readable_grid<T> &a, grid<T> &b)
{
    if (&a == &b) {
        return true;
    }
    if (a.size() == b.size()) {
        size2u sz(a.size());
        for (point2i p(0, 0); p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                b.set(p, a.get(p));
            }
        }
        return true;
    }
    return false;
}

template <typename T>
bool equal(const readable_grid<T> &a, const readable_grid<T> &b)
{
    if (&a == &b) {
        return true;
    }
    if (a.size() != b.size()) {
        return false;
    }
    size2u sz(a.size());
    for (point2i p(0, 0); p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++) {
            if (a.get(p) != b.get(p)) {
                return false;
            }
        }
    }
    return true;
}

template <typename T>
size_t count_non_nulls(const readable_grid<T> &a, region area = region())
{
    size2u s(a.size());
    point2i b(0, 0), e(int(s.w), int(s.h));
    if (area.valid()) {
        b = area.min;
        e = point2i(area.max + point2i(1, 1));
    }
    size_t count = 0;
    T null_value = a.null_value();
    for (point2i q(b); q.y < e.y; q.y++) {
        for (q.x = b.x; q.x < e.x; q.x++) {
            if (a.get(q) != null_value) {
                count++;
            }
        }
    }
    return count;
}

template <typename T>
void extrema(region &w, const readable_grid<T> &grid, region area = region())
{
    size2u s(grid.size());
    point2i b(0, 0), e(int(s.w), int(s.h));
    if (area.valid()) {
        b = area.min;
        e = point2i(area.max + point2i(1, 1));
    }
    w.reset();
    for (point2i p(b); p.y < e.y; p.y++) {
        for (p.x = b.x; p.x < e.x; p.x++) {
            if (grid.get(p) != grid.null_value()) {
                w.insert(p);
            }
        }
    }
}

template <typename T>
inline bool inside(const readable_grid<T> &grid, const point2i &p)
{
    size2u s(grid.size());
    return (p.x > 0 && p.x < s.w && p.y > 0 && p.y < s.h);
}

template <typename T>
inline std::array<T, 9> window_3x3(const readable_grid<T> &a, const point2i &p)
{
    return std::array<T, 9>({
        a.get(p + point2i(-1, -1)), a.get(p + point2i( 0, -1)), a.get(p + point2i( 1, -1)),
        a.get(p + point2i(-1,  0)), a.get(p + point2i( 0,  0)), a.get(p + point2i( 1,  0)),
        a.get(p + point2i(-1,  1)), a.get(p + point2i( 0,  1)), a.get(p + point2i( 1,  1)),
    });
}

template <typename T>
inline std::array<T, 2> vector_3x3(const std::array<T, 9> &a)
{
    T dy = (a[0] - a[3]) + (a[3] - a[6])
         + (a[1] - a[4]) + (a[4] - a[7])
         + (a[2] - a[5]) + (a[5] - a[8])
         + (a[0] - a[4]) + (a[4] - a[8]) // diagonal
         + (a[2] - a[4]) + (a[4] - a[6]);
    T dx = (a[0] - a[1]) + (a[1] - a[2])
         + (a[3] - a[4]) + (a[4] - a[5])
         + (a[6] - a[7]) + (a[7] - a[8])
         + (a[0] - a[4]) + (a[4] - a[8]) // diagonal
         + (a[6] - a[4]) + (a[4] - a[2]);
    return std::array<T, 2>({ dx, dy });
}

template <typename T>
inline std::array<T, 2> vector_3x3(const readable_grid<T> &grid, const point2i &p)
{
    return vector_3x3(window_3x3(grid, p));
}

template <typename T>
point2i find_first_set_point(const readable_grid<T> &a, region area = region())
{
    size2u s(a.size());
    point2i b(0, 0), e(int(s.w), int(s.h));
    if (area.valid()) {
        b = area.min;
        e = point2i(area.max + point2i(1, 1));
    }
    T null_value = a.null_value();
    for (point2i q(b); q.y < e.y; q.y++) {
        for (q.x = b.x; q.x < e.x; q.x++) {
            if (a.get(q) != null_value) {
                return q;
            }
        }
    }
    return point2i(-1, -1);
}

template <typename T>
inline unsigned direction(const readable_grid<T> &grid, const point2i &p)
{
    static const unsigned map[] = {
        point2i::CENTER,    // 0
        point2i::WEST,      // 1
        point2i::EAST,      // 2
        point2i::CENTER,    // 3 // both east and west?
        
        point2i::NORTH,     // 4
        point2i::NORTHWEST, // 5
        point2i::NORTHEAST, // 6
        point2i::NORTH,     // 7 // north + both east and west?
        
        point2i::SOUTH,     // 8
        point2i::SOUTHWEST, // 9
        point2i::SOUTHEAST, // 10
        point2i::SOUTH,     // 11 // south + both east and west?
        
        point2i::CENTER,    // 12 // both north and south
        point2i::WEST,      // 13 // west + both north and south
        point2i::EAST,      // 14 // east + both north and south
        point2i::CENTER,    // 15 // north + south + east + west
    };
    auto v = vector(grid, p);
    return map[(v[0] < 0 ? 1 : 0) | (v[0] > 0 ? 2 : 0) | (v[1] < 0 ? 4 : 0) | (v[1] > 0 ? 8 : 0)];
}

template <typename T>
std::ostream &print(std::ostream &os, const readable_grid<T> &grid)
{
    size2u sz(grid.size());
    for (point2i p(0, 0); p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++)
            os << grid.get(p) << " ";
        os << std::endl;
    }
    return os;
}

template <typename T> inline std::ostream &operator<<(std::ostream &os, const readable_grid<T> &grid) { return print(os, grid); }

template <typename T>
void clear(grid<T> &grid, const T &value)
{
    point2i p(0, 0);
    size2u sz(grid.size());
    for (; p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++) {
            grid.set(p, value);
        }
    }
}

template <typename T>
std::ostream &print_as_json(std::ostream &os, const readable_grid<T> &grid, int field_width = 3)
{
    size2u sz(grid.size());
    for (point2i p(0, 0); p.y < sz.h; p.y++) {
        os << "    ";
        for (p.x = 0; p.x < sz.w; p.x++)
            os << std::fixed << std::setw(field_width) << std::setfill(' ') << grid.get(p) << ", ";
        os << std::endl;
    }
    return os;
}

template <typename T, typename Functor>
void fill(grid<T> &grid, const Functor &fn)
{
    size2u sz(grid.size());
    for (point2i p(0, 0); p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++) {
            grid.set(p, fn(grid, p));
        }
    }
}

template <typename SourceT, typename TargetT, typename Functor>
void transform(const readable_grid<SourceT> &source, grid<TargetT> &target, const Functor &fn)
{
    size2u sz(source.size());
    for (point2i p(0, 0); p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++) {
            target.set(p, fn(source, p));
        }
    }
}

template <typename T>
std::array<T, 2> range(const readable_grid<T> &grid)
{
    std::array<T, 2> r({ std::numeric_limits<T>::max(), std::numeric_limits<T>::min() });
    size2u sz(grid.size());
    for (point2i p(0, 0); p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++) {
            T val = grid.get(p);
            r = std::array<T, 2>({ std::min(r[0], val), std::max(r[1], val) });
        }
    }
    return r;
}

template <typename FloatT>
void normalize(grid<FloatT> &grid, FloatT min = FloatT(0), FloatT max = FloatT(1))
{
    auto r = range(grid);
    auto dist = r[1] - r[0];
    auto target_r = max - min;
    size2u sz(grid.size());
    for (point2i p(0, 0); p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++) {
            grid.set(p, ((grid.get(p) - r[0]) / dist) * target_r + min);
        }
    }
}

template <typename FloatT>
void clamp_grid(grid<FloatT> &grid, FloatT min = FloatT(0), FloatT max = FloatT(1))
{
    size2u sz(grid.size());
    for (point2i p(0, 0); p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++) {
            grid.set(p, clamp(grid.get(p), min, max));
        }
    }
}

template <typename SourceT, typename TargetT>
struct null_transform_element
{
    typedef readable_grid<SourceT> source_grid_t;
    inline TargetT operator()(const source_grid_t &grid, const point2i &p) { return TargetT(grid.get(p)); }
};

}
