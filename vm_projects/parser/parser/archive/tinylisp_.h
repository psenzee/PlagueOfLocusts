#pragma once

#include <string_view>
#include <iostream>
#include <sstream>
#include <array>
#include <string>
#include <cctype>
#include <algorithm>
#include <string_view>
#include "strs.h"
#include "elems.h"
#include "functions.h"

namespace tl
{
typedef std::string_view::const_iterator iter_t;

struct elem
{
    static constexpr const char *_names[] = { "NIL", "STRING", "SYMBOL", "INT", "FLOAT", "PAIR", "LAMBDA", "FUNCTION" };
    
    enum Type : uint8_t { NIL, STRING, SYMBOL, INT, FLOAT, PAIR, LAMBDA, FUNCTION };
    
    Type _type;
    
    const char             *type_name()              const { return _names[_type]; }
    bool                    is(Type type)            const { return _type == type; }
    bool                    is_atom()                const { return _type < PAIR; }
    bool                    is_number()              const { return _type == INT || _type == FLOAT; }
    virtual std::ostream   &print(std::ostream &os)  const { return os << type_name(); }
    virtual bool            equals(const elem *e) const { return e->_type == _type; }
    
    elem(Type type = NIL) : _type(type) {}
};

template <typename T> constexpr elem::Type element_type()              { return elem::NIL; }
template <>           constexpr elem::Type element_type<int64_t>()     { return elem::INT; }
template <>           constexpr elem::Type element_type<double>()      { return elem::FLOAT; }

template <typename T> T *cast(elem *t)             { return t && t->is(elem::Type(T::TYPE)) ? static_cast<T *>(t) : nullptr; }
template <typename T> const T *cast(const elem *t) { return t && t->is(elem::Type(T::TYPE)) ? static_cast<const T *>(t) : nullptr; }

template <elem::Type TypeT>
struct typed_elem : public elem
{
    static constexpr const elem::Type TYPE = TypeT;
    typed_elem(Type type = TypeT) : elem(type) {}
};

struct nil : public typed_elem<elem::NIL>
{
    std::ostream &print(std::ostream &os) const { return os << "()"; }
};

struct pair : public typed_elem<elem::PAIR>
{
    elem *head, *tail;
    pair(elem *head = nullptr, elem *tail = nullptr) : typed_elem<elem::PAIR>(), head(head), tail(tail) {}
    bool is_nil() const { return !head && !tail; }
};

struct function_call : public typed_elem<elem::FUNCTION>
{
    size_t _arity;
    std::string_view _name;
    function_call(size_t arity, std::string_view internal_name) : _arity(arity), _name(internal_name) {}
    size_t arity() const { return _arity; }
    virtual elem *call(elem **args) { return nullptr; }
    std::ostream &print(std::ostream &os) const { return os << "#fn_" << _name << "$" << arity(); }
};

struct lambda : public typed_elem<elem::LAMBDA>
{
    pair *instructions, *params;
    lambda(pair *params, pair *instructions) : typed_elem<elem::LAMBDA>(), params(params), instructions(instructions) {}
};

template <typename ValueT, elem::Type TypeT = element_type<ValueT>()>
struct box : public typed_elem<TypeT>
{
    ValueT value;
    box(const ValueT &v = ValueT{}) : typed_elem<TypeT>(), value(v) {}
    std::ostream &print(std::ostream &os) const { return os << value; }
    bool equals(const elem *e) const { return e && e->_type == elem::_type && cast< box<ValueT> >(e)->value == value; }
};

template <elem::Type TypeT>
struct str_base : public typed_elem<TypeT>
{
    str_base() : typed_elem<TypeT>() {}
    virtual std::string_view view() const { return std::string_view(""); }
    std::ostream &print(std::ostream &os) const { return os << view(); }
    bool equals(const elem *e) const { return e && e->_type == elem::_type && cast< str_base<TypeT> >(e)->view() == view(); }
    bool equals(std::string_view s) const { return s == view(); }
};

template <elem::Type TypeT>
struct string : public str_base<TypeT>
{
    std::string value;
    string(iter_t b, iter_t e) : str_base<TypeT>(), value(std::string_view(b, e)) {}
    string(std::string_view s) : str_base<TypeT>(), value(s) {}
    string(const string<TypeT> &s) : str_base<TypeT>(), value(s.value) {}
    std::string_view view() const { return std::string_view(value.begin(), value.end()); }
};

typedef string<elem::STRING> string_t;
typedef box<double>          number_t;

struct symbol : public string<elem::SYMBOL>
{
    typedef string<elem::SYMBOL> base_t;
    int id;
    symbol(iter_t b, iter_t e) : base_t(b, e), id(0) { _resolve(); }
    symbol(std::string_view s) : base_t(s), id(0) { _resolve(); }
    symbol(const symbol &s) : base_t(s), id(s.id) { _resolve(); }
    std::ostream &print(std::ostream &os) const { os << view(); if (id) os << "[" << std::dec << id << "]"; return os; }
private:
    void _resolve() { id = symbol_id::resolve(value); }
};

elem *rebox(elem *value)       { return value; }
elem *rebox(float value)       { return new box<double>(value); }
elem *rebox(double value)      { return new box<double>(value); }
elem *rebox(int64_t value)     { return new box<int64_t>(value); }
elem *rebox(size_t value)      { return new box<int64_t>(int64_t(value)); }
elem *rebox(int32_t value)     { return new box<int64_t>(value); }
elem *rebox(bool value)        { return value ? new symbol("T") : nullptr; }
elem *rebox(const char *value) { return new string_t(value); }

template <typename ReturnT, size_t Arity> struct function : function_call
{
    typedef ReturnT (*fptr_t)(elem *, size_t);
    fptr_t _fptr;
    function(fptr_t fptr, std::string_view internal_name) : function_call(Arity, internal_name), _fptr(fptr) {}
    elem *call(elem **args) { return rebox(_fptr(args, arity())); }
};

template <typename ReturnT> struct function<ReturnT, 0> : function_call
{
    typedef ReturnT (*fptr_t)();
    fptr_t _fptr;
    function(fptr_t fptr, std::string_view internal_name) : function_call(0, internal_name), _fptr(fptr) {}
    elem *call(elem **args) { return rebox(_fptr()); }
};

template <typename ReturnT> struct function<ReturnT, 1> : function_call
{
    typedef ReturnT (*fptr_t)(elem *);
    fptr_t _fptr;
    function(fptr_t fptr, std::string_view internal_name) : function_call(1, internal_name), _fptr(fptr) {}
    elem *call(elem **args) { return rebox(_fptr(args[0])); }
};

template <typename ReturnT> struct function<ReturnT, 2> : function_call
{
    typedef ReturnT (*fptr_t)(elem *, elem *);
    fptr_t _fptr;
    function(fptr_t fptr, std::string_view internal_name) : function_call(2, internal_name), _fptr(fptr) {}
    elem *call(elem **args) { return rebox(_fptr(args[0], args[1])); }
};

template <typename ReturnT> struct function<ReturnT, 3> : function_call
{
    typedef ReturnT (*fptr_t)(elem *, elem *, elem *);
    fptr_t _fptr;
    function(fptr_t fptr, std::string_view internal_name) : function_call(3, internal_name), _fptr(fptr) {}
    elem *call(elem **args) { return rebox(_fptr(args[0], args[1], args[2])); }
};

template <typename ReturnT> struct function<ReturnT, 4> : function_call
{
    typedef ReturnT (*fptr_t)(elem *, elem *, elem *, elem *);
    fptr_t _fptr;
    function(fptr_t fptr, std::string_view internal_name) : function_call(4, internal_name), _fptr(fptr) {}
    elem *call(elem **args) { return rebox(_fptr(args[0], args[1], args[2], args[3])); }
};

pair           *_(elem *head = nullptr, elem *tail = nullptr)     { return new pair(head, tail); }
elem           *new_pair(elem *head, elem *tail)                  { return new pair(head, tail); }
elem           *head(pair *list)                                  { return list ? list->head : nullptr; }
bool            is(elem *e, elem::Type type)                      { return e ? e->is(type) : false; }
bool            is_atom(elem *e)                                  { return e ? e->is_atom() : false; }
bool            is_number(elem *e)                                { return e ? e->is_number() : false; }
bool            is_symbol(elem *e)                                { return is(e, elem::SYMBOL); }
bool            is_list(elem *e)                                  { return !e || e->is(elem::PAIR); }
bool            is_nil(pair *p)                                   { return !p || (!p->head && !p->tail); }
bool            is_nil(elem *e)                                   { return !e || e->is(elem::NIL) || (e->is(elem::PAIR) && is_nil(cast<pair>(e))); }
pair           *head_pair(pair *list)                             { return cast<pair>(head(list)); }
symbol         *head_symbol(pair *list)                           { return cast<symbol>(head(list)); }
elem           *head(elem *list)                                  { return head(cast<pair>(list)); }
pair           *head_pair(elem *list)                             { return head_pair(cast<pair>(list)); }
symbol         *head_symbol(elem *list)                           { return head_symbol(cast<pair>(list)); }
elem           *tail(pair *list)                                  { return list ? list->tail : nullptr; }
pair           *tail_pair(pair *list)                             { return cast<pair>(tail(list)); }
elem           *tail(elem *list)                                  { return tail(cast<pair>(list)); }
pair           *tail_pair(elem *list)                             { return tail_pair(cast<pair>(list)); }
bool            equals(elem *a, elem *b)                          { return a == b || (a && b && a->equals(b)); }
bool            match_dot(pair *dot, elem *key)                   { return dot ? equals(head(dot), key) : false; }

struct op_add { template <typename T, typename U> auto operator()(T a, U b) { return a + b; } };
struct op_sub { template <typename T, typename U> auto operator()(T a, U b) { return a - b; } };
struct op_mul { template <typename T, typename U> auto operator()(T a, U b) { return a * b; } };
struct op_div { template <typename T, typename U> auto operator()(T a, U b) { return a / b; } };

template <typename T0, typename T1, typename Op>
elem *elem_op(elem *a, elem *b) { Op op; auto va = cast< box<T0> >(a); auto vb = cast< box<T1> >(b); return va && vb ? rebox(op(va->value, vb->value)) : nullptr; }

template <typename Op>
elem *math_op(elem *a, elem *b)
{
    if (!is_number(a) || !is_number(b)) {
        return nullptr;
    }
    int option = (is(a, elem::FLOAT) ? 2 : 0) + (is(b, elem::FLOAT) ? 1 : 0);
    switch (option) {
    case 0: return elem_op<int64_t, int64_t, Op>(a, b);
    case 1: return elem_op<int64_t, double,  Op>(a, b);
    case 2: return elem_op<double,  int64_t, Op>(a, b);
    case 3: return elem_op<double,  double,  Op>(a, b);
    default: break;
    }
    return nullptr;
}

size_t          linear(pair *p, elem **into, size_t max)          { size_t c = 0; for (; p && c < max; p = tail_pair(p), c++) *into++ = head(p); return c; }

// iterative preferred here
pair           *nth_pair(pair *list, size_t n)                    { for (size_t i = 0; i < n && list; ++i) { list = tail_pair(list); } return list; }
elem           *nth_elem(pair *list, size_t n)                    { return head(nth_pair(list, n)); }

bool            is_dotted(pair *p)                                { while (p) { if (is_atom(p->tail)) return true; p = tail_pair(p); } return false; }
bool            is_dotted(elem *e)                                { return is_dotted(cast<pair>(e)); }

size_t          length(pair *p)                                   { size_t c = 0; for (; p; c++) p = tail_pair(p); return c; }
size_t          length(elem *e)                                   { return length(cast<pair>(e)); }

pair           *list(elem **a, size_t n)                          { return n == 0 ? nullptr : _(*a, list(a + 1, n - 1)); }
pair           *list(std::span<elem *> a)                         { return list(a.data(), a.size()); }
pair           *list(elem *a, elem *b)                            { return _(a, _(b)); }
pair           *list(elem *a, elem *b, elem *c)                   { return _(a, list(b, c)); }
pair           *list(elem *a, elem *b, elem *c, elem *d)          { return _(a, list(b, c, d)); }
pair           *list(elem *a, elem *b, elem *c, elem *d, elem *e) { return _(a, list(b, c, d, e)); }

pair           *last_pair(pair *p)                                { if (is_nil(p)) return nullptr; pair *q = p; while (p) { q = p; p = tail_pair(p); } return q; }
pair           *copy_list(pair *p)                                { return is_nil(p) ? nullptr : _(p->head, copy_list(tail_pair(p->tail))); }

pair           *get_assoc_at(pair *p, elem *key)                  { while (p) { if (match_dot(head_pair(p), key)) return p; p = tail_pair(p); } return nullptr; }
elem           *get_assoc(pair *list, elem *key)                  { pair *at = get_assoc_at(list, key); return head(at); }

bool replace_assoc_destructive(pair *list, elem *key, elem *value)
{
    pair *at = get_assoc_at(list, key);
    if (at) {
        pair *dot = head_pair(at); // this is a dotted pair
        if (dot) {
            dot->tail = value; /* NOTE FOR GC/REFCOUNT */
            return true;
        }
    }
    return false;
}

pair *set_assoc_destructive(pair *list, elem *key, elem *value)
{
    if (!replace_assoc_destructive(list, key, value)) {
        return _(_(key, value), list);
    }
    return list;
}

pair *new_env(elem *expression)
{
    return list(expression, nullptr, nullptr);
}

pair *env_push_frame(pair *env, pair *frame)
{
    // (expr (... frame stack ...) (... globals assoc ...))
    pair *fs = tail_pair(env);
    pair *f = _(frame, fs->head);
    fs->head = f;
    return f;
}

elem *env_set_expr(pair *env, elem *expr)
{
    // (expr (... frame stack ...) (... globals assoc ...))
    env->head = expr;
    return expr;
}
    
elem *env_get_expr(pair *env)
{
    return head(env);
}

pair *env_get_frame_stack(pair *env)
{
    return head_pair(tail_pair(env));
}

elem *env_get_global(pair *env, elem *key)
{
    pair *ga = head_pair(tail_pair(tail_pair(env)));
    return get_assoc(ga, key);
}
    
pair *env_set_global(pair *env, elem *key, elem *value)
{
    // (expr (... frame stack ...) (... globals assoc ...))
    pair *a = tail_pair(env);
    pair *b = tail_pair(a);
    pair *p = set_assoc_destructive(head_pair(b), key, value);
    b->head = p;
    return p;
}

struct lexer
{
    static bool is_lisp_word_start_char(int ch)
    {
        return std::strchr("!$%&*+-./:<=>?^_~|\\@", ch) != nullptr || std::isalpha(ch) || ch == '_';
    }
    
    static bool is_lisp_word_char(int ch)
    {
        return is_lisp_word_start_char(ch) || std::isdigit(ch) || ch == '#';
    }

    static iter_t read_lisp_word(iter_t i, iter_t e)
    {
        if (!is_lisp_word_start_char(*i)) {
            return i;
        }
        i++;
        for (; i < e && is_lisp_word_char(*i); ++i) ;
        return i;
    }

    static iter_t read_space(iter_t i, iter_t e)
    {
        for (; i < e && std::isspace(*i); ++i) ;
        return i < e ? i : e;
    }

    static iter_t read_until_with_escaped(iter_t i, iter_t e, uint32_t until_char, uint32_t escape_char = '\\')
    {
        for (; i < e && *i != until_char; ++i) {
            if (*i == escape_char) {
                i++;
            }
        }
        return i < e ? i : e;
    }

    static iter_t read_quoted(iter_t i, iter_t e, int quote_char = '"', int escape_char = '\\')
    {
        if (i == e || *i != quote_char) {
            return i;
        }
        i = read_until_with_escaped(++i, e, quote_char, escape_char);
        if (i == e && *i != quote_char) {
            return i; // this is an error -- unterminated
        }
        return ++i;
    }
    
    static iter_t read_double(iter_t i, iter_t e, double *value)
    {
        iter_t b = i;
        char *end = 0;
        *value = std::strtod(b, &end);
        i = end;
        return i;
    }
};

struct reader
{
    std::string  _text;
    elem     *_element;
    iter_t       _at;

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
        i = quoted ? lexer::read_quoted(i, e) : lexer::read_lisp_word(i, e);
        return i == b ? nullptr : new ElemT(b, i);
    }

    elem *read_number(iter_t &i, iter_t e)
    {
        iter_t b = i;
        double v = 0;
        i = lexer::read_double(i, e, &v);
        return i == b ? nullptr : new number_t(v);
    }

    elem *read_tail(iter_t &i, iter_t e)
    {
        i = lexer::read_space(i, e);
        if (*i == ')') {
            ++i;
            return nullptr;
        }
        elem *p = new pair(),
                *h = read_expr(i, e),
                *t = nullptr;
        cast<pair>(p)->head = h;
        i = lexer::read_space(i, e);
        if (*i == '.' && std::isspace(*(i + 1))) {
            i = lexer::read_space(++i, e);
            t = read_expr(i, e);
        } else {
            t = read_tail(i, e);
        }
        cast<pair>(p)->tail = t;
        return p;
    }

    elem *read_pair(iter_t &i, iter_t e)
    {
        i = lexer::read_space(i, e);
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
        i = lexer::read_space(i, e);
        if (*i != '\'') {
            return 0;
        }
        i = lexer::read_space(++i, e);
        auto expr = read_expr(i, e);
        return new pair(new symbol(quote), new pair(expr));
    }

    elem *read_expr(iter_t &i, iter_t e)
    {
        i = lexer::read_space(i, e);
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

struct printer
{
    std::ostream &print(std::ostream &os, elem *e, bool has_tail = false) const
    {
        if (!e) {
            return os << "#nil";
        }
        if (e->is_atom()) {
            return e->print(os);
        }
        auto *p = cast<pair>(e);
        if (p) {
            os << (has_tail ? "" : "(");
            print(os, head(p), false);
            auto *t = tail(p);
            if (t) {
                print(os << (is_atom(t) ? " . " : " "), t, true);
            }
            return os << (has_tail ? "" : ")");
        }
        return os << "#nil";
    }
};

struct environment
{
    pair *expr;
    pair *frames;
    pair *globals;
};

std::array<function_call *, symbol_id::_MAX_SYMBOLS> construct_builtins()
{
    std::array<function_call *, symbol_id::_MAX_SYMBOLS> list = { nullptr };
    
    //elem *t = new symbol("t");
    list[symbol_id::T]        = nullptr;//new function<elem *,  0>([t]() { return (elem *)t; },       "t");
    list[symbol_id::NIL]      = new function<elem *,  0>([]()  { return (elem *)nullptr; }, "nil");
    list[symbol_id::ENDP]     =
    list[symbol_id::NILP]     = new function<bool,    1>(is_nil,    "nil?");
    list[symbol_id::LIST]     = nullptr; // placeholder
    list[symbol_id::LISTP]    = new function<bool,    1>(is_list,   "list?");
    list[symbol_id::DEF]      =
    list[symbol_id::DEFINE]   = nullptr; // placeholder
    list[symbol_id::FUNCTION] = nullptr; // placeholder
    list[symbol_id::SETF]     = nullptr; // placeholder
    list[symbol_id::SETP]     = nullptr; // placeholder
    list[symbol_id::LAMBDA]   = nullptr; // placeholder
    list[symbol_id::LET]      = nullptr; // placeholder
    list[symbol_id::LEN]      =
    list[symbol_id::LENGTH]   = new function<size_t,  1>(length,    "length");
    list[symbol_id::FIRST]    =
    list[symbol_id::HEAD]     = new function<elem *,  1>(head,      "head");
    list[symbol_id::REST]     =
    list[symbol_id::TAIL]     = new function<elem *,  1>(tail,      "tail");
    list[symbol_id::CONS]     =
    list[symbol_id::PAIR]     = new function<elem *,  2>(new_pair,  "pair");
    list[symbol_id::CONSP]    =
    list[symbol_id::PAIRP]    = new function<bool,    1>(is_list,   "pair?");
    list[symbol_id::MAP]      = nullptr; // placeholder
    list[symbol_id::APPLY]    = nullptr; // placeholder
    list[symbol_id::ATOMP]    = new function<bool,    1>(is_atom,   "atom?");
    list[symbol_id::NUMBERP]  = new function<bool,    1>(is_number, "number?");
    list[symbol_id::APPEND]   =
    list[symbol_id::SYMBOLP]  = new function<bool,    1>(is_symbol, "symbol?");
    list[symbol_id::ADD]      = new function<elem *,  2>(math_op<op_add>, "+");
    list[symbol_id::SUB]      = new function<elem *,  2>(math_op<op_sub>, "-");
    list[symbol_id::MUL]      = new function<elem *,  2>(math_op<op_mul>, "*");
    list[symbol_id::DIV]      = new function<elem *,  2>(math_op<op_div>, "/");
    return list;
}

struct evaluator
{
    std::array<function_call *, symbol_id::_MAX_SYMBOLS> _builtins;

    evaluator() : _builtins(construct_builtins()) {}

    elem *eval_define(pair *expr, pair *env)
    {
        symbol *sym = head_symbol(expr);
        if (sym->equals("def")) {
            expr = tail_pair(expr);
            symbol *name = head_symbol(expr);
            expr = tail_pair(expr);
            elem *body = head(expr);
            env_set_global(env, name, body);
        }
        return nullptr;
    }
    
    elem *eval_user_symbol(symbol *sym, pair *environ)
    {
        // todo
        return nullptr;
    }
    
    elem *eval_symbol(symbol *sym, pair *environ)
    {
        int id = sym->id;
        switch (id) {
        case symbol_id::NIL:     return nullptr;
        case symbol_id::LIST:
        case symbol_id::QUOTE:   return sym; // eval to itself
        case symbol_id::UNKNOWN: return eval_user_symbol(sym, environ);
        default:                 break;
        }
        return _builtins[id];
    }
    
    elem *eval_nonnil_list(pair *expr, pair *environ)
    {
        std::array<elem *, 64> args { nullptr };
        printer pr;

        elem *first = expr->head; // will need to evaluate at the end - which gives us the possibility of multiple dispatch
        if (!first) { // a nil head is an error, so we don't even have to check tail
            return nullptr;
        }
        pair *rest = cast<pair>(expr->tail);

        symbol *sym = cast<symbol>(first);
        // special forms - quote
        if (sym && sym->id == symbol_id::QUOTE) {
            return rest->head;
        }

        size_t n = linear(rest, args.data(), args.size());
        for (size_t i = 0; i < n; ++i) {
            args[i] = eval(args[i], environ); // this is recursive right now just to get it working
            pr.print(std::cout, args[i]) << std::endl;
        }

        // special forms - list
        if (sym && sym->id == symbol_id::LIST) {
            return list(args.data(), n);
        }
        
        auto result = eval(first, environ);
        if (result && result->is(elem::FUNCTION)) {
            auto fn = cast<function_call>(result);
            if (fn) {
                return fn->call(args.data());
            }
        }
        // error here
        return nullptr;
    }
    
    elem *eval(elem *expr, pair *environ)
    {
        if (!expr) {
            return nullptr;
        }
        switch (expr->_type) {
        case elem::NIL:    return nullptr;
        case elem::SYMBOL: return eval_symbol(static_cast<symbol *>(expr), environ);
        case elem::PAIR:   return eval_nonnil_list(static_cast<pair *>(expr), environ);
        default:           break;
        }
        return expr;
    }
        
    elem *evaluate(elem *expr, pair *environ = nullptr)
    {
        pair *expression = cast<pair>(expr);
        if (!environ) {
            environ = new_env(expression);
        }
        pair *frame = nullptr, *expr_ptr = expression;
        std::vector<pair *> stack; // just for now...
        stack.push_back(expression);
        while (expr_ptr) {
            eval_define(expr_ptr, environ);
            pair *p = head_pair(expr_ptr), *t = tail_pair(expr_ptr);
            symbol *sym = head_symbol(expr_ptr);
            expr_ptr = tail_pair(p);
        }
        printer pr;
        pr.print(std::cout << std::endl, environ);
        return nullptr;
    }
};

}
