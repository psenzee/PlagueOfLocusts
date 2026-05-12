#pragma once

#include <array>
#include <vector>

// An ugly hack since at least in this version of C++ we can't partially specialize functions, and for these fixed size I need both type and count independently
#define DEFINE__bit_operations(T) \
template <size_t N>             inline size_t bit_count      (const T *d)             { return bit_count<1>(d) + bit_count<N - 1>(d + 1); }                               \
template <>                     inline size_t bit_count<1>   (const T *d)             { return bit_count(*d); }                                                           \
template <size_t N>             inline int    bit_compare    (const T *a, const T *b) { auto d(*a - *b); if (d) return (int)d; return bit_compare<N - 1>(a + 1, b + 1); } \
template <>                     inline int    bit_compare<1> (const T *a, const T *b) { return int(*a - *b); }                                                            \
template <size_t N>             inline bool   bit_equal      (const T *a, const T *b) { return bit_equal<1>(a, b) && bit_equal<N - 1>(a + 1, b + 1); }                    \
template <>                     inline bool   bit_equal<1>   (const T *a, const T *b) { return *a == *b; }                                                                \
template <size_t N>             inline bool   bit_empty      (const T *a)             { return bit_empty<1>(a) && bit_empty<N - 1>(a + 1); }                              \
template <>                     inline bool   bit_empty<1>   (const T *a)             { return *a == 0; }                                                                 \
template <size_t N>             inline bool   bit_full       (const T *a)             { return bit_full<1>(a) && bit_full<N - 1>(a + 1); }                                \
template <>                     inline bool   bit_full<1>    (const T *a)             { return ~*a == 0; }                                                                \
template <size_t N>             inline void   bit_not        (T *d)                   { bit_not<1>(d); bit_not<N - 1>(d + 1); }                                           \
template <>                     inline void   bit_not<1>     (T *d)                   { *d = ~*d; }                                                                       \
template <size_t N>             inline void   bit_or         (T *d, const T *e)       { bit_or<1>(d, e); bit_or<N - 1>(d + 1, e + 1); }                                   \
template <>                     inline void   bit_or<1>      (T *d, const T *e)       { *d |= *e; }                                                                       \
template <size_t N>             inline void   bit_and        (T *d, const T *e)       { bit_and<1>(d, e); bit_and<N - 1>(d + 1, e + 1); }                                 \
template <>                     inline void   bit_and<1>     (T *d, const T *e)       { *d &= *e; }                                                                       \
template <size_t N>             inline void   bit_xor        (T *d, const T *e)       { bit_xor<1>(d, e); bit_xor<N - 1>(d + 1, e + 1); }                                 \
template <>                     inline void   bit_xor<1>     (T *d, const T *e)       { *d ^= *e; }                                                                       \
template <size_t N>             inline void   bit_nor        (T *d, const T *e)       { bit_nor<1>(d, e); bit_nor<N - 1>(d + 1, e + 1); }                                 \
template <>                     inline void   bit_nor<1>     (T *d, const T *e)       { *d = ~(*d | *e); }                                                                \
template <size_t N>             inline void   bit_nand       (T *d, const T *e)       { bit_nand<1>(d, e); bit_nand<N - 1>(d + 1, e + 1); }                               \
template <>                     inline void   bit_nand<1>    (T *d, const T *e)       { *d = ~(*d & *e); }                                                                \
template <size_t N>             inline void   bit_xnor       (T *d, const T *e)       { bit_xnor<1>(d, e); bit_xnor<N - 1>(d + 1, e + 1); }                               \
template <>                     inline void   bit_xnor<1>    (T *d, const T *e)       { *d = ~(*d ^ *e); }

namespace {
static constexpr size_t _lg(size_t n) { return n <= 1 ? 0 : (1 + _lg(n >> 1)); }
template <typename U> struct _traits { enum { BITS = sizeof(U) * 8, LG = _lg(BITS), MASK = BITS - 1 }; };
}

inline size_t bit_count(uint64_t v)          { return size_t(__builtin_popcountl(v)); }
inline size_t bit_count(int64_t v)           { return size_t(__builtin_popcountl(v)); }
inline size_t bit_count(size_t v)            { return bit_count((uint64_t)v); }
inline size_t bit_count(ssize_t v)           { return bit_count((uint64_t)v); }
inline size_t bit_count(uint32_t v)          { return size_t(__builtin_popcount(v)); }
inline size_t bit_count(int32_t v)           { return size_t(__builtin_popcount(v)); }
inline size_t bit_count(uint16_t v)          { return bit_count((uint32_t)v); }
inline size_t bit_count(int16_t v)           { return bit_count((uint16_t)v); }
inline size_t bit_count(uint8_t v)           { return bit_count((uint32_t)v); }
inline size_t bit_count(int8_t v)            { return bit_count((uint8_t)v); }

inline size_t bit_ffs(uint64_t v)            { return size_t(ffsll(v)); }
inline size_t bit_ffs(int64_t v)             { return size_t(ffsll(v)); }
inline size_t bit_ffs(size_t v)              { return bit_ffs((uint64_t)v); }
inline size_t bit_ffs(ssize_t v)             { return bit_ffs((uint64_t)v); }
inline size_t bit_ffs(uint32_t v)            { return size_t(ffsl(v)); }
inline size_t bit_ffs(int32_t v)             { return size_t(ffsl(v)); }
inline size_t bit_ffs(uint16_t v)            { return bit_ffs((uint32_t)v); }
inline size_t bit_ffs(int16_t v)             { return bit_ffs((uint16_t)v); }
inline size_t bit_ffs(uint8_t v)             { return bit_ffs((uint32_t)v); }
inline size_t bit_ffs(int8_t v)              { return bit_ffs((uint8_t)v); }

template <typename T> inline size_t bit_find_next_set_bit(T v, size_t bit, T xor_mask = 0)
{
    v = (v ^ xor_mask) >> bit;
    return !v ? sizeof(T) * 8 : (bit + bit_ffs(v) - 1);
}

template <typename T> size_t bit_find_next_set_bit(const T *v, size_t elem_count, size_t bit, T xor_mask = 0)
{
    constexpr size_t BITS = sizeof(T) * 8, MASK = BITS - 1, LGB = _lg(BITS);
    size_t w = bit >> LGB, b = bit & MASK;
    if (!(v[w] >> b)) {
        w++; b = 0;
        for (; !(v[w] ^ xor_mask) && w < elem_count; w++) ;
    }
    return w < elem_count ? (w * BITS + b + bit_ffs((v[w] ^ xor_mask) >> b) - 1) : elem_count * BITS;
}

template <typename T> size_t bit_list_set_bits(const T *data, size_t *into, size_t max_bits, size_t bit = 0, T xor_mask = 0)
{
    size_t c = 0;
    for (size_t i = bit; i < max_bits; i++) {
        i = bit_find_next_set_bit(data, max_bits / (sizeof(T) * 8), i, xor_mask);
        into[c++] = i;
    }
    return c;
}

template <typename T, size_t N> size_t bit_list_set_bits(const T *data, size_t elem_count, std::array<size_t, N> &v, size_t bit = 0, T xor_mask = 0)
{
    size_t c = 0; v = { 0 };
    for (size_t i = bit; i < N; i++) {
        i = bit_find_next_set_bit(data, elem_count, i, xor_mask);
        if (i < N) v[c++] = i;
    }
    return c;
}

template <typename T, size_t N> ssize_t bit_nth_set_bit(const T *data, size_t elem_count, size_t n, size_t bit = 0, T xor_mask = 0)
{
    size_t c = 0;
    for (size_t i = bit; i < N; i++) {
        i = bit_find_next_set_bit(data, elem_count, i, xor_mask);
        if (c == n) return ssize_t(i);
        c++;
    }
    return -1;
}

// raw fixed size
DEFINE__bit_operations(uint64_t)
DEFINE__bit_operations(int64_t)
DEFINE__bit_operations(size_t)
DEFINE__bit_operations(ssize_t)
DEFINE__bit_operations(uint32_t)
DEFINE__bit_operations(int32_t)
DEFINE__bit_operations(uint16_t)
DEFINE__bit_operations(int16_t)
DEFINE__bit_operations(uint8_t)
DEFINE__bit_operations(int8_t)

#undef DEFINE__bit_operations

// raw variable size
template <typename T>           inline size_t bit_count   (const T *d, size_t sz)                                        { size_t acc = 0; for (auto e = d + sz; d != e;) acc += bit_count(*d++); return acc; }
template <typename T>           inline int    bit_compare (const T *a, const T *b, size_t sz)                            { for (auto e = a + sz; a != e;) { auto d(*a++ - *b++); if (d) return (int)d; } return 0; }
template <typename T>           inline bool   bit_equal   (const T *a, const T *b, size_t sz)                            { for (auto e = a + sz; a != e;) if (*a++ != *b++) return false; return true; }
template <typename T>           inline bool   bit_empty   (const T *a, size_t sz)                                        { for (auto e = a + sz; a != e;) if (*a++ != 0) return false; return true; }
template <typename T>           inline bool   bit_full    (const T *a, size_t sz)                                        { for (auto e = a + sz; a != e;) if (*a++ != ~T(0)) return false; return true; }

template <typename T>           inline void   bit_not     (T *d, size_t sz)                                              { for (auto e = d + sz; d != e; ++d) *d = ~*d; }
template <typename T>           inline void   bit_or      (T *a, const T *b, size_t sz)                                  { for (auto e = a + sz; a != e;) *a++ |= *b++; }
template <typename T>           inline void   bit_and     (T *a, const T *b, size_t sz)                                  { for (auto e = a + sz; a != e;) *a++ &= *b++; }
template <typename T>           inline void   bit_xor     (T *a, const T *b, size_t sz)                                  { for (auto e = a + sz; a != e;) *a++ ^= *b++; }
template <typename T>           inline void   bit_nor     (T *a, const T *b, size_t sz)                                  { for (auto e = a + sz; a != e; ++a, ++b) *a = ~(*a | *b); }
template <typename T>           inline void   bit_nand    (T *a, const T *b, size_t sz)                                  { for (auto e = a + sz; a != e; ++a, ++b) *a = ~(*a & *b); }
template <typename T>           inline void   bit_xnor    (T *a, const T *b, size_t sz)                                  { for (auto e = a + sz; a != e; ++a, ++b) *a = ~(*a ^ *b); }

template <typename U>           inline void   bit_set     (U *d, size_t u)                                               { typedef _traits<U> T; d[u >> T::LG] |= (U(1) << (u & T::MASK)); }
template <typename U>           inline void   bit_unset   (U *d, size_t u)                                               { typedef _traits<U> T; d[u >> T::LG] &= ~(U(1) << (u & T::MASK)); }
template <typename U>           inline bool   bit_test    (const U *d, size_t u)                                         { typedef _traits<U> T; return (d[u >> T::LG] & (U(1) << (u & T::MASK))) > 0; }

// std::array
template <typename U, size_t N> inline size_t bit_count   (const std::array<U, N> &a)                                    { return bit_count  <N>(a.data()); }
template <typename U, size_t N> inline int    bit_compare (const std::array<U, N> &a, const std::array<U, N> &b)         { return bit_compare<N>(a.data(), b.data()); }
template <typename U, size_t N> inline bool   bit_equal   (const std::array<U, N> &a, const std::array<U, N> &b)         { return bit_equal  <N>(a.data(), b.data()); }
template <typename U, size_t N> inline bool   bit_empty   (const std::array<U, N> &a)                                    { return bit_empty  <N>(a.data()); }
template <typename U, size_t N> inline bool   bit_full    (const std::array<U, N> &a)                                    { return bit_full   <N>(a.data()); }

template <typename U, size_t N> inline void   bit_not     (std::array<U, N> &a)                                          { bit_not <N>(a.data()); }
template <typename U, size_t N> inline void   bit_or      (std::array<U, N> &a, const std::array<U, N> &b)               { bit_or  <N>(a.data(), b.data()); }
template <typename U, size_t N> inline void   bit_and     (std::array<U, N> &a, const std::array<U, N> &b)               { bit_and <N>(a.data(), b.data()); }
template <typename U, size_t N> inline void   bit_xor     (std::array<U, N> &a, const std::array<U, N> &b)               { bit_xor <N>(a.data(), b.data()); }
template <typename U, size_t N> inline void   bit_nor     (std::array<U, N> &a, const std::array<U, N> &b)               { bit_nor <N>(a.data(), b.data()); }
template <typename U, size_t N> inline void   bit_nand    (std::array<U, N> &a, const std::array<U, N> &b)               { bit_nand<N>(a.data(), b.data()); }
template <typename U, size_t N> inline void   bit_xnor    (std::array<U, N> &a, const std::array<U, N> &b)               { bit_xnor<N>(a.data(), b.data()); }

template <typename U, size_t N> inline void   bit_set     (std::array<U, N> &d, size_t u)                                { bit_set(d.data(), u); }
template <typename U, size_t N> inline void   bit_unset   (std::array<U, N> &d, size_t u)                                { bit_unset(d.data(), u); }
template <typename U, size_t N> inline bool   bit_test    (const std::array<U, N> &d, size_t u)                          { return bit_test(d.data(), u); }

//template <typename U, size_t N> inline std::array<U, N>  operator|(const std::array<U, N> &a, const std::array<U, N> &b) { auto c(a); bit_or (c, b); return c; }
//template <typename U, size_t N> inline std::array<U, N>  operator&(const std::array<U, N> &a, const std::array<U, N> &b) { auto c(a); bit_and(c, b); return c; }
//template <typename U, size_t N> inline std::array<U, N>  operator^(const std::array<U, N> &a, const std::array<U, N> &b) { auto c(a); bit_xor(c, b); return c; }
//template <typename U, size_t N> inline std::array<U, N> &operator|=(std::array<U, N> &a, const std::array<U, N> &b)      { bit_or (a, b);  return a; }
//template <typename U, size_t N> inline std::array<U, N> &operator&=(std::array<U, N> &a, const std::array<U, N> &b)      { bit_and(a, b); return a; }
//template <typename U, size_t N> inline std::array<U, N> &operator^=(std::array<U, N> &a, const std::array<U, N> &b)      { bit_xor(a, b); return a; }

// std::vector
template <typename T>           inline size_t bit_count   (const std::vector<T> &a)                                      { return bit_count  (a.data(), a.size()); }
template <typename T>           inline int    bit_compare (const std::vector<T> &a, const std::vector<T> &b)             { return bit_compare(a.data(), b.data(), std::min(a.size(), b.size())); }
template <typename T>           inline bool   bit_equal   (const std::vector<T> &a, const std::vector<T> &b)             { return bit_equal  (a.data(), b.data(), std::min(a.size(), b.size())); }
template <typename T>           inline bool   bit_empty   (const std::vector<T> &a)                                      { return bit_empty  (a.data(), a.size()); }
template <typename T>           inline bool   bit_full    (const std::vector<T> &a)                                      { return bit_full   (a.data(), a.size()); }

template <typename T>           inline void   bit_not     (std::vector<T> &a)                                            { bit_not  (a.data(), a.size()); }
template <typename T>           inline void   bit_or      (std::vector<T> &a, const std::vector<T> &b)                   { bit_or   (a.data(), b.data(), std::min(a.size(), b.size())); }
template <typename T>           inline void   bit_and     (std::vector<T> &a, const std::vector<T> &b)                   { bit_and  (a.data(), b.data(), std::min(a.size(), b.size())); }
template <typename T>           inline void   bit_xor     (std::vector<T> &a, const std::vector<T> &b)                   { bit_xor  (a.data(), b.data(), std::min(a.size(), b.size())); }
template <typename T>           inline void   bit_nor     (std::vector<T> &a, const std::vector<T> &b)                   { bit_nor  (a.data(), b.data(), std::min(a.size(), b.size())); }
template <typename T>           inline void   bit_nand    (std::vector<T> &a, const std::vector<T> &b)                   { bit_nand (a.data(), b.data(), std::min(a.size(), b.size())); }
template <typename T>           inline void   bit_xnor    (std::vector<T> &a, const std::vector<T> &b)                   { bit_xnor (a.data(), b.data(), std::min(a.size(), b.size())); }

template <typename U>           inline void   bit_set     (std::vector<U> &d, size_t u)                                  { bit_set   (d.data(), u); }
template <typename U>           inline void   bit_unset   (std::vector<U> &d, size_t u)                                  { bit_unset (d.data(), u); }
template <typename U>           inline bool   bit_test    (const std::vector<U> &d, size_t u)                            { return bit_test(d.data(), u); }

template <typename T>           inline std::vector<T>  operator|(const std::vector<T> &a, const std::vector<T> &b)       { auto c(a); bit_or (c, b); return c; }
template <typename T>           inline std::vector<T>  operator&(const std::vector<T> &a, const std::vector<T> &b)       { auto c(a); bit_and(c, b); return c; }
template <typename T>           inline std::vector<T>  operator^(const std::vector<T> &a, const std::vector<T> &b)       { auto c(a); bit_xor(c, b); return c; }
template <typename T>           inline std::vector<T> &operator|=(std::vector<T>  &a, const std::vector<T> &b)           { bit_or (a, b); return a; }
template <typename T>           inline std::vector<T> &operator&=(std::vector<T>  &a, const std::vector<T> &b)           { bit_and(a, b); return a; }
template <typename T>           inline std::vector<T> &operator^=(std::vector<T>  &a, const std::vector<T> &b)           { bit_xor(a, b); return a; }
