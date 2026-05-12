#pragma once

#include <string_view>
#include <iostream>
#include <array>
#include <string>
#include "lex.h"
#include "lex_numeric.h"

typedef std::string_view::const_iterator iter_t;

struct tag
{
    enum Type : uint8_t { TYPE__ERROR = 0, TYPE_NIL, TYPE_PAIR, TYPE_SYMBOL, TYPE_NUMBER, TYPE_STRING, TYPE_ARRAY, _TYPE_COUNT };

    Type _type;
    
    union data {
        struct { tag *head, *tail; };
        struct { iter_t b, e; int64_t ival; double fval; };
        struct { std::array<uint64_t, 4> _; };
        data() : _({ 0 }) {}
        data(tag *head, tag *tail) : head(head), tail(tail) {}
        data(iter_t b, iter_t e, int64_t i = 0, double f = 0.) : b(b), e(e), ival(i), fval(f) {}
    } _data;

    const char           *type()                  const { return type(_type); }
    bool                  is(Type type)           const { return _type == type; }
    virtual bool          is_atom()               const { return _type != TYPE_PAIR && _type != TYPE_ARRAY; }
    virtual std::ostream &print(std::ostream &os) const
    {
        switch (_type) {
        case TYPE_STRING:
        case TYPE_SYMBOL: return os << std::string_view(_data.b, _data.e);
        case TYPE_NUMBER:
                if (_data.fval != 0.) {
                    return os << _data.fval;
                }
                return os << _data.ival;
        case TYPE__ERROR:
        default: break;
        }
        return os << type();
    }

    tag(Type type) : _type(type) {}
    inline tag(iter_t b, iter_t e, int64_t i = 0, double f = 0.) : _type(TYPE_NUMBER), _data(b, e, i, f) {}
    inline tag(tag *head, tag *tail) : _type(TYPE_PAIR), _data(head, tail) {}
    inline tag(Type type, iter_t b, iter_t e) : _type(type), _data(b, e) {}

    inline static const char *type(Type type)
    {
        static constexpr const char *names[] = { "_ERROR_", "NIL", "PAIR", "SYMBOL", "NUMBER", "STRING", "ARRAY" };
        return names[type < _TYPE_COUNT ? type : TYPE__ERROR];
    }
};

tag       *cast(tag *u, tag::Type type)       { return u && u->is(type) ? u : nullptr; }
const tag *cast(const tag *u, tag::Type type) { return u && u->is(type) ? u : nullptr; }

std::ostream &print(std::ostream &os, const tag *t, bool tail = false)
{
    if (t == nullptr) {
        return os << "#nil";
    }
    if (t->is_atom()) {
        return t->print(os);
    }
    const tag *p = cast(t, tag::TYPE_PAIR);
    if (p) {
        os << (tail ? "" : "(");
        const tag *b = p->_data.tail;
        print(os, p->_data.head, false);
        if (b) {
            print(os << (b->is_atom() ? " . " : " "), b, true);
        }
        return os << (tail ? "" : ")");
    }
    return os << "#nil";
}

std::ostream &operator<<(std::ostream &os, const tag *t)
{
    return print(os, t, false);
}

struct tag_reader
{
    tag *read_symbol(iter_t &i, iter_t e)
    {
        iter_t b = i;
        i = ::read_word(i, e);
        return i == b ? nullptr : new tag(tag::TYPE_SYMBOL, b, i);
    }

    tag *read_string(iter_t &i, iter_t e)
    {
        iter_t b = i;
        auto r = ::read_quoted(i, e);
        return (i = r.first) == b ? nullptr : new tag(tag::TYPE_STRING, b, i);
    }

    tag *read_number(iter_t &i, iter_t e)
    {
        iter_t b = i;
        auto r = ::read_number(i, e, false);
        return (i = r.first) == b ? nullptr : new tag(b, i, r.second.int_value(), r.second.double_value());
    }

    tag *read_tail(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        if (*i == ')') {
            ++i;
            return nullptr;
        }
        tag *head = read_expr(i, e);
        i = read_space(i, e);
        if (*i == '.' && chars_utf8::is_space(*(i + 1))) {
            i = read_space(++i, e);
            return new tag(head, read_expr(i, e));
        }
        return new tag(head, read_tail(i, e));
    }

    tag *read_pair(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        return *i != '(' ? nullptr : read_tail(++i, e);
    }

    tag *read_atom(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        tag *a = nullptr;
        if ((a = read_number(i, e))) { return a; }
        if ((a = read_string(i, e))) { return a; }
        if ((a = read_symbol(i, e))) { return a; }
        return a;
    }

    tag *read_expr(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        tag *a = nullptr;
        if (*i == ')')             { return nullptr; }
        if ((a = read_pair(i, e))) { return a; }
        if ((a = read_atom(i, e))) { return a; }
        return a;
    }
};

void read_lispesq()
{
    typedef std::string_view::const_iterator iter_t;
    std::string_view s("(anom sdfkj as d () \"hello\" 10 11 3.234e-10 der (quote kjl) (\"from\" . the_other_side))");
    std::cout << std::endl << s;
    iter_t i = s.begin(), e = s.end();
    tag_reader r;
    tag *a = r.read_expr(i, e);
    std::cout << std::endl << a;
}
