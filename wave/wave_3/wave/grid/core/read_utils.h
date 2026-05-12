#pragma once

#include <iostream>
#include <iomanip>
#include <algorithm>
#include "core_grid.h"

namespace grid {

namespace {
template <typename T, typename U> constexpr T _mod(U value, T div)          { return (value % div + div) % div; } // correctly handle negatives for this purpose
template <typename T>             constexpr T _clamp(T value, T min, T max) { return std::max(std::min(value, max), min); }
}

constexpr size_t  index_clamp(const point2i &p, const size2u &sz)  { return _clamp(p.x, 0, int(sz.w) - 1) + _clamp(p.y, 0, int(sz.h) - 1) * sz.w; }
constexpr size_t  index_wrap(const point2i &p, const size2u &sz)   { return _mod(p.x, sz.w) + _mod(p.y, sz.h) * sz.w; }
constexpr size_t  index(const point2i &p, const size2u &sz)        { return index_clamp(p, sz); }
constexpr point2i normalize(const point2i &p, const size2u &sz)    { return point2i(int(_mod(p.x, sz.w)), int(_mod(p.y, sz.h))); }
constexpr bool    inside(const size2u &s, const point2i &p)        { return (p.x > 0 && p.x < s.w && p.y > 0 && p.y < s.h); }

template <typename T> region valid_region(const readable<T> &source, const region &area = region())
{
    region main(point2i(0, 0), source.size());
    return area.valid() ? main.intersect(area) : main;
}

template <typename T>
region resolve_region(const readable<T> &a, const readable<T> &b, const region &area)
{
    return valid_region(a, area).intersect(valid_region(b, area));
}

template <typename T>
bool equal(const readable<T> &a, const readable<T> &b, const region &area = region())
{
    if (&a == &b) {
        return true;
    }

    auto r = resolve_region(a, b, area);
    if (r.empty()) {
        return false;
    }

    for (point2i p(r.min); p.y <= r.max.y; p.y++) {
        for (p.x = r.min.x; p.x <= r.max.x; p.x++) {
            if (a.get(p) != b.get(p)) {
                return false;
            }
        }
    }
    return true;
}

template <typename T>
size_t count_non_nulls(const readable<T> &grid, const region &area = region())
{
    size_t count = 0;

    auto r = valid_region(grid, area);
    if (r.empty()) {
        return count;
    }
    
    T null_value = grid.null_value();
    for (point2i p(r.min); p.y <= r.max.y; p.y++) {
        for (p.x = r.min.x; p.x <= r.max.x; p.x++) {
            if (grid.get(p) != null_value) {
                count++;
            }
        }
    }
    return count;
}

template <typename T>
region extrema(const readable<T> &grid, const region &area = region())
{
    region w;

    auto r = valid_region(grid, area);
    if (r.empty()) {
        return w;
    }

    for (point2i p(r.min); p.y <= r.max.y; p.y++) {
        for (p.x = r.min.x; p.x <= r.max.x; p.x++) {
            if (grid.get(p) != grid.null_value()) {
                w.insert(p);
            }
        }
    }

    return w;
}

template <typename T>
constexpr bool inside(const readable<T> &grid, const point2i &p)
{
    return inside(grid.size(), p);
}

template <typename T>
point2i find_first_set_point(const readable<T> &grid, const region &area = region())
{
    auto r = valid_region(grid, area);
    if (r.empty()) {
        return point2i(-1, -1);
    }

    T null_value = grid.null_value();
    for (point2i p(r.min); p.y <= r.max.y; p.y++) {
        for (p.x = r.min.x; p.x <= r.max.x; p.x++) {
            if (grid.get(p) != null_value) {
                return p;
            }
        }
    }
    return point2i(-1, -1);
}

template <typename T>
std::ostream &print(std::ostream &os, const readable<T> &grid, const region &area = region())
{
    auto r = valid_region(grid, area);
    if (r.empty()) {
        return os;
    }

    for (point2i p(r.min); p.y <= r.max.y; p.y++) {
        for (p.x = r.min.x; p.x <= r.max.x; p.x++) {
            os << grid.get(p) << " ";
        }
        os << std::endl;
    }
    return os;
}

template <typename T>
std::ostream &print_as_json(std::ostream &os, const readable<T> &grid, int field_width = 3, const region &area = region())
{
    auto r = valid_region(grid, area);
    if (r.empty()) {
        return os;
    }

    for (point2i p(r.min); p.y <= r.max.y; p.y++) {
        os << "    ";
        for (p.x = r.min.x; p.x <= r.max.x; p.x++) {
            os << std::fixed << std::setw(field_width) << std::setfill(' ') << grid.get(p) << ", ";
        }
        os << std::endl;
    }
    return os;
}

template <typename T>
std::array<T, 2> range(const readable<T> &grid, const region &area = region())
{
    std::array<T, 2> ran({ std::numeric_limits<T>::max(), std::numeric_limits<T>::min() });
    auto r = valid_region(grid, area);
    if (r.empty()) {
        return ran;
    }

    for (point2i p(r.min); p.y <= r.max.y; p.y++) {
        for (p.x = r.min.x; p.x <= r.max.x; p.x++) {
            T val = grid.get(p);
            ran = std::array<T, 2>({ std::min(ran[0], val), std::max(ran[1], val) });
        }
    }
    return ran;
}

template <typename T> inline std::ostream &operator<<(std::ostream &os, const readable<T> &grid) { return print(os, grid); }

template <typename T> inline bool operator==(const readable<T> &a, const readable<T> &b) { return equal(a, b); }

}
