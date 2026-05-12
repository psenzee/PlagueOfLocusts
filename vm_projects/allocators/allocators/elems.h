#pragma once

#include <string_view>
#include <iostream>
#include <sstream>
#include <array>
#include <string>
#include <cctype>
#include <algorithm>
#include <string_view>

struct elem
{
    enum Type : uint8_t { NIL, STRING, SYMBOL, INT, FLOAT, PAIR, LAMBDA, FUNCTION, _MAX_TYPE };
    enum Meta : uint8_t { CONST = 1 };
    
    Type    _type;
    uint8_t _meta;
    
    bool                    is(Type type)            const { return _type == type; }
    bool                    is_atom()                const { return _type < PAIR; }
    bool                    is_number()              const { return _type == INT || _type == FLOAT; }
    virtual std::ostream   &print(std::ostream &os)  const { return os << "[elem]"; }
    virtual bool            equals(const elem *e)    const { return e->_type == _type; }

    elem(Type type = NIL, uint8_t meta = 0) : _type(type), _meta(meta) {}
};

template <typename T> constexpr elem::Type element_type()              { return elem::NIL; }
template <>           constexpr elem::Type element_type<int64_t>()     { return elem::INT; }
template <>           constexpr elem::Type element_type<double>()      { return elem::FLOAT; }
template <>           constexpr elem::Type element_type<int32_t>()     { return elem::INT; }
template <>           constexpr elem::Type element_type<uint32_t>()    { return elem::INT; }
template <>           constexpr elem::Type element_type<float>()       { return elem::FLOAT; }

template <typename T> T       *cast(elem *t)       { return t && t->is(elem::Type(T::TYPE)) ? static_cast<T *>(t) : nullptr; }
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

inline bool elem_value(const elem *e, int64_t *value)
{
    auto ev = cast< box<int64_t> >(e);
    if (ev) {
        *value = ev->value;
    }
    return ev != nullptr;
}

inline bool elem_value(const elem *e, float *value)
{
    auto fe = cast< box<float> >(e);
    if (fe) {
        *value = fe->value;
        return true;
    }
    auto de = cast< box<double> >(e);
    if (de) {
        *value = de->value;
        return true;
    }
    return false;
}
