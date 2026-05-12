#include "function.h"
#include "opcodes.h"

std::ostream &function_vm::print(std::ostream &os) const
{
    os << "[";
    bool first = true;
    for (size_t i = 0; i < count; i++) {
        os << (first ? "" : " ");
        uint16_t p = ptr[i];
        if (p < 0) { os << p; }
        else { os << opcodes::name(static_cast<opcodes::Code>(p)); }
        first = false;
    }
    return os << "]";
}

std::ostream &function::print(std::ostream &os) const
{
    if (type == function::Type::NFUNC) {
        return os << "fn/" << name << "#" << (void *)fptr;
    }
    os << "fp/" << name << "#";
    return fip.print(os);
}
