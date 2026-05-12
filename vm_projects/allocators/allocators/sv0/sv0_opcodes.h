#pragma once

#include <iostream>
#include <cmath>
#include <array>
#include <vector>

#include "types.h"
#include "function.h"

namespace sv0
{

struct opcodes
{
    enum class Code : code_t
    {
        HALT = 0, RET, PUSH, DROP, DUPE, SWAP, OVER,
        
        // print/debug
        PRTOP, PRINT,
        
        // s-expr
        PUSHNIL, PAIR, HEAD, TAIL, LENGTH,
        
        // functions
        CALL, FUNC, YIELD,
        
        // bool, math, compares
        NOT, BOOL, AND, OR, ADD, SUB, MUL, DIV, MOD, EQ, LT, LTE, GT, GTE, INC, DEC,
       
        JMP, // how we gonna do this?
    };
    
    static constexpr const char *name(Code code)
    {
        constexpr const char *names[] = {

        };
        return names[code];
    }
};

}
