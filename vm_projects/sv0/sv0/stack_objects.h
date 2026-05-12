#pragma once

#include <iostream>
#include <sstream>
#include "tag.h"
#include "stack.h"
#include "object.h"

class stack_objects
{
public:
    
    stack_objects(stack<tag> &s) : _s(s) {}

    object             *top_object()                     { tag &t = _s.top(); return t.safe_object(); }
    pair               *top_pair()                       { tag &t = _s.top(); return t.safe_pair(); }
    
    void                repw_pair()                      { pair *p = _(); _s.at(1) = p; _s.drop(); }
    void                repw_head()                      { pair *p = top_pair(); _s.top() = p ? p->head() : tag::nil; }
    void                repw_tail()                      { pair *p = top_pair(); _s.top() = p ? p->tail() : tag::nil; }
    void                repw_length()                    { pair *p = top_pair(); if (p) _s.top().set(p->length()); else _s.top().set(tag::nil); }

    pair               *repw_list(uint16_t count)        { pair *p = list    (_s._, count); _s.drop(count - 1); _s.top() = p; return p; }
    pair               *repw_listr(uint16_t count)       { pair *p = list_rev(_s._, count); _s.drop(count - 1); _s.top() = p; return p; }

private:

    stack<tag> &_s;
    
    pair               *_(size_t i)                      { return new pair(_s.at(i), tag::nil); }
    pair               *_(size_t i, size_t j)            { return new pair(_s.at(i), _s.at(j)); }
    pair               *_(size_t i, pair *p)             { return new pair(_s.at(i), p); }
    pair               *_(size_t i, const tag &t)        { return new pair(_s.at(i), t); }
    pair               *_()                              { pair *p = _(1, size_t(0)); return p; }
    
    static pair        *list(const tag *a, size_t n)     { return !n ? nullptr : new pair(*a, list(a + 1, n - 1)); }
    static pair        *list_rev(const tag *a, size_t n) { return !n ? nullptr : new pair(*(a + n - 1), list_rev(a, n - 1)); }
};
