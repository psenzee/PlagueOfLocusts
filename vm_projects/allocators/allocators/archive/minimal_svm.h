#include <iostream>
#include <cmath>
#include <array>
#include <vector>

#include "types.h"
#include "function.h"

struct tag : public tag_type
{
    typedef void (*fp_t)(svm *, env *);

    Type          type;
    uint16_t      signature_id;
    union {
        object       *obj;
        int64_t       i;
        float         f;
        void         *p;
        const code_t *fip;
        fp_t          fptr;
    };

    std::ostream &print(std::ostream &os) const;

    tag &seti(int64_t v)                        { type = INT;     i   = v; return *this; }
    tag &setf(float v)                          { type = FLOAT;   f   = v; return *this; }
    tag &set(object *v)                         { type = OBJECT;  obj = v; return *this; }
    tag &set(uint16_t id, const signature &fun) { type = fun.type; signature_id = id; if (type == FUNC) fip = fun.fip.fip; else fptr = fun.fptr; return *this; }

    tag &f2i()                      { return type == FLOAT ? seti(static_cast<int64_t>(f)) : *this; }
    tag &i2f()                      { return type == INT   ? setf(static_cast<float>(i))   : *this; }
};

inline std::ostream &operator<<(std::ostream &os, const tag &t) { return t.print(os); }

struct object : public tag_type
{
    Type type;

    bool                       is(Type t)               const { return type == t; }
    bool                       is_atom()                const { return type < PAIR; }
    bool                       is_number()              const { return type == INT || type == FLOAT; }
    virtual std::ostream      &print(std::ostream &os)  const { return os << "[object]"; }
    virtual bool               equals(const object *e)  const { return e->type == type; }

    object(Type type = INT) : type(type) {}
};

struct pair : public object
{
    tag head, tail;
    pair(tag head, tag tail) : object(tag::PAIR), head(head), tail(tail) {}
    bool is_nil() const { return head.is_nil() && tail.is_nil(); }
    virtual std::ostream &print(std::ostream &os)  const { return os << "(" << head << " " << tail << ")"; }
};

inline void test_tag()
{
    tag t;
    t.seti(17);
    std::cout << "test tag " << t << std::endl;
}

struct env
{
    stack<tag>            operands;
    stack<const code_t *> returns;
};

struct opcodes
{
    enum Code : code_t
    {
        _HALT = 0, _RET,

        // stack
        _PUSH, _DROP, _DUPE, _SWAP, _OVER,

        // conversion
        _F2I, _I2F, _IBOX, _FBOX, _BOX, _UNBOX,

        // print/debug
        _PRTOP, _PRINT,

        // s-expr
        _PUSHNIL, _PAIR, _HEAD, _TAIL, _LENGTH,

        // bool
        _NOT, _BOOL, _AND, _OR,
        
        _CALL,
        _FUNC,

        // untyped
        _ADD, _SUB, _MUL, _DIV, _MOD, _EQ, _LT, _LTE, _GT, _GTE, _SHL, _SHR, _XOR, _INC, _DEC,

        // integer
        _IPUSHC, _IPUSHM, _IPUSH0, _IPUSH1, _INEG,
        _IADD, _ISUB, _IMUL, _IDIV, _IMOD, _IEQ, _ILT, _ILTE, _IGT, _IGTE,
        _ISHL, _ISHR, _IAND, _IOR,  _IXOR, _IINC, _IDEC, _INOT, _ISHR1, _ISHL1,

        // float
        _FPUSHC, _FPUSH0, _FPUSH1, _FNEG,
        _FADD, _FSUB, _FMUL, _FDIV, _FEQ, _FLT, _FLTE, _FGT, _FGTE, _FSIN,

        _JMP, // how we gonna do this?
    };

    static constexpr const char *name(Code code)
    {
        constexpr const char *names[] = {
            "HALT", "RET",

            // stack
            "PUSH", "DROP", "DUPE", "SWAP", "OVER",

            // conversion
            "F2I", "I2F", "IBOX", "FBOX", "BOX", "UNBOX",

            // print/debug
            "PRTOP", "PRINT",

            // s-expr
            "PUSHNIL", "PAIR", "HEAD", "TAIL", "LENGTH",

            // bool
            "NOT", "BOOL", "AND", "OR",
            
            "CALL",
            "FUNC",

            // untyped
            "ADD", "SUB", "MUL", "DIV", "MOD", "EQ", "LT", "LTE", "GT", "GTE",
            "SHL", "SHR", "XOR", "INC", "DEC",

            // integer
            "IPUSHC", "IPUSHM", "IPUSH0", "IPUSH1", "INEG",
            "IADD", "ISUB", "IMUL", "IDIV", "IMOD", "IEQ", "ILT", "ILTE", "IGT", "IGTE",
            "ISHL", "ISHR", "IAND", "IOR",  "IXOR", "IINC", "IDEC", "INOT", "ISHR1", "ISHL1",

            // float
            "FPUSHC", "FPUSH0", "FPUSH1", "FNEG",
            "FADD", "FSUB", "FMUL", "FDIV", "FEQ", "FLT", "FLTE", "FGT", "FGTE", "FSIN",

            "JMP",
        };
        return names[code];
    }
    /*
    static Code get_code(const char *name)
    {
        struct code_name { const char *name; Code code; bool operator<(const code_name &u) { return strcasecmp(name, u.name) < 0; } };
        static constexpr const code_name codes[] = {
            { "ADD",     _ADD },
            { "AND",     _AND },
            { "BOOL",    _BOOL },
            { "BOX",     _BOX },
            { "CALL",    _CALL },
            { "DEC",     _DEC },
            { "DIV",     _DIV },
            { "DROP",    _DROP },
            { "DUPE",    _DUPE },
            { "EQ",      _EQ },
            { "F2I",     _F2I },
            { "FADD",    _FADD },
            { "FBOX",    _FBOX },
            { "FDIV",    _FDIV },
            { "FEQ",     _FEQ },
            { "FGT",     _FGT },
            { "FGTE",    _FGTE },
            { "FLT",     _FLT },
            { "FLTE",    _FLTE },
            { "FMUL",    _FMUL },
            { "FNEG",    _FNEG },
            { "FPUSH0",  _FPUSH0 },
            { "FPUSH1",  _FPUSH1 },
            { "FPUSHC",  _FPUSHC },
            { "FSIN",    _FSIN },
            { "FSUB",    _FSUB },
            { "FUNC",    _FUNC },
            { "GT",      _GT },
            { "GTE",     _GTE },
            { "HALT",    _HALT },
            { "HEAD",    _HEAD },
            { "I2F",     _I2F },
            { "IADD",    _IADD },
            { "IAND",    _IAND },
            { "IBOX",    _IBOX },
            { "IDEC",    _IDEC },
            { "IDIV",    _IDIV },
            { "IEQ",     _IEQ },
            { "IGT",     _IGT },
            { "IGTE",    _IGTE },
            { "IINC",    _IINC },
            { "ILT",     _ILT },
            { "ILTE",    _ILTE },
            { "IMOD",    _IMOD },
            { "IMUL",    _IMUL },
            { "INC",     _INC },
            { "INEG",    _INEG },
            { "INOT",    _INOT },
            { "IOR",     _IOR },
            { "IPUSH0",  _IPUSH0 },
            { "IPUSH1",  _IPUSH1 },
            { "IPUSHC",  _IPUSHC },
            { "IPUSHM",  _IPUSHM },
            { "ISHL",    _ISHL },
            { "ISHL1",   _ISHL1 },
            { "ISHR",    _ISHR },
            { "ISHR1",   _ISHR1 },
            { "ISUB",    _ISUB },
            { "IXOR",    _IXOR },
            { "JMP",     _JMP },
            { "LENGTH",  _LENGTH },
            { "LT",      _LT },
            { "LTE",     _LTE },
            { "MOD",     _MOD },
            { "MUL",     _MUL },
            { "NOT",     _NOT },
            { "OR",      _OR },
            { "OVER",    _OVER },
            { "PAIR",    _PAIR },
            { "PRINT",   _PRINT },
            { "PRTOP",   _PRTOP },
            { "PUSH",    _PUSH },
            { "PUSHNIL", _PUSHNIL },
            { "RET",     _RET },
            { "SHL",     _SHL },
            { "SHR",     _SHR },
            { "SUB",     _SUB },
            { "SWAP",    _SWAP },
            { "TAIL",    _TAIL },
            { "UNBOX",   _UNBOX },
            { "XOR",     _XOR },
        };
        const code_name *b = codes, *e = codes + (sizeof(codes) / sizeof(codes[0]));
        const code_name *i = std::upper_bound(b, e);
    }*/
};

struct svm : public opcodes
{
    const code_t *_ip;
    tag           *_stack, *_back, *_; // underscore is top of stack
    
    std::ostream &print_stack(std::ostream &os) const;

    svm(size_t stack_count, const code_t *data);

    size_t run(std::ostream &os);
    
    typedef float   (*ff_fp)(float);
    void          _ff(ff_fp fn)           { _->f = fn(_->f); }
};

void test_svm();
