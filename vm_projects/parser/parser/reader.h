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
    typedef element_allocator::handle_t handle_t;

    element_allocator    &_pool;
    std::string           _owner;
    std::string_view      _text;

    enum Literal { QUOTED = 0, C_WORD, L_WORD };

    read_context(element_allocator &pool, std::string_view text, bool own = true) : _pool(pool), _text(text)
    {
        if (own) {
            _owner = text;
            _text = _owner;
        }
    }

    iter_t read(iter_t &i, iter_t e, Literal literal)
    {
        switch (literal) {
            case QUOTED: {
                auto r = ::read_quoted_err(i, e);
                return r.first; // TODO handle error
            }
            case C_WORD: return ::read_cword(i, e);
            case L_WORD: return ::read_lword(i, e);
        }
        return i;
    }

    handle_t read_type(iter_t &i, iter_t e)
    {
        iter_t b = i;
        if (*i != '@') {
            return 0;
        }
        b = ++i;
        i = ::read_cword(i, e);
        return i == b ? 0 : _pool.new_value<types::Type>(types::from_name(std::string_view(b, i)));
    }
    
    template <typename ElemT> handle_t read_string_type(iter_t &i, iter_t e, Literal literal = L_WORD, int prefix_char = 0)
    {
        iter_t b = i;
        if (prefix_char) {
            if (*i != prefix_char) {
                return 0;
            }
            b = ++i;
        }
        i = read(i, e, literal);
        return i == b ? 0 : _pool.new_literal<ElemT>(b, i);
    }

    handle_t read_number(iter_t &i, iter_t e)
    {
        iter_t b = i;
        auto r = ::read_number(i, e, false);
        return (i = r.first) == b ? 0 : _pool.new_number(r.second);
    }

    handle_t read_tail(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        if (*i == ')') {
            ++i;
            return 0;
        }
        handle_t p = _pool.new_pair(0, 0),
                 h = read_expr(i, e), t = 0;
        _pool.cast<elem_pair>(p)->head = h;
        i = read_space(i, e);
        if (*i == '.' && chars_utf8::is_space(*(i + 1))) {
            i = read_space(++i, e);
            t = read_expr(i, e);
        } else {
            t = read_tail(i, e);
        }
        _pool.cast<elem_pair>(p)->tail = t;
        return p;
    }
    
    handle_t read_quote(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        if (*i != '\'') {
            return 0;
        }
        i = read_space(++i, e);
        auto expr = read_expr(i, e);
        return _pool.new_quote(expr);
    }
    
    handle_t replace_quote(handle_t handle)
    {
        const elem_pair *p = _pool.cast<elem_pair>(handle);
        if (p) {
            const elem_symbol_t *s = _pool.cast<elem_symbol_t>(p->head);
            handle_t tail = p->tail;
            if (s && tail && str_compare_ci(s->view(), "quote") == 0) {
                p = _pool.cast<elem_pair>(tail);
                if (p && !p->tail) {
                    return _pool.new_quote(p->head);
                }
            }
        }
        return handle;
    }

    handle_t read_pair(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        if (*i != '(') {
            return 0;
        }
        ++i;
        if (*i == ')') {
            ++i;
            return _pool.new_nil();
        }
        return replace_quote(read_tail(i, e));
    }

    handle_t read_atom(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        handle_t a = 0;
        if ((a = read_number(i, e)))                               { return a; }
        if ((a = read_string_type<elem_string_t>(i, e, QUOTED)))   { return a; }
        if ((a = read_string_type<elem_token>(i, e, C_WORD, '?'))) { return a; }
        if ((a = read_type(i, e)))                                 { return a; }
        if ((a = read_string_type<elem_symbol_t>(i, e, L_WORD)))   { return a; }
        return a;
    }

    handle_t read_expr(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        handle_t a = 0;
        if (*i == ')')              { return 0; }
        if ((a = read_pair (i, e))) { return a; }
        if ((a = read_quote(i, e))) { return a; }
        if ((a = read_atom (i, e))) { return a; }
        return a;
    }
};

