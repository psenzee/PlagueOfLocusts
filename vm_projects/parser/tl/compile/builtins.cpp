#include "builtins.h"

#include <array>
#include "strs.h"
#include "elems.h"
#include "functions.h"
#include "symbols.h"

namespace tl
{

std::array<function_call *, symbols::_MAX_SYMBOLS> construct_builtins()
{
    std::array<function_call *, symbols::_MAX_SYMBOLS> list = { nullptr };
    
    //elem *t = new symbol("t");
    list[symbols::T]        = nullptr;//new function<elem *,  0>([t]() { return (elem *)t; },       "t");
    list[symbols::NIL]      = new function<elem *,  0>([]()  { return (elem *)nullptr; }, "nil", "PUSHNIL");
    list[symbols::ENDP]     =
    list[symbols::NILP]     = new function<bool,    1>(is_nil,          "nil?",    "NIL?");
    list[symbols::LIST]     = nullptr; // placeholder
    list[symbols::LISTP]    = new function<bool,    1>(is_list,         "list?",   "LIST?");
    list[symbols::DEF]      =
    list[symbols::DEFINE]   = nullptr; // placeholder
    list[symbols::FUNCTION] = nullptr; // placeholder
    list[symbols::SETF]     = nullptr; // placeholder
    list[symbols::SETP]     = nullptr; // placeholder
    list[symbols::LAMBDA]   = nullptr; // placeholder
    list[symbols::LET]      = nullptr; // placeholder
    list[symbols::LEN]      =
    list[symbols::LENGTH]   = new function<size_t,  1>(length,          "length",  "LEN");
    list[symbols::FIRST]    =
    list[symbols::HEAD]     = new function<elem *,  1>(head,            "head",    "HEAD");
    list[symbols::REST]     =
    list[symbols::TAIL]     = new function<elem *,  1>(tail,            "tail",    "TAIL");
    list[symbols::CONS]     =
    list[symbols::PAIR]     = new function<elem *,  2>(new_pair,        "pair",    "PAIR");
    list[symbols::CONSP]    =
    list[symbols::PAIRP]    = new function<bool,    1>(is_list,         "pair?",   "PAIR?");
    list[symbols::MAP]      = nullptr; // placeholder
    list[symbols::APPLY]    = nullptr; // placeholder
    list[symbols::ATOMP]    = new function<bool,    1>(is_atom,         "atom?",   "ATOM?");
    list[symbols::NUMBERP]  = new function<bool,    1>(is_number,       "number?", "NUMBER?");
    list[symbols::APPEND]   =
    list[symbols::SYMBOLP]  = new function<bool,    1>(is_symbol,       "symbol?", "SYMBOL?");
    list[symbols::ADD]      = new function<elem *,  2>(math_op<op_add>, "+",       "ADD");
    list[symbols::SUB]      = new function<elem *,  2>(math_op<op_sub>, "-",       "SUB");
    list[symbols::MUL]      = new function<elem *,  2>(math_op<op_mul>, "*",       "MUL");
    list[symbols::DIV]      = new function<elem *,  2>(math_op<op_div>, "/",       "DIV");
    return list;
}

}
