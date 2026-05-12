#include <iostream>
#include <cmath>

typedef uint16_t code_t;

struct tag_type { enum Type: uint8_t { NONE = 0, INT, FLOAT, OBJECT, PAIR, FNN0, FNN1, FNN2, FNN3, FNP }; };

struct object;

struct tag : public tag_type
{
    typedef tag (*fp0)();
    typedef tag (*fp1)(tag *);
    typedef tag (*fp2)(tag *, tag *);
    typedef tag (*fp3)(tag *, tag *, tag *);

    union {
        Type          type;
        uint64_t      _;
    };
    union {
        object       *obj;
        int64_t       i;
        float         f;
        void         *p;
        const code_t *fnp;
        fp0           fn0;
        fp1           fn1;
        fp2           fn2;
        fp3           fn3;
    };
    
    std::ostream &print(std::ostream &os) const;

    tag &seti(int64_t v)            { type = INT;    i   = v; return *this; }
    tag &setf(float v)              { type = FLOAT;  f   = v; return *this; }
    tag &set(object *v)             { type = OBJECT; obj = v; return *this; }
    tag &set(fp0 v)                 { type = FNN0;   fn0 = v; return *this; }
    tag &set(fp1 v)                 { type = FNN1;   fn1 = v; return *this; }
    tag &set(fp2 v)                 { type = FNN2;   fn2 = v; return *this; }
    tag &set(fp3 v)                 { type = FNN3;   fn3 = v; return *this; }
    tag &set(const code_t *v)       { type = FNP;    fnp = v; return *this; }

    bool is_nil()             const { return type == NONE; }
    bool is_int()             const { return type == INT; }
    bool is_float()           const { return type == FLOAT; }
    bool is_native_function() const { return type == FNN0 || type == FNN1 || type == FNN2 || type == FNN3; }
    bool is_vm_function()     const { return type == FNP; }
    bool is_function()        const { return is_vm_function() || is_native_function(); }

    tag &f2i()                      { return is_float() ? seti(static_cast<int64_t>(f)) : *this; }
    tag &i2f()                      { return is_int() ?   setf(static_cast<float>(i))   : *this; }
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

template <typename T>
struct stack
{
    T *_front, *_back, *_;
    stack(size_t count): _front(new T [count]), _back(_front + count), _(_back) {}
    ~stack() { delete [] _front; }
};

struct env
{
    stack<tag>            operands;
    stack<const code_t *> returns;
};

struct opcodes
{
    enum Code : code_t
    {
        _HALT = 0,

        // stack
        _PUSH, _DROP, _DUPE, _SWAP, _OVER, _TUCK, _NIP,

        // conversion
        _F2I, _I2F, _IBOX, _FBOX, _BOX, _UNBOX,

        // print/debug
        _PRTOP, _PRINT,

        // s-expr
        _PUSHNIL, _PAIR, _HEAD, _TAIL, _LENGTH,

        // bool
        _NOT, _BOOL, _AND, _OR,

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
            "HALT",

            // stack
            "PUSH", "DROP", "DUPE", "SWAP", "OVER", "TUCK", "NIP",

            // conversion
            "F2I", "I2F", "IBOX", "FBOX", "BOX", "UNBOX",

            // print/debug
            "PRTOP", "PRINT",

            // s-expr
            "PUSHNIL", "PAIR", "HEAD", "TAIL", "LENGTH",

            // bool
            "NOT", "BOOL", "AND", "OR",

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
};

struct svm : public opcodes
{
    /*
    enum Code : code_t
    {
        _HALT = 0,

        // stack
        _PUSH, _DROP, _DUPE, _SWAP, _OVER, _TUCK, _NIP,

        // conversion
        _F2I, _I2F, _IBOX, _FBOX, _BOX, _UNBOX,

        // print/debug
        _PRTOP, _PRINT,

        // s-expr
        _PUSHNIL, _PAIR, _HEAD, _TAIL, _LENGTH,

        // bool
        _NOT, _BOOL, _AND, _OR,
        
        // untyped
        _ADD, _SUB, _MUL, _DIV, _MOD, _EQ, _LT, _LTE, _GT, _GTE, _SHL, _SHR, _XOR, _INC, _DEC,

        // integer
        _IPUSHC, _IPUSHM, _IPUSH0, _IPUSH1, _INEG,
        _IADD, _ISUB, _IMUL, _IDIV, _IMOD, _IEQ,  _ILT, _ILTE, _IGT, _IGTE,
        _ISHL, _ISHR, _IAND, _IOR,  _IXOR, _IINC, _IDEC, _INOT, _ISHR1, _ISHL1,

        // float
        _FPUSHC, _FPUSH0, _FPUSH1, _FNEG,
        _FADD, _FSUB, _FMUL, _FDIV, _FEQ, _FLT, _FLTE, _FGT, _FGTE, _FSIN,

        _JMP, // how we gonna do this?
    };
    */

    const code_t *_ip;
    tag           *_stack, *_back, *_; // underscore is top of stack
    
    std::ostream &print_stack(std::ostream &os) const;

    svm(size_t stack_count, const code_t *data);

    size_t run(std::ostream &os);
    
    typedef float   (*ff_fp)(float);
    void          _ff(ff_fp fn)           { _->f = fn(_->f); }
};

struct typevm : public opcodes
{
    /*
    enum Code : code_t
    {
        _HALT = 0,

        // stack
        _PUSH, _DROP, _DUPE, _SWAP, _OVER, _TUCK, _NIP,

        // conversion
        _F2I, _I2F, _IBOX, _FBOX, _BOX, _UNBOX,

        // print/debug
        _PRTOP, _PRINT,

        // s-expr
        _PUSHNIL, _PAIR, _HEAD, _TAIL, _LENGTH,

        // bool
        _NOT, _BOOL, _AND, _OR,

        // integer
        _IPUSHC, _IPUSHM, _IPUSH0, _IPUSH1, _INEG,
        _IADD, _ISUB, _IMUL, _IDIV, _IMOD, _IIS0, _IEQ, _ILT, _ILTE, _IGT, _IGTE,
        _ISHL, _ISHR, _IAND, _IOR,  _IXOR, _IINC, _IDEC, _INOT, _ISHR1, _ISHL1,

        // float
        _FPUSHC, _FPUSH0, _FPUSH1, _FNEG,
        _FADD, _FSUB, _FMUL, _FDIV, _FIS0, _FEQ, _FLT, _FLTE, _FGT, _FGTE, _FSIN,

        _JMP, // how we gonna do this?
    };
    */

    const code_t *_ip;
    tag           *_stack, *_back, *_; // underscore is top of stack
    
    std::ostream &print_stack(std::ostream &os) const;

    typevm(size_t stack_count, const code_t *data);

    size_t run(std::ostream &os);
    
    typedef float   (*ff_fp)(float);
    void          _ff(ff_fp fn)           { _->f = fn(_->f); }
};

void test_svm();
