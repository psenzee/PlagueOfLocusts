#pragma once

#include <array>

template <typename T, size_t N> std::array<T, 4> wxyz(const std::array<T, N> &a)             { return std::array<T, 4> { a[3], a[1], a[2], a[0] }; }
template <typename T, size_t N> std::array<T, 4> wxzy(const std::array<T, N> &a)             { return std::array<T, 4> { a[3], a[2], a[1], a[0] }; }
template <typename T, size_t N> std::array<T, 4> wyxz(const std::array<T, N> &a)             { return std::array<T, 4> { a[3], a[0], a[2], a[1] }; }
template <typename T, size_t N> std::array<T, 4> wyzx(const std::array<T, N> &a)             { return std::array<T, 4> { a[3], a[2], a[0], a[1] }; }
template <typename T, size_t N> std::array<T, 4> wzxy(const std::array<T, N> &a)             { return std::array<T, 4> { a[3], a[0], a[1], a[2] }; }
template <typename T, size_t N> std::array<T, 4> wzyx(const std::array<T, N> &a)             { return std::array<T, 4> { a[3], a[1], a[0], a[2] }; }
template <typename T, size_t N> std::array<T, 4> xwyz(const std::array<T, N> &a)             { return std::array<T, 4> { a[0], a[3], a[1], a[2] }; }
template <typename T, size_t N> std::array<T, 4> xwzy(const std::array<T, N> &a)             { return std::array<T, 4> { a[0], a[3], a[2], a[1] }; }
template <typename T, size_t N> std::array<T, 4> xywz(const std::array<T, N> &a)             { return std::array<T, 4> { a[0], a[1], a[3], a[2] }; }
template <typename T, size_t N> std::array<T, 4> xyzw(const std::array<T, N> &a)             { return std::array<T, 4> { a[0], a[1], a[2], a[3] }; }
template <typename T, size_t N> std::array<T, 4> xzwy(const std::array<T, N> &a)             { return std::array<T, 4> { a[0], a[2], a[3], a[1] }; }
template <typename T, size_t N> std::array<T, 4> xzyw(const std::array<T, N> &a)             { return std::array<T, 4> { a[0], a[2], a[1], a[3] }; }
template <typename T, size_t N> std::array<T, 4> ywxz(const std::array<T, N> &a)             { return std::array<T, 4> { a[1], a[3], a[0], a[2] }; }
template <typename T, size_t N> std::array<T, 4> ywzx(const std::array<T, N> &a)             { return std::array<T, 4> { a[1], a[3], a[2], a[0] }; }
template <typename T, size_t N> std::array<T, 4> yxwz(const std::array<T, N> &a)             { return std::array<T, 4> { a[1], a[0], a[3], a[2] }; }
template <typename T, size_t N> std::array<T, 4> yxzw(const std::array<T, N> &a)             { return std::array<T, 4> { a[1], a[0], a[2], a[3] }; }
template <typename T, size_t N> std::array<T, 4> yzwx(const std::array<T, N> &a)             { return std::array<T, 4> { a[1], a[2], a[3], a[0] }; }
template <typename T, size_t N> std::array<T, 4> yzxw(const std::array<T, N> &a)             { return std::array<T, 4> { a[1], a[2], a[0], a[3] }; }
template <typename T, size_t N> std::array<T, 4> zwxy(const std::array<T, N> &a)             { return std::array<T, 4> { a[2], a[3], a[0], a[1] }; }
template <typename T, size_t N> std::array<T, 4> zwyx(const std::array<T, N> &a)             { return std::array<T, 4> { a[2], a[3], a[1], a[0] }; }
template <typename T, size_t N> std::array<T, 4> zxwy(const std::array<T, N> &a)             { return std::array<T, 4> { a[2], a[0], a[3], a[1] }; }
template <typename T, size_t N> std::array<T, 4> zxyw(const std::array<T, N> &a)             { return std::array<T, 4> { a[2], a[0], a[1], a[3] }; }
template <typename T, size_t N> std::array<T, 4> zywx(const std::array<T, N> &a)             { return std::array<T, 4> { a[2], a[1], a[3], a[0] }; }
template <typename T, size_t N> std::array<T, 4> zyxw(const std::array<T, N> &a)             { return std::array<T, 4> { a[2], a[1], a[0], a[3] }; }

template <typename T, size_t N> std::array<T, 3> xzy(const std::array<T, N> &a)              { return std::array<T, 3> { a[0], a[2], a[1] }; }
template <typename T, size_t N> std::array<T, 3> zyx(const std::array<T, N> &a)              { return std::array<T, 3> { a[2], a[1], a[0] }; }
template <typename T, size_t N> std::array<T, 3> yxz(const std::array<T, N> &a)              { return std::array<T, 3> { a[1], a[0], a[2] }; }
template <typename T, size_t N> std::array<T, 3> xyz(const std::array<T, N> &a)              { return std::array<T, 3> { a[0], a[1], a[2] }; }
template <typename T, size_t N> std::array<T, 3> yzx(const std::array<T, N> &a)              { return std::array<T, 3> { a[1], a[2], a[0] }; }
template <typename T, size_t N> std::array<T, 3> zxy(const std::array<T, N> &a)              { return std::array<T, 3> { a[2], a[0], a[1] }; }

template <typename T, size_t N> std::array<T, 2> xz(const std::array<T, N> &a)               { return std::array<T, 2> { a[0], a[2] }; }
template <typename T, size_t N> std::array<T, 2> zy(const std::array<T, N> &a)               { return std::array<T, 2> { a[2], a[1] }; }
template <typename T, size_t N> std::array<T, 2> yx(const std::array<T, N> &a)               { return std::array<T, 2> { a[1], a[0] }; }
template <typename T, size_t N> std::array<T, 2> xy(const std::array<T, N> &a)               { return std::array<T, 2> { a[0], a[1] }; }
template <typename T, size_t N> std::array<T, 2> yz(const std::array<T, N> &a)               { return std::array<T, 2> { a[1], a[2] }; }
template <typename T, size_t N> std::array<T, 2> zx(const std::array<T, N> &a)               { return std::array<T, 2> { a[2], a[0] }; }

template <typename T, size_t N> T                x(const std::array<T, N> &a)                { return a[0]; }
template <typename T, size_t N> T                y(const std::array<T, N> &a)                { return a[1]; }
template <typename T, size_t N> T                z(const std::array<T, N> &a)                { return a[2]; }
template <typename T, size_t N> T                w(const std::array<T, N> &a)                { return a[3]; }
