#pragma once

#include "opcodes.h"
#include "object.h"
#include "frame.h"
#include "func.h"
#include "vm.h"

class user_func : public func
{
    const code_t *_ip;
    uint8_t       _locals;

public:

    user_func(const code_t *ip, uint8_t args, uint8_t locals) : func(args), _ip(ip), _locals(locals) {}

    void call(vm *vm)
    {
        frame *f = vm->_frame->child_frame();
        // .. todo
        vm->_frame = f;
        f->_ip = _ip;
    }

    std::ostream &print(std::ostream &os) const { return os << "#<fn-user " << _ip << "$" << arg_count() << "$" << _locals << ">"; }
};
