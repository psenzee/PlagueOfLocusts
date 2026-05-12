#pragma once

#include <string_view>
#include <iostream>
#include <iomanip>
#include <array>
#include <string>
#include "lex_numeric.h"
#include "tag.h"

struct elem_nil : public tag
{
    enum { TYPE = NIL };
    elem_nil() : tag(NIL) {}
    std::ostream &print(std::ostream &os) const { return os << "()"; }
};

template <typename HandleT, size_t Slots>
struct elem_frame : public tag
{
    enum { TYPE = FRAME };
    elem_frame() : tag(FRAME), slots({ 0 }), parent(0) {}
    uint64_t parent;
    std::array<HandleT, Slots> slots;
    std::ostream &print(std::ostream &os) const { return os << "{frame}"; }
};

template <typename ValueT, types::Type TypeT = value_type<ValueT>()>
struct elem_value : public tag
{
    enum { TYPE = value_type<ValueT>() };
    ValueT value;
    elem_value(const ValueT &v) : tag(TypeT), value(v) {}
    std::ostream &print(std::ostream &os) const { return os << value; }
};

typedef std::string_view::const_iterator iter_t;

template <types::Type TypeT>
struct elem_str : public tag
{
    enum { TYPE = TypeT };
    iter_t b, e;
    elem_str(iter_t b, iter_t e) : tag(TypeT), b(b), e(e) {}
    std::ostream &print(std::ostream &os) const { return os << std::string_view(b, e); }
    std::string_view view() const { return std::string_view(b, e); }
};

struct elem_token : public elem_str<types::TOKEN>
{
    elem_token(iter_t b, iter_t e) : elem_str(b, e) {}
    std::ostream &print(std::ostream &os) const { return os << "?" << std::string_view(b, e); }
};

template <types::Type TypeT>
struct elem_range : public tag
{
    enum { TYPE = TypeT };
    uint64_t b, e;
    elem_range(uint64_t b, uint64_t e) : tag(TypeT), b(b), e(e) {}
    std::ostream &print(std::ostream &os) const { return os << "[range " << b << "-" << e << "]"; }
};

struct elem_pair : public tag
{
    enum { TYPE = types::PAIR };
    uint64_t head, tail;
    elem_pair(uint64_t head, uint64_t tail) : tag(types::PAIR), head(head), tail(tail) {}
};

template <types::Type TypeT>
struct elem_ref : public tag
{
    enum { TYPE = TypeT };
    uint64_t ref;
    elem_ref(uint64_t ref) : tag(TypeT), ref(ref) {}
};

typedef elem_str<types::STRING>           elem_string_t;
typedef elem_str<types::SYMBOL>           elem_symbol_t;
typedef elem_value<int32_t>               elem_int32_t;
typedef elem_value<int32_t>               elem_int32_t;
typedef elem_value<int64_t>               elem_int64_t;
typedef elem_value<float>                 elem_float_t;
typedef elem_value<double>                elem_double_t;
typedef elem_value<types::Type>           elem_type_t;
typedef elem_range<types::ARRAY>          elem_array_t;
typedef elem_ref<types::QUOTE>            elem_quote_t;

struct unpacked_handle
{
    types::Type type;
    uint8_t     category;
    uint16_t    info;
    uint32_t    handle;
    tag        *ptr;
    unpacked_handle() : ptr(nullptr) {}
    unpacked_handle(tag *p) : ptr(p) {}
    std::ostream &print(std::ostream &os) { return os << ":" << std::hex << ptr; }
};

template <typename T> struct pool
{
    enum { TYPE = T::TYPE };
    std::vector<T> list;
    
    uint64_t allocate(const T &e)
    {
        uint64_t handle = uint64_t(TYPE) << 56;
        handle |= list.size();
        list.emplace_back(e);
        return handle;
    }
};

namespace {
typedef std::tuple<
    std::vector< elem_nil >,                 // NIL
    std::vector< elem_str<types::STRING> >,  // STRING
    std::vector< elem_str<types::SYMBOL> >,  // SYMBOL
    std::vector< elem_token >,               // TOKEN
    std::vector< elem_value<int32_t> >,      // INT32
    std::vector< elem_value<int64_t> >,      // INT64
    std::vector< elem_value<float> >,        // FLOAT
    std::vector< elem_value<double> >,       // DOUBLE
    std::vector< elem_value<types::Type> >,  // TYPE
    std::vector< elem_pair >,                // PAIR
    std::vector< elem_range<types::ARRAY> >, // ARRAY
    std::vector< elem_ref<types::QUOTE> >,   // QUOTE
    std::vector< elem_frame<uint64_t, 64> >  // FRAME(64)
> pools_t;

template <size_t N> tag       *ls(pools_t &pools, size_t n)       { return &(std::get<N>(pools)[n]); }
template <size_t N> const tag *ls(const pools_t &pools, size_t n) { return &(std::get<N>(pools)[n]); }
typedef tag * (*_list_fn)(pools_t &pools, size_t n);
typedef const tag * (*_const_list_fn)(const pools_t &pools, size_t n);
}

class element_allocator
{
    pools_t _pools;

    template <size_t N> auto       &_pool()       { return std::get<N>(_pools); }
    template <size_t N> const auto &_pool() const { return std::get<N>(_pools); }

public:

    typedef uint64_t handle_t;
    
    element_allocator() { allocate(elem_nil()); allocate(elem_nil()); }

    template <typename T> uint64_t allocate(const T &e)
    {
        uint64_t handle = uint64_t(T::TYPE) << 56;
        auto &list = _pool<T::TYPE>();
        handle |= list.size();
        list.emplace_back(e);
        return handle;
    }
    
    template <typename T> uint64_t new_literal(iter_t i, iter_t e)             { return allocate(T(i, e)); }
    template <typename T> uint64_t new_value(const T &value)                   { return allocate(elem_value<T>(value)); }

    uint64_t                       new_nil()                                   { return 1; }
    uint64_t                       new_pair(uint64_t head, uint64_t tail)      { return allocate(elem_pair(head, tail)); }
    uint64_t                       new_quote(uint64_t ref)                     { return allocate(elem_quote_t(ref)); }
    uint64_t                       new_number(const core_number &n)            { return n.is_int() ? new_value<int64_t>(n.int_value()) : new_value<double>(n.double_value()); }

    template <typename T> T       *cast(uint64_t handle)                       { return ::cast<T>(get(handle)); }

    tag *get(uint64_t i)
    {
        static constexpr std::array<_list_fn, std::tuple_size<pools_t>::value > fn {
            ls<0>, ls<1>, ls<2>, ls<3>, ls<4>, ls<5>, ls<6>, ls<7>, ls<8>, ls<9>, ls<10>, ls<11>
        };
        return (fn[i >> 56])(_pools, i & 0x00ffffffffffffffull);
    }

    const tag *get(uint64_t i) const
    {
        static constexpr std::array<_const_list_fn, std::tuple_size<pools_t>::value > fn {
            ls<0>, ls<1>, ls<2>, ls<3>, ls<4>, ls<5>, ls<6>, ls<7>, ls<8>, ls<9>, ls<10>, ls<11>
        };
        return (fn[i >> 56])(_pools, i & 0x00ffffffffffffffull);
    }
};
