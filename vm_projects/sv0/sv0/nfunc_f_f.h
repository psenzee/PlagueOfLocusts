#pragma once

#include "native_func.h"

class nfunc_f_f : public native_func
{
    typedef float (*fptr_t)(float);

    fptr_t _fptr;

public:
    
    nfunc_f_f(fptr_t fptr) : native_func(1), _fptr(fptr) {}
    
    void call(vm *vm) { vm->_frame->_s.top() = _fptr(vm->_frame->_s.top().as_float()); }
};

