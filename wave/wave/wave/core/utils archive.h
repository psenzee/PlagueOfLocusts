#pragma once

#include <iostream>

#define ARRAY_COUNT(ARRAY) (sizeof((ARRAY)) / sizeof((ARRAY)[0]))

template <typename T, typename Float> inline T             lerp(const T &a, const T &b, Float t)                   { return (1 - t) * a + t * b; }

template <typename T, typename U = T> inline bool          any_of(T v, U a, U b)                                   { return v == a || v == b; }
template <typename T, typename U = T> inline bool          any_of(T v, U a, U b, U c)                              { return v == a || v == b || v == c; }
template <typename T, typename U = T> inline bool          any_of(T v, U a, U b, U c, U d)                         { return v == a || v == b || v == c || v == d; }
template <typename T, typename U = T> inline bool          any_of(T v, U a, U b, U c, U d, U e)                    { return v == a || v == b || v == c || v == d || v == e; }
template <typename T, typename U = T> inline bool          any_of(T v, U a, U b, U c, U d, U e, U f)               { return v == a || v == b || v == c || v == d || v == e || v == f; }
template <typename T, typename U = T> inline bool          any_of(T v, U a, U b, U c, U d, U e, U f, U g)          { return v == a || v == b || v == c || v == d || v == e || v == f || v == g; }

template <typename T, typename U = T> inline bool          any_of_until(T v, const U *vs, U end)                   { const U *s(vs); while (*s++ != end) if (*s == v) return true; return false; }
template <typename T, typename U = T> inline bool          any_of_z(T v, const U *vs)                              { return any_of_until(v, vs, T(0)); }
template <typename T, typename U = T> inline ssize_t       index_of_until(T v, const U *vs, U end)                 { const U *s(vs); while (*s++ != end) if (*s == v) return s - vs; return -1; }
template <typename T, typename U = T> inline ssize_t       index_of_z(T v, const U *vs)                            { return index_of_until(v, vs, T(0)); }
template <typename T, typename U = T> inline size_t        count_until(const T *a, U end)                          { const T *s(a); while (*s != end) s++; return s - a; }
template <typename T>                 inline size_t        count_z(const T *a)                                     { return count_until(a, T(0)); }
template <typename T, typename U = T> inline T            *copy_until(const T *a, T *b, U end)                     { const T *s(a); while (*s != end) *b++ = *s++; return b; }
template <typename T, typename U = T> inline T            *copy_max_until(const T *a, T *b, size_t max, U end)     { const T *s(a), *e(b + max); while (*s != end && b != e) *b++ = *s++; return b; }
template <typename T>                 inline T            *copy_z(const T *a, T *b)                                { return copy_until(a, b, T(0)); }
template <typename T>                 inline T            *copy_max_z(const T *a, T *b, size_t max)                { return copy_max_until(a, b, max, T(0)); }

template <typename T>                 inline std::ostream &print_array(std::ostream &os, const T *b, const T *e)   { const T *s(b); while (b != e) os << (b != s ? " " : "") << *b++; return os; }
template <typename T>                 inline std::ostream &print_array(std::ostream &os, const T *b, size_t count) { return print_array(os, b, b + count); }
template <typename T, typename U = T> inline std::ostream &print_array_until(std::ostream &os, const T *b, U end)  { return print_array(os, b, count_until(b, end)); }
template <typename T>                 inline std::ostream &print_array_z(std::ostream &os, const T *b)             { return print_array_until(os, b, T(0)); }

template <typename T, typename U = T> inline T            *duplicate_elements_until(const T *seq, T *to, size_t count, size_t dupes, U seq_end)
{
    const T *a(seq), *s(a), *e(to + count);
    size_t d(dupes);
    while (to != e) { if (*s == seq_end) s = seq; *to++ = *s; if (!--d) { d = dupes; s++; } }
    return to;
}

template <typename T> inline T                            *duplicate_elements_z(const T *seq, T *to, size_t count, size_t dupes) { return duplicate_elements_until(seq, to, count, dupes, T(0)); }

void test_repeat_sequence_until()
{
    const int data[] = { 32, 234, 3, 9, 7, 5, 11, 1, 0 };
    
    int into[400];
    int *end = duplicate_elements_until(data, into, 30, 3, 0);
    *end = 0;
    print_array(std::cout, into, 30) << std::endl;
    print_array(std::cout, into, end) << std::endl;
    print_array_z(std::cout, into) << std::endl;
    
    end = duplicate_elements_until(data, into, 30, 1, 0);
    *end = 0;
    print_array(std::cout, into, 30) << std::endl;
    print_array(std::cout, into, end) << std::endl;
    print_array_z(std::cout, into) << std::endl;
}
