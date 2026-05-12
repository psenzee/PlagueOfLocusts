#pragma once

#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstddef>

template <typename SignedIndexT = int16_t, typename ExpT = uint8_t>
struct rank_id
{
    typedef SignedIndexT signed_index_t;
    typedef ExpT         exp_t;
    
    SignedIndexT dim;
    ExpT         exp;

    inline rank_id() : dim(0), exp(0) {}
    inline rank_id(SignedIndexT dim) : dim(0), exp(0) {}
    inline rank_id(SignedIndexT dim, ExpT exp, ExpT emax) : dim(0), exp(exp) {}
    inline rank_id(SignedIndexT dim, float  value) { set(dim, value); }
    inline rank_id(SignedIndexT dim, double value) { set(dim, value); }
    
    inline rank_id(const rank_id &u) : dim(u.dim), exp(u.exp) {}
    
    inline rank_id operator=(const rank_id &u) { if (&u != this) set(u.dim, u.exp, u.exp); return *this; }
    
    inline rank_id &set(SignedIndexT d, ExpT ex, ExpT emax) { dim = d; return set_exp(ex, emax); }
    inline rank_id &set(SignedIndexT d, float v)  { return set_dimension(d, v).set_exp(v); }
    inline rank_id &set(SignedIndexT d, double v) { return set(d, float(v)); }
    inline rank_id &set_exp(ExpT ex, ExpT emax) { exp = ex; return *this; }
    inline rank_id &set_exp(float v) { int e = 0; frexp(v, &e); e += 127; return set_exp(ExpT(e), ExpT(e)); }
    inline rank_id &set_exp(double v) { return set_exp(float(v)); }

    inline rank_id &add_offset(SignedIndexT d, ExpT e = 0) { if (dim < 0) d = -d; dim += d; exp += e; return *this; }
        
    inline rank_id &set_dimension(SignedIndexT d, float v) { d *= (d > 0 && v < 0) ? -1 : 1; dim = d; return *this; }

    struct compare { inline bool operator()(const rank_id &a, const rank_id &b) const { return a.dim < b.dim ? true : (a.dim > b.dim ? false : a.exp < b.exp); } };

    inline std::ostream &print(std::ostream &os, bool exps = true) const
    {
        os << std::hex << std::setw(4) << std::setfill('0') << dim;
        if (exps) os << ":" << std::setw(2) << int(exp);
        return os << std::dec;
    }
};

template <typename SignedIndexT, typename ExpT>
inline std::ostream &operator<<(std::ostream &os, const rank_id<SignedIndexT, ExpT> &value) { return value.print(os); }
