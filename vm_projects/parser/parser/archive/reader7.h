#pragma once

#include <string_view>
#include <iostream>
#include <array>
#include <string>
#include "elements.h"
#include "lex.h"
#include "lex_numeric.h"
#include "printer.h"

typedef std::string_view::const_iterator iter_t;

struct read_context
{
    element_allocator    &_pool;
    std::string           _owner;
    std::string_view      _text;
    
    enum { _NIL = 0 };
    
    read_context(element_allocator &pool, std::string_view text, bool own = true) : _pool(pool), _text(text)
    {
        if (own) {
            _owner = text;
            _text = _owner;
        }
    }

    uint64_t read_symbol(iter_t &i, iter_t e)
    {
        iter_t b = i;
        i = ::read_word(i, e);
        return i == b ? _NIL : _pool.allocate_symbol(b, i);
    }

    uint64_t read_string(iter_t &i, iter_t e)
    {
        iter_t b = i;
        auto r = ::read_quoted(i, e);
        return (i = r.first) == b ? _NIL : _pool.allocate_string(b, i);
    }

    uint64_t read_number(iter_t &i, iter_t e)
    {
        iter_t b = i;
        auto r = ::read_number(i, e, false);
        return (i = r.first) == b ? _NIL : _pool.allocate_number(r.second);
    }

    uint64_t read_tail(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        if (*i == ')') {
            ++i;
            return 0;
        }
        uint64_t p = _pool.allocate_pair(_NIL, _NIL),
                 h = read_expr(i, e), t = _NIL;
        _pool.unsafe_cast_pair(p)->head = h;
        i = read_space(i, e);
        if (*i == '.' && chars_utf8::is_space(*(i + 1))) {
            i = read_space(++i, e);
            t = read_expr(i, e);
        } else {
            t = read_tail(i, e);
        }
        _pool.unsafe_cast_pair(p)->tail = t;
        return p;
    }

    uint64_t read_pair(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        if (*i != '(') {
            return 0;
        }
        ++i;
        if (*i == ')') {
            ++i;
            return _pool.allocate_nil();
        }
        return read_tail(i, e);
    }

    uint64_t read_atom(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        uint64_t a = 0;
        if ((a = read_number(i, e))) { return a; }
        if ((a = read_string(i, e))) { return a; }
        if ((a = read_symbol(i, e))) { return a; }
        return a;
    }

    uint64_t read_expr(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        uint64_t a = 0;
        if (*i == ')')             { return 0; }
        if ((a = read_pair(i, e))) { return a; }
        if ((a = read_atom(i, e))) { return a; }
        return a;
    }
};

struct elements
{
    element_allocator &_pool;
    
    elements(element_allocator &pool) : _pool(pool) {}
    
    uint64_t head(uint64_t p)   { tag *t = _pool.cast_pair(p); return t ? t->head : 0; }
    uint64_t tail(uint64_t p)   { tag *t = _pool.cast_pair(p); return t ? t->tail : 0; }
    uint64_t length(uint64_t p) { size_t count = 0; while (p = tail(p)) count++; return count; }
    
};

void read_lispesq()
{
    typedef std::string_view::const_iterator iter_t;
    std::string_view s("(anom sdfkj as d () \"hello\" 10 11 3.234e-10 der (quote kjl) (\"from\" . the_other_side))");
    std::cout << std::endl << s;
    iter_t i = s.begin(), e = s.end();
    element_allocator pool;
    read_context r(pool, s);
    uint64_t a = (r.read_expr(i, e));
    std::cout << std::endl << "#" << std::hex << a << std::endl;
    std::cout << "sizeof tag " << sizeof(tag) << std::endl;
    printer p(pool);
    p.print(std::cout << std::endl, a);
}
