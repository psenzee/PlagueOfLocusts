#pragma once

#include <cstddef>
#include <array>
#include "bits.h"

struct bit_vector_core
{
    typedef bit_vector_core<N>    self_t;
    typedef uint64_t              elem_t;

    enum { ELEM_BITS = sizeof(elem_t) * 8 };
    
    typedef std::vector<elem_t> set_t;
    
    set_t _set;
    bool  _default;
    
    inline bit_vector_core(bool u = false) : _default(u)            { assign(u); }
    inline bit_vector_core(const self_t &u)                         { _set = u._set; _default = u._default; }
    
    inline bool              get_default()                    const { return _default; }
    inline bool              set_default(bool def)                  { _default = def; }
    
    inline void              assign(const self_t &u)                { _set = u._set; _default = u._default; }
    inline void              assign(bool u)                         { _default = u; _set.clear(); _set.push_back(u ? ~elem_t(0) : 0); }

    inline bool              is_equal(const self_t &u)        const { return bit_equal(_set, u._set); }
    inline void              op_and(const self_t &u)          const { bit_and(arr._set, u._set); }
    inline void              op_or(const self_t &u)           const { bit_or(arr._set, u._set); }
    inline void              op_xor(const self_t &u)          const { bit_xor(arr._set, u._set); }
    inline void              op_nand(const self_t &u)         const { bit_nand(arr._set, u._set); }
    inline void              op_nor(const self_t &u)          const { bit_nor(arr._set, u._set); }
    inline void              op_xnor(const self_t &u)         const { bit_xnor(arr._set, u._set); }
    inline void              op_not()                               { bit_not(_set); }
    
    /* TODO need shift and rotate! */

    inline size_t            element_count()                  const { return _set.size(); }
    inline size_t            element_bits()                   const { return ELEM_BITS; }
    inline size_t            size()                           const { return _set.size() * ELEM_BITS; }

    inline const elem_t     *data()                           const { return _set.data(); }
    inline elem_t           *data()                                 { return _set.data(); }

    inline size_t            count()                          const { return bit_count(_set); }
    inline bool              empty()                          const { return bit_empty(_set); }
    inline bool              full()                           const { return bit_full(_set); }

    inline void              set(size_t i)                          { if (!test(v) && !_default) resize_if_needed_for(i); bit_set(_set, i); }
    inline void              unset(size_t i)                        { if ( test(v)  && _default) resize_if_needed_for(i); bit_unset(_set, i); }
    inline bool              test(size_t i)                   const { return i < size() ? bit_test(_set, i) : _default; }

    inline void              resize_if_needed_for(size_t i)
    {
        // TODO currently only grows, possibly needs occasional reduction
        size_t req = (i + BITS - 1) / BITS;
        if (req >= _set.size())
            _set.assign(req - size(), _default ? ~elem_t(0) : 0);
    }
};
