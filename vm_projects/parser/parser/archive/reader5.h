#pragma once

#include <string_view>
#include <iostream>
#include <array>
#include <string>
#include "lex.h"
#include "lex_numeric.h"

typedef std::string_view::const_iterator iter_t;

struct read_context;

namespace {
inline constexpr size_t word_count(size_t bytes) { enum { BYTES = sizeof(uint64_t) }; return (bytes + BYTES - 1) / BYTES; }
}

class types
{
    static constexpr const size_t   _widths[] = {        1,      3,        3,        5,     1,     1,     1,     2,     2,     2,      3  };
    static constexpr const char    *_names[] =  { "NIL",   "STRING", "SYMBOL", "NUMBER", "I32", "U32", "F32", "I64", "U64", "F64", "PAIR" };
public:
    enum Type : uint8_t                         {  NIL = 0, STRING,   SYMBOL,   NUMBER,   I32,   U32,   F32,   I64,   U64,   F64,   PAIR,  _TYPE_COUNT };

    static const char       *type_name(Type type)    { return _names[type]; }
    
    static constexpr bool    is_atom(Type type)      { return type < PAIR; }
    static constexpr size_t  width(Type type)        { return _widths[type]; }

    static constexpr Type    type(const uint32_t &v) { return U32; }
    static constexpr Type    type(const int32_t &v)  { return I32; }
    static constexpr Type    type(const float &v)    { return F32; }
    static constexpr Type    type(const uint64_t &v) { return U64; }
    static constexpr Type    type(const int64_t &v)  { return I64; }
    static constexpr Type    type(const double &v)   { return F64; }
};

struct tag : public types
{
    Type _type;

    const char     *type_name()             const { return types::type_name(_type); }
    size_t          width()                 const { return types::width(_type); }
    bool            is(Type type)           const { return _type == type; }
    bool            is_atom()               const { return types::is_atom(_type); }
    std::ostream   &print(std::ostream &os) const { return os << type_name(); }

    tag(Type type = NIL) : _type(type) {}
};

template <typename IntT, typename UIntT, typename FloatT>
union number_union {

    IntT   sign;
    UIntT  unsign;
    FloatT real;

    number_union() : sign(0) {}
    explicit number_union(const IntT &v)   : sign(v)   {}
    explicit number_union(const UIntT &v)  : unsign(v) {}
    explicit number_union(const FloatT &v) : real(v)   {}
    
    std::ostream &print(std::ostream &os, types::Type type) const
    {
        switch (type) {
        case types::I32: case types::I64: return os << sign;
        case types::U32: case types::U64: return os << unsign;
        case types::F32: case types::F64: return os << real;
        default: return os;
        }
    }
};

template <typename IntT, typename UIntT, typename FloatT>
struct element_number : public tag
{
    typedef number_union<IntT, UIntT, FloatT> num_t;
    num_t data;
    template <typename NumT> explicit element_number(const NumT &v) : tag(type(v)), data(v) {}
    std::ostream &print(std::ostream &os) const { return data.print(os, _type); }
};

typedef element_number<int32_t, uint32_t, float>  element_8b;
typedef element_number<int64_t, uint64_t, double> element_16b;

struct element_24b : public tag
{
    union _data {
        struct { uint64_t head, tail; };
        struct { iter_t b, e; };
        _data() : head(0), tail(0) {}
        explicit _data(uint64_t head, uint64_t tail) : head(head), tail(tail) {}
        explicit _data(iter_t b, iter_t e) : b(b), e(e) {}
    } data;

    enum { SIZE = sizeof(_data) + sizeof(tag) };

    std::ostream &print(std::ostream &os) const { return os << std::string_view(data.b, data.e); }

    element_24b(Type type) : tag(type) {}
    element_24b(uint64_t head, uint64_t tail) : tag(PAIR), data(head, tail) {}
    explicit element_24b(Type type, iter_t b, iter_t e) : tag(type), data(b, e) {}
};

template <size_t N>
struct element_array : public tag
{
    typedef std::array<uint64_t, N> array_t;
    array_t data;
    enum { SIZE = sizeof(array_t) + sizeof(tag) };
    element_array(const array_t &v) : tag(type(v), SIZE), data(v) {}
    std::ostream &print(std::ostream &os) const { return os << type_name(); }
};

typedef element_array<3> element_32b;

struct element_40b : public tag
{
    iter_t b, e;
    int64_t ival;
    double fval;
    element_40b(iter_t b, iter_t e, int64_t i, double f) : tag(NUMBER), b(b), e(e), ival(i), fval(f) {}
    std::ostream &print(std::ostream &os) const { return fval != 0. ? (os << fval) : (os << ival); }
};

tag       *cast(tag *u, types::Type type)       { return u && u->is(type) ? u : nullptr; }
const tag *cast(const tag *u, types::Type type) { return u && u->is(type) ? u : nullptr; }

class element_pool
{
    typedef std::tuple<
        std::vector<tag>, // index zero not used
        std::vector<element_8b>,
        std::vector<element_16b>,
        std::vector<element_24b>,
        std::vector<element_32b>,
        std::vector<element_40b>
    > pools_t;

    pools_t _pools;

    template <size_t N> auto       &_pool()       { return std::get<N>(_pools); }
    template <size_t N> const auto &_pool() const { return std::get<N>(_pools); }

public:

    template <typename ElemT> uint64_t allocate(const ElemT &e)
    {
        constexpr size_t w = word_count(sizeof(e));
        uint64_t handle = uint64_t(w) << 32;
        auto &list = _pool<w>();
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
        return _pool.allocate(element_24b(head, tail));
    }

    uint64_t allocate_string(iter_t i, iter_t e)
    {
        return _pool.allocate(element_24b(types::STRING, i, e));
    }

    uint64_t allocate_symbol(iter_t i, iter_t e)
    {
        return _pool.allocate(element_24b(types::SYMBOL, i, e));
    }

    uint64_t allocate_general_number(iter_t i, iter_t e, const core_number &n)
    {
        return _pool.allocate(element_40b(i, e, n.int_value(), n.double_value()));
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
        return (i = r.first) == b ? _NIL : allocate_general_number(b, i, r.second);
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
        unsafe_cast_pair(p)->data.head = h;
        i = read_space(i, e);
        if (*i == '.' && chars_utf8::is_space(*(i + 1))) {
            i = read_space(++i, e);
            t = read_expr(i, e);
        } else {
            t = read_tail(i, e);
        }
        unsafe_cast_pair(p)->data.tail = t;
        return p;
    }

    static element_24b       *unsafe_cast_pair(tag *t)                { return static_cast<element_24b *>(t); }
    static const element_24b *unsafe_cast_pair(const tag *t)          { return static_cast<const element_24b *>(t); }
    element_24b              *unsafe_cast_pair(uint64_t handle)       { return unsafe_cast_pair(_pool.get(handle)); }
    const element_24b        *unsafe_cast_pair_const(uint64_t handle) const { return unsafe_cast_pair(_pool.get(handle)); }
    
    static element_24b       *cast_pair(tag *t)                       { return t && t->is(types::PAIR) ? unsafe_cast_pair(t) : nullptr; }
    static const element_24b *cast_pair(const tag *t)                 { return t && t->is(types::PAIR) ? unsafe_cast_pair(t) : nullptr; }
    element_24b              *cast_pair(uint64_t handle)              { return cast_pair(_pool.get(handle)); }
    const element_24b        *cast_pair_const(uint64_t handle)        const { return cast_pair(_pool.get(handle)); }

    std::ostream &print_atom(std::ostream &os, const tag *t) const
    {
        if (!t) {
            return os << "#nil";
        }
        switch (t->width()) {
            case 0:  return os << "#nil";
            case 1:  return static_cast<const element_8b  *>(t)->print(os);
            case 2:  return static_cast<const element_16b *>(t)->print(os);
            case 3:  return static_cast<const element_24b *>(t)->print(os);
            case 4:  return static_cast<const element_32b *>(t)->print(os);
            case 5:  return static_cast<const element_40b *>(t)->print(os);
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
            uint64_t b = p->data.tail;
            print(os, p->data.head, false);
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
    std::cout << "sizeof element_8b " << sizeof(element_8b) << std::endl;
    std::cout << "sizeof element_16b " << sizeof(element_16b) << std::endl;
    std::cout << "sizeof element_24b " << sizeof(element_24b) << std::endl;
    std::cout << "sizeof element_32b " << sizeof(element_32b) << std::endl;
    std::cout << "sizeof element_40b " << sizeof(element_40b) << std::endl;
    std::cout << "sizeof tag " << sizeof(tag) << std::endl;
    r.print(std::cout << std::endl, a);
}
