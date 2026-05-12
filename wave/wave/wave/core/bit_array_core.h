#pragma once

#include <cstddef>
#include <array>
#include "bits.h"

template <size_t N>
struct bit_array_core
{
    typedef bit_array_core<N>     self_t;
    typedef uint64_t              elem_t;

    enum { BITS = N, ELEM_BITS = sizeof(elem_t) * 8, ELEM_COUNT = N / ELEM_BITS };

    typedef std::array<elem_t, ELEM_COUNT> set_t;

    set_t _set;

    inline bit_array_core() : _set({ 0 }) {}
    inline bit_array_core(bool u) : _set({ u ? ~elem_t(0) : 0 }) {}
    inline bit_array_core(const self_t &u) : _set(u._set) {}

    inline bool              get_default()                    const { return 0; }
    inline bool              set_default(bool def)                  {}

    inline void              assign(const self_t &u)                {  _set = u._set; }
    inline void              assign(bool u)                         { _set = set_t({ u ? ~elem_t(0) : 0 }); }

    inline bool              is_equal(const self_t &u)        const { return bit_equal(_set, u._set); }
    inline void              op_and(const self_t &u)          const { bit_and(_set, u._set); }
    inline void              op_or(const self_t &u)           const { bit_or(_set, u._set); }
    inline void              op_xor(const self_t &u)          const { bit_xor(_set, u._set); }
    inline void              op_nand(const self_t &u)         const { bit_nand(_set, u._set); }
    inline void              op_nor(const self_t &u)          const { bit_nor(_set, u._set); }
    inline void              op_xnor(const self_t &u)         const { bit_xnor(_set, u._set); }
    inline void              op_not()                               { bit_not(_set); }

    inline constexpr size_t  element_count()                  const { return ELEM_COUNT; }
    inline constexpr size_t  element_bits()                   const { return ELEM_BITS; }
    inline constexpr size_t  size()                           const { return BITS; }

    inline const elem_t     *data()                           const { return _set.data(); }
    inline elem_t           *data()                                 { return _set.data(); }

    inline size_t            count()                          const { return bit_count(_set); }
    inline bool              empty()                          const { return bit_empty(_set); }
    inline bool              full()                           const { return bit_full(_set); }

    inline void              set(size_t i)                          { bit_set(_set, i); }
    inline void              unset(size_t i)                        { bit_unset(_set, i); }
    inline bool              test(size_t i)                   const { return bit_test(_set, i); }

    /* TODO need shift and rotate! */

    inline void              resize_if_needed_for(size_t i)         {}
};
