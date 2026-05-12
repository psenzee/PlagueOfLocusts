#pragma once

#include <string_view>
#include <iostream>
#include <array>
#include <string>
#include "lex.h"
#include "lex_numeric.h"

typedef std::string_view::const_iterator iter_t;

struct read_context;

struct tag
{
    enum Type : uint8_t { TYPE_NIL = 0, TYPE_PAIR, TYPE_SYMBOL, TYPE_NUMBER, TYPE_STRING, TYPE_ARRAY, _TYPE_COUNT };

    Type _type;
    
    union data {
        struct { uint32_t head, tail; };
        struct { iter_t b, e; int64_t ival; double fval; };
        struct { std::array<uint64_t, 4> _; };
        data() : _({ 0 }) {}
        data(uint32_t head, uint32_t tail) : head(head), tail(tail) {}
        data(iter_t b, iter_t e, int64_t i, double f) : b(b), e(e), ival(i), fval(f) {}
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
        default: break;
        }
        return os << type();
    }

    tag(Type type) : _type(type) {}
    tag(iter_t b, iter_t e, int64_t i, double f) : _type(TYPE_NUMBER), _data(b, e, i, f) {}
    tag(uint32_t head, uint32_t tail) : _type(TYPE_PAIR), _data(head, tail) {}
    explicit tag(Type type, iter_t b, iter_t e) : _type(type), _data(b, e, 0, 0.) {}

    inline static const char *type(Type type)
    {
        static constexpr const char *names[] = { "NIL", "PAIR", "SYMBOL", "NUMBER", "STRING", "ARRAY" };
        return names[type < _TYPE_COUNT ? type : TYPE_NIL];
    }
};

tag       *cast(tag *u, tag::Type type)       { return u && u->is(type) ? u : nullptr; }
const tag *cast(const tag *u, tag::Type type) { return u && u->is(type) ? u : nullptr; }

struct read_context
{
    std::vector<tag> _tags;
    std::string      _owner;
    std::string_view _text;
    
    enum { _NIL = ~0 };
    
    read_context(std::string_view text, bool own = true) : _text(text)
    {
        if (own) {
            _owner = text;
            _text = _owner;
            _tags.reserve(std::distance(_text.begin(), _text.end()) / 3); // start with a reasonable size
        }
    }
    
    uint32_t read_symbol(iter_t &i, iter_t e)
    {
        iter_t b = i;
        i = ::read_word(i, e);
        return i == b ? _NIL : allocate(tag(tag::TYPE_SYMBOL, b, i));
    }

    uint32_t read_string(iter_t &i, iter_t e)
    {
        iter_t b = i;
        auto r = ::read_quoted(i, e);
        return (i = r.first) == b ? _NIL : allocate(tag(tag::TYPE_STRING, b, i));
    }

    uint32_t read_number(iter_t &i, iter_t e)
    {
        iter_t b = i;
        auto r = ::read_number(i, e, false);
        return (i = r.first) == b ? _NIL : allocate(tag(b, i, r.second.int_value(), r.second.double_value()));
    }

    uint32_t read_tail(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        if (*i == ')') {
            ++i;
            return _NIL;
        }
        uint32_t p = allocate(tag(_NIL, _NIL)),
                 h = read_expr(i, e), t = _NIL;
        tag_at(p)->_data.head = h;
        i = read_space(i, e);
        if (*i == '.' && chars_utf8::is_space(*(i + 1))) {
            i = read_space(++i, e);
            t = read_expr(i, e);
        } else {
            t = read_tail(i, e);
        }
        tag_at(p)->_data.tail = t;
        return p;
    }

    uint32_t read_pair(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        return *i != '(' ? _NIL : read_tail(++i, e);
    }

    uint32_t read_atom(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        uint32_t a = _NIL;
        if ((a = read_number(i, e)) != _NIL) { return a; }
        if ((a = read_string(i, e)) != _NIL) { return a; }
        if ((a = read_symbol(i, e)) != _NIL) { return a; }
        return a;
    }

    uint32_t read_expr(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        uint32_t a = _NIL;
        if (*i == ')')                     { return _NIL; }
        if ((a = read_pair(i, e)) != _NIL) { return a; }
        if ((a = read_atom(i, e)) != _NIL) { return a; }
        return a;
    }
    
    tag       *tag_at(uint32_t i)       { return i >= _tags.size() ? nullptr : &_tags[i]; }
    const tag *tag_at(uint32_t i) const { return i >= _tags.size() ? nullptr : &_tags[i]; }
    
    std::ostream &print(std::ostream &os, uint32_t t, bool has_tail = false) const
    {
        if (t == _NIL) {
            return os << "#nil";
        }
        const tag *tp = tag_at(t);
        if (tp->is_atom()) {
            return tp->print(os);
        }
        const tag *p = cast(tp, tag::TYPE_PAIR);
        if (p) {
            os << (has_tail ? "" : "(");
            uint32_t b = p->_data.tail;
            print(os, p->_data.head, false);
            if (b != _NIL) {
                print(os << (tag_at(b)->is_atom() ? " . " : " "), b, true);
            }
            return os << (has_tail ? "" : ")");
        }
        return os << "#nil";
    }
    
private:
    
    uint32_t allocate(const tag &t)
    {
        uint32_t handle = static_cast<uint32_t>(_tags.size());
        _tags.push_back(t);
        return handle;
    }
};

void read_lispesq()
{
    typedef std::string_view::const_iterator iter_t;
    std::string_view s("(anom sdfkj as d () \"hello\" 10 11 3.234e-10 der (quote kjl) (\"from\" . the_other_side))");
    std::cout << std::endl << s;
    iter_t i = s.begin(), e = s.end();
    read_context r(s);
    uint32_t a = (r.read_expr(i, e));
    std::cout << std::endl << "#" << a << std::endl;
    r.print(std::cout << std::endl, a);
}
