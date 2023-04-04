#pragma once

#include "Formula.h"

typedef std::vector<const Formula *> formula_list_t;

namespace formula
{

class Registry
{
public:

    static Registry *instance();

    void add(const Formula *formula);
    void destroy();
    
private:
    
    formula_list_t _list;

    static Registry *_instance;
};

class Identity : public Formula
{
public:
    double at(double t) const { return t; }
    
    static const Identity IDENTITY;
private:
    inline Identity() {}
};

class Constant : public Formula
{
    double _constant;
public:
    inline Constant(double constant) : Formula(), _constant(constant) {}
    double at(double t) const { return _constant; }

    static const Constant CONSTANT_POS1;
    static const Constant CONSTANT_0;
    static const Constant CONSTANT_PI;
    static const Constant CONSTANT_NEG1;
    static const Constant CONSTANT_E;
};

struct Composed : public Formula
{
    Composed();
    Composed(const formula_list_t &formulas);
    Composed(const Formula **a, size_t count);
    Composed(const Formula *a[4]);
    Composed(const Formula *a = nullptr, const Formula *b = nullptr, const Formula *c = nullptr, const Formula *d = nullptr,
             const Formula *e = nullptr, const Formula *f = nullptr, const Formula *g = nullptr, const Formula *h = nullptr);
    Composed &add(const Formula *f);
    Composed &add(const Formula **f, size_t count);
    
    const formula_list_t &formulas() const;
    const Formula *formula(size_t index) const;
    size_t size() const;

private:
    formula_list_t _formulas;
};

struct Transform : public Composed
{
    Transform(const Formula *f, const xform1d_t &t = xform1d_t()) : Composed(f), _transform(t) {}
    double at(double t) const { return _transform.apply(formula(0)->at(t)); }
private:
    xform1d_t _transform;
};

class Accumulate : public Composed
{
    double (*_fn)(double, double);
public:
    Accumulate(double (*fn)(double, double), const formula_list_t &formulas) : Composed(formulas), _fn(fn) {}
    Accumulate(double (*fn)(double, double), const Formula **formulas, size_t count) : Composed(formulas, count), _fn(fn) {}
    Accumulate(double (*fn)(double, double),
               const Formula *a = nullptr, const Formula *b = nullptr, const Formula *c = nullptr, const Formula *d = nullptr,
               const Formula *e = nullptr, const Formula *f = nullptr, const Formula *g = nullptr, const Formula *h = nullptr)
        : Composed(a, b, c, d, e, f, g, h), _fn(fn) {}
    double at(double t) const;
};

class Clamp : public Composed
{
    range_t _range;
public:
    inline Clamp(const Formula *formula, const range_t &r) : Composed(formula), _range(r) {}
    double at(double t) const { return _range.clamp(formula(0)->at(t)); }
};

struct HardLimit : public Composed
{
    inline HardLimit(const Formula *formula) : Composed(formula) {}
    double at(double t) const;
};

struct Normalize : public Composed
{
    Normalize(const Formula *formula, size_t samples = 1000);
    double at(double t) const;

private:

    double _magnitude;
    double _factor;
};

/*
 
class SmoothFormula (Formula):

def __init__(self, formula, smoothing_samples = 9):
 self.formula = formula
 self.samples = [0.0] * smoothing_samples
 self.prev_x = -1.0;
 
def reset(self):
 self.samples = [0.0] * len(self.samples)
 self.prev_x = -1.0;
 return self

def value(self, x):
 if x < self.prev_x:
   self.reset()
 y = self.formula.value(x)
 self.samples.append(y)
 self.samples.pop(0)
 self.prev_x = x
 return sum(self.samples) / len(self.samples)


class ShapeFormula (Formula):

def __init__(self, exp = (10, 10), yscale = (1.0, 1.0), xscale = (1.0, 1.0), xtrans = (0, 0)):
 self.exp = exp
 self.yscale = yscale
 self.xscale = xscale
 self.xtrans = xtrans

def value(self, x):
 (fexp, rexp) = self.exp
 (fys, rys) = self.yscale
 (fxs, rxs) = self.xscale
 (fxt, rxt) = self.xtrans
 return (math.pow(1 - ((x + fxt) * fxs), fexp) * fys + math.pow(((x + rxt) * rxs), rexp) * rys)

@staticmethod
def formula(name):
 named = {
   'ring-vibrato' : ((10.0, 10.0), (1.0,  1.0), (1.0,  1.0), (0.0,   0.0)),
   'ring'         : ((10.0, 10.0), (1.0,  1.0), (1.0,  1.0), (0.0,  -1.0)),
   'vibrato'      : ((10.0, 10.0), (1.0,  1.0), (1.0,  1.0), (1.0,   0.0)),
   'boing'        : ((10.0, 10.0), (0.4, -0.8), (1.6, -0.8), (0.35, 0.35)),
 }
 if name in named:
   (exp, yscale, xscale, xtrans) = named[name]
   return ShapeFormula(exp, yscale, xscale, xtrans)
 return None
 
class RandomAmplitudeFormula (Formula):

def __init__(self, formula, min = 0.5, max = 1.0):
 self.formula = formula
 self.amplitude = random.uniform(min, max)

def value(self, x):
 return self.formula.value(x) * self.amplitude

 
-------
 
 
 
class Formulas (object):

@staticmethod
def formula(name, min_amplitude = 0.5, max_amplitude = 1.0):
 f = ShapeFormula.formula(name)
 if not f is None:
   return RandomAmplitudeFormula(Formulas._shaped_sine(f), min_amplitude, max_amplitude)
 return None
 
@staticmethod
def available():
 return [
   'ring-vibrato',
   'ring',
   'vibrato',
   'boing',
 ]

@staticmethod
def _shaped_sine(formula):
 return NormalizeFormula(SmoothFormula(MultiplyFormula([formula, SineFormula()])))

def _test_formulas():
duration = 1.0
samples = 100
sample_length = duration / samples
formula = Formulas.formula('ring-vibrato')
for i in range(0, samples + 1):
 t = i * sample_length
 value = formula.value(t)
 print(int(value * 8192))

if __name__ == '__main__':
_test_formulas()

 
 
 */
}
