#pragma once

#include <iostream>
#include <iomanip>
#include "core_grid.h"
#include "read_utils.h"

namespace grid {

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

    for (point2i p(r.min); p[1] <= r.max[1]; p[1]++) {
        for (p[0] = r.min[0]; p[0] <= r.max[0]; p[0]++) {
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

    for (point2i p(r.min); p[1] <= r.max[1]; p[1]++) {
        for (p[0] = r.min[0]; p[0] <= r.max[0]; p[0]++) {
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

    for (point2i p(r.min); p[1] <= r.max[1]; p[1]++) {
        for (p[0] = r.min[0]; p[0] <= r.max[0]; p[0]++) {
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

    for (point2i p(r.min); p[1] <= r.max[1]; p[1]++) {
        for (p[0] = r.min[0]; p[0] <= r.max[0]; p[0]++) {
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

    for (point2i p(r.min); p[1] <= r.max[1]; p[1]++) {
        for (p[0] = r.min[0]; p[0] <= r.max[0]; p[0]++) {
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

    for (point2i p(r.min); p[1] <= r.max[1]; p[1]++) {
        for (p[0] = r.min[0]; p[0] <= r.max[0]; p[0]++) {
            grid.set(p, clamp(grid.get(p), min, max));
        }
    }
}

}
