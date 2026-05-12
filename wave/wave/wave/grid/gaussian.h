#pragma once

#include <iostream>
#include <iomanip>

#include "grid.h"
#include "base_grid.h"

namespace wave {

template <typename T>
constexpr std::array<T, 9> gaussian_kernel_3x3_integer()
{
    return std::array<T, 9>({
        1, 2, 1,
        2, 4, 2,
        1, 2, 1,
    });
}

template <typename T>
constexpr std::array<T, 9> gaussian_kernel_3x3_float()
{
    constexpr double d = 16.;
    return std::array<T, 9>({
        1 / d, 2 / d, 1 / d,
        2 / d, 4 / d, 2 / d,
        1 / d, 2 / d, 1 / d,
    });
}

template <typename T>
constexpr std::array<T, 25> gaussian_kernel_5x5_float()
{
    constexpr double d = 273.;
    return std::array<T, 25>({
        1 / d,  4 / d,  7 / d,  4 / d, 1 / d,
        4 / d, 16 / d, 26 / d, 16 / d, 4 / d,
        7 / d, 26 / d, 41 / d, 26 / d, 7 / d,
        4 / d, 16 / d, 26 / d, 16 / d, 4 / d,
        1 / d,  4 / d,  7 / d,  4 / d, 1 / d,
    });
}

template <typename T>
inline T gaussian_smooth_3x3_point(const readable_grid<T> &grid, const point2i &p)
{
    static constexpr auto k = gaussian_kernel_3x3_float<T>();
    static constexpr auto a(point2i::kernel_offsets<3, 3>());
    T sum = 0;
    size_t i = 0;
    for (const auto &q : a)
        sum += grid.get(p + q) * k[i++];
    return sum;
}

template <typename T>
inline T gaussian_smooth_5x5_point(const readable_grid<T> &grid, const point2i &p)
{
    static constexpr auto k = gaussian_kernel_5x5_float<T>();
    static constexpr auto a(point2i::kernel_offsets<5, 5>());
    T sum = 0;
    size_t i = 0;
    for (const auto &q : a)
        sum += grid.get(p + q) * k[i++];
    return sum;
}

template <typename T>
void gaussian_smooth_3x3(const readable_grid<T> &from, grid<T> &into, region area = region())
{
    area = valid_region(from, area);
    point2i b(area.min), e(area.max);
    for (point2i p(b); p.y < e.y; p.y++) {
        for (p.x = b.x; p.x < e.x; p.x++) {
            into.set(point2i(p.x - b.x, p.y - b.y), gaussian_smooth_3x3_point(from, p));
        }
    }
}

template <typename T>
void gaussian_smooth_5x5(const readable_grid<T> &from, grid<T> &into, region area = region())
{
    area = valid_region(from, area);
    point2i b(area.min), e(area.max);
    for (point2i p(b); p.y < e.y; p.y++) {
        for (p.x = b.x; p.x < e.x; p.x++) {
            into.set(point2i(p.x - b.x, p.y - b.y), gaussian_smooth_5x5_point(from, p));
        }
    }
}

template <typename T>
void copy_region_small_to_large(const readable_grid<T> &small, grid<T> &large, const point2i &at)
{
    size2u sz(small.size());
    for (point2i p(0, 0); p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++) {
            large.set(point2i(p.x + at.x, p.y + at.y), small.get(p));
        }
    }
}

// inefficient -- could use optimizations, ie, rolling scanlines, row interface
template <typename T>
void gaussian_smooth_3x3(grid<T> &grid, region area = region())
{
    area = valid_region(grid, area);
    base_grid<T> out(area.size());
    gaussian_smooth_3x3(grid, out, area);
    copy_region_small_to_large(out, grid, area.min);
}

template <typename T>
void gaussian_smooth_5x5(grid<T> &grid, region area = region())
{
    area = valid_region(grid, area);
    base_grid<T> out(area.size());
    gaussian_smooth_5x5(grid, out, area);
    copy_region_small_to_large(out, grid, area.min);
}

}
