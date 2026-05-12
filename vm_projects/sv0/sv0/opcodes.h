#pragma once

#include <cstddef>
#include <cstdint>
#include "utils.h"

typedef uint8_t code_t;

enum class Code : code_t
{
    HALT = 0,
    
    I8, I16, I32, I64, U8, U16, U32, U64, F32, NIL,
    
    DEFINE_CONST, CONST, LABEL, LOCAL, SET,
    
    RET, DROP, DUPE, SWAP, OVER,
    
    JMP, JLTZ, JLTEZ, JGTZ, JGTEZ, JZ, JNZ,
    
    MIN, MAX,
    
    // print/debug
    PRTOP, PRINT,
    
    // s-expr
    PAIR, HEAD, TAIL, LENGTH, LIST, LISTR,
    
    // functions
    CALL, FUNC, YIELD,
    
    // bool, math, compares
    NOT, BOOL, BIT_AND, BIT_OR, BIT_XOR, BIT_NOT, ADD, SUB, MUL, DIV, MOD, INC, DEC,
    
    COMPARE, // signed compare like strcmp or simple subtraction
    
    ERROR, // code gen error
    
    _MAX_
};

struct opcodes
{
    enum { IMMEDIATE = 0, CONSTANT_ID = 8 };
    enum { VAL_NONE = 0, VAL_INT8 = 1, VAL_INT16 = 2, VAL_INT32 = 3, VAL_INT64 = 4, VAL_UINT8 = 6, VAL_UINT16 = 7, VAL_UINT32 = 8, VAL_UINT64 = 9, VAL_FLOAT32 = 10 };
    enum { I0 = 0 << 4, I1 = 1 << 4, I2 = 2 << 4, I3 = 3 << 4, I4 = 4 << 4, I5 = 5 << 4, I6 = 6 << 4, I7 = 7 << 4, I8 = 8 << 4, IM = 9 << 4, IM2 = 10 << 4,
           O0 = 0 << 8, O1 = 1 << 8, O2 = 2 << 8, O3 = 3 << 8, FX = 4 << 8 };

    static constexpr bool    is_immediate_signed(Code code)    { return is_in(code, Code::I8, Code::I16, Code::I32, Code::I64); }
    static constexpr bool    is_immediate_unsigned(Code code)  { return is_in(code, Code::U8, Code::U16, Code::U32, Code::U64) || is_instruction_uint16(code); }
    static constexpr bool    is_instruction_uint16(Code code)  { return is_in(code, Code::CONST, Code::LOCAL, Code::SET, Code::LIST, Code::LISTR, Code::CALL); }
    static constexpr bool    is_immediate_integer(Code code)   { return is_immediate_signed(code) || is_immediate_unsigned(code); }
    static constexpr bool    is_immediate_float(Code code)     { return is_in(code, Code::F32); }
    static constexpr bool    is_immediate(Code code)           { return is_immediate_integer(code) || is_immediate_float(code); }
    static constexpr uint8_t immediate_size(Code code)
    {
        switch (code) {
        case Code::U8:    case Code::I8:        return 1;
        case Code::JMP:   case Code::JLTZ:
        case Code::JLTEZ: case Code::JGTZ:
        case Code::JGTEZ: case Code::JZ:
        case Code::JNZ:   case Code::CONST:
        case Code::LOCAL: case Code::SET:
        case Code::LIST:  case Code::LISTR:
        case Code::CALL:
        case Code::U16:   case Code::I16:       return 2;
        case Code::U32:
        case Code::I32:   case Code::F32:       return 4;
        case Code::U64:   case Code::I64:       return 8;
        default:                                return 0;
        }
    }
    static constexpr bool    is_const(Code code)               { return is_in(code, Code::CONST); }
    static constexpr bool    is_jump(Code code)                { return is_in(code, Code::JMP, Code::JLTZ, Code::JLTEZ, Code::JGTZ, Code::JGTEZ, Code::JZ, Code::JNZ); }

    struct info_t
    {
        const char                            *name;
        uint64_t                               meta;
        std::array<Code, sizeof(uint64_t) + 1> sizes;
    };
    
    struct alias_t
    {
        const char *alias, *name;
    };
    
    static constexpr const info_t *info(code_t code) { return code < static_cast<uint8_t>(Code::_MAX_) ? &INFO[code] : nullptr; }
    static constexpr const info_t *info(Code code)   { return info(static_cast<code_t>(code)); }
    static constexpr const char   *name(code_t code) { auto inf = info(code); return inf ? inf->name : "<unknown>"; }
    static constexpr const char   *name(Code code)   { return name(static_cast<code_t>(code)); }
    
    static int find(const char *opcode)
    {
        for (code_t i = 0; ALIAS[i].alias; i++) {
            if (strcasecmp(opcode, ALIAS[i].alias) == 0) {
                opcode = ALIAS[i].name;
                break;
            }
        }
        for (code_t i = 0; i < static_cast<code_t>(Code::_MAX_); ++i) {
            if (strcasecmp(opcode, name(i)) == 0) {
                return i;
            }
        }
        return -1;
    }
    
private:
    
    static constexpr const alias_t ALIAS[] = {
        
        { "J<0",       "JLTZ"         },
        { "J<=0",      "JLTEZ"        },
        { "J>0",       "JGTZ"         },
        { "J>=0",      "JGTEZ"        },
        { "J==0",      "JZ"           },
        { "J!=0",      "JNZ"          },
        { "I8",        "PUSH.I8"      },
        { "I16",       "PUSH.I16"     },
        { "I32",       "PUSH.I32"     },
        { "PUSH",      "PUSH.I32"     },
        { "PUSH.I",    "PUSH.I32"     },
        { "I64",       "PUSH.I64"     },
        { "U8",        "PUSH.U8"      },
        { "U16",       "PUSH.U16"     },
        { "U32",       "PUSH.U32"     },
        { "PUSH.U",    "PUSH.U32"     },
        { "U64",       "PUSH.U64"     },
        { "F32",       "PUSH.F32"     },
        { "PUSH.F",    "PUSH.F32"     },
        { "NIL",       "PUSH.NIL"     },
        
        { nullptr, nullptr }
    };
   
    static constexpr const info_t INFO[] = {
        
        info_t { "HALT",         0 },
        
        info_t { "PUSH.I8",      VAL_INT8    },
        info_t { "PUSH.I16",     VAL_INT16   },
        info_t { "PUSH.I32",     VAL_INT32   },
        info_t { "PUSH.I64",     VAL_INT64   },
        info_t { "PUSH.U8",      VAL_UINT8   },
        info_t { "PUSH.U16",     VAL_UINT16  },
        info_t { "PUSH.U32",     VAL_UINT32  },
        info_t { "PUSH.U64",     VAL_UINT64  },
        info_t { "PUSH.F32",     VAL_FLOAT32 },
        info_t { "PUSH.NIL",     0 },

        info_t { "DEFINE_CONST", IM2 },
        info_t { "CONST",        VAL_UINT16  },
        info_t { "LABEL",        IM },
        info_t { "LOCAL",        VAL_UINT16  },
        info_t { "SET",          VAL_UINT16  },

        info_t { "RET",          0 },
        
        info_t { "DROP",         I1 },
        info_t { "DUPE",         I1 | O2 },
        info_t { "SWAP",         I2 | O2 },
        info_t { "OVER",         I3 | O3 },
        
        info_t { "JMP",          IM },             
        info_t { "JLTZ",         IM },
        info_t { "JLTEZ",        IM },
        info_t { "JGTZ",         IM },
        info_t { "JGTEZ",        IM },
        info_t { "JZ",           IM },
        info_t { "JNZ",          IM },

        info_t { "MIN",          I2 | O2 },
        info_t { "MAX",          I2 | O2 },

        info_t { "PRTOP",        I1 | O1 | FX },
        info_t { "PRINT",        I1 | O1 | FX },
        info_t { "PAIR",         I2 | O1    },
        info_t { "HEAD",         I1 | O1    },
        info_t { "TAIL",         I1 | O1    },
        info_t { "LENGTH",       I1 | O1    },
        
        info_t { "LIST",         IM | O1 | VAL_UINT16 },
        info_t { "LISTR",        IM | O1 | VAL_UINT16 },

        info_t { "CALL",         0 },
        info_t { "FUNC",         0 },
        info_t { "YIELD",        0 },

        info_t { "NOT",          I1 | O1 },        
        info_t { "BOOL",         I1 | O1 },

        info_t { "BIT_AND",      I2 | O1 },
        info_t { "BIT_OR",       I2 | O1 },
        info_t { "BIT_XOR",      I2 | O1 },
        info_t { "BIT_NOT",      I2 | O1 },
        info_t { "ADD",          I2 | O1 },
        info_t { "SUB",          I2 | O1 },
        info_t { "MUL",          I2 | O1 },
        info_t { "DIV",          I2 | O1 },
        info_t { "MOD",          I2 | O1 },

        info_t { "INC",          I1 | O1 },
        info_t { "DEC",          I1 | O1 },

        info_t { "COMPARE",      I2 | O1 },
        
        info_t { "ERROR",        0 },
    };
};
