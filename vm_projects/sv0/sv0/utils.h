#pragma once

#include <iostream>
#include <iomanip>

std::ostream &print_hex(std::ostream &os, const std::string &s, char delimit = 0);

template <typename T>                   inline constexpr bool is_in(const T &v) { return false; }
template <typename T, typename... Args> inline constexpr bool is_in(const T &v, const T &first, const Args&... args) { return v == first || is_in(v, args...); }

template <typename T, size_t N> inline T    *local_alloc(size_t n, std::array<T, N> &buf) { return n >= buf.size() ? new T [n] : buf.data(); }
template <typename T, size_t N> inline void  local_free (T *p,     std::array<T, N> &buf) { if (p != buf.data()) delete [] p; }
