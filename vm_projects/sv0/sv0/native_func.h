#pragma once

#include "opcodes.h"
#include "object.h"
#include "frame.h"
#include "func.h"
#include "vm.h"

struct native_func : public func
{
    native_func(uint8_t args) : func(args) {}
    
    bool is_native() const { return true; }

    std::ostream &print(std::ostream &os) const { return os << "#<fn-native " << this << ">"; }
};

