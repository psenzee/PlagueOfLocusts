#pragma once

#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstddef>

template <typename IndexT = int16_t, typename ExpT = uint8_t>
struct dim_exp
{
    typedef IndexT index_t;
    typedef ExpT   exp_t;

    index_t dim;
    exp_t   exp;

    struct compare {
        inline bool operator()(const dim_exp &a, const dim_exp &b) const
            { return a.dim < b.dim ? true : (a.dim > b.dim ? false : a.exp < b.exp); }
    };

    inline dim_exp() : dim(0), exp(0) {}
    inline dim_exp(index_t dim) : dim(0), exp(0) {}
    inline dim_exp(index_t dim, exp_t exp) : dim(0), exp(exp) {}
    inline dim_exp(index_t dim, float value) { set(dim, value); }
    inline dim_exp(index_t dim, double value) { set(dim, value); }

    inline dim_exp(const dim_exp &u) : dim(u.dim), exp(u.exp) {}

    inline dim_exp operator=(const dim_exp &u) { if (&u != this) set(u.dim, u.exp, u.exp); return *this; }

    inline dim_exp &set(index_t d, exp_t ex) { dim = d; return set_exp(ex); }
    inline dim_exp &set(index_t d, float v)  { return set_dimension(d, v).set_exp(v); }
    inline dim_exp &set(index_t d, double v) { return set(d, float(v)); }
    inline dim_exp &set_exp(exp_t ex) { exp = ex; return *this; }
    inline dim_exp &set_exp(float v)  { int e = 0; frexp(v, &e); e += 127; return set_exp(exp_t(e), exp_t(e)); }
    inline dim_exp &set_exp(double v) { return set_exp(float(v)); }

    inline dim_exp &add(index_t d, exp_t e = 0) { if (dim < 0) d = -d; dim += d; exp += e; return *this; }

    inline dim_exp &set_dimension(index_t d, float v)  { d *= (d > 0 && v < 0) ? -1 : 1; dim = d; return *this; }
    inline dim_exp &set_dimension(index_t d, double v) { return set_dimension(d, float(v)); }

    std::ostream &print(std::ostream &os) const { return os << std::hex << std::setw(4) << std::setfill('0') << dim << ":" << std::setw(2) << int(exp) << std::dec; }
};

template <typename IndexT, typename ExpT> inline std::ostream &operator<<(std::ostream &os, const dim_exp<IndexT, ExpT> &value) { return value.print(os); }
