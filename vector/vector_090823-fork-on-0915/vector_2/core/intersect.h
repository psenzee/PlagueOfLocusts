#pragma once

#include <cstddef>
#include <array>
#include "simple_timer.h"

namespace { template <typename T, typename U> inline bool _(T a, U b) { return (a == b)|(a == -b); } }

// find
template <typename T, typename U> inline int find(T a, const std::array<U, 1> &b) { return a==b[0]?0:-1; }
template <typename T, typename U> inline int find(T a, const std::array<U, 2> &b) { return a==b[0]?0:(a==b[1]?1:-1); }
template <typename T, typename U> inline int find(T a, const std::array<U, 3> &b) { return a==b[0]?0:(a==b[1]?1:(a==b[2]?2:-1)); }
template <typename T, typename U> inline int find(T a, const std::array<U, 4> &b) { return a==b[0]?0:(a==b[1]?1:(a==b[2]?2:(a==b[3]?3:-1))); }
template <typename T, typename U> inline int find(T a, const std::array<U, 5> &b) { return a==b[0]?0:(a==b[1]?1:(a==b[2]?2:(a==b[3]?3:(a==b[4]?4:-1)))); }
template <typename T, typename U> inline int find(T a, const std::array<U, 6> &b) { return a==b[0]?0:(a==b[1]?1:(a==b[2]?2:(a==b[3]?3:(a==b[4]?4:(a==b[5]?5:-1))))); }
template <typename T, typename U> inline int find(T a, const std::array<U, 7> &b) { return a==b[0]?0:(a==b[1]?1:(a==b[2]?2:(a==b[3]?3:(a==b[4]?4:(a==b[5]?5:(a==b[6]?6:-1)))))); }
template <typename T, typename U> inline int find(T a, const std::array<U, 8> &b) { return a==b[0]?0:(a==b[1]?1:(a==b[2]?2:(a==b[3]?3:(a==b[4]?4:(a==b[5]?5:(a==b[6]?6:(a==b[7]?7:-1))))))); }

template <typename T, typename U, size_t N>
inline int find(T a, const std::array<U, N> &b) { for (int i = 0; i < N; i++) if (a == b[i]) return i; return -1; }

// find_abs
/*
template <typename T, typename U> inline int find_abs(T a, const std::array<U, 1> &b) { return _(abs(a), b[0]) ? 0 : -1; }
template <typename T, typename U> inline int find_abs(T a, const std::array<U, 2> &b) { a=abs(a); return _(a,b[0])?0:(_(a,b[1])?1:-1); }
template <typename T, typename U> inline int find_abs(T a, const std::array<U, 3> &b) { a=abs(a); return _(a,b[0])?0:(_(a,b[1])?1:(_(a,b[2])?2:-1)); }
template <typename T, typename U> inline int find_abs(T a, const std::array<U, 4> &b) { a=abs(a); return _(a,b[0])?0:(_(a,b[1])?1:(_(a,b[2])?2:(_(a,b[3])?3:-1))); }
template <typename T, typename U> inline int find_abs(T a, const std::array<U, 5> &b) { a=abs(a); return _(a,b[0])?0:(_(a,b[1])?1:(_(a,b[2])?2:(_(a,b[3])?3:(_(a,b[4])?4:-1)))); }
template <typename T, typename U> inline int find_abs(T a, const std::array<U, 6> &b) { a=abs(a); return _(a,b[0])?0:(_(a,b[1])?1:(_(a,b[2])?2:(_(a,b[3])?3:(_(a,b[4])?4:(_(a,b[5])?5:-1))))); }
template <typename T, typename U> inline int find_abs(T a, const std::array<U, 7> &b) { a=abs(a); return _(a,b[0])?0:(_(a,b[1])?1:(_(a,b[2])?2:(_(a,b[3])?3:(_(a,b[4])?4:(_(a,b[5])?5:(_(a,b[6])?6:-1)))))); }
template <typename T, typename U> inline int find_abs(T a, const std::array<U, 8> &b) { a=abs(a); return _(a,b[0])?0:(_(a,b[1])?1:(_(a,b[2])?2:(_(a,b[3])?3:(_(a,b[4])?4:(_(a,b[5])?5:(_(a,b[6])?6:(_(a,b[7])?7:-1))))))); }
*/

template <typename T, typename U> inline int find_abs(T a, const std::array<U, 1> &b) { return _(a,b[0])?0:-1; }
template <typename T, typename U> inline int find_abs(T a, const std::array<U, 2> &b) { return _(a,b[0])?0:(_(a,b[1])?1:-1); }
template <typename T, typename U> inline int find_abs(T a, const std::array<U, 3> &b) { return _(a,b[0])?0:(_(a,b[1])?1:(_(a,b[2])?2:-1)); }
template <typename T, typename U> inline int find_abs(T a, const std::array<U, 4> &b) { return _(a,b[0])?0:(_(a,b[1])?1:(_(a,b[2])?2:(_(a,b[3])?3:-1))); }
template <typename T, typename U> inline int find_abs(T a, const std::array<U, 5> &b) { return _(a,b[0])?0:(_(a,b[1])?1:(_(a,b[2])?2:(_(a,b[3])?3:(_(a,b[4])?4:-1)))); }
template <typename T, typename U> inline int find_abs(T a, const std::array<U, 6> &b) { return _(a,b[0])?0:(_(a,b[1])?1:(_(a,b[2])?2:(_(a,b[3])?3:(_(a,b[4])?4:(_(a,b[5])?5:-1))))); }
template <typename T, typename U> inline int find_abs(T a, const std::array<U, 7> &b) { return _(a,b[0])?0:(_(a,b[1])?1:(_(a,b[2])?2:(_(a,b[3])?3:(_(a,b[4])?4:(_(a,b[5])?5:(_(a,b[6])?6:-1)))))); }
template <typename T, typename U> inline int find_abs(T a, const std::array<U, 8> &b) { return _(a,b[0])?0:(_(a,b[1])?1:(_(a,b[2])?2:(_(a,b[3])?3:(_(a,b[4])?4:(_(a,b[5])?5:(_(a,b[6])?6:(_(a,b[7])?7:-1))))))); }

template <typename T, typename U, size_t N>
inline int find_abs(T a, const std::array<U, N> &b) { a = abs(a); for (int i = 0; i != N; ++i) if (_(a, b[i])) return i; return -1; }

// count
template <typename T, typename U> inline size_t count(T a, const std::array<U, 1> &b) { return (a==b[0]); }
template <typename T, typename U> inline size_t count(T a, const std::array<U, 2> &b) { return (a==b[0])+(a==b[1]); }
template <typename T, typename U> inline size_t count(T a, const std::array<U, 3> &b) { return (a==b[0])+(a==b[1])+(a==b[2]); }
template <typename T, typename U> inline size_t count(T a, const std::array<U, 4> &b) { return (a==b[0])+(a==b[1])+(a==b[2])+(a==b[3]); }
template <typename T, typename U> inline size_t count(T a, const std::array<U, 5> &b) { return (a==b[0])+(a==b[1])+(a==b[2])+(a==b[3])+(a==b[4]); }
template <typename T, typename U> inline size_t count(T a, const std::array<U, 6> &b) { return (a==b[0])+(a==b[1])+(a==b[2])+(a==b[3])+(a==b[4])+(a==b[5]); }
template <typename T, typename U> inline size_t count(T a, const std::array<U, 7> &b) { return (a==b[0])+(a==b[1])+(a==b[2])+(a==b[3])+(a==b[4])+(a==b[5])+(a==b[6]); }
template <typename T, typename U> inline size_t count(T a, const std::array<U, 8> &b) { return (a==b[0])+(a==b[1])+(a==b[2])+(a==b[3])+(a==b[4])+(a==b[5])+(a==b[6])+(a==b[7]); }

template <typename T, typename U, size_t N>
inline size_t count(T a, const std::array<U, N> &b) { size_t count = 0; for (auto u : b) count += (a == u); return count; }

// has
template <typename T, typename U> inline bool has(T a, const std::array<U, 1> &b) { return a==b[0]; }
template <typename T, typename U> inline bool has(T a, const std::array<U, 2> &b) { return a==b[0]||a==b[1]; }
template <typename T, typename U> inline bool has(T a, const std::array<U, 3> &b) { return a==b[0]||a==b[1]||a==b[2]; }
template <typename T, typename U> inline bool has(T a, const std::array<U, 4> &b) { return a==b[0]||a==b[1]||a==b[2]||a==b[3]; }
template <typename T, typename U> inline bool has(T a, const std::array<U, 5> &b) { return a==b[0]||a==b[1]||a==b[2]||a==b[3]||a==b[4]; }
template <typename T, typename U> inline bool has(T a, const std::array<U, 6> &b) { return a==b[0]||a==b[1]||a==b[2]||a==b[3]||a==b[4]||a==b[5]; }
template <typename T, typename U> inline bool has(T a, const std::array<U, 7> &b) { return a==b[0]||a==b[1]||a==b[2]||a==b[3]||a==b[4]||a==b[5]||a==b[6]; }
template <typename T, typename U> inline bool has(T a, const std::array<U, 8> &b) { return a==b[0]||a==b[1]||a==b[2]||a==b[3]||a==b[4]||a==b[5]||a==b[6]||a==b[7]; }

template <typename T, typename U, size_t N>
inline bool has(T a, const std::array<U, N> &b) { for (auto u : b) if (u == a) return true; return false; }

// count_intersect
template <typename T, typename U> inline size_t count_intersect(const std::array<T, 1> &a, const std::array<U, 1> &b) { return a[0] == b[0]; }
template <typename T, typename U> inline size_t count_intersect(const std::array<T, 2> &a, const std::array<U, 2> &b) { return (a[0]==b[0])+(a[0]==b[1])+(a[1]==b[0])+(a[1]==b[1]); }

template <typename T, typename U> inline size_t count_intersect(const std::array<T, 3> &a, const std::array<U, 3> &b)
{
   return (a[0]==b[0])+(a[0]==b[1])+(a[0]==b[2])+(a[1]==b[0])+(a[1]==b[1])+(a[1]==b[2])+(a[2]==b[0])+(a[2]==b[1])+(a[2]==b[2]);
}

template <typename T, typename U> inline size_t count_intersect(const std::array<T, 4> &a, const std::array<U, 4> &b)
{
   return (a[0] == b[0]) + (a[0] == b[1]) + (a[0] == b[2]) + (a[0] == b[3]) + (a[1] == b[0]) + (a[1] == b[1]) + (a[1] == b[2]) + (a[1] == b[3]) +
          (a[2] == b[0]) + (a[2] == b[1]) + (a[2] == b[2]) + (a[2] == b[3]) + (a[3] == b[0]) + (a[3] == b[1]) + (a[3] == b[2]) + (a[3] == b[3]);
}

template <typename T, typename U> inline size_t count_intersect_(const std::array<T, 5> &a, const std::array<U, 5> &b)
{
   return count(a[0], b) + count(a[1], b) + count(a[2], b) + count(a[3], b) + count(a[4], b);
}

template <typename T, typename U> inline size_t count_intersect(const std::array<T, 5> &a, const std::array<U, 5> &b)
{
   return (a[0] == b[0]) + (a[0] == b[1]) + (a[0] == b[2]) + (a[0] == b[3]) + (a[0] == b[4]) +
          (a[1] == b[0]) + (a[1] == b[1]) + (a[1] == b[2]) + (a[1] == b[3]) + (a[1] == b[4]) +
          (a[2] == b[0]) + (a[2] == b[1]) + (a[2] == b[2]) + (a[2] == b[3]) + (a[2] == b[4]) +
          (a[3] == b[0]) + (a[3] == b[1]) + (a[3] == b[2]) + (a[3] == b[3]) + (a[3] == b[4]) +
          (a[4] == b[0]) + (a[4] == b[1]) + (a[4] == b[2]) + (a[4] == b[3]) + (a[4] == b[4]);
}

template <typename T, typename U> inline size_t count_intersect(const std::array<T, 6> &a, const std::array<U, 6> &b)
{
   return (a[0] == b[0]) + (a[0] == b[1]) + (a[0] == b[2]) + (a[0] == b[3]) + (a[0] == b[4]) + (a[0] == b[5]) +
          (a[1] == b[0]) + (a[1] == b[1]) + (a[1] == b[2]) + (a[1] == b[3]) + (a[1] == b[4]) + (a[1] == b[5]) +
          (a[2] == b[0]) + (a[2] == b[1]) + (a[2] == b[2]) + (a[2] == b[3]) + (a[2] == b[4]) + (a[2] == b[5]) +
          (a[3] == b[0]) + (a[3] == b[1]) + (a[3] == b[2]) + (a[3] == b[3]) + (a[3] == b[4]) + (a[3] == b[5]) +
          (a[4] == b[0]) + (a[4] == b[1]) + (a[4] == b[2]) + (a[4] == b[3]) + (a[4] == b[4]) + (a[4] == b[5]) +
          (a[5] == b[0]) + (a[5] == b[1]) + (a[5] == b[2]) + (a[5] == b[3]) + (a[5] == b[4]) + (a[5] == b[5]);
}

template <typename T, typename U> inline size_t count_intersect(const std::array<T, 7> &a, const std::array<U, 7> &b)
{
   return (a[0] == b[0]) + (a[0] == b[1]) + (a[0] == b[2]) + (a[0] == b[3]) + (a[0] == b[4]) + (a[0] == b[5]) + (a[0] == b[6]) +
          (a[1] == b[0]) + (a[1] == b[1]) + (a[1] == b[2]) + (a[1] == b[3]) + (a[1] == b[4]) + (a[1] == b[5]) + (a[1] == b[6]) +
          (a[2] == b[0]) + (a[2] == b[1]) + (a[2] == b[2]) + (a[2] == b[3]) + (a[2] == b[4]) + (a[2] == b[5]) + (a[2] == b[6]) +
          (a[3] == b[0]) + (a[3] == b[1]) + (a[3] == b[2]) + (a[3] == b[3]) + (a[3] == b[4]) + (a[3] == b[5]) + (a[3] == b[6]) +
          (a[4] == b[0]) + (a[4] == b[1]) + (a[4] == b[2]) + (a[4] == b[3]) + (a[4] == b[4]) + (a[4] == b[5]) + (a[4] == b[6]) +
          (a[5] == b[0]) + (a[5] == b[1]) + (a[5] == b[2]) + (a[5] == b[3]) + (a[5] == b[4]) + (a[5] == b[5]) + (a[5] == b[6]) +
          (a[6] == b[0]) + (a[6] == b[1]) + (a[6] == b[2]) + (a[6] == b[3]) + (a[6] == b[4]) + (a[6] == b[5]) + (a[6] == b[6]);
}

template <typename T, typename U> inline size_t count_intersect(const std::array<T, 8> &a, const std::array<U, 8> &b)
{
   return (a[0] == b[0]) + (a[0] == b[1]) + (a[0] == b[2]) + (a[0] == b[3]) + (a[0] == b[4]) + (a[0] == b[5]) + (a[0] == b[6]) + (a[0] == b[7]) +
          (a[1] == b[0]) + (a[1] == b[1]) + (a[1] == b[2]) + (a[1] == b[3]) + (a[1] == b[4]) + (a[1] == b[5]) + (a[1] == b[6]) + (a[1] == b[7]) +
          (a[2] == b[0]) + (a[2] == b[1]) + (a[2] == b[2]) + (a[2] == b[3]) + (a[2] == b[4]) + (a[2] == b[5]) + (a[2] == b[6]) + (a[2] == b[7]) +
          (a[3] == b[0]) + (a[3] == b[1]) + (a[3] == b[2]) + (a[3] == b[3]) + (a[3] == b[4]) + (a[3] == b[5]) + (a[3] == b[6]) + (a[3] == b[7]) +
          (a[4] == b[0]) + (a[4] == b[1]) + (a[4] == b[2]) + (a[4] == b[3]) + (a[4] == b[4]) + (a[4] == b[5]) + (a[4] == b[6]) + (a[4] == b[7]) +
          (a[5] == b[0]) + (a[5] == b[1]) + (a[5] == b[2]) + (a[5] == b[3]) + (a[5] == b[4]) + (a[5] == b[5]) + (a[5] == b[6]) + (a[5] == b[7]) +
          (a[6] == b[0]) + (a[6] == b[1]) + (a[6] == b[2]) + (a[6] == b[3]) + (a[6] == b[4]) + (a[6] == b[5]) + (a[6] == b[6]) + (a[6] == b[7]) +
          (a[7] == b[0]) + (a[7] == b[1]) + (a[7] == b[2]) + (a[7] == b[3]) + (a[7] == b[4]) + (a[7] == b[5]) + (a[7] == b[6]) + (a[7] == b[7]);
}

template <typename T, typename U, size_t N>
inline size_t count_intersect(const std::array<T, N> &a, const std::array<U, N> &b)
{ size_t count = 0; for (auto u : a) count += has(u, b); return count; }

template <typename T, typename U, size_t N>
inline size_t count_intersect_sorted(const std::array<T, N> &a, const std::array<U, N> &b)
{
    size_t count = 0;
    auto ai = a.cbegin(), ae = a.cend();
    auto bi = b.cbegin(), be = b.cend();
    while (ai != ae && bi != be) {
        auto u = *ai;
        auto v = *bi;
        if (u == v) {
            count++;
            ai++;
            bi++;
        } else if (u > v) {
            bi++;
        } else {
            ai++;
        }
    }
    return count;
}

template <typename T, typename U, size_t N>
inline size_t count_intersect_sort(const std::array<T, N> &a, const std::array<U, N> &b)
{
    std::array<T, N> sa(a);
    std::array<U, N> sb(b);
    std::sort(sa.begin(), sa.end());
    std::sort(sb.begin(), sb.end());
    return count_intersect_sorted(sa, sb);
}

template <typename T, typename U, size_t N>
inline size_t count_intersect_first_sorted(const std::array<T, N> &a, const std::array<U, N> &b)
{
    std::array<U, N> sb(b);
    std::sort(sb.begin(), sb.end());
    return count_intersect_sorted(a, sb);
}

// count_intersect
inline bool has_intersect(const std::array<int16_t, 1> &a, const std::array<int16_t, 1> &b)
{
   return (a[0] == b[0]);
}

inline bool has_intersect(const std::array<int16_t, 2> &a, const std::array<int16_t, 2> &b)
{
   return (a[0] == b[0]) || (a[0] == b[1]) || (a[1] == b[0]) || (a[1] == b[1]);
}

inline bool has_intersect(const std::array<int16_t, 3> &a, const std::array<int16_t, 3> &b)
{
   return (a[0] == b[0]) || (a[0] == b[1]) || (a[0] == b[2]) || (a[1] == b[0]) || (a[1] == b[1]) || (a[1] == b[2]) ||
          (a[2] == b[0]) || (a[2] == b[1]) || (a[2] == b[2]);
}

inline bool has_intersect(const std::array<int16_t, 4> &a, const std::array<int16_t, 4> &b)
{
   return (a[0] == b[0]) || (a[0] == b[1]) || (a[0] == b[2]) || (a[0] == b[3]) || (a[1] == b[0]) || (a[1] == b[1]) || (a[1] == b[2]) || (a[1] == b[3]) ||
          (a[2] == b[0]) || (a[2] == b[1]) || (a[2] == b[2]) || (a[2] == b[3]) || (a[3] == b[0]) || (a[3] == b[1]) || (a[3] == b[2]) || (a[3] == b[3]);
}

inline bool has_intersect(const std::array<int16_t, 5> &a, const std::array<int16_t, 5> &b)
{
   return (a[0] == b[0]) || (a[0] == b[1]) || (a[0] == b[2]) || (a[0] == b[3]) || (a[0] == b[4]) ||
          (a[1] == b[0]) || (a[1] == b[1]) || (a[1] == b[2]) || (a[1] == b[3]) || (a[1] == b[4]) ||
          (a[2] == b[0]) || (a[2] == b[1]) || (a[2] == b[2]) || (a[2] == b[3]) || (a[2] == b[4]) ||
          (a[3] == b[0]) || (a[3] == b[1]) || (a[3] == b[2]) || (a[3] == b[3]) || (a[3] == b[4]) ||
          (a[4] == b[0]) || (a[4] == b[1]) || (a[4] == b[2]) || (a[4] == b[3]) || (a[4] == b[4]);
}

inline bool has_intersect(const std::array<int16_t, 6> &a, const std::array<int16_t, 6> &b)
{
   return (a[0] == b[0]) || (a[0] == b[1]) || (a[0] == b[2]) || (a[0] == b[3]) || (a[0] == b[4]) || (a[0] == b[5]) ||
          (a[1] == b[0]) || (a[1] == b[1]) || (a[1] == b[2]) || (a[1] == b[3]) || (a[1] == b[4]) || (a[1] == b[5]) ||
          (a[2] == b[0]) || (a[2] == b[1]) || (a[2] == b[2]) || (a[2] == b[3]) || (a[2] == b[4]) || (a[2] == b[5]) ||
          (a[3] == b[0]) || (a[3] == b[1]) || (a[3] == b[2]) || (a[3] == b[3]) || (a[3] == b[4]) || (a[3] == b[5]) ||
          (a[4] == b[0]) || (a[4] == b[1]) || (a[4] == b[2]) || (a[4] == b[3]) || (a[4] == b[4]) || (a[4] == b[5]) ||
          (a[5] == b[0]) || (a[5] == b[1]) || (a[5] == b[2]) || (a[5] == b[3]) || (a[5] == b[4]) || (a[5] == b[5]);
}

inline bool has_intersect(const std::array<int16_t, 7> &a, const std::array<int16_t, 7> &b)
{
   return (a[0] == b[0]) || (a[0] == b[1]) || (a[0] == b[2]) || (a[0] == b[3]) || (a[0] == b[4]) || (a[0] == b[5]) || (a[0] == b[6]) ||
          (a[1] == b[0]) || (a[1] == b[1]) || (a[1] == b[2]) || (a[1] == b[3]) || (a[1] == b[4]) || (a[1] == b[5]) || (a[1] == b[6]) ||
          (a[2] == b[0]) || (a[2] == b[1]) || (a[2] == b[2]) || (a[2] == b[3]) || (a[2] == b[4]) || (a[2] == b[5]) || (a[2] == b[6]) ||
          (a[3] == b[0]) || (a[3] == b[1]) || (a[3] == b[2]) || (a[3] == b[3]) || (a[3] == b[4]) || (a[3] == b[5]) || (a[3] == b[6]) ||
          (a[4] == b[0]) || (a[4] == b[1]) || (a[4] == b[2]) || (a[4] == b[3]) || (a[4] == b[4]) || (a[4] == b[5]) || (a[4] == b[6]) ||
          (a[5] == b[0]) || (a[5] == b[1]) || (a[5] == b[2]) || (a[5] == b[3]) || (a[5] == b[4]) || (a[5] == b[5]) || (a[5] == b[6]) ||
          (a[6] == b[0]) || (a[6] == b[1]) || (a[6] == b[2]) || (a[6] == b[3]) || (a[6] == b[4]) || (a[6] == b[5]) || (a[6] == b[6]);
}

template <typename T, typename U>
inline bool has_intersect(const std::array<T, 8> &a, const std::array<U, 8> &b)
{
   return (a[0] == b[0]) || (a[0] == b[1]) || (a[0] == b[2]) || (a[0] == b[3]) || (a[0] == b[4]) || (a[0] == b[5]) || (a[0] == b[6]) || (a[0] == b[7]) ||
          (a[1] == b[0]) || (a[1] == b[1]) || (a[1] == b[2]) || (a[1] == b[3]) || (a[1] == b[4]) || (a[1] == b[5]) || (a[1] == b[6]) || (a[1] == b[7]) ||
          (a[2] == b[0]) || (a[2] == b[1]) || (a[2] == b[2]) || (a[2] == b[3]) || (a[2] == b[4]) || (a[2] == b[5]) || (a[2] == b[6]) || (a[2] == b[7]) ||
          (a[3] == b[0]) || (a[3] == b[1]) || (a[3] == b[2]) || (a[3] == b[3]) || (a[3] == b[4]) || (a[3] == b[5]) || (a[3] == b[6]) || (a[3] == b[7]) ||
          (a[4] == b[0]) || (a[4] == b[1]) || (a[4] == b[2]) || (a[4] == b[3]) || (a[4] == b[4]) || (a[4] == b[5]) || (a[4] == b[6]) || (a[4] == b[7]) ||
          (a[5] == b[0]) || (a[5] == b[1]) || (a[5] == b[2]) || (a[5] == b[3]) || (a[5] == b[4]) || (a[5] == b[5]) || (a[5] == b[6]) || (a[5] == b[7]) ||
          (a[6] == b[0]) || (a[6] == b[1]) || (a[6] == b[2]) || (a[6] == b[3]) || (a[6] == b[4]) || (a[6] == b[5]) || (a[6] == b[6]) || (a[6] == b[7]) ||
          (a[7] == b[0]) || (a[7] == b[1]) || (a[7] == b[2]) || (a[7] == b[3]) || (a[7] == b[4]) || (a[7] == b[5]) || (a[7] == b[6]) || (a[7] == b[7]);
}

template <typename T, typename U, size_t N>
inline bool has_intersect(const std::array<T, N> &a, const std::array<U, N> &b)
{
    for (auto u : a) for (auto v : b) if (u == v) return true;
    return false;
}

template <typename T, typename U, size_t N>
inline size_t has_intersect_sorted(const std::array<T, N> &a, const std::array<U, N> &b)
{
    auto bi = b.cbegin(), be = b.cend();
    auto ai = a.cbegin(), ae = a.cend();
    while (ai != ae && bi != be) {
        auto u = *ai;
        auto v = *bi;
        if (u == v) {
            return true;
            ai++;
            bi++;
        } else if (u > v) {
            bi++;
        } else {
            ai++;
        }
    }
    return false;
}

template <typename T, typename U, size_t N>
inline size_t has_intersect_sort(const std::array<T, N> &a, const std::array<U, N> &b)
{
    std::array<T, N> sa(a);
    std::array<U, N> sb(b);
    std::sort(sa.begin(), sa.end());
    std::sort(sb.begin(), sb.end());
    return has_intersect_sorted(sa, sb);
}

template <typename T, typename U, size_t N>
inline size_t has_intersect_first_sorted(const std::array<T, N> &a, const std::array<U, N> &b)
{
    std::array<U, N> sb(b);
    std::sort(sb.begin(), sb.end());
    return has_intersect_sorted(a, sb);
}

template <size_t N>
void test_intersect_timing_cross_over()
{
    std::array<size_t, N> a, b;
    for (size_t i = 0; i < N; i++) {
        a[i] = rand() % (N * 2);
        b[i] = rand() % (N * 2);
    }
    std::array<size_t, N> sa(a);
    std::sort(sa.begin(), sa.end());
    enum { COUNT = 100000 };
    simple_timer t0;
    size_t count0 = 0;
    for (size_t i = 0; i < COUNT; i++)
        count0 = count_intersect(a, b);
    t0.print_elapsed_only(std::cout << "size " << N << " base - n\u00b2 ");
    simple_timer t1;
    size_t count1 = 0;
    for (size_t i = 0; i < COUNT; i++)
        count1 = count_intersect_sort(a, b);
    t1.print_elapsed_only(std::cout << " n lg(n) ");
    simple_timer t2;
    size_t count2 = 0;
    for (size_t i = 0; i < COUNT; i++)
        count2 = count_intersect_first_sorted(sa, b);
    t2.print_elapsed_only(std::cout << " n lg(n) [fs] ");
    if (count0 != count1 || count0 != count2)
        std::cout << " MISMATCH COUNT! " << count0 << " vs " << count1 << " vs " << count2;
    std::cout << std::endl;
}
