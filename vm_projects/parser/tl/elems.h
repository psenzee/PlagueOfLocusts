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
#include "symbols.h"

namespace tl
{
typedef std::string_view::const_iterator iter_t;

struct elem
{
    static constexpr const char *_names[] = { "NIL", "STRING", "SYMBOL", "INT", "FLOAT", "PAIR", "LAMBDA", "FUNCTION" };
    
    enum Type : uint8_t { NIL, STRING, SYMBOL, INT, FLOAT, PAIR, LAMBDA, FUNCTION, _MAX_TYPE };
    enum Meta : uint8_t { CONST = 1 };
    
    Type    _type;
    uint8_t _meta;
    
    const char             *type_name()              const { return _names[_type]; }
    bool                    is(Type type)            const { return _type == type; }
    bool                    is_atom()                const { return _type < PAIR; }
    bool                    is_number()              const { return _type == INT || _type == FLOAT; }
    virtual std::ostream   &print(std::ostream &os)  const { return os << type_name(); }
    virtual bool            equals(const elem *e) const { return e->_type == _type; }
    
    elem(Type type = NIL, uint8_t meta = 0) : _type(type), _meta(meta) {}
};

template <typename T> constexpr elem::Type element_type()              { return elem::NIL; }
template <>           constexpr elem::Type element_type<int64_t>()     { return elem::INT; }
template <>           constexpr elem::Type element_type<double>()      { return elem::FLOAT; }
template <>           constexpr elem::Type element_type<int32_t>()     { return elem::INT; }
template <>           constexpr elem::Type element_type<uint32_t>()    { return elem::INT; }
template <>           constexpr elem::Type element_type<float>()       { return elem::FLOAT; }

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
    void _resolve() { id = symbols::resolve(value); }
};

}
