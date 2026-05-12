#pragma once

#include <cstddef>
#include <cstdint>

template <typename T>
struct stack
{
    T *_base, *_back, *_;
    stack(T *data, size_t count): _base(data), _back(_base + count), _(_back) {}
    
    const T &top()                       const { return *_; }
    T       &top()                             { return *_; }
    const T &at(size_t n)                const { return _[n]; }
    T       &at(size_t n)                      { return _[n]; }
    bool     eq()                        const { return _[0] == _[1]; }
    bool     eq(size_t n)                const { return _[n] == _[n + 1]; }
    void     set(const T &v, size_t n)   const { _[n] = v; }
    void     set(const T &v)             const { *_ = v; }
    size_t   size()                      const { return size_t(_back - _); }
    void     push(const T &v)                  { *(--_) = v; }
    void     push(const T &v, size_t n)        { for (size_t i = 0; i < n; i++) *(--_) = v; } // a faster way to do this or let the compiler handle it?
    void     drop()                            { ++_; }
    void     drop(size_t n)                    { _ += n; }
    const T &pop()                             { return *_++; }
    void     swap()                            { T t(*_); *_ = _[1]; _[1] = t; }
    void     swap(size_t n)                    { T t(_[n]); _[n] = _[n + 1]; _[n + 1] = t; }
    void     dupe()                            { *(--_) = *_; }
    void     dupe(size_t n)                    { for (size_t i = 0; i < n; i++) *(--_) = *_; }
    void     over()                            { *(--_) = _[1]; }
    bool     empty()                     const { return _ >= _back; }
    
    template <typename Op> void set_op_unary()          { static constexpr const Op op; *_ = op(*_); }
    template <typename Op> void set_op_binary()         { static constexpr const Op op; _[1] = op(_[1], *_); ++_; }
    template <typename Op> bool pred_unary()      const { static constexpr const Op op; return op(*_); }
    template <typename Op> bool pred_binary()     const { static constexpr const Op op; return op(_[1], *_); }
    
    void     op_neg()                          { *_ = -*_; }
    void     op_not()                          { *_ = ~*_; }
    void     op_shr(size_t n)                  { *_ >>= n; }
    void     op_shl(size_t n)                  { *_ >>= n; }
    void     op_add()                          { _[1] += *_; ++_; }
    void     op_sub()                          { _[1] -= *_; ++_; }
    void     op_mul()                          { _[1] *= *_; ++_; }
    void     op_div()                          { _[1] /= *_; ++_; }
    void     op_mod()                          { _[1] %= *_; ++_; }
    void     op_and()                          { _[1] &= *_; ++_; }
    void     op_or()                           { _[1] |= *_; ++_; }
    void     op_xor()                          { _[1] ^= *_; ++_; }

    bool     logical_and()               const { return _[1] && *_; }
    bool     logical_or()                const { return _[1] || *_; }
    bool     logical_not()               const { return !*_; }
    bool     logical_bool()              const { return !!*_; }
    bool     logical_lt()                const { return _[1] <  *_; }
    bool     logical_lte()               const { return _[1] <= *_; }
    bool     logical_eq()                const { return _[1] == *_; }
    bool     logical_gte()               const { return _[1] >= *_; }
    bool     logical_gt()                const { return _[1] >  *_; }

    bool     predicate_and()                   { bool r = _[1] && *_; _[1] = r; ++_; return r; }
    bool     predicate_or()                    { bool r = _[1] || *_; _[1] = r; ++_; return r; }
    bool     predicate_not()                   { return *_ = !*_; }
    bool     predicate_bool()                  { return *_ = !!*_; }
    bool     predicate_lt()                    { bool r = _[1] <  *_; _[1] = r; ++_; return r; }
    bool     predicate_lte()                   { bool r = _[1] <= *_; _[1] = r; ++_; return r; }
    bool     predicate_eq()                    { bool r = _[1] == *_; _[1] = r; ++_; return r; }
    bool     predicate_gte()                   { bool r = _[1] >= *_; _[1] = r; ++_; return r; }
    bool     predicate_gt()                    { bool r = _[1] >  *_; _[1] = r; ++_; return r; }
};

template <typename T, size_t N>
struct stack_n : public stack<T>
{
    std::array<T, N> data;
    
    stack_n() : data({}), stack<T>(data.data(), N) {}
};
