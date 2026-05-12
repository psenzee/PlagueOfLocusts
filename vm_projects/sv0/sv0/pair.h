#pragma once

#include <iostream>
#include <sstream>
#include <algorithm>
#include "tag.h"
#include "stack.h"
#include "object.h"
#include "utils.h"

class pair : public object
{
    tag _head, _tail;

    static constexpr const size_t MAX_LOCAL = 256;

public:
    
    pair(const tag &head, const tag &tail = tag()) : _head(head), _tail(tail) {}
    pair(pair *head, pair *tail) : _head(head), _tail(tail) {}
    pair(const tag &head, pair *tail) : _head(head), _tail(tail) {}
    pair(pair *head, const tag &tail) : _head(head), _tail(tail) {}
    
    virtual Type      type()                           const { return PAIR; }
    
    void              set_head(const tag &u)                 { _head = u; }
    void              set_tail(const tag &u)                 { _tail = u; }
    
    const tag        &head()                           const { return _head; }
    const tag        &tail()                           const { return _tail; }
    
    object           *head_obj()                             { return _head.safe_object(); }
    object           *tail_obj()                             { return _tail.safe_object(); }
    
    pair             *head_pair()                            { return _head.safe_pair(); }
    pair             *tail_pair()                            { return _tail.safe_pair(); }
    
    const pair       *head_pair()                      const { return _head.safe_pair(); }
    const pair       *tail_pair()                      const { return _tail.safe_pair(); }
    
    uint32_t          length()                         const { return length(this); }

    std::ostream     &print(std::ostream &os)          const
    {
        const pair *p = this;
        os << "(";
        while (p) {
            os << p->_head;
            if (p->_tail.is_nil()) {
                break;
            }
            const pair *q = p->tail_pair();
            if (!q) {
                return os << " . " << p->_tail << ")";
            }
            os << " ";
            p = q;
        }
        return os << ")";
    }

    // utilities
    typedef void (*fn_ptr_t)(pair *);
    
    static pair      *list(const tag *a, size_t n)           { return !n ? nullptr : new pair(*a, list(a + 1, n - 1)); }
    static void       traverse(pair *p, fn_ptr_t fp)         { if (!p) return; fp(p); traverse(p->tail_pair(), fp); }
    
    static pair      *copy_into(const tag *t, pair *p)       { pair *q = p ? p->tail_pair() : p; if (!q) return p; p->set_head(*t); return copy_into(t + 1, q); }
    static size_t     copy_into(const pair *p, tag *into)    { tag *st(into); for (; p; p = p->tail_pair()) *into++ = p->_head; return into - st; }
    
    static uint32_t   length(const pair *p)                  { uint32_t c = 0; for (; p; c++) p = p->tail_pair(); return c; }
    static pair      *nth_pair(pair *p, size_t n)            { for (size_t i = 0; i < n && p; ++i) { p = p->tail_pair(); } return p; }
    static const tag &nth(pair *p, size_t n)                 { p = nth_pair(p, n); return p ? p->_head : tag::nil; }
    static object    *nth_object(pair *p, size_t n)          { return nth(p, n).safe_object(); }

    static pair      *reverse_copy(pair *p)                  { pair *q = new pair(p->_head, tag::nil); while ((p = p->tail_pair())) q = new pair(p->_head, q); return q; }

    /*
    static pair      *reverse(pair *p, bool copy = true)
    {
        size_t sz = length(p);
        std::array<tag, 256> buf;
        tag *tags = local_alloc(sz, buf);
        copy_into(p, tags);
        std::reverse(tags, tags + sz);
        if (copy) {
            p = list(tags, sz);
        } else {
            copy_into(tags, p);
        }
        local_free(tags, buf);
        return p;
    }*/
    
    static pair      *reverse_local(pair *p)
    {
        size_t sz = length(p);
        std::array<tag, MAX_LOCAL> buf;
        copy_into(p, buf.data());
        std::reverse(buf.data(), buf.data() + sz);
        copy_into(buf.data(), p);
        return p;
    }
    
    static pair      *reverse(pair *p, bool copy = true)
    {
        size_t sz = length(p);
        std::array<tag, MAX_LOCAL> buf;
        tag *tags = local_alloc(sz, buf);
        copy_into(p, tags);
        std::reverse(tags, tags + sz);
        if (copy) {
            p = list(tags, sz);
        } else {
            copy_into(tags, p);
        }
        local_free(tags, buf);
        return p;
    }
    
private:
    /*
    typedef std::pair<tag *, size_t> linear_t;
    
    static size_t     linear(pair *p, tag *into, size_t max) { size_t c = 0; for (; p && c < max; p = p->tail_pair(), c++) *into++ = p->_head; return c; }
    static size_t     linear(pair *p, tag *into)             { tag *st(into); for (; p; p = p->tail_pair()) *into++ = p->_head; return into - st; }
    static linear_t   linear_alloc(pair *p)                  { size_t sz = length(p); tag *into = new tag [sz]; linear(p, into); return linear_t(into, sz); }
    static void       linear_free(linear_t &lin)             { delete [] lin.first; lin.first = nullptr; }
    static linear_t   linear_reverse(tag *into, size_t n)    { std::reverse(into, into + n); return linear_t(into, n); }
    static linear_t   linear_reverse(linear_t &lin)          { return linear_reverse(lin.first, lin.second); }
    */
};

