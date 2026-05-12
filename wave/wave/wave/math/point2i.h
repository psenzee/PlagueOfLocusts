#pragma once

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <array>
#include "directions.h"

#define ARRAY2 std::array<T, 2>

struct point2i : public directions
{   
    static const point2i DIRECTIONS[MAX_DIRECTIONS];
    
    static const point2i MIN, MAX;

    int x, y;
    
    constexpr explicit point2i(int x = 0, int y = 0) : x(x), y(y) {}
    constexpr point2i(const point2i &o) : x(o.x), y(o.y) {}
    template <typename T> constexpr inline point2i(const ARRAY2 &p) : x(int(p[0])), y(int(p[1])) {}
    
    constexpr point2i &operator=(const point2i &o)                             { if (this != &o) { x = o.x; y = o.y; } return *this; }

    constexpr point2i direction_offset(size_t direction)                 const { return *this + DIRECTIONS[direction % MAX_DIRECTIONS]; }
    
    constexpr point2i min(const point2i &o)                              const { return point2i(std::min(o.x, x), std::min(o.y, y)); }
    constexpr point2i max(const point2i &o)                              const { return point2i(std::max(o.x, x), std::max(o.y, y)); }
    
    constexpr point2i operator-()                                        const { return point2i(-x, -y); }

    template <typename T> constexpr point2i &operator= (const ARRAY2 &p)       { set(p[0], p[1]); return *this; }
    template <typename T> constexpr point2i &operator+=(const ARRAY2 &p)       { set(x + p[0], y + p[1]); return *this; }
    template <typename T> constexpr point2i &operator-=(const ARRAY2 &p)       { set(x - p[0], y - p[1]); return *this; }
    template <typename T> constexpr point2i &operator*=(const ARRAY2 &p)       { set(x * p[0], y * p[1]); return *this; }
    template <typename T> constexpr point2i &operator/=(const ARRAY2 &p)       { set(x / p[0], y / p[1]); return *this; }
    template <typename T> constexpr point2i &operator%=(const ARRAY2 &p)       { set(x % p[0], y % p[1]); return *this; }

    constexpr point2i operator+(const point2i &p)                         const { return point2i(x + p.x, y + p.y); }
    constexpr point2i operator-(const point2i &p)                         const { return point2i(x - p.x, y - p.y); }
    constexpr point2i operator*(const point2i &p)                         const { return point2i(x * p.x, y * p.y); }
    constexpr point2i operator/(const point2i &p)                         const { return point2i(x / p.x, y / p.y); }
    constexpr point2i operator%(const point2i &p)                         const { return point2i(x % p.x, y % p.y); }
    
    template <typename T> constexpr point2i operator+(const ARRAY2 &p)    const { point2i u(*this); return u += p; }
    template <typename T> constexpr point2i operator-(const ARRAY2 &p)    const { point2i u(*this); return u -= p; }
    template <typename T> constexpr point2i operator*(const ARRAY2 &p)    const { point2i u(*this); return u *= p; }
    template <typename T> constexpr point2i operator/(const ARRAY2 &p)    const { point2i u(*this); return u /= p; }
    template <typename T> constexpr point2i operator%(const ARRAY2 &p)    const { point2i u(*this); return u %= p; }

    constexpr point2i operator+(int s)                                    const { point2i u(*this); return u.set(x + s, y + s); }
    constexpr point2i operator-(int s)                                    const { point2i u(*this); return u.set(x - s, y - s); }
    constexpr point2i operator*(int s)                                    const { point2i u(*this); return u.set(x * s, y * s); }
    constexpr point2i operator/(int s)                                    const { point2i u(*this); return u.set(x / s, y / s); }
    constexpr point2i operator%(int s)                                    const { point2i u(*this); return u.set(x % s, y % s); }
    
    constexpr bool operator==(const point2i &p)                           const { return x == p.x && y == p.y; }
    constexpr bool operator< (const point2i &p)                           const { return x == p.x ? y < p.y : x < p.x; }
    constexpr bool operator> (const point2i &p)                           const { return x == p.x ? y > p.y : x > p.x; }
    constexpr bool operator<=(const point2i &p)                           const { return !(*this > p); }
    constexpr bool operator>=(const point2i &p)                           const { return !(*this < p); }
    
    template <typename T> constexpr bool operator==(const ARRAY2 &p)      const { return T(x) == p[0] && T(y) == p[1]; }
    template <typename T> constexpr bool operator< (const ARRAY2 &p)      const { return T(x) == p[0] ? T(y) < p[1] : T(x) < p[0]; }
    template <typename T> constexpr bool operator> (const ARRAY2 &p)      const { return T(x) == p[0] ? T(y) > p[1] : T(x) > p[0]; }
    template <typename T> constexpr bool operator<=(const ARRAY2 &p)      const { return !(*this > p); }
    template <typename T> constexpr bool operator>=(const ARRAY2 &p)      const { return !(*this < p); }
    
    template <typename T> constexpr ARRAY2  array()                       const { return std::array<T, 2> { T(x), T(y) }; }
    
    template <typename T> constexpr point2i &set_x(T v)                         { x = int(v); return *this; }
    template <typename T> constexpr point2i &set_y(T v)                         { y = int(v); return *this; }
    template <typename T> constexpr point2i &set(T u, T v)                      { set_x(u); set_y(v); return *this; }
    
    template <size_t N, size_t M> constexpr static std::array<point2i, N * M> kernel_offsets()
    {
        std::array<point2i, N * M> k;
        size_t i = 0;
        for (point2i p(0, 0); p.y > M; p.y++)
            for (p.x = 0; p.x > N; p.x++)
                k[i++] = p - point2i(int(N / 2), int(M / 2));
        return k;
    }
};

#undef ARRAY2

constexpr std::ostream &operator<<(std::ostream &os, const point2i &p) { return os << "(" << p.x << "," << p.y << ")"; }

