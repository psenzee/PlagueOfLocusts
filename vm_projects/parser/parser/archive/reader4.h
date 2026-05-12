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
inline constexpr size_t word_count(size_t bytes)
{
    enum { WORD_BYTES = sizeof(uint64_t) };
    return (bytes + WORD_BYTES - 1) / WORD_BYTES;
}
}

struct types
{
    enum Type : uint8_t { NIL = 0, STRING, SYMBOL, NUMBER, I32, U32, F32, I64, U64, F64, PAIR, ARRAY3, _TYPE_COUNT };

    inline static const char *type_name(Type type)
    {
        static constexpr const char *names[] = { "NIL", "STRING", "SYMBOL", "NUMBER", "I32", "U32", "F32", "I64", "U64", "F64", "PAIR", "ARRAY3" };
        return names[type < _TYPE_COUNT ? type : NIL];
    }
    
    static constexpr bool is_atom(Type type)      { return type < PAIR; }

    static constexpr Type type(const uint32_t &v) { return U32; }
    static constexpr Type type(const int32_t &v)  { return I32; }
    static constexpr Type type(const float &v)    { return F32; }
    static constexpr Type type(const uint64_t &v) { return U64; }
    static constexpr Type type(const int64_t &v)  { return I64; }
    static constexpr Type type(const double &v)   { return F64; }
    
    template <typename T, size_t N> static constexpr Type type(const std::array<T, N> &v)
    {
        switch (N) {
            case 2:  return PAIR;
            case 3:  return ARRAY3;
            default: return NIL;
        }
    }
};

struct tag : public types
{
    Type    _type;
    uint8_t _words, _count, _flags;

    const char     *type_name()             const { return types::type_name(_type); }
    bool            is(Type type)           const { return _type == type; }
    bool            is_atom()               const { return types::is_atom(_type); }
    std::ostream   &print(std::ostream &os) const { return os << type_name(); }

    tag(Type type = NIL, uint8_t bytes = sizeof(tag), uint8_t count = 0, uint8_t flags = 0) :
        _type(type), _words(word_count(bytes)), _count(count), _flags(flags) {}
};

struct handle
{
    enum { MARK = 0x93AD };
    uint16_t mark;
    uint8_t  words;
    uint8_t  type;
    uint32_t id;
    handle(uint8_t words = 0, uint32_t id = 0) : mark(MARK), words(words), type(0), id(id) {}
};

union resolvable_handle
{
    handle  handle;
    tag    *pointer;
    
    bool resolved() const { return handle.mark != handle::MARK; }
};

struct number_or_string
{
    iter_t b, e; int64_t ival; double fval;
    
    std::ostream &print_as_string(std::ostream &os) const { return os << std::string_view(b, e); }
    std::ostream &print_as_number(std::ostream &os) const { return fval != 0. ? (os << fval) : (os << ival); }
};

struct handle_pair { uint32_t head, tail; };

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
    enum { SIZE = sizeof(num_t) + sizeof(tag) };
    num_t data;
    element_number() : tag(NIL, SIZE) {}
    template <typename NumT> explicit element_number(const NumT &v) : tag(type(v), SIZE), data(v) {}
    std::ostream &print(std::ostream &os) const { return print(data, _type); }
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

    element_24b(Type type) : tag(type, SIZE) {}
    element_24b(uint64_t head, uint64_t tail) : tag(PAIR, SIZE), data(head, tail) {}
    explicit element_24b(Type type, iter_t b, iter_t e) : tag(type, SIZE), data(b, e) {}
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
    enum { SIZE = sizeof(iter_t) * 2 + sizeof(int64_t) + sizeof(double) + sizeof(tag) };
    iter_t b, e;
    int64_t ival;
    double fval;
    element_40b(iter_t b, iter_t e, int64_t i, double f) : tag(NUMBER, SIZE), b(b), e(e), ival(i), fval(f) {}
    std::ostream &print(std::ostream &os) const { return fval != 0. ? (os << fval) : (os << ival); }
};
/*
struct element_8 : public tag
{
    typedef number_union<int32_t, uint32_t, float> num_t;
    enum { SIZE = sizeof(num_t) + sizeof(tag) };
    num_t data;
    element_8() : tag(NIL, SIZE) {}
    template <typename NumT> explicit element_8(NumT v) : tag(type(v), SIZE), data(v) {}
};

struct element_16 : public tag
{
    typedef number_union<int64_t, uint64_t, double> num_t;
    enum { SIZE = sizeof(num_t) + sizeof(tag) };
    num_t data;
    element_16() : tag(NIL, SIZE) {}
    template <typename NumT> explicit element_16(NumT v) : tag(type(v), SIZE), data(v) {}
};
/*
struct element_16 : public tag
{
    enum { SIZE = 16 /* sizeof(element_16) * / };
    typedef number_union<int64_t, uint64_t, double> number_t;
    union _data {
        struct { uint32_t head, tail; };
        number_t number;
        _data() : head(0), tail(0) {}
        explicit _data(const number_t &n) : number(n) {}
        explicit _data(uint32_t head, uint32_t tail) : head(head), tail(tail) {}
    } data;
    element_16() : tag(NIL, SIZE) {}
    template <typename NumT> explicit element_16(NumT v) : tag(type64(v), SIZE), data(number_t(v)) {}
};

/*
struct element_16_ : public tag
{
    enum { SIZE = 16 /* sizeof(element_16) * / };
    typedef number_union<int64_t, uint64_t, double> number_t;
    union _data {
        struct { uint32_t head, tail; };
        number_t number;
        _data() : head(0), tail(0) {}
        explicit _data(const number_t &n) : number(n) {}
        explicit _data(uint32_t head, uint32_t tail) : head(head), tail(tail) {}
    } data;
    element_16_() : tag(NIL, SIZE) {}
    template <typename NumT> explicit element_16_(NumT v) : tag(type64(v), SIZE), data(number_t(v)) {}
};*/

/*
struct element_16 : public tag
{
    union _data {
        struct { uint32_t head, tail; };
        int64_t  int_value;
        uint64_t uint_value;
        double   double_value;
        _data(uint32_t head, uint32_t tail) : head(head), tail(tail) {}
        _data(uint64_t v) : uint_value(v) {}
        _data(int64_t v) : int_value(v) {}
        _data(double v) : double_value(v) {}
    } data;
    element_16(uint32_t head, uint32_t tail) : tag(PAIR, sizeof(element_16)), data(head, tail) {}
    element_16(uint64_t v) : tag(U64, sizeof(element_16)), data(v) {}
    element_16(int64_t v) : tag(I64, sizeof(element_16)), data(v) {}
    element_16(double v) : tag(F64, sizeof(element_16)), data(v) {}
};
*/

struct element : public tag
{
    union _data {
        struct { uint32_t head, tail; };
        struct { iter_t b, e; int64_t ival; double fval; };
        _data() : head(0), tail(0) {}
        _data(uint32_t head, uint32_t tail) : head(head), tail(tail) {}
        _data(iter_t b, iter_t e, int64_t i, double f) : b(b), e(e), ival(i), fval(f) {}
    } data;

    std::ostream &print(std::ostream &os) const
    {
        switch (_type) {
        case STRING:
        case SYMBOL: return os << std::string_view(data.b, data.e);
        case NUMBER: return data.fval != 0. ? (os << data.fval) : (os << data.ival);
        default: break;
        }
        return os << type_name();
    }

    element(Type type) : tag(type, sizeof(element)) {}
    element(iter_t b, iter_t e, int64_t i, double f) : tag(NUMBER, sizeof(element)), data(b, e, i, f) {}
    element(uint32_t head, uint32_t tail) : tag(PAIR, sizeof(element)), data(head, tail) {}
    explicit element(Type type, iter_t b, iter_t e) : tag(type, sizeof(element)), data(b, e, 0, 0.) {}
};

element       *cast(element *u, element::Type type)       { return u && u->is(type) ? u : nullptr; }
const element *cast(const element *u, element::Type type) { return u && u->is(type) ? u : nullptr; }

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
        uint64_t handle = uint64_t(ElemT::SIZE) << 32;
        auto &list = _pool<ElemT::SIZE>();
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


struct read_context2
{
    element_pool         _pool;
    std::string          _owner;
    std::string_view     _text;
    
    enum { _NIL = ~0 };
    
    read_context2(std::string_view text, bool own = true) : _text(text)
    {
        if (own) {
            _owner = text;
            _text = _owner;
        }
    }
    
    uint32_t read_symbol(iter_t &i, iter_t e)
    {
        iter_t b = i;
        i = ::read_word(i, e);
        return i == b ? _NIL : _pool.allocate(element_24b(types::SYMBOL, b, i));
    }

    uint32_t read_string(iter_t &i, iter_t e)
    {
        iter_t b = i;
        auto r = ::read_quoted(i, e);
        return (i = r.first) == b ? _NIL : _pool.allocate(element_24b(types::STRING, b, i));
    }

    uint32_t read_number(iter_t &i, iter_t e)
    {
        iter_t b = i;
        auto r = ::read_number(i, e, false);
        return (i = r.first) == b ? _NIL : _pool.allocate(element_40b(b, i, r.second.int_value(), r.second.double_value()));
    }

    uint32_t read_tail(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        if (*i == ')') {
            ++i;
            return _NIL;
        }
        uint32_t p = allocate(element(_NIL, _NIL)),
                 h = read_expr(i, e), t = _NIL;
        element_at(p)->data.head = h;
        i = read_space(i, e);
        if (*i == '.' && chars_utf8::is_space(*(i + 1))) {
            i = read_space(++i, e);
            t = read_expr(i, e);
        } else {
            t = read_tail(i, e);
        }
        element_at(p)->data.tail = t;
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
    
    element       *element_at(uint32_t i)       { return i >= _elements.size() ? nullptr : &_elements[i]; }
    const element *element_at(uint32_t i) const { return i >= _elements.size() ? nullptr : &_elements[i]; }
    
    std::ostream &print(std::ostream &os, uint32_t t, bool has_tail = false) const
    {
        if (t == _NIL) {
            return os << "#nil";
        }
        const element *tp = element_at(t);
        if (tp->is_atom()) {
            return tp->print(os);
        }
        const element *p = cast(tp, element::PAIR);
        if (p) {
            os << (has_tail ? "" : "(");
            uint32_t b = p->data.tail;
            print(os, p->data.head, false);
            if (b != _NIL) {
                print(os << (element_at(b)->is_atom() ? " . " : " "), b, true);
            }
            return os << (has_tail ? "" : ")");
        }
        return os << "#nil";
    }
    
private:

    uint32_t allocate(const element &t)
    {
        uint32_t handle = static_cast<uint32_t>(_elements.size());
        _elements.push_back(t);
        return handle;
    }
};


struct read_context
{
    std::vector<element> _elements;
    std::string          _owner;
    std::string_view     _text;
    
    enum { _NIL = ~0 };
    
    read_context(std::string_view text, bool own = true) : _text(text)
    {
        if (own) {
            _owner = text;
            _text = _owner;
            _elements.reserve(std::distance(_text.begin(), _text.end()) / 3); // start with a reasonable size
        }
    }
    
    uint32_t read_symbol(iter_t &i, iter_t e)
    {
        iter_t b = i;
        i = ::read_word(i, e);
        return i == b ? _NIL : allocate(element(element::SYMBOL, b, i));
    }

    uint32_t read_string(iter_t &i, iter_t e)
    {
        iter_t b = i;
        auto r = ::read_quoted(i, e);
        return (i = r.first) == b ? _NIL : allocate(element(element::STRING, b, i));
    }

    uint32_t read_number(iter_t &i, iter_t e)
    {
        iter_t b = i;
        auto r = ::read_number(i, e, false);
        return (i = r.first) == b ? _NIL : allocate(element(b, i, r.second.int_value(), r.second.double_value()));
    }

    uint32_t read_tail(iter_t &i, iter_t e)
    {
        i = read_space(i, e);
        if (*i == ')') {
            ++i;
            return _NIL;
        }
        uint32_t p = allocate(element(_NIL, _NIL)),
                 h = read_expr(i, e), t = _NIL;
        element_at(p)->data.head = h;
        i = read_space(i, e);
        if (*i == '.' && chars_utf8::is_space(*(i + 1))) {
            i = read_space(++i, e);
            t = read_expr(i, e);
        } else {
            t = read_tail(i, e);
        }
        element_at(p)->data.tail = t;
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
    
    element       *element_at(uint32_t i)       { return i >= _elements.size() ? nullptr : &_elements[i]; }
    const element *element_at(uint32_t i) const { return i >= _elements.size() ? nullptr : &_elements[i]; }
    
    std::ostream &print(std::ostream &os, uint32_t t, bool has_tail = false) const
    {
        if (t == _NIL) {
            return os << "#nil";
        }
        const element *tp = element_at(t);
        if (tp->is_atom()) {
            return tp->print(os);
        }
        const element *p = cast(tp, element::PAIR);
        if (p) {
            os << (has_tail ? "" : "(");
            uint32_t b = p->data.tail;
            print(os, p->data.head, false);
            if (b != _NIL) {
                print(os << (element_at(b)->is_atom() ? " . " : " "), b, true);
            }
            return os << (has_tail ? "" : ")");
        }
        return os << "#nil";
    }
    
private:

    uint32_t allocate(const element &t)
    {
        uint32_t handle = static_cast<uint32_t>(_elements.size());
        _elements.push_back(t);
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
    std::cout << "sizeof element " << sizeof(element) << std::endl;
    std::cout << "sizeof element_8b " << sizeof(element_8b) << std::endl;
    std::cout << "sizeof element_16b " << sizeof(element_16b) << std::endl;
    std::cout << "sizeof handle " << sizeof(handle) << std::endl;
    std::cout << "sizeof element._data " << sizeof(element::_data) << std::endl;
    std::cout << "sizeof resolvable_handle " << sizeof(resolvable_handle) << std::endl;
    std::cout << "sizeof tag " << sizeof(tag) << std::endl;
    r.print(std::cout << std::endl, a);
}
