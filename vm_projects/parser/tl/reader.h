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
#include "lexer.h"

namespace tl
{
typedef std::string_view::const_iterator iter_t;

struct reader
{
    typedef lexer<iter_t> lexer_t;

    std::string                        _text;
    elem                              *_element;
    iter_t                             _at;
    lexer_t                            _lex;
    word_style_lookup<word_style_lisp> _word_style;

    reader(std::string_view text) : _text(text) {}

    elem *read()
    {
        std::string_view s(_text);
        _at = s.begin();
        try {
            return read_expr(_at, s.end());
        } catch (std::runtime_error *error) {
            std::cout << error->what() << std::endl;
        }
        return nullptr;
    }

    template <typename ElemT> elem *read_string(iter_t &i, iter_t e, bool quoted)
    {
        iter_t b = i;
        i = quoted ? _lex.read_quoted(i, e) : _lex.read_word(_word_style, i, e);
        return i == b ? nullptr : new ElemT(b, i);
    }

    elem *read_number(iter_t &i, iter_t e)
    {
        iter_t b = i;
        double v = 0;
        i = _lex.read_double(i, e, &v);
        return i == b ? nullptr : new number_t(v);
    }

    elem *read_tail(iter_t &i, iter_t e)
    {
        i = _lex.read_space(i, e);
        if (*i == ')') {
            ++i;
            return nullptr;
        }
        elem *p = new pair(),
                *h = read_expr(i, e),
                *t = nullptr;
        cast<pair>(p)->head = h;
        i = _lex.read_space(i, e);
        if (*i == '.' && std::isspace(*(i + 1))) {
            i = _lex.read_space(++i, e);
            t = read_expr(i, e);
        } else {
            t = read_tail(i, e);
        }
        cast<pair>(p)->tail = t;
        return p;
    }

    elem *read_pair(iter_t &i, iter_t e)
    {
        i = _lex.read_space(i, e);
        if (*i != '(') {
            return nullptr;
        }
        if (*++i == ')') {
            ++i;
            return new nil;
        }
        return read_tail(i, e);
    }

    elem *read_quote(iter_t &i, iter_t e)
    {
        static constexpr std::string_view quote("quote");
        i = _lex.read_space(i, e);
        if (*i != '\'') {
            return 0;
        }
        i = _lex.read_space(++i, e);
        auto expr = read_expr(i, e);
        return new pair(new symbol(quote), new pair(expr));
    }

    elem *read_expr(iter_t &i, iter_t e)
    {
        i = _lex.read_space(i, e);
        elem *a = nullptr;
        if (i == e || *i == ')')                      { return a; }
        if ((a = read_pair(i, e)))                    { return a; }
        if ((a = read_quote(i, e)))                   { return a; }
        if ((a = read_number(i, e)))                  { return a; }
        if ((a = read_string<string_t>(i, e, true)))  { return a; }
        if ((a = read_string<symbol>  (i, e, false))) { return a; }
        error(i, e);
        return a;
    }

    void error(iter_t &i, iter_t e)
    {
        std::ostringstream oss;
        oss << std::endl << "ERROR UNRECOGNIZED TOKEN : " << std::string_view(i, e);
        throw new std::runtime_error(oss.str());
    }
};

}
