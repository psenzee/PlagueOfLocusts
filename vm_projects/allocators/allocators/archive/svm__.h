#include <iostream>
#include <cmath>

#include "elems.h"
#include "utils.h"

struct tag_type { enum Type: uint8_t { POINTER = 0, TAG_INT, TAG_FLOAT, INT, FLOAT, PAIR, FUNCTION, NONE = 0xff }; };

union tag
{
    struct { tag_type::Type type; uint8_t _[7]; };
    object *ptr;
    struct { uint32_t __; union { int32_t i; float f; }; };
    
    tag &set(int32_t v)   { type = tag_type::INT;     i = v;   return *this; }
    tag &set(float v)     { type = tag_type::FLOAT;   f = v;   return *this; }
    tag &set(object *p)   { type = tag_type::POINTER; ptr = p; return *this; }
    
    bool is_nil()   const { return ptr == 0; }
    bool is_int()   const { return type == tag_type::INT; }
    bool is_float() const { return type == tag_type::FLOAT; }
    
    tag &f2i()            { return is_float() ? set(static_cast<int32_t>(f)) : *this; }
    tag &i2f()            { return is_int() ?   set(static_cast<float>(i))   : *this; }
};

inline std::ostream &operator<<(std::ostream &os, const tag &t)
{
    switch (t.type) {
    case tag_type::INT:     return os << "[i:" << t.i   << "]";
    case tag_type::FLOAT:   return os << "[f:" << t.f   << "]";
    case tag_type::POINTER: return os << "[*"  << t.ptr << "]";
    default:                return os;
    }
}

struct signature : public tag_type
{
    const Type *types;
    Type        return_type;
    uint8_t     type_count;
    signature(Type ret, const Type *types, uint8_t count) : return_type(ret), types(types), type_count(count) {}
};

template <size_t N>
class signature_t : public signature
{
    std::array<Type, N> _data;
public:
    signature_t(Type ret) : signature(ret, _data.data(), _data.size()) {}
};

struct object : public tag_type
{
    Type type;

    bool                       is(Type t)               const { return type == t; }
    bool                       is_atom()                const { return type < PAIR; }
    bool                       is_number()              const { return type == INT || type == FLOAT; }
    virtual std::ostream      &print(std::ostream &os)  const { return os << "[object]"; }
    virtual bool               equals(const object *e)  const { return e->type == type; }
    /*
    virtual const signature_t &signature()              const
    {
        static constexpr const signature_t s { signature_t::NONE_TYPES, tag_type::NONE, 0 };
        return s;
    }
     */

    object(Type type = INT) : type(type) {}
};

template <typename Function, tag_type::Type ReturnType>
struct function0 : public object
{
    Function fptr;
    function0() : object(tag_type::FUNCTION) {}
};

inline void test_tag()
{
    tag t;
    t.set(17);
    std::cout << "test tag " << t << std::endl;
    //std::cout << t.type << std::endl;
}

struct svm
{
    enum Code : int16_t
    {
        _HALT = 0,
        
        // stack
        _PUSH, _DROP, _DUPE, _SWAP, _OVER, _TUCK, _NIP,
        
        // conversion
        _F2I, _I2F, _IBOX, _FBOX, _BOX, _UNBOX,
        
        // print/debug
        _PRTOP, _PRINT,
        
        // s-expr
        _PAIR, _HEAD, _TAIL, _LENGTH,
        
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

    struct item_t
    {
        enum Type : uint8_t { TYPE_INT, TYPE_FLOAT, TYPE_ELEM };
        uint8_t type;
        union {
            int64_t  i;
            float    f;
            elem    *e;
        };
        
        const item_t &seti(int64_t v) { type = TYPE_INT;   i = v; return *this; }
        const item_t &setf(float v)   { type = TYPE_FLOAT; f = v; return *this; }
        const item_t &set(elem *v)    { type = TYPE_ELEM;  e = v; return *this; }
        const item_t &unbox()
        {
            if (type == TYPE_ELEM) {
                int64_t iv = 0;
                float fv = 0.;
                switch (e->_type) {
                case elem::INT:   if (elem_value(e, &iv)) seti(iv); return *this;
                case elem::FLOAT: if (elem_value(e, &fv)) setf(fv); return *this;
                default:                                            return *this;
                }
            }
            return *this;
        }
        elem *box()
        {
            switch (type) {
            case TYPE_ELEM:  return e;
            case TYPE_INT:   return wrap(i);
            case TYPE_FLOAT: return wrap(f);
            default:         break;
            }
            return new nil();
        }

        std::ostream &print(std::ostream &os) const;
    };
    
    const int16_t *_ip;
    item_t        *_stack, *_back, *_; // underscore is top of stack
    
    std::ostream &print_stack(std::ostream &os) const;

    svm(size_t stack_count, const int16_t *data);

    typedef float   (*ffff_fp)(float, float, float);
    typedef float   (*fff_fp)(float, float);
    typedef int64_t (*iiii_fp)(int64_t, int64_t, int64_t);
    typedef int64_t (*iii_fp)(int64_t, int64_t);
    typedef int64_t (*iff_fp)(float, float);
    typedef float   (*fii_fp)(int64_t, int64_t);
    typedef float   (*ff_fp)(float);
    typedef int64_t (*ii_fp)(int64_t);
    typedef int64_t (*if_fp)(float);
    typedef float   (*fi_fp)(int64_t);

    void          _iiii(iiii_fp fn)       { _[2].i =  fn(_[2].i, _[1].i, _[0].i); _ += 2; }
    void          _ffff(ffff_fp fn)       { _[2].f =  fn(_[2].f, _[1].f, _[0].f); _ += 2; }
    void          _fff(fff_fp fn)         { _[1].f =  fn(_[1].f, _[0].f);  ++_; }
    void          _iii(iii_fp fn)         { _[1].i =  fn(_[1].i, _[0].i);  ++_; }
    void          _iff(iff_fp fn)         { _[1].seti(fn(_[1].f, _[0].f)); ++_; }
    void          _fii(fii_fp fn)         { _[1].setf(fn(_[1].i, _[0].i)); ++_; }
    void          _if(if_fp fn)           { _->seti(fn(_->f)); }
    void          _fi(fi_fp fn)           { _->setf(fn(_->i)); }
    void          _ii(ii_fp fn)           { _->i = fn(_->i); }
    void          _ff(ff_fp fn)           { _->f = fn(_->f); }

    size_t run(std::ostream &os);
};

void test_svm();
