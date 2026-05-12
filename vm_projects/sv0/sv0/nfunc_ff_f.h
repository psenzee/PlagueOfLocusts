#pragma once

#include "native_func.h"

class nfunc_ff_f : public native_func
{
    typedef float (*fptr_t)(float, float);

    fptr_t _fptr;

public:
    
    nfunc_ff_f(fptr_t fptr) : native_func(2), _fptr(fptr) {}
    
    void call(vm *vm) { float a = vm->_s.at(0).as_float(), b = vm->_s.at(1).as_float(); vm->_s.at(1) = _fptr(a, b); vm->_s.drop(); }
};

