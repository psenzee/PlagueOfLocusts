#pragma once

#include <iostream>
#include "types.h"
#include "type_list.h"

struct function_vm
{
    const code_t   *ptr;
    uint16_t        count;
    
    constexpr function_vm() : ptr(nullptr), count(0) {}
    constexpr function_vm(const code_t *ptr, uint16_t count) : ptr(ptr), count(count) {}

    std::ostream &print(std::ostream &os) const;
};

struct env;

struct function
{
    typedef type_list<TypeMask, 4> type_list_t;
    typedef void (*fp_t)(env *);

    enum Type : uint8_t { NFUNC = 0, VFUNC };
    enum { FLAG_SFX = 1, FLAG_COMMUTATIVE = 2 };

    Type            type;
    const char     *name;
    fp_t            fptr;
    function_vm     fip;
    type_list_t     in, out;
    uint16_t        options;

    std::ostream &print(std::ostream &os) const;
    
    constexpr function() : type(NFUNC), name(nullptr), fptr(nullptr), options(0) {}
    constexpr function(const char *name, fp_t fptr, const type_list_t &in, const type_list_t &out, uint16_t options = 0)
        : type(NFUNC), name(name), fptr(fptr), in(in), out(out), options(options) {}
};
