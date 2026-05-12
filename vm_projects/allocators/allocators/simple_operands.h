#pragma once

#include "types.h"
#include "stack.h"
#include "functions.h"
#include "tag_op.h"

struct simple_operands
{
    static constexpr const size_t COUNT = 128;
    
    stack_n<tag_op, COUNT> ops;
    
    operands() {}

    bool            is_top(Type t)            const { return ops.top().type == t; }
    bool            is_at(Type t, size_t n)   const { return ops.at(n).type == t; }
    
    const tag_op   &top()                     const { return _ops.top(); }
    tag_op         &top()                           { return _ops.top(); }
    
    void            pushi(int64_t v)                { tags.push(Type::INT);   ints.push(v);   }
    void            pushf(float v)                  { tags.push(Type::FLOAT); floats.push(v); }
    void            push(const function *f)         { tags.push(Type::FUNC);  funcs.push(f);  }
    void            seti(int64_t v)                 { ops.set(tag_op(v)); }
    void            setf(float v)                   { ops.set(tag_op(v)); }
    void            set(const function *f)          { ops.set(tag_op(f)); }
    void            drop()                          { ops.drop(); }
    void            swap()                          { ops.swap(); }
    void            over()                          { dupe_from(1); }
    void            f2i()                           { top().f2i(); }
    void            i2f()                           { top().i2f(); }
    tag_op         &at(size_t n)                    { return ops.at(n); }
    const tag_op   &at(size_t n)              const { return ops.at(n); }
    tag_op          pop()                           { return ops.pop(); }
    void            dupe(size_t n = 1)              { ops.dupe(n); }
    void            dupe_from(size_t at = 1)        { ops.push(ops.at(at)); }
    void            push(const tag_op &v)           { ops.push(v); }

    void            op_not()                        { top().op_not(); }
    void            op_neg()                        { top().op_neg(); }
    void            op_shl(size_t n)                { top().op_shl(n); }
    void            op_shr(size_t n)                { top().op_shr(n); }
    void            op_add()                        { top().op_add(at(1)); }
    void            op_mul()                        { top().op_mul(at(1)); }
    void            op_div()                        { top().op_div(at(1)); }
    void            op_sub()                        { top().op_sub(at(1)); }
    void            op_mod()                        { top().op_mod(at(1)); }
    void            op_and()                        { top().op_and(at(1)); }
    void            op_or()                         { top().op_or (at(1)); }
    void            op_xor()                        { top().op_xor(at(1)); }

    void            pred_lt()                       { top().pred_lt  (at(1)); }
    void            pred_lte()                      { top().pred_lte (at(1)); }
    void            pred_eq()                       { top().pred_eq  (at(1)); }
    void            pred_neq()                      { top().pred_neq (at(1)); }
    void            pred_gte()                      { top().pred_gte (at(1)); }
    void            pred_gt()                       { top().pred_gt  (at(1)); }
    void            pred_and()                      { top().pred_and (at(1)); }
    void            pred_or()                       { top().pred_or  (at(1)); }
    void            pred_not()                      { top().pred_not (at(1)); }
    void            pred_bool()                     { top().pred_bool(at(1)); }

    std::ostream &print(std::ostream &os) const
    {
        for (int n = static_cast<int>(tags.size()) - 1; n >= 0; --n) {
            at(n).print(os << " ");
        }
        return os;
    }
};

