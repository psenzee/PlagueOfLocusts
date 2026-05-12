#pragma once

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <array>

#define ARRAY2 std::array<T, 2>

struct size2u
{
    size_t w, h;
    
    constexpr explicit size2u(size_t w, size_t h) : w(w), h(h) {}
    constexpr size2u(const size2u &s) : w(s.w), h(s.h) {}
    template <typename T> constexpr size2u(const ARRAY2 &s) : w(size_t(s[0])), h(size_t(s[1])) {}
    
    constexpr size2u &operator=(const size2u &s)                              { if (this != &s) { w = s.w; h = s.h; } return *this; }
    
    template <typename T> constexpr size2u &operator= (const ARRAY2 &s)       { set(s[0], s[1]); return *this; }
    template <typename T> constexpr size2u &operator+=(const ARRAY2 &s)       { set(w + s[0], h + s[1]); return *this; }
    template <typename T> constexpr size2u &operator-=(const ARRAY2 &s)       { set(w - s[0], h - s[1]); return *this; }
    template <typename T> constexpr size2u &operator*=(const ARRAY2 &s)       { set(w * s[0], h * s[1]); return *this; }
    template <typename T> constexpr size2u &operator/=(const ARRAY2 &s)       { set(w / s[0], h / s[1]); return *this; }
    template <typename T> constexpr size2u &operator%=(const ARRAY2 &s)       { set(w % s[0], h % s[1]); return *this; }

    constexpr size2u operator+(const size2u &s)                         const { return size2u(w + s.w, h + s.h); }
    constexpr size2u operator-(const size2u &s)                         const { return size2u(w - s.w, h - s.h); }
    constexpr size2u operator*(const size2u &s)                         const { return size2u(w * s.w, h * s.h); }
    constexpr size2u operator/(const size2u &s)                         const { return size2u(w / s.w, h / s.h); }
    constexpr size2u operator%(const size2u &s)                         const { return size2u(w % s.w, h % s.h); }
    
    template <typename T> constexpr size2u operator+(const ARRAY2 &s)   const { size2u u(*this); return u += s; }
    template <typename T> constexpr size2u operator-(const ARRAY2 &s)   const { size2u u(*this); return u -= s; }
    template <typename T> constexpr size2u operator*(const ARRAY2 &s)   const { size2u u(*this); return u *= s; }
    template <typename T> constexpr size2u operator/(const ARRAY2 &s)   const { size2u u(*this); return u /= s; }
    template <typename T> constexpr size2u operator%(const ARRAY2 &s)   const { size2u u(*this); return u %= s; }

    constexpr size2u operator+(int s)                                   const { size2u u(*this); return u.set(w + s, h + s); }
    constexpr size2u operator-(int s)                                   const { size2u u(*this); return u.set(w - s, h - s); }
    constexpr size2u operator*(int s)                                   const { size2u u(*this); return u.set(w * s, h * s); }
    constexpr size2u operator/(int s)                                   const { size2u u(*this); return u.set(w / s, h / s); }
    constexpr size2u operator%(int s)                                   const { size2u u(*this); return u.set(w % s, h % s); }
    
    constexpr bool operator==(const size2u &s)                          const { return w == s.w && h == s.h; }
    constexpr bool operator< (const size2u &s)                          const { return w == s.w ? h < s.h : w < s.w; }
    constexpr bool operator> (const size2u &s)                          const { return w == s.w ? h > s.h : w > s.w; }
    constexpr bool operator<=(const size2u &s)                          const { return !(*this > s); }
    constexpr bool operator>=(const size2u &s)                          const { return !(*this < s); }
    
    constexpr size_t                        area()                      const { return w * h; }

    template <typename T> constexpr ARRAY2  array()                     const { return ARRAY2{ T(w), T(h) }; }
    
    template <typename T> constexpr size2u &set_w(T v)                        { w = size_t(std::max(v, T(0))); return *this; }
    template <typename T> constexpr size2u &set_h(T v)                        { h = size_t(std::max(v, T(0))); return *this; }
    template <typename T> constexpr size2u &set(T u, T v)                     { set_w(u); set_h(v); return *this; }
};

#undef ARRAY2

constexpr std::ostream &operator<<(std::ostream &os, const size2u &s) { return os << "(w:" << s.w << " h:" << s.h << ")"; }

