#include "Formulas.h"

#include <stddef.h>
#include <stdint.h>
#include <float.h>
#include <vector>
#include <math.h>

#include "Formula.h"
#include "FormulaImpl.h"

using namespace formula;

range_t Formulas::range(const Formula *formula, size_t samples)
{
    range_t r;
    double interval = 1.0 / samples;
    for (size_t i = 0; i <= samples; i++) {
        r.insert(formula->at(i * interval));
    }
    return r;
}

const Formula *Formulas::constant(double constant) { return new Constant(constant); }
const Formula *Formulas::const_pi()                { return &Constant::CONSTANT_PI; }
const Formula *Formulas::const_e()                 { return &Constant::CONSTANT_E; }
const Formula *Formulas::const_0()                 { return &Constant::CONSTANT_0; }
const Formula *Formulas::const_1()                 { return &Constant::CONSTANT_POS1; }
const Formula *Formulas::const_neg1()              { return &Constant::CONSTANT_NEG1; }

const Formula *Formulas::normalize(const Formula *f)
{
    return new Normalize(f);
}

const Formula *Formulas::clamp(const Formula *f, const range_t &r)
{
    return new Clamp(f, r);
}
/*
const Formula *Formulas::add(const Formula *a, const Formula *b)
{
    return new Accumulate(_add, a, b);
}

const Formula *Formulas::sub(const Formula *a, const Formula *b)
{
    return new Accumulate(_sub, a, b);
}

const Formula *Formulas::mul(const Formula *a, const Formula *b)
{
    return new Accumulate(_mul, a, b);
}

const Formula *Formulas::div(const Formula *a, const Formula *b)
{
    return new Accumulate(_div, a, b);
}
*/
static double _sinc(double t)  { return t == 0.0 ? 1.0 : (sin(t) / t); }
static double _sincn(double t) { return _sinc(t * MATH_PI); }
static double _sinc_(double t[1])  { return t[0] == 0.0 ? 1.0 : (sin(t[0]) / t[0]); }
