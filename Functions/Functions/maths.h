#pragma once

#include <stddef.h>
#include <stdint.h>
#include <float.h>
#include <vector>
#include <math.h>

#define _MATH_PI (3.14159265358)
#define _MATH_E  (2.71828182846)

static constexpr const double MATH_PI = _MATH_PI;
static constexpr const double MATH_E  = _MATH_E;

struct range_t
{
    double lo, hi;
    inline range_t(double lo = DBL_MAX, double hi = -DBL_MAX) : lo(lo), hi(hi) {}

    inline double        sign()            const { return _sn(diff()); }
    inline double        diff()            const { return hi - lo; }
    inline bool          valid()           const { return hi >= lo; }
    inline range_t      &swap()                  { double t = hi; hi = lo; lo = t; return *this; }
    inline range_t      &normalize()             { if (!valid()) swap(); return *this; }
    inline double        magnitude()       const { return ::abs(diff()); }
    inline double        max()             const { return _mx(lo, hi); }
    inline double        min()             const { return _mn(lo, hi); }
    inline double        center()          const { return diff() * 0.5; }
    inline double        clamp(double v)   const { return _mn(hi, _mx(lo, v)); }
    inline bool          inside(double v)  const { return (v >= lo && v <= hi); }
    inline range_t      &insert(double v)        { hi = _mx(v, hi); lo = _mn(v, lo); return *this; }
    
private:
    
    static inline double _mx(double a, double b) { return b >= a ? b : a; }
    static inline double _mn(double a, double b) { return a <= b ? a : b; }
    static inline double _sn(double d)           { return d == 0.0 ? 0.0 : (d > 0.0 ? 1.0 : (d < 0.0 ? -1.0 : NAN)); }
};

struct xform1d_t
{
    double scale, translate;

    inline xform1d_t(double scale = 1.0, double translate = 0.0) : scale(scale), translate(translate) {}

    inline double apply(double value)   const { return value * scale + translate; }
    inline double unapply(double value) const { return (value - translate) / scale; }
    inline bool   isidentity()          const { return scale == 1.0 && translate == 0.0; }
};

struct fn_xform1d_t
{
    xform1d_t domain, range;
    inline fn_xform1d_t(const xform1d_t &domain = xform1d_t(), const xform1d_t &range = xform1d_t()) : domain(domain), range(range) {}
};
