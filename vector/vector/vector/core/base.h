#pragma once

#include <cstddef>
#include <iostream>

static constexpr size_t _compile_log2_(size_t n) { return ((n < 2) ? 1 : (1 + _compile_log2_(n / 2))); }

template <typename T> inline T _square(const T &u) { return u * u; };

template <typename T0, typename T1> struct op_add      { inline T0 operator()(const T0 &a, const T1 &b) const { return a + b; } };
template <typename T0, typename T1> struct op_multiply { inline T0 operator()(const T0 &a, const T1 &b) const { return a * b; } };
template <typename T0, typename T1> struct op_subtract { inline T0 operator()(const T0 &a, const T1 &b) const { return a - b; } };
template <typename T0, typename T1> struct op_divide   { inline T0 operator()(const T0 &a, const T1 &b) const { return a / b; } };

template <typename T0, typename T1, typename OutT, typename OperatorT>
inline void zip(const T0 &a, const T1 &b, OutT &c, OperatorT &op)
{
    auto j = b.begin(); auto k = c.begin();
    for (auto i = a.begin(), e = a.end(); i != e; i++, j++, k++) *k = op(*i, *j);
}

template <typename T0, typename T1, typename OperatorT>
inline void combine(T0 &a, const T1 &b, OperatorT op)
{
    auto j = b.begin(), je = b.end();
    for (auto i = a.begin(), ie = a.end(); i != ie && j != je; i++, j++) *i = op(*i, *j);
}

template <typename T0, typename T1, typename OperatorT, typename AccumulatorT>
inline typename T0::value_type accumulate_combine(const T0 &a, const T1 &b, OperatorT combine, typename T0::value_type initial = typename T0::value_type(0), AccumulatorT accumulate = op_add<T0, T1>())
{
    auto j = b.begin(), je = b.end();
    typename T0::value_type accumulator(initial);
    for (auto i = a.begin(), ie = a.end(); i != ie && j != je; i++, j++) accumulator = accumulate(accumulator, combine(*i, *j));
    return accumulator;
}

template <typename T0, typename T1> inline void subtract (T0 &a, const T1 &b) { combine(a, b, op_subtract <typename T0::value_type, typename T1::value_type>()); }
template <typename T0, typename T1> inline void add      (T0 &a, const T1 &b) { combine(a, b, op_add      <typename T0::value_type, typename T1::value_type>()); }
template <typename T0, typename T1> inline void multiply (T0 &a, const T1 &b) { combine(a, b, op_multiply <typename T0::value_type, typename T1::value_type>()); }
template <typename T0, typename T1> inline void divide   (T0 &a, const T1 &b) { combine(a, b, op_divide   <typename T0::value_type, typename T1::value_type>()); }

template <typename T0, typename T1, typename OutT> inline void subtract (const T0 &a, const T1 &b, OutT &c) { zip(a, b, c, op_subtract <typename T0::value_type, typename T1::value_type>()); }
template <typename T0, typename T1, typename OutT> inline void add      (const T0 &a, const T1 &b, OutT &c) { zip(a, b, c, op_add      <typename T0::value_type, typename T1::value_type>()); }
template <typename T0, typename T1, typename OutT> inline void multiply (const T0 &a, const T1 &b, OutT &c) { zip(a, b, c, op_multiply <typename T0::value_type, typename T1::value_type>()); }
template <typename T0, typename T1, typename OutT> inline void divide   (const T0 &a, const T1 &b, OutT &c) { zip(a, b, c, op_divide   <typename T0::value_type, typename T1::value_type>()); }

template <typename ForwardIterator, typename Compare>
size_t count_unique_on_sorted(ForwardIterator first, ForwardIterator last, Compare compare)
{
    if (last - first <= 0) return 0;
    size_t count = 1;
    for (auto i = first, v = first; i != last; ++i) {
        bool i_v = compare(*i, *v), v_i = compare(*v, *i);
        if (i_v || v_i) count++;
        v = i;
    }
    return count;
}

