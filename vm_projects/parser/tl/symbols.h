#pragma once

#include <array>
#include <cctype>
#include <algorithm>
#include <string_view>
#include "strs.h"

namespace tl
{

struct symbols
{
    enum Symbol {
        UNKNOWN = 0,
        QUOTE, T, NIL, NILP, LIST, LISTP, DEF, DEFINE, ENDP, FUNCTION, SETF, SETP,
        LAMBDA, LET, LENGTH, LEN, FIRST, REST, HEAD, TAIL, PAIR, PAIRP, CONS, CONSP,
        MAP, APPLY, ATOMP, NUMBERP, APPEND, SYMBOLP, ADD, SUB, MUL, DIV,
        _MAX_SYMBOLS
    };
    
private:
    
    struct item {
        std::string_view name;
        Symbol id;
        int operator<=>(const item &u) const { return str_compare_ci(name, u.name); }
    };
    static bool _inited;
    static std::array<item, _MAX_SYMBOLS> _items;
    
    static const item *_find(std::string_view sv)
    {
        auto i = std::lower_bound(_items.begin(), _items.end(), item({ sv }));
        if (i == _items.end() || str_compare_ci((*i).name, sv) != 0) {
            return nullptr;
        }
        return &(*i);
    }
    
public:
    
    static Symbol resolve(std::string_view symbol)
    {
        if (!_inited) {
            std::sort(_items.begin(), _items.end());
            _inited = true;
        }
        const item *sym = _find(symbol);
        return sym ? sym->id : UNKNOWN;
    }
};

}
