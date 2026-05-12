#pragma once

#include <cstddef>
#include <cstdint>

typedef uint8_t code_t;

enum class Code : code_t
{
    HALT = 0, RET, DROP, DUPE, SWAP, OVER,

    PUSHNIL, PUSH0, PUSH1, PUSHF,
    
    PUSHI8, PUSHI16, PUSHI32,
    PUSHC8, PUSHC16, PUSHC32,

    // I feel like we only need a 16-bit jump
    JMP8,    JMP16,   JMP32,
    JLTZ8,   JLTZ16,  JLTZ32,
    JLTEZ8,  JLTEZ16, JLTEZ32,
    JGTZ8,   JGTZ16,  JGTZ32,
    JGTEZ8,  JGTEZ16, JGTEZ32,
    JZ8,     JZ16,    JZ32,
    JNZ8,    JNZ16,   JNZ32,
    
    // print/debug
    PRTOP, PRINT,
    
    // s-expr
    PAIR, HEAD, TAIL, LENGTH,
    
    // functions
    CALL, FUNC, YIELD,
    
    // bool, math, compares
    NOT, BOOL, AND, OR, ADD, SUB, MUL, DIV, MOD, EQ, LT, LTE, GT, GTE, INC, DEC,
    
    COMPARE, // this is a 3-way compare that only consumes its top arg. For numbers, just a sub that only consumes the top
    
    _MAX_
};

struct opcodes
{
    enum { IMM_NONE = 0, IMM_INT8 = 1, IMM_INT16 = 2, IMM_INT32 = 3, IMM_FLOAT = 4 };
    enum { CID_NONE = 0, CID_INT8 = 1, CID_INT16 = 2, CID_INT32 = 3 };
    
    struct info_t
    {
        const char          *name;
        uint64_t             meta;
        std::array<Code, 5>  sizes;
    };
    
    struct alias_t
    {
        const char *alias, *name;
    };

    static constexpr const info_t *info(code_t code) { return code < static_cast<uint8_t>(Code::_MAX_) ? &INFO[code] : nullptr; }
    static constexpr const info_t *info(Code code)   { return info(static_cast<code_t>(code)); }
    static constexpr const char   *name(code_t code) { auto inf = info(code); return inf ? inf->name : nullptr; }
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

        { "PUSH",  "PUSHI8" },
        { "JMP",   "JMP8"   },

        { "JLTZ",  "JLTZ8"  },
        { "J<0",   "JLTZ8"  },
        
        { "JLTEZ", "JLTEZ8" },
        { "J<=0",  "JLTEZ8" },
        
        { "JGTZ",  "JGTZ8"  },
        { "J>0",   "JGTZ8"  },
        
        { "JGTEZ", "JGTEZ8" },
        { "J>=0",  "JGTEZ8" },
        
        { "JZ",    "JZ8"    },
        { "J==0",  "JZ8"    },
        
        { "JNZ",   "JNZ8"   },
        { "J!=0",  "JNZ8"   },

        { nullptr, nullptr  }
    };
    
    static constexpr const info_t  INFO[] = {

        info_t { "HALT",    0 }, info_t { "RET",     0 }, info_t { "DROP",    0 }, info_t { "DUPE",    0 },
        info_t { "SWAP",    0 }, info_t { "OVER",    0 },
        
        info_t { "PUSHNIL", 0 }, info_t { "PUSH0",   0 }, info_t { "PUSH1",   0 },

        info_t { "PUSHF",   IMM_FLOAT, { Code::HALT, Code::PUSHF,  Code::PUSHF,   Code::HALT, Code::PUSHF   } },
        
        info_t { "PUSHI8",  IMM_INT8,  { Code::HALT, Code::PUSHI8, Code::PUSHI16, Code::HALT, Code::PUSHI32 } },
        info_t { "PUSHI16", IMM_INT16, { Code::HALT, Code::PUSHI8, Code::PUSHI16, Code::HALT, Code::PUSHI32 } },
        info_t { "PUSHI32", IMM_INT32, { Code::HALT, Code::PUSHI8, Code::PUSHI16, Code::HALT, Code::PUSHI32 } },

        info_t { "PUSHC8",  CID_INT8,  { Code::HALT, Code::PUSHC8, Code::PUSHC16, Code::HALT, Code::PUSHC32 } },
        info_t { "PUSHC16", CID_INT16, { Code::HALT, Code::PUSHC8, Code::PUSHC16, Code::HALT, Code::PUSHC32 } },
        info_t { "PUSHC32", CID_INT32, { Code::HALT, Code::PUSHC8, Code::PUSHC16, Code::HALT, Code::PUSHC32 } },

        info_t { "JMP8",    CID_INT8,  { Code::HALT, Code::JMP8,   Code::JMP16,   Code::HALT, Code::JMP32   } },
        info_t { "JMP16",   CID_INT16, { Code::HALT, Code::JMP8,   Code::JMP16,   Code::HALT, Code::JMP32   } },
        info_t { "JMP32",   CID_INT32, { Code::HALT, Code::JMP8,   Code::JMP16,   Code::HALT, Code::JMP32   } },
        
        info_t { "JLTZ8",   CID_INT8,  { Code::HALT, Code::JLTZ8,  Code::JLTZ16,  Code::HALT, Code::JLTZ32  } },
        info_t { "JLTZ16",  CID_INT16, { Code::HALT, Code::JLTZ8,  Code::JLTZ16,  Code::HALT, Code::JLTZ32  } },
        info_t { "JLTZ32",  CID_INT32, { Code::HALT, Code::JLTZ8,  Code::JLTZ16,  Code::HALT, Code::JLTZ32  } },
        
        info_t { "JLTEZ8",  CID_INT8,  { Code::HALT, Code::JLTEZ8, Code::JLTEZ16, Code::HALT, Code::JLTEZ32  } },
        info_t { "JLTEZ16", CID_INT16, { Code::HALT, Code::JLTEZ8, Code::JLTEZ16, Code::HALT, Code::JLTEZ32  } },
        info_t { "JLTEZ32", CID_INT32, { Code::HALT, Code::JLTEZ8, Code::JLTEZ16, Code::HALT, Code::JLTEZ32  } },
        
        info_t { "JGTZ8",   CID_INT8,  { Code::HALT, Code::JLTZ8,  Code::JLTZ16,  Code::HALT, Code::JGTZ32   } },
        info_t { "JGTZ16",  CID_INT16, { Code::HALT, Code::JLTZ8,  Code::JLTZ16,  Code::HALT, Code::JGTZ32   } },
        info_t { "JGTZ32",  CID_INT32, { Code::HALT, Code::JLTZ8,  Code::JLTZ16,  Code::HALT, Code::JGTZ32   } },
        
        info_t { "JGTEZ8",  CID_INT8,  { Code::HALT, Code::JGTEZ8, Code::JGTEZ16, Code::HALT, Code::JGTEZ32  } },
        info_t { "JGTEZ16", CID_INT16, { Code::HALT, Code::JGTEZ8, Code::JGTEZ16, Code::HALT, Code::JGTEZ32  } },
        info_t { "JGTEZ32", CID_INT32, { Code::HALT, Code::JGTEZ8, Code::JGTEZ16, Code::HALT, Code::JGTEZ32  } },
        
        info_t { "JZ8",     CID_INT8,  { Code::HALT, Code::JZ8,    Code::JZ16,    Code::HALT, Code::JZ32     } },
        info_t { "JZ16",    CID_INT16, { Code::HALT, Code::JZ8,    Code::JZ16,    Code::HALT, Code::JZ32     } },
        info_t { "JZ32",    CID_INT32, { Code::HALT, Code::JZ8,    Code::JZ16,    Code::HALT, Code::JZ32     } },
        
        info_t { "JNZ8",    CID_INT8,  { Code::HALT, Code::JNZ8,   Code::JNZ16,   Code::HALT, Code::JNZ32    } },
        info_t { "JNZ16",   CID_INT16, { Code::HALT, Code::JNZ8,   Code::JNZ16,   Code::HALT, Code::JNZ32    } },
        info_t { "JNZ32",   CID_INT32, { Code::HALT, Code::JNZ8,   Code::JNZ16,   Code::HALT, Code::JNZ32    } },

        info_t { "PRTOP",   0 }, info_t { "PRINT",   0 }, info_t { "PAIR",    0 }, info_t { "HEAD",    0 },
        info_t { "TAIL",    0 }, info_t { "LENGTH",  0 }, info_t { "CALL",    0 }, info_t { "FUNC",    0 },
        info_t { "YIELD",   0 }, info_t { "NOT",     0 }, info_t { "BOOL",    0 }, info_t { "AND",     0 },
        info_t { "OR",      0 }, info_t { "ADD",     0 }, info_t { "SUB",     0 }, info_t { "MUL",     0 },
        info_t { "DIV",     0 }, info_t { "MOD",     0 }, info_t { "EQ",      0 }, info_t { "LT",      0 },
        info_t { "LTE",     0 }, info_t { "GT",      0 }, info_t { "GTE",     0 }, info_t { "INC",     0 },
        info_t { "DEC",     0 }, info_t { "COMPARE", 0 },
    };
};
