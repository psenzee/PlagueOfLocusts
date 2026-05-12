#pragma once

#include <string_view>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <array>
#include <string>
#include <cctype>
#include <algorithm>
#include <string_view>
#include <deque>
#include <vector>
#include "strs.h"
#include "elems.h"
#include "reader.h"
#include "printer.h"
#include "functions.h"
#include "constants.h"
#include "builtins.h"

namespace tl
{

struct base_evaluator
{
    std::array<function_call *, symbols::_MAX_SYMBOLS> _builtins;

    base_evaluator() : _builtins(construct_builtins()) {}
    
    elem *eval_user_symbol(symbol *sym, pair *environ)
    {
        // todo
        return nullptr;
    }
    
    elem *eval_symbol(symbol *sym, pair *environ)
    {
        int id = sym->id;
        switch (id) {
        case symbols::NIL:     return nullptr;
        case symbols::LIST:
        case symbols::QUOTE:   return sym; // eval to itself
        case symbols::UNKNOWN: return eval_user_symbol(sym, environ);
        default:               break;
        }
        return _builtins[id];
    }
    
    void error()
    {
        std::cout << "ERROR HERE" << std::endl;
    }

    pair *pair_sequence(pair *p)
    {
        if (!p) {
            return nullptr;
        }
        symbol *pair_sym = new symbol("pair");
        linear_t lin = linear_alloc(p);
        elem **data = lin.first;
        size_t sz = lin.second;
        pair *out = nullptr;
        for (size_t i = 0; i < sz; ++i) {
            out = out ? list(pair_sym, data[sz - i - 1], out)
                      : list(pair_sym, data[sz - i - 1]);
        }
        linear_free(lin);
        return out;
    }

    pair *list_macro(pair *p)
    {
        symbol *sym = head_symbol(p);
        if (sym && sym->id == symbols::LIST) {
            return pair_sequence(tail_pair(p));
        }
        return p; // don't do anything if it's not a list
    }
};

}
