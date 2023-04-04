#pragma once

#include <stddef.h>
#include <stdint.h>
#include <array>
#include <string>
#include <vector>
#include <map>

#include "fn.h"

class State
{
public:

    const int *start;
    const int *p;
    const int *end;
    
    fn_vector_type<double> queue;
       
    inline State(const int *start, const int *end, const int *p = nullptr) : start(start), end(end), p(p ? p : start) {}
    
    inline bool done() const { return p >= end; }
    
    inline State &push(const fn<double> *f) { queue.push_back(f); return *this; }
    inline const fn<double> *pop() { const fn<double> *f = queue.back(); queue.pop_back(); return f; }
};

struct Functions
{
    typedef fn<double>             func_double_t;

    typedef fn_constant<double>    const0_t;
    typedef fn_function<double, 1> func1_t;
    typedef fn_function<double, 2> func2_t;
    typedef fn_function<double, 3> func3_t;

    const fn<double> *constant(double v);
    const fn<double> *chain(const fn<double> *a, const fn<double> *b, const fn<double> *c = nullptr);

    // no arg
    static const0_t

        _pi2, __6, _e2, __5, __4, _pi, __3, _e, __2, __1, _0, _1, _2, e, _3, pi, _4, _5, e2, _6, pi2;

    static fn_identity<double> identity;

    // one arg
    static func1_t

        abs, sin, sinh, cos, cosh, tan, tanh, asin, asinh, acos, acosh,
        atan, atanh, sinc, sincn, exp, exp2, log, log2, log10, sqrt, cbrt,
        erf, ceil, floor, round, inc, dec, x2, half, x3, third,
        sin1pi, cos1pi, tan1pi, atan1pi,
        invert, square, cube, negate, one_minus;

    // two args
    static func2_t

        fmod, atan2, hypot, pow, fmax, fmin, add, sub, mul, div, sinpi, cospi, tanpi, atanpi, average;

    // three args
    static func3_t

        fma, interp, sinpiz, cospiz, tanpiz, atanpiz,
        add3, sub3, mul3, div3;

    const fn<double> *vm_function(State &state);
    const fn<double> *op(State &s);

    ~Functions();
    
    const fn<double> *bind2to1(const fn<double> *f, const fn<double> *bind);
    const fn<double> *bind3to1(const fn<double> *f, const fn<double> *bind0, const fn<double> *bind1);
    const fn<double> *chain(const fn<double> *f0, const fn<double> *f1);
    
protected:
    
    const fn<double> *_op_constant(int value);
    const fn<double> *_op1(int value);
    const fn<double> *_op2(int value);
    const fn<double> *_op3(int value);

    const fn<double> *register_fn(const fn<double> *f);
    
    void destroy_fn(const fn<double> *f);
    
    inline const fn<double> *get_fn(const char *name) { return name && *name && _functions.count(name) ? _functions[name] : nullptr; }
    
private:
    
    std::vector<const fn<double> *>            _registered;
    std::map<const char *, const fn<double> *> _functions;
    std::map<const fn<double> *, size_t>       _pointers;

    inline bool _is_registered_fn(const fn<double> *f) { return f && _pointers.count(f); }
    
    void _remove_function(const char *name, const fn<double> *f = nullptr);
};
