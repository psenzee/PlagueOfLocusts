#pragma once

#include <array>
#include <vector>
#include <algorithm>

#include "types.h"

// atomic flag -- always on for actual atoms, defaults on for pair(s) but turns off once another....
// no too complicated because what happens when a pair several deep loses its atomic flag?
// still.. a pair with two atomic children is atomic

namespace symbols {

// TODO -- reintroduce new/delete factories later for memory management.  Pulling them out now, want to keep it simple here

enum Type { TYPE_NONE = 0, TYPE_NIL = 1, TYPE_PAIR = 3, TYPE_ATOM = 4, TYPE__MASK = 15 };

struct term
{
    // ref counting mechanism?
    typedef const types::type_id & data_type_t;

    enum Depth     { DEPTH_NONE = 0, DEPTH_SHALLOW, DEPTH_STRUCTURE, DEPTH_DEEP };
    
    static constexpr Type term_type = TYPE_NONE;
    
    virtual ~term() {}

    virtual Type          type()                                const { return term_type; }
    virtual data_type_t   data_type()                           const { return types::type_none(); }
    virtual term         *copy(Depth depth)                           { return nullptr; }
    virtual void          destroy(Depth depth)                        {}
    
    virtual std::ostream &print(std::ostream &os)               const { return os; }
    
    inline static void    destroy(term *t, Depth depth)               { if (t) t->destroy(depth); }
    inline static term   *copy(const term *t, Depth depth)            { return t ? t->copy(t, depth) : nullptr; }
    inline static Type    type(const term *t)                         { return !t ? TYPE_NIL : t->type(); }
};

struct pairable : public term
{
    static constexpr Type term_type = TYPE_PAIR;
    
    Type type() const { return term_type; }
    
    virtual       term    *at(size_t i)       = 0;
    virtual const term    *at(size_t i) const = 0;
    virtual       term    *head()             = 0;
    virtual const term    *head()       const = 0;
    virtual       term    *tail()             = 0;
    virtual const term    *tail()       const = 0;
    virtual       size_t   size()       const = 0;
};

inline                       bool     is_nil (const term *t)          { return !t; }
template <typename T> inline bool     is(const term *t)               { return t && t->type()  == T::term_type; }
template <typename T> inline bool     is_or_nil(const term *t)        { return !t || t->type() == T::term_type; }
template <typename T> inline T       *unsafe_cast(term *t)            { return dynamic_cast<T *>(t); }
template <typename T> inline const T *unsafe_cast(const term *t)      { return dynamic_cast<const T *>(t); }
template <typename T> inline T       *safe_cast(term *t)              { return is<T>(t) ? unsafe_cast<T>(t) : nullptr; }
template <typename T> inline const T *safe_cast(const term *t)        { return is<T>(t) ? unsafe_cast<T>(t) : nullptr; }

inline static void                    destroy(term *t, term::Depth depth)               { if (t) t->destroy(depth); }
inline static term                   *copy(const term *t, term::Depth depth)            { return t ? t->copy(t, depth) : nullptr; }
inline static Type                    type(const term *t)                         { return !t ? TYPE_NIL : t->type(); }

inline                      term     *head(pairable *p)                                      { return p ? p->head() : nullptr; }
inline                      term     *head(term *t)                                      { return head(safe_cast<pairable>(t)); }
inline                const term     *head(const pairable *p)                                { return p ? p->head() : nullptr; }
inline                const term     *head(const term *t)                                { return head(safe_cast<pairable>(t)); }
inline                      term     *tail(pairable *p)                                      { return p ? p->tail() : nullptr; }
inline                      term     *tail(term *t)                                      { return tail(safe_cast<pairable>(t)); }
inline                const term     *tail(const pairable *p)                                { return p ? p->tail() : nullptr; }
inline                const term     *tail(const term *t)                                { return tail(safe_cast<pairable>(t)); }

inline                      pairable *tail_pairable(pairable *p)                              { return safe_cast<pairable>(tail(p)); }
inline                      pairable *tail_pairable(term *t)                              { return safe_cast<pairable>(tail(t)); }
inline                const pairable *tail_pairable(const pairable *p)                        { return safe_cast<pairable>(tail(p)); }
inline                const pairable *tail_pairable(const term *t)                        { return safe_cast<pairable>(tail(t)); }

inline                      pairable *last_pairable(pairable *p)                            { pairable *e(p);       while (p) p = safe_cast<pairable>(tail(e = p)); return e; }
inline                const pairable *last_pairable(const pairable *p)                      { const pairable *e(p); while (p) p = safe_cast<pairable>(tail(e = p)); return e; }
inline                      pairable *last_pairable(term *t)                              { return last_pairable(safe_cast<pairable>(t)); }
inline                const pairable *last_pairable(const term *t)                        { return last_pairable(safe_cast<pairable>(t)); }

inline bool                           is_proper_list(const pairable *p)                     { while (p && is<pairable>(p->tail())) p = safe_cast<pairable>(tail(p)); return !p; }
inline bool                           is_proper_list(const term *t)                       { return is_proper_list(safe_cast<pairable>(t)); }
inline bool                           is_assoc(const pairable *p)                           { return p && !is_proper_list(p); }
inline bool                           is_assoc(const term *t)                             { return is_assoc(safe_cast<pairable>(t)); }
inline size_t                         length(const pairable *p)                             { if (!p) return 0; size_t inc(p->size() - 1), sz(inc); for (; p; sz += inc) p = safe_cast<pairable>(tail(p)); return sz; }
inline size_t                         length(const term *t)                               { return length(safe_cast<pairable>(t)); }

std::ostream &print(std::ostream &os, const term *e);
/*
struct pair : public pairable
{
    // ref counted & only delete if there are no container children? ie, "atomic" -- atomic is nil or an atom -- or a pair with two 'atomic' children - could have an 'atomic flag' so as not to recompute all the time
    typedef std::pair<term *, term *> terms_t;
    
    static constexpr Type term_type = TYPE_PAIR;
    
    term *_head, *_tail;
    
    inline pair(term *head = nullptr, term *tail = nullptr) : _head(head), _tail(tail) {}
    inline pair(terms_t terms) : _head(terms.first), _tail(terms.second) {}
    
    inline Type               type()                                       const { return term_type; }
    term                     *copy(Depth depth)                                  { return depth == DEPTH_NONE ? this : create(_copy_pair(depth == DEPTH_SHALLOW ? DEPTH_NONE : depth)); }
    void                      destroy(Depth depth)                               { _destroy_pair(depth == DEPTH_SHALLOW ? DEPTH_NONE : depth); _head = _tail = nullptr; delete this; }
    inline static       pair *create(term *head = nullptr, term *tail = nullptr) { return new pair(head, tail); }
    inline static       pair *create(terms_t terms = terms_t(nullptr, nullptr))  { return new pair(terms); }
    
    inline static       term *head(pair *p)                                      { return p ? p->_head : nullptr; }
    inline static       term *head(term *t)                                      { return head(safe_cast<pair>(t)); }
    inline static const term *head(const pair *p)                                { return p ? p->_head : nullptr; }
    inline static const term *head(const term *t)                                { return head(safe_cast<pair>(t)); }
    inline static       term *tail(pair *p)                                      { return p ? p->_tail : nullptr; }
    inline static       term *tail(term *t)                                      { return tail(safe_cast<pair>(t)); }
    inline static const term *tail(const pair *p)                                { return p ? p->_tail : nullptr; }
    inline static const term *tail(const term *t)                                { return tail(safe_cast<pair>(t)); }
    inline static       pair *tail_pair(pair *p)                                 { return safe_cast<pair>(tail(p)); }
    inline static       pair *tail_pair(term *t)                                 { return safe_cast<pair>(tail(t)); }
    inline static const pair *tail_pair(const pair *p)                           { return safe_cast<pair>(tail(p)); }
    inline static const pair *tail_pair(const term *t)                           { return safe_cast<pair>(tail(t)); }
    
    inline static       bool  is_all_nil(const pair *p)                          { return !p || (is_all_nil(head(p)) && is_all_nil(tail(p))); }
    inline static       bool  is_all_nil(const term *t)                          { if (!t) return true; const pair *p(safe_cast<pair>(t)); return p && is_all_nil(p); }
    
    inline static       pair *last_pair(pair *p)                                 { pair *e(p);       while (p) p = tail_pair(e = p); return e; }
    inline static const pair *last_pair(const pair *p)                           { const pair *e(p); while (p) p = tail_pair(e = p); return e; }
    inline static       pair *last_pair(term *t)                                 { return last_pair(safe_cast<pair>(t)); }
    inline static const pair *last_pair(const term *t)                           { return last_pair(safe_cast<pair>(t)); }
    inline static bool        is_proper_list(const pair *p)                      { while (p && is<pair>(p->_tail)) p = tail_pair(p); return !p; }
    inline static bool        is_proper_list(const term *t)                      { return is_proper_list(safe_cast<pair>(t)); }
    inline static bool        is_assoc(const pair *p)                            { return p && !is_proper_list(p); }
    inline static bool        is_assoc(const term *t)                            { return is_assoc(safe_cast<pair>(t)); }
    inline static size_t      length(const pair *p)                              { if (!p) return 0; size_t sz = 1; for (; p; ++sz) p = tail_pair(p); return sz; }
    inline static size_t      length(const term *t)                              { return length(safe_cast<pair>(t)); }
    
private:

    inline terms_t _copy_pair(Depth depth)    { return (depth == DEPTH_NONE) ? terms_t(_head, _tail) : terms_t(term::copy(_head, depth), term::copy(_tail, depth)); }
    inline void    _destroy_pair(Depth depth) { if (depth != DEPTH_NONE) { term::destroy(_head, depth); term::destroy(_tail, depth); } }
};

struct pairable
{
    typedef pairable self_t;
    
    virtual ~pairable() {}
    
    virtual             term    *at(size_t i)                                        = 0;
    virtual       const term    *at(size_t i)                                  const = 0;
    virtual             term    *head()                                              = 0;
    virtual       const term    *head()                                        const = 0;
    virtual             term    *tail()                                              = 0;
    virtual       const term    *tail()                                        const = 0;
    virtual             size_t   size()                                        const = 0;

    inline static       term    *head(self_t *p)                                     { return p ? p->head() : nullptr; }
    inline static       term    *head(term *t)                                       { return head(safe_cast<self_t>(t)); }
    inline static const term    *head(const self_t *p)                               { return p ? p->head() : nullptr; }
    inline static const term    *head(const term *t)                                 { return head(safe_cast<self_t>(t)); }
    inline static       term    *tail(self_t *p)                                     { return p ? p->tail() : nullptr; }
    inline static       term    *tail(term *t)                                       { return tail(safe_cast<self_t>(t)); }
    inline static const term    *tail(const self_t *p)                               { return p ? p->tail() : nullptr; }
    inline static const term    *tail(const term *t)                                 { return tail(safe_cast<self_t>(t)); }

    inline static       self_t  *tail_pairable(pair *p)                              { return safe_cast<self_t>(tail(p)); }
    inline static       self_t  *tail_pairable(term *t)                              { return safe_cast<self_t>(tail(t)); }
    inline static const self_t  *tail_pairable(const pair *p)                        { return safe_cast<self_t>(tail(p)); }
    inline static const self_t  *tail_pairable(const term *t)                        { return safe_cast<self_t>(tail(t)); }
    inline static       self_t  *last_pairable(self_t *p)                            { self_t *e(p);       while (p) p = safe_cast<self_t>(tail(e = p)); return e; }
    inline static const self_t  *last_pairable(const self_t *p)                      { const self_t *e(p); while (p) p = safe_cast<self_t>(tail(e = p)); return e; }
    inline static       self_t  *last_pairable(term *t)                              { return last_pairable(safe_cast<self_t>(t)); }
    inline static const self_t  *last_pairable(const term *t)                        { return last_pairable(safe_cast<self_t>(t)); }
    inline static bool           is_proper_list(const self_t *p)                     { while (p && is<self_t>(p->tail())) p = safe_cast<self_t>(tail(p)); return !p; }
    inline static bool           is_proper_list(const term *t)                       { return is_proper_list(safe_cast<self_t>(t)); }
    inline static bool           is_assoc(const self_t *p)                           { return p && !is_proper_list(p); }
    inline static bool           is_assoc(const term *t)                             { return is_assoc(safe_cast<self_t>(t)); }
    inline static size_t         length(const self_t *p)                             { if (!p) return 0; size_t inc(p->size() - 1), sz(inc); for (; p; sz += inc) p = safe_cast<self_t>(tail(p)); return sz; }
    inline static size_t         length(const term *t)                               { return length(safe_cast<self_t>(t)); }
};
*/
template <size_t N>
struct tuple : public term
{
    typedef tuple<N>              self_t;
    typedef std::array<term *, N> terms_t;
    
    static constexpr size_t COUNT = N, LAST = N - 1;
    
    static constexpr Type term_type = TYPE_TUPLE;
    
    terms_t _terms;
    
    inline tuple(const terms_t &terms) : _terms(terms) {}
    inline tuple() : _terms({ nullptr }) {}
    
    inline Type                 type()                                       const { return term_type; }
    term                       *copy(Depth depth)                                  { return depth == DEPTH_NONE ? this : create(_copy_tuple(depth == DEPTH_SHALLOW ? DEPTH_NONE : depth)); }
    void                        destroy(Depth depth)                               { _destroy_tuple(depth == DEPTH_SHALLOW ? DEPTH_NONE : depth); _head = _tail = nullptr; delete this; }
    inline              term   *at(size_t i)                                       { return _terms[i]; }
    inline       const  term   *at(size_t i)                                 const { return _terms[i]; }
    inline              term   *head()                                             { return _terms[0]; }
    inline       const  term   *head()                                       const { return _terms[0]; }
    inline              term   *tail()                                             { return _terms[LAST]; }
    inline       const  term   *tail()                                       const { return _terms[LAST]; }
    inline              size_t  size()                                       const { return COUNT; }
    
    inline static       self_t *create(const terms_t &terms)                       { return new tuple(terms); }
    inline static       self_t *create()                                           { return new tuple(); }
/*
    inline static       term   *head(self_t *p)                                      { return p ? p->head() : nullptr; }
    inline static       term   *head(term *t)                                      { return head(safe_cast<tuple>(t)); }
    inline static const term   *head(const self_t *p)                                { return p ? p->head() : nullptr; }
    inline static const term   *head(const term *t)                                { return head(safe_cast<tuple>(t)); }
    inline static       term   *tail(self_t *p)                                      { return p ? p->tail() : nullptr; }
    inline static       term   *tail(term *t)                                      { return tail(safe_cast<tuple>(t)); }
    inline static const term   *tail(const self_t *p)                                { return p ? p->tail() : nullptr; }
    inline static const term   *tail(const term *t)                                { return tail(safe_cast<tuple>(t)); }
    
    inline static       bool   is_all_nil(const pair *p)                          { return !p || (is_all_nil(head(p)) && is_all_nil(tail(p))); }
    inline static       bool   is_all_nil(const term *t)                          { if (!t) return true; const pair *p(safe_cast<pair>(t)); return p && is_all_nil(p); }
    
    inline static       pair *last_pair(pair *p)                                 { pair *e(p);       while (p) p = tail_pair(e = p); return e; }
    inline static const pair *last_pair(const pair *p)                           { const pair *e(p); while (p) p = tail_pair(e = p); return e; }
    inline static       pair *last_pair(term *t)                                 { return last_pair(safe_cast<pair>(t)); }
    inline static const pair *last_pair(const term *t)                           { return last_pair(safe_cast<pair>(t)); }
    inline static bool        is_proper_list(const pair *p)                      { while (p && is<pair>(p->_tail)) p = tail_pair(p); return !p; }
    inline static bool        is_proper_list(const term *t)                      { return is_proper_list(safe_cast<pair>(t)); }
    inline static bool        is_assoc(const pair *p)                            { return p && !is_proper_list(p); }
    inline static bool        is_assoc(const term *t)                            { return is_assoc(safe_cast<pair>(t)); }
    inline static size_t      length(const pair *p)                              { if (!p) return 0; size_t sz = 1; for (; p; ++sz) p = tail_pair(p); return sz; }
    inline static size_t      length(const term *t)                              { return length(safe_cast<pair>(t)); }
    
private:

    inline terms_t _copy_pair(Depth depth)    { return (depth == DEPTH_NONE) ? terms_t(_head, _tail) : terms_t(term::copy(_head, depth), term::copy(_tail, depth)); }
    inline void    _destroy_pair(Depth depth) { if (depth != DEPTH_NONE) { term::destroy(_head, depth); term::destroy(_tail, depth); } }
*/
};

struct atom : public term
{
    static constexpr Type term_type = TYPE_ATOM;
    
    Type type()               const { return term_type; }
    void destroy(Depth depth) const { if (depth == DEPTH_DEEP) delete this; }
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
    inline self_t              *copy(Depth depth)              const { return depth == DEPTH_DEEP ? create(_value) : this; }

    inline static       self_t *create(const T &value)               { return new self_t(value); }
    inline static       self_t *create()                             { return new self_t; }
};

template <typename T>
pair *series(T start, T end, T increment)
{
    typedef box<T> box_t;
    pair *a = nullptr, *p = nullptr;
    for (T i = start; i < end; i += increment) {
        pair *q = pair::create(box_t::create(i));
        if (!a) { a = q; p = a; continue; }
        p->_tail = q;
        p = q;
    }
    return a;
}

template <typename T>
pair *copy_list(const T *at, const T *end)
{
    typedef box<T> box_t;
    pair *first = nullptr, *p = nullptr;
    for (; at != end; ++at) {
        pair *elem = pair::create(box_t::create(*at));
        if (!first) { first = elem; p = first; continue; }
        p->_tail = elem;
        p = elem;
    }
    return first;
}

template <typename T, size_t N> pair *copy_list(const std::array<T, N> &u) { return copy_list(u.data(), u.data() + u.size()); }
template <typename T>           pair *copy_list(const std::vector<T>   &u) { return copy_list(u.data(), u.data() + u.size()); }

std::ostream &print(std::ostream &os, const term *e)
{
    if (is_nil(e)) return os << "nil";
    const term *pe = safe_cast<pair>(e), *et = nullptr;
    if (!pe) return e->print(os);
    os << "(";
    do {
        if (et) os << " ";
        print(os, pair::head(pe));
        et = pair::tail(pe);
        if (!et) break;
        const pair *pet = safe_cast<pair>(et);
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
