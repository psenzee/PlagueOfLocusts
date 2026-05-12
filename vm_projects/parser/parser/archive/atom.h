#pragma once

#include <string_view>
#include <iostream>
#include <array>
#include <string>
#include "lex.h"
#include "lex_numeric.h"

struct tag
{
    enum Type : uint8_t { TYPE__ERROR = 0, TYPE_NIL, TYPE_ATOM, TYPE_TUPLE, TYPE_SYMBOL, TYPE_NUMBER, TYPE_STRING, TYPE_ARRAY, _TYPE_COUNT };

    Type     _type;
    uint8_t  _bytes;
    uint16_t _flags;

    inline static const char *type(Type type)
    {
        static constexpr const char *names[] = { "_ERROR_", "NIL", "ATOM", "TUPLE", "SYMBOL", "NUMBER", "STRING", "ARRAY" };
        return names[type < _TYPE_COUNT ? type : TYPE__ERROR];
    }

    inline const char           *type()                  const { return type(_type); }
    inline bool                  is(Type type)           const { return _type == type; }
    inline virtual bool          is_atom()               const { return true; }
    inline virtual tag          *get(size_t i)                 { return nullptr; }
    inline virtual const tag    *get(size_t i)           const { return nullptr; }
    inline virtual void          set(size_t i, tag *t)         {}
    inline virtual size_t        count()                 const { return 0; }
    inline virtual std::ostream &print(std::ostream &os) const { return os << type(); }

    inline tag(Type type, uint16_t flags = 0, uint8_t bytes = 0) : _type(type), _bytes(bytes), _flags(flags) {}
};

template <size_t N> struct tuple : public tag
{
    static constexpr const Type this_type = TYPE_TUPLE;
    std::array<tag *, N> _data;
    inline tuple(tag *a = nullptr, tag *b = nullptr) : tag(this_type, 0, sizeof(tuple<N>)), _data({ a, b }) {}
    
    inline bool          is_atom()             const { return false; }
    inline tag          *get(size_t i)               { return _data[i]; }
    inline const tag    *get(size_t i)         const { return _data[i]; }
    inline void          set(size_t i, tag *t)       { _data[i] = t; }
    inline size_t        count()               const { return N; }
};

typedef tuple<2> pair_t;

struct string : public tag
{
    static constexpr const Type this_type = TYPE_STRING;
    typedef std::string::const_iterator iter_t;
    std::string_view _data;

    inline string() : tag(this_type, 0, sizeof(string)) {}
    inline string(std::string_view s, Type type = this_type) : tag(this_type, 0, sizeof(string)), _data(s) {}
    inline std::ostream &print(std::ostream &os) const { return os << _data; }
};

struct symbol : public string
{
    static constexpr const Type this_type = TYPE_SYMBOL;
    inline symbol(std::string_view s) : string(s, this_type) {}
};

struct number : public string
{
    static constexpr const Type this_type = TYPE_NUMBER;
    core_number _value;
    inline std::ostream &print(std::ostream &os) const { return os << _value.double_value(); }
    inline number(std::string_view s, const core_number &n) : string(s, this_type), _value(n) {}
};

template <typename T> T       *cast(tag *u)       { return u && u->is(T::this_type) ? (T *)u       : nullptr; }
template <typename T> const T *cast(const tag *u) { return u && u->is(T::this_type) ? (const T *)u : nullptr; }

std::ostream &print(std::ostream &os, const tag *t, bool tail = false)
{
    if (t == nullptr) {
        return os << "#nil";
    }
    if (t->is_atom()) {
        return t->print(os);
    }
    const pair_t *p = cast<pair_t>(t);
    if (p) {
        os << (tail ? "" : "(");
        const tag *b = p->get(1);
        print(os, p->get(0), false);
        if (b) {
            print(os << " ", b, true);
        }
        return os << (tail ? "" : ")");
    }
    return os << "#nil";
}

std::ostream &operator<<(std::ostream &os, const tag *t)
{
    return print(os, t, false);
}

typedef std::string_view::const_iterator iter_t;

tag *read_sexpr(iter_t &i, iter_t e);

symbol *read_symbol(iter_t &i, iter_t e)
{
    iter_t b = i;
    i = read_word(i, e);
    return i == b ? nullptr : new symbol(std::string_view(b, i));
}

string *read_string(iter_t &i, iter_t e)
{
    iter_t b = i;
    auto r = read_quoted(i, e);
    return (i = r.first) == b ? nullptr : new string(std::string_view(b, i));
}

number *read_number(iter_t &i, iter_t e)
{
    iter_t b = i;
    auto r = read_number(i, e, false);
    return (i = r.first) == b ? nullptr : new number(std::string_view(b, i), r.second);
}

template <size_t N> tuple<N> *read_tail(iter_t &i, iter_t e)
{
    i = read_space(i, e);
    if (*i == ')') {
        i++;
        return nullptr;
    }
    return new tuple<N>(read_sexpr(i, e), read_tail<N>(i, e));
}

template <size_t N> tuple<N> *read_tuple(iter_t &i, iter_t e)
{
    i = read_space(i, e);
    return *i != '(' ? nullptr : read_tail<N>(++i, e);
}

tag *read_atom(iter_t &i, iter_t e)
{
    i = read_space(i, e);
    tag *a = nullptr;
    if ((a = read_number(i, e))) { return a; }
    if ((a = read_string(i, e))) { return a; }
    if ((a = read_symbol(i, e))) { return a; }
    // ...
    // other atoms... wip
    return a;
}

tag *read_sexpr(iter_t &i, iter_t e)
{
    i = read_space(i, e);
    tag *a = nullptr;
    if (*i == ')')                 { return nullptr; }
    if ((a = read_tuple<2>(i, e))) { return a; }
    if ((a = read_atom(i, e)))     { return a; }
    return a;
}

void read_lispesq()
{
    typedef std::string_view::const_iterator iter_t;
    std::string_view s("(anom sdfkj as d () \"hello\" 10 11 3.234e-10 der (quote kjl))");
    iter_t i = s.begin(), e = s.end();
    tag *a = read_sexpr(i, e);
    std::cout << std::endl << a;
}
