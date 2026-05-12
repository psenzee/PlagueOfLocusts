#pragma once

#include <iostream>
#include <deque>
#include "constants.h"
#include "elems.h"

namespace tl
{
struct compile_context
{
    constants               consts;
    std::deque<std::string> assembly;
    
    std::ostream &print(std::ostream &os)
    {
        os << std::endl;
        consts.print(os);
        for (const auto &line : assembly) {
            os << line << std::endl;
        }
        return os;
    }
    
    void push(elem *value)
    {
        static constexpr std::array<const char *, elem::_MAX_TYPE> opcodes = {
            "PUSHNIL", "PUSHSTR", "PUSHSYM", "PUSHI", "PUSHF", "PUSHLIST", "PUSHLAMBDA", "PUSHFN"
        };
        const char *opcode = opcodes[value ? value->_type : elem::NIL];
        std::ostringstream oss;
        value->print(oss << opcode << " #" << consts.add(value) << " ; ");
        assembly.push_front(oss.str());
    }
};
}
