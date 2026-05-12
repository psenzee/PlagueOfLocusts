#pragma once

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <array>

typedef std::array<std::size_t, 2> size2u_t;

struct size2u
{
    std::size_t w, h;
    
    constexpr explicit size2u(std::size_t w, std::size_t h) : w(w), h(h) {}
    constexpr size2u(const size2u &s) : w(s.w), h(s.h) {}
    template <typename T> constexpr size2u(const std::array<T, 2> &s) : w(std::size_t(s[0])), h(std::size_t(s[1])) {}
    
    constexpr size2u &operator=(const size2u &s)                                        { if (this != &s) { w = s.w; h = s.h; } return *this; }
    
    template <typename T> constexpr size2u &operator= (const std::array<T, 2> &s)       { set(s[0], s[1]); return *this; }
    template <typename T> constexpr size2u &operator+=(const std::array<T, 2> &s)       { set(w + s[0], h + s[1]); return *this; }
    template <typename T> constexpr size2u &operator-=(const std::array<T, 2> &s)       { set(w - s[0], h - s[1]); return *this; }
    template <typename T> constexpr size2u &operator*=(const std::array<T, 2> &s)       { set(w * s[0], h * s[1]); return *this; }
    template <typename T> constexpr size2u &operator/=(const std::array<T, 2> &s)       { set(w / s[0], h / s[1]); return *this; }
    template <typename T> constexpr size2u &operator%=(const std::array<T, 2> &s)       { set(w % s[0], h % s[1]); return *this; }

    constexpr size2u operator+(const size2u &s)                                   const { return size2u(w + s.w, h + s.h); }
    constexpr size2u operator-(const size2u &s)                                   const { return size2u(w - s.w, h - s.h); }
    constexpr size2u operator*(const size2u &s)                                   const { return size2u(w * s.w, h * s.h); }
    constexpr size2u operator/(const size2u &s)                                   const { return size2u(w / s.w, h / s.h); }
    constexpr size2u operator%(const size2u &s)                                   const { return size2u(w % s.w, h % s.h); }
    
    template <typename T> constexpr size2u operator+(const std::array<T, 2> &s)   const { size2u u(*this); return u += s; }
    template <typename T> constexpr size2u operator-(const std::array<T, 2> &s)   const { size2u u(*this); return u -= s; }
    template <typename T> constexpr size2u operator*(const std::array<T, 2> &s)   const { size2u u(*this); return u *= s; }
    template <typename T> constexpr size2u operator/(const std::array<T, 2> &s)   const { size2u u(*this); return u /= s; }
    template <typename T> constexpr size2u operator%(const std::array<T, 2> &s)   const { size2u u(*this); return u %= s; }

    constexpr size2u operator+(int s)                                             const { size2u u(*this); return u.set(w + s, h + s); }
    constexpr size2u operator-(int s)                                             const { size2u u(*this); return u.set(w - s, h - s); }
    constexpr size2u operator*(int s)                                             const { size2u u(*this); return u.set(w * s, h * s); }
    constexpr size2u operator/(int s)                                             const { size2u u(*this); return u.set(w / s, h / s); }
    constexpr size2u operator%(int s)                                             const { size2u u(*this); return u.set(w % s, h % s); }
    
    constexpr bool operator==(const size2u &s)                                    const { return w == s.w && h == s.h; }
    constexpr bool operator< (const size2u &s)                                    const { return w == s.w ? h < s.h : w < s.w; }
    constexpr bool operator> (const size2u &s)                                    const { return w == s.w ? h > s.h : w > s.w; }
    constexpr bool operator<=(const size2u &s)                                    const { return !(*this > s); }
    constexpr bool operator>=(const size2u &s)                                    const { return !(*this < s); }
    
    constexpr size2u mins(const size2u &s)                                        const { return size2u(std::min(w, s.w), std::min(h, s.h)); }
    constexpr size2u maxs(const size2u &s)                                        const { return size2u(std::max(w, s.w), std::max(h, s.h)); }
    
    constexpr std::size_t area()                                                  const { return w * h; }

    template <typename T> constexpr std::array<T, 2> array()                      const { return std::array<T, 2>{ T(w), T(h) }; }
    
    template <typename T> constexpr size2u &set_w(T v)                                  { w = std::size_t(std::max(v, T(0))); return *this; }
    template <typename T> constexpr size2u &set_h(T v)                                  { h = std::size_t(std::max(v, T(0))); return *this; }
    template <typename T> constexpr size2u &set(T u, T v)                               { set_w(u); set_h(v); return *this; }
};

constexpr std::ostream &operator<<(std::ostream &os, const size2u &s) { return os << "(w:" << s.w << " h:" << s.h << ")"; }

namespace std {
    template <>
    struct hash<size2u> {
        std::size_t operator()(const size2u& s) const {
            return s.w ^ (s.h << 32);
        }
    };
}
