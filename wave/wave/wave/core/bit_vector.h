#pragma once

#include <array>
#include <vector>
#include <iostream>
#include <iomanip>
#include "bits.h"

struct bit_vector
{
    typedef bit_vector   self_t;
    typedef uint64_t     elem_t;
    
    typedef std::vector<elem_t> set_t;
    
    set_t _set;
    bool  _default;
    
    inline bit_vector(bool u = false) : _default(u) { _set.push_back(u ? ~elem_t(0) : 0); }
    inline bit_vector(const self_t &u) : _set(u._set), _default(u._default) {}
    inline bit_vector(const size_t *arr, size_t count, bool default_value = false) : _default(default_value) { set(arr, count, true); }
    
    inline self_t &operator=(const self_t &u)                                       { _set = u._set; _default = u._default; return *this; }
    inline self_t &operator=(bool u)                                                { _set.resize(0); _default = u; return *this; }
    
    inline bool    operator==(const self_t &u)                                const { return bit_equal(_set, u._set); }
    inline bool    operator!=(const self_t &u)                                const { return !bit_equal(_set, u._set); }
    inline self_t  operator&(const self_t &other)                             const { self_t arr(*this); bit_and(arr._set, other._set); return arr; }
    inline self_t  operator|(const self_t &other)                             const { self_t arr(*this); bit_or (arr._set, other._set); return arr; }
    inline self_t  operator~()                                                const { self_t arr(*this); bit_not(arr._set); return arr; }
    inline self_t &operator&=(const self_t &other)                                  { bit_and(_set, other._set); return *this; }
    inline self_t &operator|=(const self_t &other)                                  { bit_or (_set, other._set); return *this; }
    inline bool    operator[](size_t index)                                   const { return bit_test(_set, index); }

    inline size_t        size()                                               const { return _set.size() * sizeof(elem_t); }

    inline const elem_t *data()                                               const { return _set.data(); }
    inline elem_t       *data()                                                     { return _set.data(); }
   
    inline size_t        count()                                              const { return bit_count(_set); }
    inline bool          empty()                                              const { return bit_empty(_set); }
    inline bool          full()                                               const { return bit_full(_set); }
    inline void          invert()                                                   { bit_not(_set); }
    inline void          set(size_t i)                                              { _expand_if_necessary(i, true);  bit_set(_set, i); }
    inline void          set_only(size_t i)                                         { _set.clear(); bit_set(_set, i); }
    inline void          set(size_t i, bool v)                                      { if (v) set(i); else unset(i); }
    inline void          set(const size_t *a, size_t n, bool v = true)              { for (size_t i = 0; i < n; i++) set(a[i], v); }
    inline void          unset(size_t i)                                            { _expand_if_necessary(i, false); bit_unset(_set, i); }
    inline void          unset(const size_t *a, size_t n)                           { for (size_t i = 0; i < n; i++) unset(a[i]); }

    inline bool          test(size_t i)                                       const { return i >= capacity_bits() ? _default : bit_test(_set, i); }
    
    inline ssize_t       find_next_set(size_t s = 0, elem_t xor_mask = 0)     const { s = bit_find_next_set_bit(_set.data(), _set.size(), s); return s < _set.size() ? s : -1; }
    inline ssize_t       find_next_unset(size_t s = 0)                        const { return find_next_set(s, ~elem_t(0)); }
    
//    inline ssize_t       nth_set(size_t n, elem_t xor_mask = 0)               const { return bit_list_nth_set_bit(_set.data(), _set.size(), n, 0, xor_mask); }
//    inline ssize_t       nth_unset(size_t n)                                  const { return nth_set(n, ~elem_t(0)); }

    //inline size_t        list_set_bits(bit_list_t &v, elem_t xor_mask = 0)    const { return bit_list_set_bits(_set.data(), _set.size(), v, 0, xor_mask); }
    //inline size_t        list_unset_bits(bit_list_t &v)                       const { return list_set_bits(v, ~elem_t(0)); }
    
    inline size_t        capacity_bits()                                      const { return _set.size() * sizeof(elem_t) * 8; }
    
    std::ostream &print(std::ostream &os) const
    {
        bool first = true;
        os << "[" << std::hex;
        for (const auto &v : _set) {
            if (!first) os << ".";
            if (v != 0) {
                os << v;
            }
            first = false;
        }
        return os << "]" << std::dec;
    }
private:
    
    inline void _expand_if_necessary(size_t i, bool value)
    {
        size_t current = capacity_bits();
        if (value != _default && i >= current)
            _set.assign((i / sizeof(elem_t) * 8) + 1 - size(), _default ? ~elem_t(0) : 0);
    }
};
