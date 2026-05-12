#pragma once

#include <string_view>
#include <iostream>
#include <sstream>
#include <array>
#include <string>
#include <cctype>
#include <algorithm>
#include <string_view>
#include "elems.h"

inline elem *wrap(elem *value)       { return value; }
inline elem *wrap(float value)       { return new box<double>(value); }
inline elem *wrap(double value)      { return new box<double>(value); }
inline elem *wrap(int64_t value)     { return new box<int64_t>(value); }
inline elem *wrap(size_t value)      { return new box<int64_t>(int64_t(value)); }
inline elem *wrap(int32_t value)     { return new box<int64_t>(value); }

inline pair           *_(elem *head = nullptr, elem *tail = nullptr)     { return new pair(head, tail); }
inline elem           *new_pair(elem *head, elem *tail)                  { return new pair(head, tail); }
inline elem           *head(pair *list)                                  { return list ? list->head : nullptr; }
inline bool            is(elem *e, elem::Type type)                      { return e ? e->is(type) : false; }
inline bool            is_atom(elem *e)                                  { return e ? e->is_atom() : false; }
inline bool            is_number(elem *e)                                { return e ? e->is_number() : false; }
inline bool            is_symbol(elem *e)                                { return is(e, elem::SYMBOL); }
inline bool            is_list(elem *e)                                  { return !e || e->is(elem::PAIR); }
inline bool            is_nil(pair *p)                                   { return !p || (!p->head && !p->tail); }
inline bool            is_nil(elem *e)                                   { return !e || e->is(elem::NIL) || (e->is(elem::PAIR) && is_nil(cast<pair>(e))); }
inline pair           *head_pair(pair *list)                             { return cast<pair>(head(list)); }
inline elem           *head(elem *list)                                  { return head(cast<pair>(list)); }
inline pair           *head_pair(elem *list)                             { return head_pair(cast<pair>(list)); }
inline elem           *tail(pair *list)                                  { return list ? list->tail : nullptr; }
inline pair           *tail_pair(pair *list)                             { return cast<pair>(tail(list)); }
inline elem           *tail(elem *list)                                  { return tail(cast<pair>(list)); }
inline pair           *tail_pair(elem *list)                             { return tail_pair(cast<pair>(list)); }

inline bool            equals(elem *a, elem *b)
{
    if (a == b || (is_nil(a) && is_nil(b))) {
        return true;
    }
    if (a->_type != b->_type) {
        return false;
    }
    pair *pa = cast<pair>(a), *pb = cast<pair>(b);
    return pa && pb ? equals(pa->head, pb->head) && equals(pa->tail, pb->tail) : a->equals(b);
}

inline bool            match_dot(pair *dot, elem *key)                   { return dot ? equals(head(dot), key) : false; }

typedef std::pair<elem **, size_t> linear_t;

inline size_t          length(pair *p)                                   { size_t c = 0; for (; p; c++) p = tail_pair(p); return c; }
inline size_t          length(elem *e)                                   { return length(cast<pair>(e)); }

inline size_t          linear(pair *p, elem **into, size_t max)          { size_t c = 0; for (; p && c < max; p = tail_pair(p), c++) *into++ = head(p); return c; }
inline linear_t        linear_alloc(pair *p)                             { size_t sz = length(p); elem **into = new elem * [sz]; linear(p, into, sz); return linear_t(into, sz); }
inline void            linear_free(linear_t &lin)                        { delete [] lin.first; lin.first = nullptr; }
inline linear_t        linear_reverse(elem **into, size_t n)             { std::reverse(into, into + n); return linear_t(into, n); }
inline linear_t        linear_reverse(linear_t &lin)                     { return linear_reverse(lin.first, lin.second); }

// iterative preferred here
inline pair           *nth_pair(pair *list, size_t n)                    { for (size_t i = 0; i < n && list; ++i) { list = tail_pair(list); } return list; }
inline elem           *nth_elem(pair *list, size_t n)                    { return head(nth_pair(list, n)); }

inline bool            is_dotted(pair *p)                                { while (p) { if (is_atom(p->tail)) return true; p = tail_pair(p); } return false; }
inline bool            is_dotted(elem *e)                                { return is_dotted(cast<pair>(e)); }

inline pair           *list(elem **a, size_t n)                          { return n == 0 ? nullptr : _(*a, list(a + 1, n - 1)); }
inline pair           *list(elem *a, elem *b)                            { return _(a, _(b)); }
inline pair           *list(elem *a, elem *b, elem *c)                   { return _(a, list(b, c)); }
inline pair           *list(elem *a, elem *b, elem *c, elem *d)          { return _(a, list(b, c, d)); }
inline pair           *list(elem *a, elem *b, elem *c, elem *d, elem *e) { return _(a, list(b, c, d, e)); }

inline pair           *reverse(pair *p)                                  { size_t sz = length(p); elem **into = new elem * [sz]; linear_reverse(into, sz); auto q = list(into, sz); delete [] into; return q; }

inline pair           *last_pair(pair *p)                                { if (is_nil(p)) return nullptr; pair *q = p; while (p) { q = p; p = tail_pair(p); } return q; }
inline pair           *copy_list(pair *p)                                { return is_nil(p) ? nullptr : _(p->head, copy_list(tail_pair(p->tail))); }
