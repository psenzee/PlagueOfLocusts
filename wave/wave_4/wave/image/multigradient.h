#pragma once

#include <cstddef>
#include <array>

template <typename T>
struct knot
{
    typedef T value_type;
    double at;
    value_type value;
};

template <typename T, size_t N>
struct multigradient : public std::array<knot<T>, N>
{
    typedef multigradient<T, N> self_t;
    
    inline T value_at(double v) const
    {
        const auto &last = self_t::at(self_t::size() - 1);
        if (v >= last.at) {
            // return last value early if we already know it's out of range
            return last.value;
        }
        for (size_t i = 1, sz = self_t::size(); i < sz; i++) {
            const auto &b = self_t::at(i - 1);
            if (v < b.at)
                return b.value;
            const auto &e = self_t::at(i);
            if (v >= b.at && v < e.at) {
                // todo change to hermite/bezier curve
                return lerp(b.value, e.value, (v - b.at) / (e.at - b.at));
            }
        }
        return last.value;
    }
};
