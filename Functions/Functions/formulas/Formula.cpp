#include <stddef.h>
#include <stdint.h>
#include <float.h>
#include <vector>
#include <math.h>

#include "Formula.h"
#include "FormulaImpl.h"
#include "Formulas.h"

Formula::~Formula() {}

Formula::Formula()
{
    //formula::FormulaRegistry::instance()->add(this);
}

namespace formula {

const Identity Identity::IDENTITY = Identity();

const Constant Constant::CONSTANT_0    = Constant(0);
const Constant Constant::CONSTANT_POS1 = Constant(1);
const Constant Constant::CONSTANT_NEG1 = Constant(-1);
const Constant Constant::CONSTANT_PI   = Constant(MATH_PI);
const Constant Constant::CONSTANT_E    = Constant(MATH_E);

Registry *Registry::_instance = nullptr;

Registry *Registry::instance()
{
    if (!_instance) { _instance = new Registry(); }
    return _instance;
}

void Registry::add(const Formula *formula)
{
    if (formula) {
        for (std::vector<const Formula *>::const_iterator i = _list.begin(), e = _list.end(); i != e; ++i) {
            if (*i == formula) {
                return;
            }
        }
        _list.push_back(formula);
    }
}

void Registry::destroy()
{
    for (std::vector<const Formula *>::const_iterator i = _list.begin(), e = _list.end(); i != e; ++i) {
        delete *i;
    }
}

Composed::Composed() : Formula()
{
}

Composed::Composed(const formula_list_t &formulas) : Formula(), _formulas(formulas)
{
}

Composed::Composed(const Formula **formulas, size_t count) : Formula()
{
    add(formulas, count);
}

Composed::Composed(
    const Formula *a, const Formula *b, const Formula *c, const Formula *d,
    const Formula *e, const Formula *f, const Formula *g, const Formula *h
) : Formula()
{
    const Formula *fs[] = { a, b, c, d, e, f, g, h };
    add(fs, 8);
}

Composed::Composed(
    const Formula *fs[4]
) : Formula()
{
//    const Formula *fs[] = { a, b, c, d, e, f, g, h };
    add(fs, 4);
}

const formula_list_t &Composed::formulas() const
{
    return _formulas;
}

const Formula *Composed::formula(size_t i) const
{
    const Formula *f = nullptr;
    if (i < _formulas.size()) {
        f = _formulas[i];
    }
    return f ? f : &Identity::IDENTITY;
}

size_t Composed::size() const
{
    return _formulas.size();
}

Composed &Composed::add(const Formula *f)
{
    if (f) {
        _formulas.push_back(f);
    }
    return *this;
}

Composed &Composed::add(const Formula **f, size_t count)
{
    for (size_t i = 0; i < count; i++) {
        add(f[i]);
    }
    return *this;
}

double Accumulate::at(double t) const
{
    size_t sz = size();
    if (!sz) {
        return t;
    }
    double accumulate = formula(0)->at(t);
    for (size_t i = 1; i < sz; i++) {
        accumulate = _fn(accumulate, formula(i)->at(t));
    }
    return accumulate;
}

Normalize::Normalize(const Formula *formula, size_t samples) : Composed(formula)
{
    range_t range = Formulas::range(formula, samples);
    _magnitude = std::max(fabs(range.lo), fabs(range.hi));
    _factor = 1.0 / _magnitude;
}

double Normalize::at(double t) const
{
    return formula(0)->at(t) * _factor;
}

double HardLimit::at(double t) const
{
    if (t < 0.0 || t > 1.0) {
        return 0.0;
    }
    return std::clamp(formula(0)->at(t), -1.0, 1.0);
}

}
