#pragma once

#include <array>
#include <vector>
#include <utility>

template<typename T, typename... Args> constexpr bool any_of(T v, Args... args) { return ((v == args) || ...); }

template <typename T, typename U, std::size_t N>
constexpr bool any_of(T v, const std::array<U, N> &a) {
    return std::apply([&](const auto&... elems) { return ((v == elems) || ...); }, a);
}

template <typename T, typename U>
constexpr bool any_of(T v, const std::vector<U> &a) {
    for (const auto &u : a) {
        if (u == v) {
            return true;
        }
    }
    return false;
}
