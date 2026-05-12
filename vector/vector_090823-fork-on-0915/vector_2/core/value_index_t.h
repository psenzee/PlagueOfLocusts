#pragma once

#include <cmath>
#include <iostream>
#include <algorithm>
#include "stl.h"

// here we enforce the convention that in a pair, the first value is the float value and the second is the integer index
template <typename PairT> inline typename PairT::first_type  _value(const PairT &p) { return p.first; }
template <typename PairT> inline typename PairT::second_type _index(const PairT &p) { return p.second; }

template <typename ValueT, typename IndexT>
inline std::ostream &print(std::ostream &os, const std::pair<ValueT, IndexT> &p, const char *first_label = "value", const char *second_label = "index")
    { return os << "[" << first_label << " " << std::dec << p.first << " " << second_label << " " << p.second << "]"; }

// ------

template <typename PairT> struct _less_0        : public std::less   <typename PairT::first_type>  {};
template <typename PairT> struct _less_1        : public std::less   <typename PairT::second_type> {};
template <typename PairT> struct _greater_0     : public std::greater<typename PairT::first_type>  {};
template <typename PairT> struct _greater_1     : public std::greater<typename PairT::second_type> {};
template <typename PairT> struct _less_abs_0    : public less_abs    <typename PairT::first_type>  {};
template <typename PairT> struct _less_abs_1    : public less_abs    <typename PairT::second_type> {};
template <typename PairT> struct _greater_abs_0 : public greater_abs <typename PairT::first_type>  {};
template <typename PairT> struct _greater_abs_1 : public greater_abs <typename PairT::second_type> {};

template <typename PairT, typename CompareT> struct compare_0 : public compare_nth<PairT, 0, CompareT> {};
template <typename PairT, typename CompareT> struct compare_1 : public compare_nth<PairT, 1, CompareT> {};

template <typename PairT, typename Comp0, typename Comp1> struct compare_0_1 : public comparator_chain_bool< PairT, compare_nth<PairT, 0, Comp0>, compare_nth<PairT, 1, Comp1> > {};
template <typename PairT, typename Comp0, typename Comp1> struct compare_1_0 : public comparator_chain_bool< PairT, compare_nth<PairT, 1, Comp0>, compare_nth<PairT, 0, Comp1> > {};

template <typename PairT> struct compare_0u     : public compare_0  < PairT, _less_0<PairT> > {};
template <typename PairT> struct compare_0d     : public compare_0  < PairT, _greater_0<PairT> > {};
template <typename PairT> struct compare_1u     : public compare_1  < PairT, _less_1<PairT> > {};
template <typename PairT> struct compare_1d     : public compare_1  < PairT, _greater_1<PairT> > {};
template <typename PairT> struct compare_0u_1u  : public compare_0_1< PairT, _less_0<PairT>, _less_1<PairT> > {};
template <typename PairT> struct compare_0u_1d  : public compare_0_1< PairT, _less_0<PairT>, _greater_1<PairT> > {};
template <typename PairT> struct compare_0d_1u  : public compare_0_1< PairT, _greater_0<PairT>, _less_1<PairT> > {};
template <typename PairT> struct compare_0d_1d  : public compare_0_1< PairT, _greater_0<PairT>, _greater_1<PairT> > {};
template <typename PairT> struct compare_1u_0u  : public compare_1_0< PairT, _less_1<PairT>, _less_0<PairT> > {};
template <typename PairT> struct compare_1u_0d  : public compare_1_0< PairT, _less_1<PairT>, _greater_0<PairT> > {};
template <typename PairT> struct compare_1d_0u  : public compare_1_0< PairT, _greater_1<PairT>, _less_0<PairT> > {};
template <typename PairT> struct compare_1d_0d  : public compare_1_0< PairT, _greater_1<PairT>, _greater_0<PairT> > {};
template <typename PairT> struct compare_0au    : public compare_0  < PairT, _less_abs_0<PairT> > {};
template <typename PairT> struct compare_0ad    : public compare_0  < PairT, _greater_abs_0<PairT> > {};

template <typename PairT> struct compare_0au_1u : public compare_0_1< PairT, _less_abs_0<PairT>, _less_1<PairT> > {};
template <typename PairT> struct compare_0au_1d : public compare_0_1< PairT, _less_abs_0<PairT>, _greater_1<PairT> > {};
template <typename PairT> struct compare_0ad_1u : public compare_0_1< PairT, _greater_abs_0<PairT>, _less_1<PairT> > {};
template <typename PairT> struct compare_0ad_1d : public compare_0_1< PairT, _greater_abs_0<PairT>, _greater_1<PairT> > {};

// given a container with a size_t index, create a
template <typename IteratorInT, typename IteratorOutT>
size_t fill_value_index(IteratorInT first, IteratorInT last, IteratorOutT out, size_t initial_index = 0)
{
    //typedef typename IteratorOutT::value_type pair_t;
    size_t index = 0;
    for (auto i = first; i != last; ++i) *out++ = (typename IteratorOutT::value_type(*i, (index++) + initial_index));
}
