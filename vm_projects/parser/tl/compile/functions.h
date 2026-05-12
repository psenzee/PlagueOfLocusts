#pragma once

#include <string_view>
#include <iostream>
#include <sstream>
#include <array>
#include <string>
#include <cctype>
#include <algorithm>
#include <string_view>
#include "elems.h"
#include "strs.h"
#include "symbols.h"
#include "utils.h"

namespace tl
{

struct lambda : public typed_elem<elem::LAMBDA>
{
    pair *instructions, *params;
    lambda(pair *params, pair *instructions) : typed_elem<elem::LAMBDA>(), params(params), instructions(instructions) {}
};

struct function_call : public typed_elem<elem::FUNCTION>
{
    size_t _arity;
    std::string_view _name, _bytecode;
    function_call(size_t arity, std::string_view internal_name, std::string_view bytecode = "") : _arity(arity), _name(internal_name), _bytecode(bytecode) {}
    size_t arity() const { return _arity; }
    virtual elem *call(elem **args) { return nullptr; }
    std::ostream &print(std::ostream &os) const { return os << "#fn_" << _name << "$" << arity(); }
    virtual std::ostream &print_bytecode(std::ostream &os) const { return os << _bytecode << "_" << arity(); }
};

template <typename ReturnT, size_t Arity> struct function : function_call
{
    typedef ReturnT (*fptr_t)(elem *, size_t);
    fptr_t _fptr;
    function(fptr_t fptr, std::string_view internal_name, std::string_view bytecode = "") : function_call(Arity, internal_name, bytecode), _fptr(fptr) {}
    elem *call(elem **args) { return wrap(_fptr(args, arity())); }
};

template <typename ReturnT> struct function<ReturnT, 0> : function_call
{
    typedef ReturnT (*fptr_t)();
    fptr_t _fptr;
    function(fptr_t fptr, std::string_view internal_name, std::string_view bytecode = "") : function_call(0, internal_name, bytecode), _fptr(fptr) {}
    elem *call(elem **args) { return wrap(_fptr()); }
};

template <typename ReturnT> struct function<ReturnT, 1> : function_call
{
    typedef ReturnT (*fptr_t)(elem *);
    fptr_t _fptr;
    function(fptr_t fptr, std::string_view internal_name, std::string_view bytecode = "") : function_call(1, internal_name, bytecode), _fptr(fptr) {}
    elem *call(elem **args) { return wrap(_fptr(args[0])); }
};

template <typename ReturnT> struct function<ReturnT, 2> : function_call
{
    typedef ReturnT (*fptr_t)(elem *, elem *);
    fptr_t _fptr;
    function(fptr_t fptr, std::string_view internal_name, std::string_view bytecode = "") : function_call(2, internal_name, bytecode), _fptr(fptr) {}
    elem *call(elem **args) { return wrap(_fptr(args[0], args[1])); }
};

template <typename ReturnT> struct function<ReturnT, 3> : function_call
{
    typedef ReturnT (*fptr_t)(elem *, elem *, elem *);
    fptr_t _fptr;
    function(fptr_t fptr, std::string_view internal_name, std::string_view bytecode = "") : function_call(3, internal_name, bytecode), _fptr(fptr) {}
    elem *call(elem **args) { return wrap(_fptr(args[0], args[1], args[2])); }
};

template <typename ReturnT> struct function<ReturnT, 4> : function_call
{
    typedef ReturnT (*fptr_t)(elem *, elem *, elem *, elem *);
    fptr_t _fptr;
    function(fptr_t fptr, std::string_view internal_name, std::string_view bytecode = "") : function_call(4, internal_name, bytecode), _fptr(fptr) {}
    elem *call(elem **args) { return wrap(_fptr(args[0], args[1], args[2], args[3])); }
};

struct op_add { template <typename T, typename U> auto operator()(T a, U b) { return a + b; } };
struct op_sub { template <typename T, typename U> auto operator()(T a, U b) { return a - b; } };
struct op_mul { template <typename T, typename U> auto operator()(T a, U b) { return a * b; } };
struct op_div { template <typename T, typename U> auto operator()(T a, U b) { return a / b; } };

template <typename T0, typename T1, typename Op>
elem *elem_op(elem *a, elem *b) { Op op; auto va = cast< box<T0> >(a); auto vb = cast< box<T1> >(b); return va && vb ? wrap(op(va->value, vb->value)) : nullptr; }

template <typename Op>
elem *math_op(elem *a, elem *b)
{
    if (!is_number(a) || !is_number(b)) {
        return nullptr;
    }
    int option = (is(a, elem::FLOAT) ? 2 : 0) + (is(b, elem::FLOAT) ? 1 : 0);
    switch (option) {
    case 0: return elem_op<int64_t, int64_t, Op>(a, b);
    case 1: return elem_op<int64_t, double,  Op>(a, b);
    case 2: return elem_op<double,  int64_t, Op>(a, b);
    case 3: return elem_op<double,  double,  Op>(a, b);
    default: break;
    }
    return nullptr;
}

}
