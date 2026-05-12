#pragma once

#include "types.h"
#include "stack.h"
#include "functions.h"
#include "tag_op.h"

#define RETURN_TAG_OP(TAG, RETURN_EXPR) \
{switch (TAG) { \
case Type::FLOAT: return tag_op(floats.RETURN_EXPR); \
case Type::INT:   return tag_op(ints.RETURN_EXPR);   \
case Type::FUNC:  return tag_op(funcs.RETURN_EXPR);  \
default:          return tag_op();                   \
}}

#define TAG_OPERATION(TAG, OPERATION) \
{switch (TAG) { \
case Type::FLOAT: floats.OPERATION;  break; \
case Type::INT:   ints.OPERATION;    break; \
case Type::FUNC:  funcs.OPERATION;   break; \
default:                             break; \
}}

#define PUSH_TAG_VALUE(TAG, FLOAT_VAL, INT_VAL, FUNC_VAL) \
{switch (TAG) { \
case Type::FLOAT: floats.push(FLOAT_VAL); break; \
case Type::INT:   ints.push(INT_VAL);     break; \
case Type::FUNC:  funcs.push(FUNC_VAL);   break; \
default:                                  break; \
}}

struct operands
{
    static constexpr const size_t TYPE_COUNT = 64, FLOAT_COUNT = 32, INT_COUNT = 32, FUNC_COUNT = 32;
    
    typedef stack<Type>              stack_tag_t;
    typedef stack<float>             stack_float_t;
    typedef stack<int64_t>           stack_int_t;
    typedef stack<const function *>  stack_func_t;
    
    stack_n<Type,             TYPE_COUNT>  tags;
    stack_n<float,            FLOAT_COUNT> floats;
    stack_n<int64_t,          INT_COUNT>   ints;
    stack_n<const function *, FUNC_COUNT>  funcs;

    stack_float_t &_stack(const float &)           { return floats; }
    stack_int_t   &_stack(const int64_t &)         { return ints; }
    stack_func_t  &_stack(const function *)        { return funcs; }
    
    template <typename T> void _drop()             { _stack(T{}).drop(); tags.drop(); }
    template <typename T> void _dropn(size_t n)    { _stack(T{}).drop(n); tags.drop(n); }
    template <typename T> void _push(const T &t)   { _stack(t).push(t); tags.push(type_for(t)); }
    
    operands() {}

    bool            is_top(Type t)            const { return tags.top() == t; }
    bool            is_at(Type t, size_t n)   const { return tags.at(n) == t; }
    
    int64_t         topi()                    const { _check(Type::INT);   return ints.top(); }
    float           topf()                    const { _check(Type::FLOAT); return floats.top(); }
    const function *topfunc()                 const { _check(Type::FUNC);  return funcs.top(); }
    
    void            pushi(int64_t v)                { tags.push(Type::INT);   ints.push(v);   }
    void            pushf(float v)                  { tags.push(Type::FLOAT); floats.push(v); }
    void            push(const function *f)         { tags.push(Type::FUNC);  funcs.push(f);  }
    void            seti(int64_t v)                 { if (is_top(Type::INT))   ints.set(v);   else { drop(); _push(v); } }
    void            setf(float v)                   { if (is_top(Type::FLOAT)) floats.set(v); else { drop(); _push(v); } }
    void            set(const function *f)          { if (is_top(Type::FUNC))  funcs.set(f);  else { drop(); _push(f);  } }
    void            drop()                          { TAG_OPERATION(tags.pop(), drop()) }
    void            swap()                          { if (tags.eq()) TAG_OPERATION(tags.top(), swap()) else tags.swap(); }
    void            over()                          { dupe_from(); }
    void            f2i()                           { _check(Type::FLOAT); tags.set(Type::INT);   ints.push(static_cast<int64_t>(floats.pop())); }
    void            i2f()                           { _check(Type::INT);   tags.set(Type::FLOAT); floats.push(static_cast<float>(ints.pop())); }
    tag_op          at(size_t n)              const { RETURN_TAG_OP(tags.at(n), at(n)) }
    tag_op          pop()                           { RETURN_TAG_OP(tags.pop(), pop()) }
    void            dupe(size_t n = 1)              { tags.dupe(n); TAG_OPERATION(tags.top(), dupe(n)) }
    void            dupe_from(size_t at = 1)        { Type type = tags.at(at); tags.push(type); PUSH_TAG_VALUE(type, floats.at(at), ints.at(at), funcs.at(at)) }
    void            push(const tag_op &v)           { tags.push(v.type); PUSH_TAG_VALUE(v.type, v.f, v.i, v.func) }

    void            inot()                          { _check_i();  ints.op_not(); }
    void            ineg()                          { _check_i();  ints.op_neg(); }
    void            ishl(size_t n)                  { _check_ii(); ints.op_shl(n); tags.drop(); }
    void            ishr(size_t n)                  { _check_ii(); ints.op_shr(n); tags.drop(); }
    void            iadd()                          { _check_ii(); ints.op_add(); tags.drop(); }
    void            imul()                          { _check_ii(); ints.op_mul(); tags.drop(); }
    void            idiv()                          { _check_ii(); ints.op_div(); tags.drop(); }
    void            isub()                          { _check_ii(); ints.op_sub(); tags.drop(); }
    void            imod()                          { _check_ii(); ints.op_mod(); tags.drop(); }
    void            iand()                          { _check_ii(); ints.op_and(); tags.drop(); }
    void            ior()                           { _check_ii(); ints.op_or();  tags.drop(); }
    void            ixor()                          { _check_ii(); ints.op_xor(); tags.drop(); }

    void            predi_lt()                      { _check_ii(); tags.drop(); ints.predicate_lt(); }
    void            predi_lte()                     { _check_ii(); tags.drop(); ints.predicate_lte(); }
    void            predi_eq()                      { _check_ii(); tags.drop(); ints.predicate_eq(); }
    void            predi_gte()                     { _check_ii(); tags.drop(); ints.predicate_gte(); }
    void            predi_gt()                      { _check_ii(); tags.drop(); ints.predicate_gt(); }
    void            predi_and()                     { _check_ii(); tags.drop(); ints.predicate_and(); }
    void            predi_or()                      { _check_ii(); tags.drop(); ints.predicate_or(); }
    void            predi_not()                     { _check_i();  ints.predicate_not(); }
    void            predi_bool()                    { _check_i();  ints.predicate_bool(); }
    
    void            fneg()                          { _check_f();  floats.op_neg(); }
    void            fadd()                          { _check_ff(); floats.op_add(); tags.drop(); }
    void            fmul()                          { _check_ff(); floats.op_mul(); tags.drop(); }
    void            fdiv()                          { _check_ff(); floats.op_div(); tags.drop(); }
    void            fsub()                          { _check_ff(); floats.op_sub(); tags.drop(); }
    
    void            predf_lt()                      { _check_ff(); bool r = floats.logical_lt();   _dropn<float>(2); pushi(r); }
    void            predf_lte()                     { _check_ff(); bool r = floats.logical_lte();  _dropn<float>(2); pushi(r); }
    void            predf_eq()                      { _check_ff(); bool r = floats.logical_eq();   _dropn<float>(2); pushi(r); }
    void            predf_gte()                     { _check_ff(); bool r = floats.logical_gte();  _dropn<float>(2); pushi(r); }
    void            predf_gt()                      { _check_ff(); bool r = floats.logical_gt();   _dropn<float>(2); pushi(r); }
    void            predf_and()                     { _check_ff(); bool r = floats.logical_and();  _dropn<float>(2); pushi(r); }
    void            predf_or()                      { _check_ff(); bool r = floats.logical_or();   _dropn<float>(2); pushi(r); }
    void            predf_not()                     { _check_f();  bool r = floats.logical_not();  seti(r); }
    void            predf_bool()                    { _check_f();  bool r = floats.logical_bool(); seti(r); }

    std::ostream &print(std::ostream &os) const
    {
        for (int n = static_cast<int>(tags.size()) - 1; n >= 0; --n) {
            at(n).print(os << " ");
        }
        return os;
    }
    
private:
   
    void _check(Type tag, size_t n = 0) const { if (tag != tags.at(n)) { std::cout << "bad type at stack[" << n << "]\n"; } }
    void _check_i()   const { _check(Type::INT); }
    void _check_f()   const { _check(Type::FLOAT); }
    void _check_ii()  const { _check(Type::INT); _check(Type::INT, 1); }
    void _check_ff()  const { _check(Type::FLOAT); _check(Type::FLOAT, 1); }

    uint16_t _tag_at(size_t n, size_t sh = 0) const { return uint16_t(tags.at(n)) << (sh * 4); }

    uint16_t _sig1() const { return _tag_at(0); }
    uint16_t _sig2() const { return _tag_at(0) | _tag_at(1, 1); }
    uint16_t _sig3() const { return _tag_at(0) | _tag_at(1, 1) | _tag_at(2, 2); }
    uint16_t _sig4() const { return _tag_at(0) | _tag_at(1, 1) | _tag_at(2, 2) | _tag_at(3, 3); }
};

