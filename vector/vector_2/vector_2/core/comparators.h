#pragma once

#include <algorithm>
#include <functional>

template <typename T> inline constexpr int _sign(T u)                         { return (T(0) < u) - (u < T(0)); }
template <typename T> inline constexpr int _compare_float(T a, T b)           { return _sign(a - b); }
template <typename T> inline constexpr int _compare_signed(T a, T b)          { return _sign(a - b); }
template <typename T> inline constexpr int _compare_unsigned(T a, T b)        { return _sign(int(a) - int(b)); }

inline constexpr int _compare(uint8_t  a, uint8_t  b) { return _compare_unsigned(a, b); }
inline constexpr int _compare(uint16_t a, uint16_t b) { return _compare_unsigned(a, b); }
inline constexpr int _compare(uint32_t a, uint32_t b) { return _compare_unsigned(a, b); }
inline constexpr int _compare(uint64_t a, uint64_t b) { return _compare_unsigned(a, b); }
inline constexpr int _compare(int8_t   a, int8_t   b) { return _compare_signed(a, b); }
inline constexpr int _compare(int16_t  a, int16_t  b) { return _compare_signed(a, b); }
inline constexpr int _compare(int32_t  a, int32_t  b) { return _compare_signed(a, b); }
inline constexpr int _compare(int64_t  a, int64_t  b) { return _compare_signed(a, b); }
inline constexpr int _compare(float    a, float    b) { return _compare_float(a, b); }
inline constexpr int _compare(double   a, double   b) { return _compare_float(a, b); }

template <typename T> inline int     _compare_exp(T a, T b, int o = 0)  { int xa, xb; frexp(a, &xa); frexp(b, &xb); return _sign(xa - xb + o); }
template <typename T> inline int     _int_sign(T v)                     { return fpclassify(v) == FP_ZERO ? 0 : (!signbit(v) ? 1 : -1);  }
template <typename T> inline int     _int_sign_0_is_pos(T v)            { return (fpclassify(v) == FP_ZERO || !signbit(v)) ? 1 : -1;  }

template <typename T, typename R> struct comparator_fn
{
    typedef R (*fn_ptr_t)(const T &a, const T &b);
    fn_ptr_t _fn;
    inline comparator_fn(fn_ptr_t fn) : _fn(fn) {}
    inline R operator()(const T &a, const T &b) const { return _fn(a, b); }
};

template <typename Value, typename Compare0, typename Compare1>
struct comparator_chain_bool
{
    Compare0 _c0;
    Compare1 _c1;
    inline comparator_chain_bool() {}
    inline comparator_chain_bool(const Compare0 &c0, const Compare1 &c1) : _c0(c0), _c1(c1) {}
    inline bool operator()(const Value &a, const Value &b) const { return _c0(a, b) ? true : (_c0(b, a) ? false : _c1(a, b)); }
};

template <typename Value, typename Compare0, typename Compare1>
struct comparator_chain_int
{
    Compare0 _c0;
    Compare1 _c1;
    inline comparator_chain_int() {}
    inline comparator_chain_int(const Compare0 &c0, const Compare1 &c1) : _c0(c0), _c1(c1) {}
    inline int operator()(const Value &a, const Value &b) const { int r = _c0(a, b); return !r ? _c1(a, b) : r; }
};

template <typename Value, typename Compare>
struct comparator_int_to_bool
{
    Compare _c;
    inline comparator_int_to_bool() {}
    inline comparator_int_to_bool(const Compare &c) : _c(c) {}
    inline bool operator()(const Value &a, const Value &b) const { return _c(a, b) < 0; }
};

template <typename Value, typename Compare>
struct comparator_bool_to_int
{
    Compare _c;
    inline comparator_bool_to_int() {}
    inline comparator_bool_to_int(const Compare &c) : _c(c) {}
    inline int operator()(const Value &a, const Value &b) const { return _c(a, b) ? -1 : (_c(b, a) ? 1 : 0); }
};

template <typename T> inline auto bool_comparator(int (*fn_ptr_t)(const T &a, const T &b))
{
    typedef comparator_fn<T, int> c_t;
    return comparator_int_to_bool<T, c_t>(c_t(fn_ptr_t));
}

template <typename T> inline auto bool_comparator(bool (*fn_ptr_t)(const T &a, const T &b))
{
    typedef comparator_fn<T, bool> c_t;
    return c_t(fn_ptr_t);
}

template <typename T> struct less_abs    { inline bool operator()(const T &a, const T &b) const { return abs(a) < abs(b); } };
template <typename T> struct greater_abs { inline bool operator()(const T &a, const T &b) const { return abs(a) > abs(b); } };

//template <typename T, typename Compare> struct compare_abs { Compare _c; inline bool operator()(const T &a, const T &b) const { return _c(abs(a), abs(b)); } };
//template <typename T, typename Compare> struct compare_abs : public compare_functor<T, Compare, std::function<int(T,T)>(std::abs)> {};

template <typename TupleOrPair, int N, typename Compare>
struct compare_nth { Compare _c; inline bool operator()(const TupleOrPair &a, const TupleOrPair &b) const { return _c(std::get<N>(a), std::get<N>(b)); } };
