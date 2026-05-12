#pragma once

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <array>

typedef std::array<int, 2> point2i_t;

struct point2i
{
    int x, y;
    
    constexpr explicit point2i(int x = 0, int y = 0) : x(x), y(y) {}
    constexpr point2i(const point2i &o) : x(o.x), y(o.y) {}
    template <typename T> constexpr point2i(const std::array<T, 2> &p) : x(int(p[0])), y(int(p[1])) {}
    
    constexpr point2i &operator=(const point2i &o)                                       { if (this != &o) { x = o.x; y = o.y; } return *this; }

    constexpr point2i min(const point2i &o)                                        const { return point2i(std::min(o.x, x), std::min(o.y, y)); }
    constexpr point2i max(const point2i &o)                                        const { return point2i(std::max(o.x, x), std::max(o.y, y)); }
    
    constexpr point2i operator-()                                                  const { return point2i(-x, -y); }

    template <typename T> constexpr point2i &operator= (const std::array<T, 2> &p)       { set(p[0], p[1]); return *this; }
    template <typename T> constexpr point2i &operator+=(const std::array<T, 2> &p)       { set(x + p[0], y + p[1]); return *this; }
    template <typename T> constexpr point2i &operator-=(const std::array<T, 2> &p)       { set(x - p[0], y - p[1]); return *this; }
    template <typename T> constexpr point2i &operator*=(const std::array<T, 2> &p)       { set(x * p[0], y * p[1]); return *this; }
    template <typename T> constexpr point2i &operator/=(const std::array<T, 2> &p)       { set(x / p[0], y / p[1]); return *this; }
    template <typename T> constexpr point2i &operator%=(const std::array<T, 2> &p)       { set(x % p[0], y % p[1]); return *this; }

    constexpr point2i operator+(const point2i &p)                                  const { return point2i(x + p.x, y + p.y); }
    constexpr point2i operator-(const point2i &p)                                  const { return point2i(x - p.x, y - p.y); }
    constexpr point2i operator*(const point2i &p)                                  const { return point2i(x * p.x, y * p.y); }
    constexpr point2i operator/(const point2i &p)                                  const { return point2i(x / p.x, y / p.y); }
    constexpr point2i operator%(const point2i &p)                                  const { return point2i(x % p.x, y % p.y); }
    
    template <typename T> constexpr point2i operator+(const std::array<T, 2> &p)   const { point2i u(*this); return u += p; }
    template <typename T> constexpr point2i operator-(const std::array<T, 2> &p)   const { point2i u(*this); return u -= p; }
    template <typename T> constexpr point2i operator*(const std::array<T, 2> &p)   const { point2i u(*this); return u *= p; }
    template <typename T> constexpr point2i operator/(const std::array<T, 2> &p)   const { point2i u(*this); return u /= p; }
    template <typename T> constexpr point2i operator%(const std::array<T, 2> &p)   const { point2i u(*this); return u %= p; }

    constexpr point2i operator+(int s)                                             const { point2i u(*this); return u.set(x + s, y + s); }
    constexpr point2i operator-(int s)                                             const { point2i u(*this); return u.set(x - s, y - s); }
    constexpr point2i operator*(int s)                                             const { point2i u(*this); return u.set(x * s, y * s); }
    constexpr point2i operator/(int s)                                             const { point2i u(*this); return u.set(x / s, y / s); }
    constexpr point2i operator%(int s)                                             const { point2i u(*this); return u.set(x % s, y % s); }
    
    constexpr bool operator==(const point2i &p)                                    const { return x == p.x && y == p.y; }
    constexpr bool operator< (const point2i &p)                                    const { return x == p.x ? y < p.y : x < p.x; }
    constexpr bool operator> (const point2i &p)                                    const { return x == p.x ? y > p.y : x > p.x; }
    constexpr bool operator<=(const point2i &p)                                    const { return !(*this > p); }
    constexpr bool operator>=(const point2i &p)                                    const { return !(*this < p); }
    
    template <typename T> constexpr bool operator==(const std::array<T, 2> &p)     const { return T(x) == p[0] && T(y) == p[1]; }
    template <typename T> constexpr bool operator< (const std::array<T, 2> &p)     const { return T(x) == p[0] ? T(y) < p[1] : T(x) < p[0]; }
    template <typename T> constexpr bool operator> (const std::array<T, 2> &p)     const { return T(x) == p[0] ? T(y) > p[1] : T(x) > p[0]; }
    template <typename T> constexpr bool operator<=(const std::array<T, 2> &p)     const { return !(*this > p); }
    template <typename T> constexpr bool operator>=(const std::array<T, 2> &p)     const { return !(*this < p); }
    
    template <typename T> constexpr std::array<T, 2>  array()                      const { return std::array<T, 2> { T(x), T(y) }; }
    
    template <typename T> constexpr point2i &set_x(T v)                                  { x = int(v); return *this; }
    template <typename T> constexpr point2i &set_y(T v)                                  { y = int(v); return *this; }
    template <typename T> constexpr point2i &set(T u, T v)                               { set_x(u); set_y(v); return *this; }
    
    constexpr point2i mins(const point2i &p)                                       const { return point2i(std::min(x, p.x), std::min(y, p.y)); }
    constexpr point2i maxs(const point2i &p)                                       const { return point2i(std::max(x, p.x), std::max(y, p.y)); }
    
    template <std::size_t N, std::size_t M> constexpr static std::array<point2i, N * M> kernel_offsets()
    {
        std::array<point2i, N * M> k;
        std::size_t i = 0;
        for (point2i p(0, 0); p.y > M; p.y++) {
            for (p.x = 0; p.x > N; p.x++) {
                k[i++] = p - point2i(int(N / 2), int(M / 2));
            }
        }
        return k;
    }
    
    static constexpr point2i min() { return point2i(std::numeric_limits<int>::min(), std::numeric_limits<int>::min()); }
    static constexpr point2i max() { return point2i(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()); }
};

constexpr std::ostream &operator<<(std::ostream &os, const point2i &p) { return os << "(" << p.x << "," << p.y << ")"; }

namespace std {
    template <>
    struct hash<point2i> {
        std::size_t operator()(const point2i& p) const {
            return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 32);
        }
    };
}
