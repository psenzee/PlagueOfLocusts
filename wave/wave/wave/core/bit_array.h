#pragma once

#include <array>
#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include "bits.h"

template <size_t N>
struct bit_array
{
    typedef bit_array<N>          self_t;
    typedef uint64_t              elem_t;
    typedef std::array<size_t, N> bit_list_t;

    enum { ELEM_BITS = sizeof(elem_t) * 8, ELEM_COUNT = N / ELEM_BITS };
    
    typedef std::array<elem_t, ELEM_COUNT> set_t;
    
    set_t _set;
    
    inline bit_array() : _set({ 0 }) {}
    inline bit_array(bool u) : _set({ (u ? ~elem_t(0) : 0) }) {}
    inline bit_array(const size_t *arr, size_t count) : _set({ 0 }) { set(arr, count, true); }
    inline bit_array(const std::array<size_t, N> &arr, size_t count) : _set({ 0 }) { set(arr, count, true); }
    inline bit_array(const self_t &u) : _set(u._set) {}

    inline self_t           &operator=(const self_t &u)                                 { _set = u._set; return *this; }
    inline self_t           &operator=(bool u)                                          { _set = set_t({ (u ? ~elem_t(0) : 0) }); return *this; }

    inline bool              operator==(const self_t &u)                          const { return bit_equal(_set, u._set); }
    inline bool              operator!=(const self_t &u)                          const { return !bit_equal(_set, u._set); }
    inline self_t            operator&(const self_t &other)                       const { self_t arr(*this); bit_and(arr._set, other._set); return arr; }
    inline self_t            operator|(const self_t &other)                       const { self_t arr(*this); bit_or (arr._set, other._set); return arr; }
    inline self_t            operator~()                                          const { self_t arr(*this); bit_not(arr._set); return arr; }
    inline self_t           &operator&=(const self_t &other)                            { bit_and(_set, other._set); return *this; }
    inline self_t           &operator|=(const self_t &other)                            { bit_or (_set, other._set); return *this; }
    inline bool              operator[](size_t index)                             const { return bit_test(_set, index); }

    inline constexpr size_t  element_count()                                      const { return ELEM_COUNT; }
    inline constexpr size_t  element_bits()                                       const { return ELEM_BITS; }
    inline constexpr size_t  size()                                               const { return _set.size() * sizeof(elem_t) * 8; }

    inline const elem_t     *data()                                               const { return _set.data(); }
    inline elem_t           *data()                                                     { return _set.data(); }

    inline size_t            count()                                              const { return bit_count(_set); }
    inline bool              empty()                                              const { return bit_empty(_set); }
    inline bool              full()                                               const { return bit_full(_set); }
    inline void              invert()                                                   { bit_not(_set); }
    inline void              set(size_t i)                                              { bit_set(_set, i); }
    inline void              set_only(size_t i)                                         { _set = set_t({ 0 }); bit_set(_set, i); }
    inline void              set(size_t i, bool v)                                      { if (v) bit_set(_set, i); else bit_unset(_set, i); }
    inline void              set(const size_t *arr, size_t n, bool v = true)            { for (size_t i = 0; i < n; i++) set(arr[i], v); }
    inline void              set(const bit_list_t &arr, size_t n, bool v = true)        { set(arr.data(), n, v); }
    inline void              unset(size_t i)                                            { bit_unset(_set, i); }
    inline void              unset(const size_t *arr, size_t n, bool v = true)          { for (size_t i = 0; i < n; i++) unset(arr[i], v); }
    inline void              unset(const bit_list_t &arr, size_t n, bool v = true)      { unset(arr.data(), n, v); }
    
    inline bool              test(size_t i)                                       const { return bit_test(_set, i); }
    
    inline ssize_t           find_next_set(size_t start = 0, elem_t xor_mask = 0) const { start = bit_find_next_set_bit(_set.data(), ELEM_COUNT, start); return start < N ? start : -1; }
    inline ssize_t           find_next_unset(size_t start = 0)                    const { return find_next_set(start, ~elem_t(0)); }
    
    inline ssize_t           nth_set(size_t n, elem_t xor_mask = 0)               const { return bit_list_nth_set_bit(_set.data(), ELEM_COUNT, n, 0, xor_mask); }
    inline ssize_t           nth_unset(size_t n)                                  const { return nth_set(n, ~elem_t(0)); }

    inline size_t            list_set_bits(bit_list_t &v, elem_t xor_mask = 0)    const { return bit_list_set_bits(_set.data(), ELEM_COUNT, v, 0, xor_mask); }
    inline size_t            list_unset_bits(bit_list_t &v)                       const { return list_set_bits(v, ~elem_t(0)); }

    std::ostream &print(std::ostream &os) const
    {
        bool first = true;
        os << "[" << std::hex;
        for (const auto &v : _set) {
            if (!first) os << ".";
            if (v != 0) os << v;
            first = false;
        }
        return os << "]" << std::dec;
    }

    std::ostream &print_bin(std::ostream &os) const
    {
        for (size_t i = 0; i < N; i++)
            os << (test(i) ? "1" : "0");
        return os;
    }
};

template <size_t N> inline std::ostream &operator<<(std::ostream &os, const bit_array<N> &o) { return o.print(os); }

template <size_t N, typename RNG>
inline size_t shuffled_bit_list(const bit_array<N> &o, typename bit_array<N>::bit_list_t &arr, RNG &rng)
{
    arr = { 0 };
    size_t count = o.list_set_bits(arr);
    std::shuffle(arr.begin(), arr.begin() + count, rng);
    return count;
}

template <size_t N, typename RNG>
inline bit_array<N> unset_random_half_bits(const bit_array<N> &o, RNG &rng)
{
    size_t count = o.count();
    return count < 2 ? o : unset_random_n_bits(o, count / 2, rng);
}

template <size_t N, typename RNG>
inline bit_array<N> unset_random_n_bits(const bit_array<N> &o, size_t n, RNG &rng)
{
    typename bit_array<N>::bit_list_t arr;
    int count = int(shuffled_bit_list(o, arr, rng));
    return bit_array<N>(arr, std::max(count - int(n), 1));
}
