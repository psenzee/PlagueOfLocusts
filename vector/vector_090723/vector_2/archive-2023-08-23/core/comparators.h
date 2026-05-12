#pragma once

#include <algorithm>

template <typename Value, typename Compare0, typename Compare1>
struct compare_chain
{
    Compare0 _c0;
    Compare1 _c1;
    inline compare_chain() {}
    inline compare_chain(const Compare0 &c0, const Compare1 &c1) : _c0(c0), _c1(c1) {}
    inline bool operator()(const Value &a, const Value &b) const { if (_c0(a, b)) return true; return (!_c0(b, a) ? _c1(a, b) : false); }
};

template <typename T> struct less_abs    { inline bool operator()(const T &a, const T &b) const { return abs(a) < abs(b); } };
template <typename T> struct greater_abs { inline bool operator()(const T &a, const T &b) const { return abs(a) > abs(b); } };

template <typename T, typename Compare> struct compare_abs { Compare _c; inline bool operator()(const T &a, const T &b) const { return _c(abs(a), abs(b)); } };

template <typename TupleOrPair, int N, typename Compare>
struct compare_nth { Compare _c; inline bool operator()(const TupleOrPair &a, const TupleOrPair &b) const { return _c(std::get<N>(a), std::get<N>(b)); } };
