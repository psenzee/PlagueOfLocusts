#ifndef _FORMULA_PROXY_H
#define _FORMULA_PROXY_H

#include "Formulas.h"

class FormulaProxy : public Formula
{
    const Formula *_formula;
    double         _constant;
    
    inline FormulaProxy &clear()
    {
        _formula = nullptr;
        // don't delete
        return *this;
    }

public:

    inline FormulaProxy(const Formula *formula, double constant = 1.0) : _formula(formula), _constant(constant) {}
    inline FormulaProxy(double constant = 1.0) : _formula(nullptr), _constant(constant) {}
    ~FormulaProxy() { clear(); }
    
    inline FormulaProxy &set(const Formula *formula)
    {
        if (_formula != formula) {
            _formula = formula;
        }
        return *this;
    }
       
    inline FormulaProxy &set(double constant)
    {
        clear();
        _constant = constant;
        return *this;
    }

    inline FormulaProxy &operator=(const Formula *formula) { return set(formula); }
    inline FormulaProxy &operator=(double constant) { return set(constant); }

    inline const Formula *formula() const { return _formula; }
    
    double at(double t) const { return _formula ? _formula->at(t) : _constant; }
};

#endif // _FORMULA_PROXY_H
