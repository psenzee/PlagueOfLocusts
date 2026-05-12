#pragma once

#include <algorithm>
#include "comparators.h"

template <typename Pair> struct _pair_less_first         : public std::less    <typename Pair::first_type>  {};
template <typename Pair> struct _pair_less_second        : public std::less    <typename Pair::second_type> {};
template <typename Pair> struct _pair_greater_first      : public std::greater <typename Pair::first_type>  {};
template <typename Pair> struct _pair_greater_second     : public std::greater <typename Pair::second_type> {};
template <typename Pair> struct _pair_less_abs_first     : public less_abs     <typename Pair::first_type>  {};
template <typename Pair> struct _pair_less_abs_second    : public less_abs     <typename Pair::second_type> {};
template <typename Pair> struct _pair_greater_abs_first  : public greater_abs  <typename Pair::first_type>  {};
template <typename Pair> struct _pair_greater_abs_second : public greater_abs  <typename Pair::second_type> {};

template <typename Pair, typename C> struct pair_compare_first  : public compare_nth<Pair, 0, C> {};
template <typename Pair, typename C> struct pair_compare_second : public compare_nth<Pair, 1, C> {};

template <typename Pair, typename CA, typename CB> struct pair_compare_first_second : public comparator_chain_bool< Pair, compare_nth<Pair, 0, CA>, compare_nth<Pair, 1, CB> > {};
template <typename Pair, typename CA, typename CB> struct pair_compare_second_first : public comparator_chain_bool< Pair, compare_nth<Pair, 1, CA>, compare_nth<Pair, 0, CB> > {};

template <typename Pair> struct pair_compare_first_less                       : public pair_compare_first        < Pair, _pair_less_first<Pair> > {};
template <typename Pair> struct pair_compare_first_greater                    : public pair_compare_first        < Pair, _pair_greater_first<Pair> > {};
template <typename Pair> struct pair_compare_second_less                      : public pair_compare_second       < Pair, _pair_less_second<Pair> > {};
template <typename Pair> struct pair_compare_second_greater                   : public pair_compare_second       < Pair, _pair_greater_second<Pair> > {};
template <typename Pair> struct pair_compare_first_less_second_less           : public pair_compare_first_second < Pair, _pair_less_first<Pair>, _pair_less_second<Pair> > {};
template <typename Pair> struct pair_compare_first_less_second_greater        : public pair_compare_first_second < Pair, _pair_less_first<Pair>, _pair_greater_second<Pair> > {};
template <typename Pair> struct pair_compare_first_greater_second_less        : public pair_compare_first_second < Pair, _pair_greater_first<Pair>, _pair_less_second<Pair> > {};
template <typename Pair> struct pair_compare_first_greater_second_greater     : public pair_compare_first_second < Pair, _pair_greater_first<Pair>, _pair_greater_second<Pair> > {};
template <typename Pair> struct pair_compare_second_less_first_less           : public pair_compare_second_first < Pair, _pair_less_second<Pair>, _pair_less_first<Pair> > {};
template <typename Pair> struct pair_compare_second_less_first_greater        : public pair_compare_second_first < Pair, _pair_less_second<Pair>, _pair_greater_first<Pair> > {};
template <typename Pair> struct pair_compare_second_greater_first_less        : public pair_compare_second_first < Pair, _pair_greater_second<Pair>, _pair_less_first<Pair> > {};
template <typename Pair> struct pair_compare_second_greater_first_greater     : public pair_compare_second_first < Pair, _pair_greater_second<Pair>, _pair_greater_first<Pair> > {};
template <typename Pair> struct pair_compare_first_less_abs                   : public pair_compare_first        < Pair, _pair_less_abs_first<Pair> > {};
template <typename Pair> struct pair_compare_first_greater_abs                : public pair_compare_first        < Pair, _pair_greater_abs_first<Pair> > {};

template <typename Pair> struct pair_compare_first_less_abs_second_less       : public pair_compare_first_second < Pair, _pair_less_abs_first<Pair>, _pair_less_second<Pair> > {};
template <typename Pair> struct pair_compare_first_less_abs_second_greater    : public pair_compare_first_second < Pair, _pair_less_abs_first<Pair>, _pair_greater_second<Pair> > {};
template <typename Pair> struct pair_compare_first_greater_abs_second_less    : public pair_compare_first_second < Pair, _pair_greater_abs_first<Pair>, _pair_less_second<Pair> > {};
template <typename Pair> struct pair_compare_first_greater_abs_second_greater : public pair_compare_first_second < Pair, _pair_greater_abs_first<Pair>, _pair_greater_second<Pair> > {};
