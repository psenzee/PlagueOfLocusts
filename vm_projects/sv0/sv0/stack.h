#pragma once

#include <iostream>
#include <cmath>

template <typename T>
struct stack
{
    typedef stack<T> self_t;

    T *_base, *_back, *_;
    stack(T *data, size_t capacity): _base(data), _back(_base + capacity), _(_back) {}
    
    self_t   substack()                        { return stack(_base, size()); }
    
    const T &top()                       const { return *_; }
    T       &top()                             { return *_; }
    const T &at(size_t n)                const { return _[n]; }
    T       &at(size_t n)                      { return _[n]; }
    bool     eq()                        const { return _[0] == _[1]; }
    bool     eq(size_t n)                const { return _[n] == _[n + 1]; }
    void     set(const T &v, size_t n)   const { _[n] = v; }
    void     set(const T &v)             const { *_ = v; }
    size_t   capacity()                  const { return size_t(_back - _base); }
    size_t   size()                      const { return size_t(_back - _); }
    T       &push()                            { return *(--_); }
    void     push(const T &v)                  { *(--_) = v; }
    void     push(const T &v, size_t n)        { for (size_t i = 0; i < n; i++) *(--_) = v; } // find faster way
    void     drop()                            { ++_; }
    void     drop(size_t n)                    { _ += n; }
    const T &pop()                             { return *_++; }
    void     swap()                            { T t(*_); *_ = _[1]; _[1] = t; }
    void     swap(size_t n)                    { T t(_[n]); _[n] = _[n + 1]; _[n + 1] = t; }
    void     dupe()                            { *(--_) = *_; }
    void     dupe(size_t n)                    { push(*_, n); }
    void     over()                            { *(--_) = _[1]; }
    bool     empty()                     const { return _ >= _back; }
    
    std::ostream &print_stack(std::ostream &os) const { for (auto v = _back; v >= _; --v) v->print(os << " "); return os; }
};
