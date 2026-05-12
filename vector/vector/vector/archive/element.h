#pragma once

#include <iostream>
#include <iomanip>
#include <array>

template <typename FloatT, typename IndexT>
struct real_at_interface
{
    inline FloatT at(IndexT index) const { return FloatT(0); }
};

template <typename FloatT, typename IndexT, int N>
struct vector_at_interface
{
    inline std::array<FloatT, N> at(IndexT index) const { return std::array<FloatT, N>(); }
};

template <typename FloatT, typename IndexT, int N>
struct vector_at_ref_interface
{
    std::array<FloatT, N> _v;
    inline const std::array<FloatT, N> &at(IndexT index) const { return _v; }
};

template <typename FloatT, typename IndexT>
struct element
{
    typedef element<FloatT, IndexT> self_t;
    
    IndexT index;
    FloatT value;
    
    inline element(IndexT index = 0, FloatT value = FloatT(0)) : index(index), value(value) {}
    inline std::ostream &print(std::ostream &os) const { return os << index << ":" << value; }
    
    struct compare         { inline bool operator()(const self_t &a, const self_t &b) const { return a.value < b.value; } };
    struct compare_reverse { inline bool operator()(const self_t &a, const self_t &b) const { return a.value > b.value; } };
    struct compare_abs     { inline bool operator()(const self_t &a, const self_t &b) const { return fabs(a.value) < fabs(b.value); } };
};

template <typename FloatT, typename IndexT>
inline std::ostream &operator<<(std::ostream &os, const element<FloatT, IndexT> &e) { return e.print(os); }
