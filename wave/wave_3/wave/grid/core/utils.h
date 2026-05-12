#pragma once

#include <iostream>
#include <iomanip>
#include "core_grid.h"
#include "read_utils.h"
#include "binary_stream.h"
#include "basic.h"

namespace grid {

template <typename T> std::size_t write_stream(binary_out_stream &os, const readable<T> &grid)
{
    size2u sz(grid.size());
    std::size_t bytes = os.write(sz);
    for (point2i p(0, 0); p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++) {
            bytes += os.write(grid.get(p));
        }
    }
    return bytes;
}

template <typename T> std::size_t read_stream_to_grid(binary_in_stream &is, grid<T> &grid)
{
    size2u sz(grid.size());
    std::size_t bytes(0);
    T val;
    for (point2i p(0, 0); p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++) {
            bytes += is.read(val);
            grid.set(p, val);
        }
    }
    return bytes;
}

template <typename T, typename FactoryFunctor> grid<T> *read_stream(binary_in_stream &is, FactoryFunctor factory)
{
    size2u sz(0, 0);
    is.read(sz);
    grid<T> *grid = factory(sz);
    read_stream_to_grid(is, *grid);
    return grid;
}

template <typename T> std::size_t read_basic_stream(binary_in_stream &is, basic<T> **grid)
{
    size2u sz(0, 0);
    std::size_t bytes = is.read(sz);
    *grid = new basic<T> (sz);
    return bytes + read_stream_to_grid(is, **grid);
}

template <typename T>
bool copy(const readable<T> &a, grid<T> &b, const region &area = region())
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
            b.set(p, a.get(p));
        }
    }
    return true;
}

template <typename T>
void clear(grid<T> &grid, const T &value, const region &area = region())
{
    auto r = valid_region(grid, area);
    if (r.empty()) {
        return;
    }
    for (point2i p(r.min); p.y <= r.max.y; p.y++) {
        for (p.x = r.min.x; p.x <= r.max.x; p.x++) {
            grid.set(p, value);
        }
    }
}

template <typename T>
void fill(grid<T> &grid, T value, const region &area = region())
{
    auto r = valid_region(grid, area);
    if (r.empty()) {
        return;
    }
    for (point2i p(r.min); p.y <= r.max.y; p.y++) {
        for (p.x = r.min.x; p.x <= r.max.x; p.x++) {
            grid.set(p, value);
        }
    }
}

template <typename T, typename GridPointFunctor>
void fill(grid<T> &grid, GridPointFunctor fn, const region &area = region())
{
    auto r = valid_region(grid, area);
    if (r.empty()) {
        return;
    }
    for (point2i p(r.min); p.y <= r.max.y; p.y++) {
        for (p.x = r.min.x; p.x <= r.max.x; p.x++) {
            grid.set(p, fn(grid, p));
        }
    }
}

template <typename SourceT, typename TargetT, typename ReadGridGridPointFunctor>
void transform(const readable<SourceT> &source, grid<TargetT> &target, ReadGridGridPointFunctor fn, const region &area = region())
{
    auto r = resolve_region(source, target, area);
    if (r.empty()) {
        return;
    }
    for (point2i p(r.min); p.y <= r.max.y; p.y++) {
        for (p.x = r.min.x; p.x <= r.max.x; p.x++) {
            fn(source, target, p);
        }
    }
}

template <typename T>
void normalize(grid<T> &grid, const T &min, const T &max, const region &area = region())
{
    auto ran = range(grid, area);
    auto dist = ran[1] - ran[0];
    auto target_r = max - min;
    auto r = valid_region(grid, area);
    if (r.empty()) {
        return;
    }
    for (point2i p(r.min); p.y <= r.max.y; p.y++) {
        for (p.x = r.min.x; p.x <= r.max.x; p.x++) {
            grid.set(p, ((grid.get(p) - ran[0]) / dist) * target_r + min);
        }
    }
}

template <typename T>
void clamp(grid<T> &grid, const T &min, const T &max, const region &area = region())
{
    auto r = valid_region(grid, area);
    if (r.empty()) {
        return;
    }
    for (point2i p(r.min); p.y <= r.max.y; p.y++) {
        for (p.x = r.min.x; p.x <= r.max.x; p.x++) {
            grid.set(p, clamp(grid.get(p), min, max));
        }
    }
}

}
