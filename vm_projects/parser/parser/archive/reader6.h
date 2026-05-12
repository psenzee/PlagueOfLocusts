#pragma once

#include <string_view>
#include <iostream>
#include <array>
#include <string>
#include "lex.h"
#include "lex_numeric.h"

typedef std::string_view::const_iterator iter_t;

struct read_context;

class types
{
    static constexpr const char    *_names[] =  { "NIL",   "STRING", "SYMBOL", "I64", "F64", "PAIR" };
public:
    enum Type : uint8_t                         {  NIL = 0, STRING,   SYMBOL,   I64,   F64,   PAIR,  _TYPE_COUNT };

    static const char       *type_name(Type type)    { return _names[type]; }
    
    static constexpr bool    is_atom(Type type)      { return type < PAIR; }

    static constexpr Type    type(const int64_t &v)  { return I64; }
    static constexpr Type    type(const double &v)   { return F64; }
};

template <typename T> inline static constexpr types::Type find_type()          { return types::NIL; }
template <>           inline        constexpr types::Type find_type<int64_t>() { return types::I64; }
template <>           inline        constexpr types::Type find_type<double>()  { return types::F64; }

struct tag : public types
{
    Type _type;

    const char     *type_name()             const { return types::type_name(_type); }
    bool            is(Type type)           const { return _type == type; }
    bool            is_atom()               const { return types::is_atom(_type); }
    std::ostream   &print(std::ostream &os) const { return os << type_name(); }

    tag(Type type = NIL) : _type(type) {}
};

template <typename NumberT>
struct elem_num : public tag
{
    enum { TYPE = find_type<NumberT>() };
    NumberT value;
    elem_num(const NumberT &v) : tag(find_type<NumberT>()), value(v) {}
    std::ostream &print(std::ostream &os) const { return os << value; }
};

template <types::Type TypeT>
struct elem_str : public tag
{
    enum { TYPE = TypeT };
    iter_t b, e;
    elem_str(iter_t b, iter_t e) : tag(TypeT), b(b), e(e) {}
    std::ostream &print(std::ostream &os) const { return os << std::string_view(b, e); }
};

struct elem_pair : public tag
{
    enum { TYPE = types::PAIR };
    uint64_t head, tail;
    elem_pair(uint64_t head, uint64_t tail) : tag(types::PAIR), head(head), tail(tail) {}
};

class element_pool
{
    typedef std::tuple<
        std::vector<tag>,                          // NIL
        std::vector< elem_str<types::STRING> >,    // STRING
        std::vector< elem_str<types::SYMBOL> >,    // SYMBOL
        std::vector< elem_num<int64_t> >,          // I64
        std::vector< elem_num<double> >,           // F64
        std::vector< elem_pair >                   // PAIR
    > pools_t;

    pools_t _pools;
    
    template <size_t N> auto       &_pool()       { return std::get<N>(_pools); }
    template <size_t N> const auto &_pool() const { return std::get<N>(_pools); }

public:

    template <typename ElemT> uint64_t allocate(const ElemT &e)
    {
        uint64_t handle = uint64_t(ElemT::TYPE) << 32;
        auto &list = _pool<ElemT::TYPE>();
        handle |= list.size();
        list.push_back(e);
        return handle;
    }

    tag *get(uint64_t i)
    {
        size_t lo = i & 0x0ffffffffull;
        switch (i >> 32) {
            case 1:  return &(_pool<1>()[lo]);
            case 2:  return &(_pool<2>()[lo]);
            case 3:  return &(_pool<3>()[lo]);
            case 4:  return &(_pool<4>()[lo]);
            case 5:  return &(_pool<5>()[lo]);
            default: return nullptr;
        }
    }

    const tag *get(uint64_t i) const
    {
        size_t lo = i & 0x0ffffffffull;
        switch (i >> 32) {
            case 1:  return &(_pool<1>()[lo]);
            case 2:  return &(_pool<2>()[lo]);
            case 3:  return &(_pool<3>()[lo]);
            case 4:  return &(_pool<4>()[lo]);
            case 5:  return &(_pool<5>()[lo]);
            default: return nullptr;
        }
    }
};

struct read_context
{
    element_pool         _pool;
    std::string          _owner;
    std::string_view     _text;
    
    enum { _NIL = 0 };
    
    read_context(std::string_view text, bool own = true) : _text(text)
    {
        if (own) {
            _owner = text;
            _text = _owner;
        }
    }

    uint64_t allocate_pair(uint64_t head, uint64_t tail)
    {
        return _pool.allocate(elem_pair(head, tail));
    }

    uint64_t allocate_string(iter_t i, iter_t e)
    {
        return _pool.allocate(elem_str<types::STRING>(i, e));
    }

    uint64_t allocate_symbol(iter_t i, iter_t e)
    {
        return _pool.allocate(elem_str<types::SYMBOL>(i, e));
    }

    uint64_t allocate_number(iter_t i, iter_t e, const core_number &n)
    {
        if (n.is_int()) {
            return _pool.allocate(elem_num<int64_t>(n.int_value()));
        }
        return _pool.allocate(elem_num<double>(n.double_value()));
    }

    uint64_t read_symbol(iter_t &i, iter_t e)
    {
        iter_t b = i;
        i = ::read_word(i, e);
        return i == b ? _NIL : allocate_symbol(b, i);
    }

    uint64_t read_string(iter_t &i, iter_t e)
    {
        iter_t b = i;
        auto r = ::read_quoted(i, e);
        return (i = r.first) == b ? _NIL : allocate_string(b, i);
    }

    uint64_t read_number(iter_t &i, iter_t e)
    {
        iter_t b = i;
        auto r = ::read_number(i, e, false);
        return (i = r.first) == b ? _NIL : allocate_number(b, i, r.second);
    }

    uint64_t read_tail(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        if (*i == ')') {
            ++i;
            return _NIL;
        }
        uint64_t p = allocate_pair(_NIL, _NIL),
                 h = read_expr(i, e), t = _NIL;
        unsafe_cast_pair(p)->head = h;
        i = read_space(i, e);
        if (*i == '.' && chars_utf8::is_space(*(i + 1))) {
            i = read_space(++i, e);
            t = read_expr(i, e);
        } else {
            t = read_tail(i, e);
        }
        unsafe_cast_pair(p)->tail = t;
        return p;
    }

    static elem_pair        *unsafe_cast_pair(tag *t)                { return static_cast<elem_pair *>(t); }
    static const elem_pair  *unsafe_cast_pair(const tag *t)          { return static_cast<const elem_pair  *>(t); }
    elem_pair               *unsafe_cast_pair(uint64_t handle)       { return unsafe_cast_pair(_pool.get(handle)); }
    const elem_pair         *unsafe_cast_pair_const(uint64_t handle) const { return unsafe_cast_pair(_pool.get(handle)); }
    
    static elem_pair        *cast_pair(tag *t)                       { return t && t->is(types::PAIR) ? unsafe_cast_pair(t) : nullptr; }
    static const elem_pair  *cast_pair(const tag *t)                 { return t && t->is(types::PAIR) ? unsafe_cast_pair(t) : nullptr; }
    elem_pair               *cast_pair(uint64_t handle)              { return cast_pair(_pool.get(handle)); }
    const elem_pair         *cast_pair_const(uint64_t handle)        const { return cast_pair(_pool.get(handle)); }

    std::ostream &print_atom(std::ostream &os, const tag *t) const
    {
        if (!t) {
            return os << "#nil";
        }
        switch (t->_type) {
            case types::NIL:    return os << "#nil";
            case types::STRING: return static_cast<const elem_str<types::STRING> *>(t)->print(os);
            case types::SYMBOL: return static_cast<const elem_str<types::SYMBOL> *>(t)->print(os);
            case types::I64:    return static_cast<const elem_num<int64_t> *>(t)->print(os);
            case types::F64:    return static_cast<const elem_num<double> *>(t)->print(os);
            default: break;
        }
        return os;
    }

    std::ostream &print_atom(std::ostream &os, uint64_t i) const
    {
        return print_atom(os, _pool.get(i));
    }

    uint64_t read_pair(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        return *i != '(' ? _NIL : read_tail(++i, e);
    }

    uint64_t read_atom(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        uint64_t a = _NIL;
        if ((a = read_number(i, e)) != _NIL) { return a; }
        if ((a = read_string(i, e)) != _NIL) { return a; }
        if ((a = read_symbol(i, e)) != _NIL) { return a; }
        return a;
    }

    uint64_t read_expr(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        uint64_t a = _NIL;
        if (*i == ')')                     { return _NIL; }
        if ((a = read_pair(i, e)) != _NIL) { return a; }
        if ((a = read_atom(i, e)) != _NIL) { return a; }
        return a;
    }

    std::ostream &print(std::ostream &os, uint64_t t, bool has_tail = false) const
    {
        if (t == _NIL) {
            return os << "#nil";
        }
        const tag *tp = _pool.get(t);
        if (tp->is_atom()) {
            return print_atom(os, tp);
        }
        const auto *p = cast_pair(tp);
        if (p) {
            os << (has_tail ? "" : "(");
            uint64_t b = p->tail;
            print(os, p->head, false);
            if (b != _NIL) {
                print(os << (_pool.get(b)->is_atom() ? " . " : " "), b, true);
            }
            return os << (has_tail ? "" : ")");
        }
        return os << "#nil";
    }
};

void read_lispesq()
{
    typedef std::string_view::const_iterator iter_t;
    std::string_view s("(anom sdfkj as d () \"hello\" 10 11 3.234e-10 der (quote kjl) (\"from\" . the_other_side))");
    std::cout << std::endl << s;
    iter_t i = s.begin(), e = s.end();
    read_context r(s);
    uint64_t a = (r.read_expr(i, e));
    std::cout << std::endl << "#" << std::hex << a << std::endl;
    std::cout << "sizeof tag " << sizeof(tag) << std::endl;
    r.print(std::cout << std::endl, a);
}
