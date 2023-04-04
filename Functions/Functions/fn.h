#pragma once

#include <stddef.h>
#include <stdint.h>
#include "strs_c.h"
#include "std_string.h"
#include "format.h"

template <typename T, int N> struct fn_traits
{
    enum { COUNT = 0 };
    typedef T val_t;
    typedef val_t (*fn_t)();
    typedef T array_t[COUNT];
    inline static val_t call(fn_t f, const val_t *a) { return f(); }
};

template <typename T> struct fn_traits<T, 0>
{
    enum { COUNT = 0 };
    typedef T val_t;
    typedef val_t (*fn_t)();
    typedef T array_t[COUNT];
    inline static val_t call(fn_t f, const val_t *a) { return f(); }
};

template <typename T> struct fn_traits<T, 1>
{
    enum { COUNT = 1 };
    typedef T val_t;
    typedef val_t (*fn_t)(val_t);
    typedef T array_t[COUNT];
    inline static val_t call(fn_t f, const val_t *a) { return f(a[0]); }
};

template <typename T> struct fn_traits<T, 2>
{
    enum { COUNT = 2 };
    typedef T val_t;
    typedef val_t (*fn_t)(val_t, val_t);
    typedef T array_t[COUNT];
    inline static val_t call(fn_t f, const val_t *a) { return f(a[0], a[1]); }
};

template <typename T> struct fn_traits<T, 3>
{
    enum { COUNT = 3 };
    typedef T val_t;
    typedef val_t (*fn_t)(val_t, val_t, val_t);
    typedef T array_t[COUNT];
    inline static val_t call(fn_t f, const val_t *a) { return f(a[0], a[1], a[2]); }
};

template <typename T> struct fn_traits<T, 4>
{
    enum { COUNT = 4 };
    typedef T val_t;
    typedef val_t (*fn_t)(val_t, val_t, val_t, val_t);
    typedef T array_t[COUNT];
    inline static val_t call(fn_t f, const val_t *a) { return f(a[0], a[1], a[2], a[3]); }
};

template <typename T> struct fn_traits<T, 5>
{
    enum { COUNT = 5 };
    typedef T val_t;
    typedef val_t (*fn_t)(val_t, val_t, val_t, val_t, val_t);
    typedef T array_t[COUNT];
    inline static val_t call(fn_t f, const val_t *a) { return f(a[0], a[1], a[2], a[3], a[4]); }
};

template <typename T> struct fn_traits<T, 6>
{
    enum { COUNT = 6 };
    typedef T val_t;
    typedef val_t (*fn_t)(val_t, val_t, val_t, val_t, val_t, val_t);
    typedef T array_t[COUNT];
    inline static val_t call(fn_t f, const val_t *a) { return f(a[0], a[1], a[2], a[3], a[4], a[5]); }
};

template <typename T> struct fn_traits<T, 7>
{
    enum { COUNT = 7 };
    typedef T val_t;
    typedef val_t (*fn_t)(val_t, val_t, val_t, val_t, val_t, val_t, val_t);
    typedef T array_t[COUNT];
    inline static val_t call(fn_t f, const val_t *a) { return f(a[0], a[1], a[2], a[3], a[4], a[5], a[6]); }
};

template <typename T> struct fn_traits<T, 8>
{
    enum { COUNT = 8 };
    typedef T val_t;
    typedef val_t (*fn_t)(val_t, val_t, val_t, val_t, val_t, val_t, val_t, val_t);
    typedef T array_t[COUNT];
    inline static val_t call(fn_t f, const val_t *a) { return f(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]); }
};

struct fn_str
{
    inline fn_str(bool heap, const char *value = nullptr) : _heap(heap), _value(nullptr) { set(value); }
    inline ~fn_str() { set(nullptr); }

    fn_str &set(const char *value)
    {
        if (value && !*value) { value = nullptr; }
        if (value == _value || (value && _value && strcmp(value, _value) == 0)) {
            return *this;
        }
        if (_heap) {
            if (value) { value = strs_strdup(value); }
            if (_value) { delete [] _value; }
        }
        _value = value;
        return *this;
    }

    inline bool        heap()   const { return _heap; }
    inline const char *str()    const { return _value ? _value : ""; }

private:

    bool        _heap;
    const char *_value;
};

struct fn_info
{
    inline fn_info(bool heap) : _name(heap), _format(heap), _heap(heap) {}

    inline fn_info &set_name(const char *s)   { _name.set(s); return *this; }
    inline fn_info &set_format(const char *s) { _format.set(s); return *this; }

    inline bool        heap()   const { return _heap; }
    inline const char *name()   const { return _name.str(); }
    inline const char *format() const { return _format.str(); }

private:

    bool   _heap;
    fn_str _name;
    fn_str _format;
};

template <typename T>
struct fn
{
    typedef fn<T> self_t;
    typedef T     val_t;

    inline fn(const char *name = "", const char *format = "", bool heap = false) : info(heap) { info.set_name(name).set_format(format); }
    virtual ~fn() {}

    virtual val_t  call(const val_t *a) const = 0;
    virtual size_t arity() const = 0;

    virtual bool           composite_count() const { return 0; }
    virtual const self_t * const *composite_fns()   const { return nullptr; }
    
    virtual std::string format() const { return info.format(); }
    
    fn_info info;
};

template <typename T>
std::string _format(const fn<T> *f)
{
    if (f == nullptr) {
        return "";
    }
    size_t count = f->composite_count();
    if (count) {
        const fn<T> * const *array = f->composite_fns();
        std::vector<std::string> vs;
        for (size_t i = 0; i < count; i++) {
            std::string s(_format(array[i]));
            if (!s.empty()) {
                vs.push_back(s);
            }
        }
        return std::string(f->info.format()) + "(" + join(vs, ",") + ")";
    }
    return f->format();
}

template <typename ContainerT>
static size_t max_arity(const ContainerT &c)
{
    size_t ar = 0;
    for (typename ContainerT::const_iterator i = c.begin(), e = c.end(); i != e; ++i) {
        size_t far = (*i)->arity();
        ar = far > ar ? far : ar;
    }
    return ar;
}

template <typename ContainerT>
static size_t min_arity(const ContainerT &c)
{
    size_t ar = UINT_MAX;
    for (typename ContainerT::const_iterator i = c.begin(), e = c.end(); i != e; ++i) {
        if (*i) {
            size_t far = (*i)->arity();
            ar = far < ar ? far : ar;
        }
    }
    return ar;
}

template <typename T, int N>
struct fn_array_type : public std::array<const fn<T> *, N>
{
    typedef std::array<const fn<T> *, N> container_type;
    size_t max_arity() const { return max_arity(*this); }
    size_t min_arity() const { return min_arity(*this); }
};

template <typename T>
struct fn_vector_type : public std::vector<const fn<T> *>
{
    typedef std::vector<const fn<T> *> container_type;
    size_t max_arity() const { return max_arity(*this); }
    size_t min_arity() const { return min_arity(*this); }
};

template <typename T, int N, int M>
inline static T call_bound(const fn<T> *f, const T *a, const T *b)
{
    const size_t nm = N - M;
    T v[N] = { 0 };
    for (size_t i = 0; i < N; i++) { v[i] = (i < nm ? a[i] : b[i - nm]); }
    return f->call(v);
}

template <typename T, int N, int M>
inline static T call_bound_fn1(const fn<T> *f, const T *a, const fn<T> **b)
{
    const size_t nm = N - M;
    T v[N] = { 0 };
    for (size_t i = 0; i < N; i++) { v[i] = (i < nm ? a[i] : call(b[i - nm], a[0])); }
    return f->call(v);
}

template <typename T> inline T call(const fn<T> *fn, T a = 0, T b = 0, T c = 0, T d = 0, T e = 0, T f = 0, T g = 0, T h = 0)
{
    T v[8] = { a, b, c, d, e, f, g, h };
    return fn->call(v);
}

template <typename T>
struct fn_identity : public fn<T>
{
    typedef fn_traits<T, 1> fptrn_t;

    inline fn_identity(bool heap = false) : fn<T>("identity", "$A", heap) {}
    inline T      call(const T *v) const { return v[0]; }
    inline size_t arity() const { return fptrn_t::COUNT; }
};
    
template <typename T>
struct fn_constant : public fn<T>
{
    typedef fn_traits<T, 0> fptrn_t;

    T constant;

    inline fn_constant(T constant, const char *name = "", const char *format = "", bool heap = false)
        : fn<T>(name, format, heap), constant(constant) {}
    inline T      call(const T *values) const { return constant; }
    inline size_t arity() const { return fptrn_t::COUNT; }
};

template <typename T, int N>
struct fn_function : public fn<T>
{
    typedef fn_traits<T, N> fptrn_t;
    typedef typename fptrn_t::fn_t fn_t;

    fn_t function;

    inline fn_function(fn_t function, const char *name = "", const char *format = "", bool heap = false)
        : fn<T>(name, format, heap), function(function) {}
    inline T      call(const T *values) const { return fptrn_t::call(function, values); }
    inline size_t arity() const { return fptrn_t::COUNT; }
};

template <typename T, int N, int M>
struct fn_bind : public fn<T>
{
    const fn<T> *function;
    double       bound[M];

    inline fn_bind(const fn<T> *f, const T *a, const char *name = "", const char *format = "", bool heap = true)
        : fn<T>(name, format, heap), function(f) { for (size_t i = 0; i < M; i++) { bound[i] = a[i]; } }
    inline T      call(const T *a) const { return call_bound<T, N, M>(function, a, bound); }
    inline size_t arity() const { return N - M; }
};

template <typename T, int N, int M>
struct fn_bind_fn1 : public fn<T>
{
    const fn<T> *func, *bound[M];

    inline fn_bind_fn1(const fn<T> *f, const fn<T> **a, const char *name = "", const char *format = "", bool heap = true)
        : fn<T>(name, format, heap), func(f) { for (size_t i = 0; i < M; i++) { bound[i] = a[i]; } }
    inline T      call(const T *a) const { return call_bound_fn1<T, N, M>(func, (const T *)a, (const fn<double> **)bound); }
    inline size_t arity() const { return N - M; }
    
    virtual bool          composite_count() const { return M; }
    virtual const fn<T> * const * composite_fns() const { return &bound[0]; }
    
    virtual std::string format() const
    {
        char buffer[64 * 1024];
        const char *formats[M] = { nullptr };
        std::vector<std::string> vs;
        for (size_t i = 0; i < M; i++) { vs.push_back(bound[i]->format()); formats[i] = vs.back().c_str(); }
        format_vars_array(buffer, func->format().c_str(), M, formats);
        return buffer;
    }
};
    
template <typename T>
struct fn_chain : public fn<T>
{
    typedef fn_traits<T, 1> fptrn_t;
    
    const fn<T> *fns[2] = { nullptr };

    inline fn_chain(const fn<T> *a, const fn<T> *b, const char *name = "", const char *format = "", bool heap = true)
    : fn<T>(name, format, heap) { fns = { a, b }; }
    inline T      call(const T *v) const { return call(fns[1], fns[0]->call(v)); }
    inline size_t arity() const { return fptrn_t::COUNT; }
    
    virtual bool          composite_count() const { return 2; }
    virtual const fn<T> **composite_fns()   const { return fns; }
};
    
template <typename T>
struct fn_list : public fn<T>
{
    typedef fn_traits<T, 1> fptrn_t;
    
    std::vector<const fn<T> *> funcs;

    inline fn_list(const std::vector<const fn<T> *> &funcs, const char *name = "", const char *format = "", bool heap = true)
        : fn<T>(name, format, heap), funcs(funcs) {}
    inline T call(const T *v) const
    {
        T u = v[0];
        for (size_t i = 0, sz = funcs.size(); i < sz; i++) { u = ::call(funcs[i], u); }
        return u;
    }
    inline size_t arity() const { return fptrn_t::COUNT; }
    
    virtual bool          composite_count() const { return funcs.size(); }
    virtual const fn<T> * const *composite_fns()   const { return funcs.data(); }
    
    std::string format() const
    {
        std::string s(funcs[0]->format());
        char buffer[64 * 1024];
        format_vars(buffer, s.c_str(), "x");
        s = buffer;
        for (size_t i = 1, sz = funcs.size(); i < sz; i++) {
            format_vars(buffer, funcs[i]->format().c_str(), s.c_str());
            s = buffer;
        }
        return s;
    }
};

template <typename T, int N>
struct fn_accumulator : public fn<T>
{
    typedef fn_traits<T, 2> fptrn_t;
    typedef fn_array_type<T, N> fn_set;
    
    fn_set funcs = { nullptr };
    const fn<T> *accumulator;

    inline fn_accumulator(const fn<T> *accumulator, const fn_set &funcs, const char *name = "", const char *format = "", bool heap = false)
        : fn<T>(name, format, heap), accumulator(accumulator), funcs(funcs) { assert(accumulator->count() == 2); }
    inline T call(const T *v) const
    {
        const fn<T> *fi = funcs[0];
        T accum = fi ? call(fi, v[0]) : v[0];
        for (size_t i = 1, sz = funcs.size(); i < sz; i++) {
            fi = funcs[i];
            accum = call(accumulator, accum, (fi ? call(fi, v[i]) : v[i]));
        }
        return accum;
    }
    inline size_t arity() const { return N; }
    
    virtual bool          composite_count() const { return funcs.size(); }
    virtual const fn<T> * const *composite_fns()   const { return funcs.data(); }
};
