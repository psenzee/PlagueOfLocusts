#pragma once

#include <cstddef>

template <typename T, typename U>
struct unary_functor
{
    virtual ~unary_functor();
    virtual T operator()(const U &) const = 0;
};

template <typename T, typename U>
unary_functor<T, U>::~unary_functor() {}

template <typename T, typename U, typename F>
struct unary_wrapper : public unary_functor<T, U>
{
    F _f;
    unary_wrapper() {}
    unary_wrapper(const F &f) : _f(f) {}
    virtual T operator()(const U &) const = 0;
};
