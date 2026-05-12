#pragma once

#include "opcodes.h"
#include "object.h"
#include "frame.h"

class vm;

class func : public object
{
    uint8_t _args;

public:

    func(uint8_t args) : _args(args) {}
    
    Type    type()      const { return FUNC; }
    
    virtual bool    is_native() const { return false; }
    virtual uint8_t arg_count() const { return _args; }
    
    virtual void    call(vm *vm) { }

    std::ostream &print(std::ostream &os) const { return os << "#<fn [unknown]$" << arg_count() << ">"; }
};
