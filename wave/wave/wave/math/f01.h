#pragma once

#include <cstddef>
#include "pow_ui.h"
#include "core_math.h"
#include "functors.h"

namespace {

template <typename T, typename U> constexpr T _catmull_rom(const T &p0, const T &p1, const T &p2, const T &p3, const U &t)
{
    return T(0.5) * ((2 * p1) + (-p0 + p2) * t + (2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t + (-p0 + 3 * p1 - 3 * p2 + p3) * t * t * t);
}

template <typename T, typename U> constexpr T _catmull_rom_01(const T &p0, const T &p3, const U &t)
{
    return T(0.5) * t * ((1 - p0) + t * ((p0 + p0 - p3 + 4) + (p3 - p0 - 3) * t));
}
}

namespace f01
{

// these functors map a value from 0-1 to another value presumably (though not necessarily) in the same range,
// however, the constraints are that fn(0) = 0 and fn(1) = 1 so that continuous effects can be made here

template <typename T>             struct linear               { inline constexpr T operator()(const T &a) const { return a; } };
template <typename T, unsigned E> struct power_ui             { inline constexpr T operator()(const T &a) const { return pow_ui<E>(a); } };
template <typename T, unsigned E> struct inv_power_ui         { inline constexpr T operator()(const T &a) const { T v(1 - a); return 1 - pow_ui<E>(v); } };
template <typename T>             struct sinusoid             { inline constexpr T operator()(const T &a) const { return sin(a * math::PI * 0.5); } };
template <typename T>             struct inv_sinusoid         { inline constexpr T operator()(const T &a) const { return (2 / math::PI) * asin(a); } };
template <typename T>             struct sine_smooth_step     { inline constexpr T operator()(const T &a) const { return 0.5 - cos(a * math::PI) * 0.5; } };
template <typename T>             struct s_curve_cubic        { inline constexpr T operator()(const T &a) const { return (a * a * (3 - 2 * a)); } };
template <typename T>             struct inv_s_curve_cubic    { inline constexpr T operator()(const T &a) const { return 0.5 - sin(asin(1 - 2 * a) / 3); } };
template <typename T>             struct s_curve_quintic      { inline constexpr T operator()(const T &a) const { T a3 = a * a * a, a4 = a3 * a, a5 = a4 * a; return (T(6) * a5) - (T(15) * a4) + (T(10) * a3); } };
template <typename T>             struct power                { T _e;     inline power(T e)            : _e(e)        {} inline constexpr T operator()(const T &a) const { return pow(a, _e); } };
template <typename T>             struct inv_power            { T _e;     inline inv_power(T e)        : _e(e)        {} inline constexpr T operator()(const T &a) const { return T(1) - pow(T(1) - a, _e); } };
template <typename T>             struct catmull_rom          { T _p, _q; inline catmull_rom(T p, T q) : _p(p), _q(q) {} inline constexpr T operator()(const T &a) const { return _catmull_rom_01(_p, _q, a); } };
template <typename T>             struct sigmoid              { T _e;     inline sigmoid(T e = 15)     : _e(e)        {} inline constexpr T operator()(const T &a) const { T d(1 + exp(-_e * (a - 0.5))); return 1 / d; } };
template <typename T>             struct smooth_step          : public s_curve_cubic<T>                           {};
template <typename T>             struct smoother_step        : public s_curve_quintic<T>                         {};
template <typename T>             struct inv_smooth_step      : public inv_s_curve_cubic<T>                       {};
template <typename T>             struct sq                   : public power_ui<T, 2>                             {}; // { inline constexpr T operator()(const T &a) const { return a * a; } };
template <typename T>             struct inv_sq               : public inv_power_ui<T, 2>                         {}; // { inline constexpr T operator()(const T &a) const { T v(T(1) - a); return T(1) - v * v; } }
template <typename T>             struct cubed                : public power_ui<T, 3>                             {}; // { inline constexpr T operator()(const T &a) const { return a * a * a; } };
template <typename T>             struct inv_cubed            : public inv_power_ui<T, 3>                         {}; // { inline constexpr T operator()(const T &a) const { T v(T(1) - a); return T(1) - v * v * v; } }

// virtual
template <typename T>             struct linear_fn            : public unary_wrapper<T, T, linear<T> >            {};
template <typename T, unsigned E> struct power_ui_fn          : public unary_wrapper<T, T, power_ui<T, E> >       {};
template <typename T, unsigned E> struct inv_power_ui_fn      : public unary_wrapper<T, T, inv_power_ui<T, E> >   {};
template <typename T>             struct sinusoid_fn          : public unary_wrapper<T, T, sinusoid<T> >          {};
template <typename T>             struct inv_sinusoid_fn      : public unary_wrapper<T, T, inv_sinusoid<T> >      {};
template <typename T>             struct sine_smooth_step_fn  : public unary_wrapper<T, T, sine_smooth_step<T> >  {};
template <typename T>             struct s_curve_cubic_fn     : public unary_wrapper<T, T, s_curve_cubic<T> >     {};
template <typename T>             struct inv_s_curve_cubic_fn : public unary_wrapper<T, T, inv_s_curve_cubic<T> > {};
template <typename T>             struct s_curve_quintic_fn   : public unary_wrapper<T, T, s_curve_quintic<T> >   {};
template <typename T>             struct power_fn             : public unary_wrapper<T, T, power<T> >             {};
template <typename T>             struct inv_power_fn         : public unary_wrapper<T, T, inv_power<T> >         {};
template <typename T>             struct catmull_rom_fn       : public unary_wrapper<T, T, catmull_rom<T> >       {};
template <typename T>             struct sigmoid_fn           : public unary_wrapper<T, T, sigmoid<T> >           {};
template <typename T>             struct smooth_step_fn       : public unary_wrapper<T, T, smooth_step<T> >       {};
template <typename T>             struct smoother_step_fn     : public unary_wrapper<T, T, smoother_step<T> >     {};
template <typename T>             struct inv_smooth_step_fn   : public unary_wrapper<T, T, inv_smooth_step<T> >   {};
template <typename T>             struct sq_fn                : public unary_wrapper<T, T, sq<T> >                {};
template <typename T>             struct inv_sq_fn            : public unary_wrapper<T, T, inv_sq<T> >            {};
template <typename T>             struct cubed_fn             : public unary_wrapper<T, T, cubed<T> >             {};
template <typename T>             struct inv_cubed_fn         : public unary_wrapper<T, T, inv_cubed<T> >         {};

template <typename T, typename F, typename G>
struct blend {
    F _f; G _g;
    constexpr blend() {}
    constexpr blend(const F &f, const G &g) : _f(f), _g(g) {}
    constexpr T operator()(const T &a) const { return lerp(_f(a), _g(a), a); }
};

template <typename T, typename F, typename G> struct blend_fn : public unary_wrapper< T, T, blend<T, F, G> > {};

template <typename T, typename F, typename G>
struct inv_blend {
    F _f; G _g;
    constexpr inv_blend() {}
    constexpr inv_blend(const F &f, const G &g) : _f(f), _g(g) {}
    constexpr T operator()(const T &a) const { return lerp(_f(a), _g(a), T(1) - a); }
};

template <typename T, typename F, typename G> struct inv_blend_fn : public unary_wrapper< T, T, inv_blend<T, F, G> > {};

template <typename T, typename F, typename G>
struct multiply {
    F _f; G _g;
    constexpr multiply() {}
    constexpr multiply(const F &f, const G &g) : _f(f), _g(g) {}
    constexpr T operator()(const T &a) const { return _f(a) * _g(a); }
};

template <typename T, typename F, typename G> struct multiply_fn : public unary_wrapper< T, T, multiply_fn<T, F, G> > {};

template <typename T, typename F, typename G>
struct divide {
    F _f; G _g;
    constexpr divide() {}
    constexpr divide(const F &f, const G &g) : _f(f), _g(g) {}
    constexpr T operator()(const T &a) const { T g(_g(a)); return g == T(0) ? T(0) : (_f(a) / g); }
};

template <typename T, typename F, typename G> struct divide_fn : public unary_wrapper< T, T, divide_fn<T, F, G> > {};

template <typename T, typename F, typename G>
struct mean {
    F _f; G _g;
    constexpr mean() {}
    constexpr mean(const F &f, const G &g) : _f(f), _g(g) {}
    constexpr T operator()(const T &a) const { return (_f(a) + _g(a)) * T(0.5); }
};

template <typename T, typename F, typename G> struct mean_fn : public unary_wrapper< T, T, mean_fn<T, F, G> > {};

template <typename T, typename F>
std::ostream &format_example(std::ostream &os, const F &f, size_t w = 40, size_t h = 20, size_t samples = 1000)
{
    double sd(samples);
    bool *data = new bool [w * h];
    memset(data, 0, sizeof(bool) * w * h);
    if (f(0) < 1.0 / sd) {
        data[0] = true;
    }
    for (size_t i = 1; i < samples; i++) {
        double v = f(i / sd);
        data[int((i / sd) * w) + int(v * h) * w] = true;
    }
    os << "// +";
    for (size_t x = 0; x < w; x++) {
        os << "-";
    }
    os << "+" << std::endl;
    for (int y = int(h) - 1; y >= 0; y--) {
        os << "// " << "|";
        for (size_t x = 0; x < w; x++) {
            os << (data[x + y * w] ? "*" : " ");
        }
        os << "|" << std::endl;
    }
    os << "// +";
    for (size_t x = 0; x < w; x++) {
        os << "-";
    }
    return os << "+" << std::endl;
}

}
