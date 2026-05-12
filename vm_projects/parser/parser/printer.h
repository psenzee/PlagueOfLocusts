#pragma once

#include <string_view>
#include <iostream>
#include <array>
#include <string>
#include "elements.h"

struct printer
{
    element_allocator &_pool;
    
    printer(element_allocator &pool) : _pool(pool) {}

    std::ostream &print_atom(std::ostream &os, const tag *t) const
    {
        if (!t) {
            return os << "#nil";
        }
        switch (t->_type) {
            case types::NIL:    return static_cast<const elem_nil      *>(t)->print(os);
            case types::STRING: return static_cast<const elem_string_t *>(t)->print(os);
            case types::SYMBOL: return static_cast<const elem_symbol_t *>(t)->print(os);
            case types::INT32:  return static_cast<const elem_int32_t  *>(t)->print(os);
            case types::INT64:  return static_cast<const elem_int64_t  *>(t)->print(os);
            case types::FLOAT:  return static_cast<const elem_float_t  *>(t)->print(os);
            case types::DOUBLE: return static_cast<const elem_double_t *>(t)->print(os);
            case types::TYPE:   return static_cast<const elem_type_t   *>(t)->print(os);
            case types::TOKEN:  return static_cast<const elem_token    *>(t)->print(os);
            default: break;
        }
        return os;
    }

    std::ostream &print_atom(std::ostream &os, uint64_t i) const
    {
        return print_atom(os, _pool.get(i));
    }

    std::ostream &print(std::ostream &os, uint64_t t, bool has_tail = false) const
    {
        if (!t) {
            return os << "#nil";
        }
        const tag *tp = _pool.get(t);
        if (tp->is_atom()) {
            return print_atom(os, tp);
        }
        const auto *q = cast<elem_quote_t>(tp);
        if (q) {
            os << "'";
            return print(os, q->ref);
        }
        const auto *p = cast<elem_pair>(tp);
        if (p) {
            os << (has_tail ? "" : "(");
            uint64_t b = p->tail;
            print(os, p->head, false);
            if (b) {
                print(os << (_pool.get(b)->is_atom() ? " . " : " "), b, true);
            }
            return os << (has_tail ? "" : ")");
        }
        return os << "#nil";
    }
};
