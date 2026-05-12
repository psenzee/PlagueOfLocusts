#pragma once

#include <array>
#include <vector>
#include <algorithm>

#include "types.h"

// atomic flag -- always on for actual atoms, defaults on for pair(s) but turns off once another....
// no too complicated because what happens when a pair several deep loses its atomic flag?
// still.. a pair with two atomic children is atomic

namespace symbols {

enum Type { TYPE_NONE = 0, TYPE_NIL = 1, TYPE_PAIR = 3, TYPE_ATOM = 4, TYPE__MASK = 15 };

struct term
{
    // ref counting mechanism?
    typedef const types::type_id & data_type_t;
    static constexpr Type term_type = TYPE_NONE;
    
    virtual ~term() {}

    virtual Type          type()                                const { return term_type; }
    virtual data_type_t   data_type()                           const { return types::type_none(); }
    virtual term         *copy()                                const { return nullptr; }
    virtual void          destroy()                                   { delete this; }
    virtual std::ostream &print(std::ostream &os)               const { return os; }
};

std::ostream &print(std::ostream &os, const term *e);

struct pairable : public term
{
    static constexpr Type term_type = TYPE_PAIR;
    
    Type type() const { return term_type; }
    
    virtual       term    *at_l(size_t i)                 = 0;
    virtual const term    *at_l(size_t i)           const = 0;
    virtual       term    *at_r(size_t i)                 = 0;
    virtual const term    *at_r(size_t i)           const = 0;
    virtual       void     set_l(size_t i, term *t)       = 0;
    virtual       void     set_r(size_t i, term *t)       = 0;
    virtual       size_t   size()                   const = 0;
};

inline                       bool     is_nil(const term *t)                  { return !t; }
template <typename T> inline bool     is(const term *t)                      { return t && t->type()  == T::term_type; }
template <typename T> inline bool     is_or_nil(const term *t)               { return !t || t->type() == T::term_type; }
template <typename T> inline T       *unsafe_cast(term *t)                   { return dynamic_cast<T *>(t); }
template <typename T> inline const T *unsafe_cast(const term *t)             { return dynamic_cast<const T *>(t); }
template <typename T> inline T       *safe_cast(term *t)                     { return is<T>(t) ? unsafe_cast<T>(t) : nullptr; }
template <typename T> inline const T *safe_cast(const term *t)               { return is<T>(t) ? unsafe_cast<T>(t) : nullptr; }

inline static void                    destroy(term *t)                       { if (t) t->destroy(); }
inline static term                   *copy(const term *t)                    { return t ? t->copy() : nullptr; }
inline static Type                    type(const term *t)                    { return !t ? TYPE_NIL : t->type(); }

inline                      term     *at_l(pairable *p, size_t i = 0)        { return p ? p->at_l(i) : nullptr; }
inline                      term     *at_l(term *t, size_t i = 0)            { return safe_cast<pairable>(t); }
inline                const term     *at_l(const pairable *p, size_t i = 0)  { return p ? p->at_l(i) : nullptr; }
inline                const term     *at_l(const term *t, size_t i = 0)      { return safe_cast<const pairable>(t); }
inline                      term     *at_r(pairable *p, size_t i = 0)        { return p ? p->at_r(i) : nullptr; }
inline                      term     *at_r(term *t, size_t i = 0)            { return safe_cast<pairable>(t); }
inline                const term     *at_r(const pairable *p, size_t i = 0)  { return p ? p->at_r(i) : nullptr; }
inline                const term     *at_r(const term *t, size_t i = 0)      { return safe_cast<const pairable>(t); }

inline bool                           is_proper_list(const pairable *p)      { while (p && is<pairable>(p->at_r(0))) p = safe_cast<pairable>(at_r(p)); return !p; }
inline bool                           is_proper_list(const term *t)          { return is_proper_list(safe_cast<pairable>(t)); }
inline bool                           is_assoc(const pairable *p)            { return p && !is_proper_list(p); }
inline bool                           is_assoc(const term *t)                { return is_assoc(safe_cast<pairable>(t)); }
inline size_t                         length(const pairable *p)              { if (!p) return 0; size_t inc(p->size() - 1), sz(inc); for (; p; sz += inc) p = safe_cast<pairable>(at_r(p)); return sz; }
inline size_t                         length(const term *t)                  { return length(safe_cast<pairable>(t)); }

template <size_t N> inline const std::array<term *, N> &copy(const std::array<term *, N> &a) { std::array<term *, N> c(a); for (auto &t : c) t = copy(t); return c; }
template <size_t N> inline void                         destroy_array(std::array<term *, N> &a)    { for (auto &t : a) { destroy(t); t = nullptr; } }

template <size_t N>
struct tuple : public pairable
{
    typedef tuple<N>              self_t;
    typedef std::array<term *, N> terms_t;
    
    static constexpr size_t COUNT = N, LAST = N - 1;

    terms_t _terms;
    
    inline tuple(const terms_t &terms) : _terms(terms) {}
    inline tuple() : _terms({ nullptr }) {}
    
    inline Type           type()                                       const { return term_type; }
    term                 *copy()                                             { return new self_t(copy(_terms)); }
    void                  destroy()                                          { destroy_array(_terms); delete this; }
    inline        term   *at_l(size_t i)                                     { return _terms[i]; }
    inline const  term   *at_l(size_t i)                               const { return _terms[i]; }
    inline        term   *at_r(size_t i)                                     { return _terms[LAST - i]; }
    inline const  term   *at_r(size_t i)                               const { return _terms[LAST - i]; }
    inline void           set_l(size_t i, term *t)                           { _terms[0] = t; }
    inline void           set_r(size_t i, term *t)                           { _terms[LAST] = t; }
    inline        size_t  size()                                       const { return COUNT; }

    inline static self_t *create(terms_t &terms)                             { return new self_t(terms); }
    inline static self_t *create(term *at_l, term *at_r = nullptr)           { terms_t t = { at_l, nullptr }; t[LAST] = at_r; return new self_t(t); }
    inline static self_t *create()                                           { return new self_t(); }
};

typedef tuple<2> pair;

struct atom : public term
{
    static constexpr Type term_type = TYPE_ATOM;
    
    Type type()               const { return term_type; }
};

template <typename T>
struct box : public atom
{
    // ref count?
    typedef box<T> self_t;

    T _value;

    inline box(const T &value = T(0)) : _value(value) {}
    
    inline data_type_t          data_type()                    const { return types::type<T>(); }
    inline std::ostream        &print(std::ostream &os)        const { return os << std::setprecision(2) << _value; }
    inline self_t              *copy()                         const { return create(_value); }

    inline static       self_t *create(const T &value)               { return new self_t(value); }
    inline static       self_t *create()                             { return new self_t; }
};

template <typename T>
pairable *copy_list(const T *at, const T *end)
{
    typedef box<T> box_t;
    pairable *first = nullptr, *p = nullptr;
    for (; at != end; ++at) {
        pairable *elem = pair::create(box_t::create(*at));
        if (!first) { first = elem; p = first; continue; }
        p->set_r(0, elem);
        p = elem;
    }
    return first;
}

template <typename T, size_t N> pairable *copy_list(const std::array<T, N> &u) { return copy_list(u.data(), u.data() + u.size()); }
template <typename T>           pairable *copy_list(const std::vector<T>   &u) { return copy_list(u.data(), u.data() + u.size()); }

std::ostream &print(std::ostream &os, const term *e)
{
    if (is_nil(e)) return os << "nil";
    const pairable *pe = safe_cast<pairable>(e);
    const term *et = nullptr;
    if (!pe) return e->print(os);
    os << "(";
    do {
        if (et) os << " ";
        print(os, at_l(pe));
        et = at_r(pe);
        if (!et) break;
        const pairable *pet = safe_cast<pairable>(et);
        if (pet) pe = pet;
        else if (et) {
            // dotted pair
            et->print(os << " . ");
            break;
        }
    } while (pe);
    os << ")";
    return os;
}

}
