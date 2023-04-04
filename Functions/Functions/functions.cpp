#include "functions.h"
#include "maths.h"
#include "format.h"
#include <math.h>
#include <array>
#include <vector>
#include <iostream>
#include <sstream>

fn_constant<double> Functions::_pi2     (-MATH_PI * 2.0, "-2pi", "-2\\pi",  false);
fn_constant<double> Functions::__6      (-6.0,             "-6",     "-6",  false);
fn_constant<double> Functions::_e2      (-MATH_E * 2.0,   "-2e",    "-2e",  false);
fn_constant<double> Functions::__5      (-5.0,             "-5",     "-5",  false);
fn_constant<double> Functions::__4      (-4.0,             "-4",     "-4",  false);
fn_constant<double> Functions::_pi      (-MATH_PI,        "-pi",  "-\\pi",  false);
fn_constant<double> Functions::__3      (-3.0,             "-3",     "-3",  false);
fn_constant<double> Functions::_e       (-MATH_E,          "-e",     "-e",  false);
fn_constant<double> Functions::__2      (-2.0,             "-2",     "-2",  false);
fn_constant<double> Functions::__1      (-1.0,             "-1",     "-1",  false);
fn_constant<double> Functions::_0       (0.0,               "0",      "0",  false);
fn_constant<double> Functions::_1       (1.0,               "1",      "1",  false);
fn_constant<double> Functions::_2       (2.0,               "2",      "2",  false);
fn_constant<double> Functions::e        (MATH_E,            "e",      "e",  false);
fn_constant<double> Functions::_3       (3.0,               "3",      "3",  false);
fn_constant<double> Functions::pi       (MATH_PI,          "pi",   "\\pi",  false);
fn_constant<double> Functions::_4       (4.0,               "4",      "4",  false);
fn_constant<double> Functions::_5       (5.0,               "5",      "5",  false);
fn_constant<double> Functions::e2       (MATH_E * 2.0,     "2e",     "2e",  false);
fn_constant<double> Functions::_6       (6.0,               "6",      "6",  false);
fn_constant<double> Functions::pi2      (MATH_PI * 2.0,    "pi",  "2\\pi",  false);

inline static bool _eq(double a, double b)
{
    return abs(a - b) < 0.00001;
}

const fn<double> *Functions::constant(double v)
{
    const double PI = MATH_PI, PI2 = PI * 2.0, E = MATH_E, E2 = E * 2.0;
    if (v >= -PI2 * 1.001 && v <= PI2 * 1.001) {
        if (v >= 0) {
            if      (_eq(v,  0.0)) { return   &_0; }
            else if (_eq(v,  1.0)) { return   &_1; }
            else if (_eq(v,  2.0)) { return   &_2; }
            else if (_eq(v,  E))   { return    &e; }
            else if (_eq(v,  3.0)) { return   &_3; }
            else if (_eq(v,  PI))  { return   &pi; }
            else if (_eq(v,  4.0)) { return   &_4; }
            else if (_eq(v,  5.0)) { return   &_5; }
            else if (_eq(v,  E2))  { return   &e2; }
            else if (_eq(v,  6.0)) { return   &_6; }
            else if (_eq(v,  PI2)) { return  &pi2; }
        } else {
            if      (_eq(v, -1.0)) { return  &__1; }
            else if (_eq(v, -2.0)) { return  &__2; }
            else if (_eq(v, -E))   { return   &_e; }
            else if (_eq(v, -3.0)) { return  &__3; }
            else if (_eq(v, -PI))  { return  &_pi; }
            else if (_eq(v, -4.0)) { return  &__4; }
            else if (_eq(v, -5.0)) { return  &__5; }
            else if (_eq(v, -E2))  { return  &_e2; }
            else if (_eq(v, -6.0)) { return  &__6; }
            else if (_eq(v, -PI2)) { return &_pi2; }
        }
    }
    char buffer[128];
    sprintf(buffer, "%.3f", v);
    return register_fn(new fn_constant<double>(v, buffer, buffer, true));
}

Functions::~Functions()
{
    for (std::vector<const fn<double> *>::const_iterator i = _registered.begin(), e = _registered.end(); i != e; ++i) {
        if ((*i)->info.heap()) {
            delete *i;
        }
    }
    _registered.clear();
    _pointers.clear();
    _functions.clear();
}

fn_identity<double> Functions::identity;

fn_function<double, 1> Functions::abs       (::abs,         "abs",       "\\abs($A)");
fn_function<double, 1> Functions::sin       (::sin,         "sin",       "\\sin($A)");
fn_function<double, 1> Functions::sinh      (::sinh,        "sinh",      "\\sinh($A)");
fn_function<double, 1> Functions::cos       (::cos,         "cos",       "\\cos($A)");
fn_function<double, 1> Functions::cosh      (::cosh,        "cosh",      "\\cosh($A)");
fn_function<double, 1> Functions::tan       (::tan,         "tan",       "\\tan($A)");
fn_function<double, 1> Functions::tanh      (::tanh,        "tanh",      "\\tanh($A)");
fn_function<double, 1> Functions::asin      (::asin,        "asin",      "\\arcsin($A)");
fn_function<double, 1> Functions::asinh     (::asinh,       "asinh",     "\\arcsinh($A)");
fn_function<double, 1> Functions::acos      (::acos,        "acos",      "\\arccos($A)");
fn_function<double, 1> Functions::acosh     (::acosh,       "acosh",     "\\arccosh($A)");
fn_function<double, 1> Functions::atan      (::atan,        "atan",      "\\arctan($A)");
fn_function<double, 1> Functions::atanh     (::atanh,       "atanh",     "\\arctanh($A)");

static double _sinc(double x) { return x == 0.0 ? 1.0 : sin(x) / x; }
static double _sincn(double x) { x *= MATH_PI; return x == 0.0 ? 1.0 : sin(x) / x; }

fn_function<double, 1> Functions::sinc      (_sinc,         "sinc",      "(\\sin($A)/$A)");
fn_function<double, 1> Functions::sincn     (_sincn,        "sincn",     "(\\sin(\\pi*$A)/\\pi*$A)");
fn_function<double, 1> Functions::exp       (::exp,         "exp",       "e^{$A}");
fn_function<double, 1> Functions::exp2      (::exp2,        "exp2",      "2^{$A}");
fn_function<double, 1> Functions::log       (::log,         "ln",        "\\ln($A)");
fn_function<double, 1> Functions::log2      (::log2,        "log2",      "\\log_{2}($A)");
fn_function<double, 1> Functions::log10     (::log10,       "log10",     "\\log_{10}($A)");
fn_function<double, 1> Functions::sqrt      (::sqrt,        "sqrt",      "\\sqrt{$A}");
fn_function<double, 1> Functions::cbrt      (::cbrt,        "cbrt",      "\\sqrt[3]{$A}");
fn_function<double, 1> Functions::erf       (::erf,         "erf",       "\\operatorname{erf}($A)");
fn_function<double, 1> Functions::ceil      (::ceil,        "ceil",      "\\operatorname{ceil}($A)");
fn_function<double, 1> Functions::floor     (::floor,       "floor",     "\\operatorname{floor}($A)");
fn_function<double, 1> Functions::round     (::round,       "round",     "\\operatorname{round}($A)");

static double _inc(double x)       { return x + 1; }
static double _dec(double x)       { return x - 1; }
static double _x2(double x)        { return x * 2; }
static double _half(double x)      { return x / 2; }
static double _x3(double x)        { return x * 3; }
static double _third(double x)     { return x / 3; }
static double _invert(double x)    { return x == 0.0 ? 0.0 : (1.0 / x); }
static double _negate(double x)    { return -x; }
static double _one_minus(double x) { return 1 - x; }
static double _square(double x)    { return x * x; }
static double _cube(double x)      { return x * x * x; }

fn_function<double, 1> Functions::inc       (_inc,          "inc",       "($A+1)");
fn_function<double, 1> Functions::dec       (_dec,          "dec",       "($A-1)");
fn_function<double, 1> Functions::x2        (_x2,           "x2",        "($A*2)");
fn_function<double, 1> Functions::half      (_half,         "half",      "($A/2)");
fn_function<double, 1> Functions::x3        (_x3,           "x3",        "($A*3)");
fn_function<double, 1> Functions::third     (_third,        "third",     "($A/3)");
fn_function<double, 1> Functions::invert    (_invert,       "invert",    "(1/$A)");
fn_function<double, 1> Functions::square    (_square,       "square",    "($A^{2})");
fn_function<double, 1> Functions::cube      (_cube,         "cube",      "($A^{3})");
fn_function<double, 1> Functions::negate    (_negate,       "negate",    "(-$A)");
fn_function<double, 1> Functions::one_minus (_one_minus,    "one_minus", "(1-$A)");

static double _sin1pi(double x)  { return sin (x * MATH_PI); }
static double _cos1pi(double x)  { return cos (x * MATH_PI); }
static double _tan1pi(double x)  { return tan (x * MATH_PI); }
static double _atan1pi(double x) { return atan(x * MATH_PI); }

fn_function<double, 1> Functions::sin1pi    (_sin1pi,       "sin1pi",    "\\sin(\\pi*$A)");
fn_function<double, 1> Functions::cos1pi    (_cos1pi,       "cos1pi",    "\\cos(\\pi*$A)");
fn_function<double, 1> Functions::tan1pi    (_tan1pi,       "tan1pi",    "\\tan(\\pi*$A)");
fn_function<double, 1> Functions::atan1pi   (_atan1pi,      "atan1pi",   "\\arctan(\\pi*$A)");

fn_function<double, 2> Functions::fmod      (::fmod,        "mod",       "\\operatorname{mod}($A,$B)");
fn_function<double, 2> Functions::hypot     (::hypot,       "hypot",     "\\sqrt{$A^{2}+$B^{2}}");
fn_function<double, 2> Functions::pow       (::pow,         "pow",       "$A^{$B}");
fn_function<double, 2> Functions::fmax      (::fmax,        "max",       "\\operatorname{max}($A,$B)");
fn_function<double, 2> Functions::fmin      (::fmin,        "min",       "\\operatorname{min}($A,$B)");

static double _add     (double x, double y) { return x + y; }
static double _sub     (double x, double y) { return x - y; }
static double _mul     (double x, double y) { return x * y; }
static double _div     (double x, double y) { return x / y; }
static double _sinpi   (double x, double y) { return sin(x * MATH_PI * y); }
static double _cospi   (double x, double y) { return cos(x * MATH_PI * y); }
static double _tanpi   (double x, double y) { return tan(x * MATH_PI * y); }
static double _atanpi  (double x, double y) { return atan(x * MATH_PI * y); }
static double _average (double x, double y) { return (x + y) * 0.5; }

fn_function<double, 2> Functions::add       (_add,          "add",       "($A+$B)");
fn_function<double, 2> Functions::sub       (_sub,          "sub",       "($A-$B)");
fn_function<double, 2> Functions::mul       (_mul,          "mul",       "($A*$B)");
fn_function<double, 2> Functions::div       (_div,          "div",       "($A/$B)");

fn_function<double, 2> Functions::sinpi     (_sinpi,        "sinpi",     "\\sin(\\pi*$A)");
fn_function<double, 2> Functions::cospi     (_cospi,        "cospi",     "\\cos(\\pi*$A)");
fn_function<double, 2> Functions::tanpi     (_tanpi,        "tanpi",     "\\tan(\\pi*$A)");
fn_function<double, 2> Functions::atanpi    (_atanpi,       "atanpi",    "\\arctan(\\pi*$A)");
fn_function<double, 2> Functions::average   (_average,      "average",   "(($A+$B)/2)");

static double _interp  (double x, double y, double z) { return (y - x) * z + x; }

static double _add3    (double x, double y, double z) { return x + y + z; }
static double _sub3    (double x, double y, double z) { return x - y - z; }
static double _mul3    (double x, double y, double z) { return x * y * z; }
static double _div3    (double x, double y, double z) { return x / y / z; }

fn_function<double, 3> Functions::fma       (::fma,         "fma",       "($A*$B+$C)");
fn_function<double, 3> Functions::interp    (_interp,       "interp",    "(($B-$A)*$C+$A)");
fn_function<double, 3> Functions::add3      (_add3,         "add3",      "($A+$B+$C)");
fn_function<double, 3> Functions::sub3      (_sub3,         "sub3",      "($A-$B-$C)");
fn_function<double, 3> Functions::mul3      (_mul3,         "mul3",      "($A*$B*$C)");
fn_function<double, 3> Functions::div3      (_div3,         "div3",      "($A/$B/$C)");

//y=(sin(x)+x*sin(x))
//y=(sin(x*pi)+x*pi*sin(x*pi))
//y=(sin(x*pi)+x*pi*cos(x*pi))
//y=(cos(x*pi)+x*pi*cos(x*pi))
//y=(tan(x*pi)+x*pi*cos(x*pi))
//y=(tan(x*pi)+x*pi*tan(x*pi))

const fn<double> *Functions::op(State &s)
{
    if (s.p >= s.end) {
        return nullptr;
    }
    int code = *s.p++ % 1024;
    if (code < 128) { return _op_constant(code); }
    if (code < 1256) { return _op1(code); }
    if (code < 512) {
        const fn<double> *p = op(s);
        if (!p) { return _op1(code); }
        return bind2to1(_op2(code), p);
    }
    if (code < 1024) {
        const fn<double> *p = op(s);
        if (!p) { return _op1(code); }
        const fn<double> *q = op(s);
        if (!q) { return bind2to1(_op2(code), p); }
        return bind3to1(_op3(code), p, q);
    }
    return nullptr;
}

const fn<double> *Functions::bind2to1(const fn<double> *f, const fn<double> *bind)
{
    const fn<double> *b[] = { bind }, *fc = new fn_bind_fn1<double, 2, 1>(f, b);
    register_fn(fc);
    return fc;
}

const fn<double> *Functions::bind3to1(const fn<double> *f, const fn<double> *bind0, const fn<double> *bind1)
{
    const fn<double> *b[] = { bind0, bind1 }, *fc = new fn_bind_fn1<double, 3, 2>(f, b);
    register_fn(fc);
    return fc;
}

const fn<double> *Functions::_op_constant(int value)
{
    static const fn<double> *ops[] = {
        &_pi2, &__6, &__5, &__4, &_pi, &__3, &_e, &__2, &__1,
        &_1, &_2, &e, &_3, &pi, &_4, &_5, &e2, &_6, &pi2
    };
    if (value & 1) {
        return ops[(value >> 1) % (sizeof(ops) / sizeof(ops[0]))];
    }
    value >>= 1;
    int sign = 1;
    if (value & 1) {
        sign = -1;
    }
    value >>= 1;
    int numer = value & 0xff, denom = (value >> 8) & 0xff;
    if (denom == 0) {
        if (numer == 0) {
            return sign == 1 ? &pi2 : &_pi2;
        }
        denom = 1;
    }
    return constant(double(sign * numer) / double(denom));
}

const fn<double> *Functions::_op1(int value)
{
    std::array<const fn<double> *, 33> ops = {
        &abs, &sin, &cos, &tanh, &asinh,
        &atan, &sinc, &sincn, &exp, &exp2, &log,
        &log2, &log10, &sqrt, &cbrt, &erf, &ceil,
        &floor, &round, &inc, &dec, &x2, &half, &x3, &third,
        &invert, &square, &cube, &negate, &one_minus,
        &sin1pi, &cos1pi, &atan1pi
    };
    return ops[value % ops.size()];
}

const fn<double> *Functions::_op2(int value)
{
    std::array<const fn<double> *, 14> ops = {
        &fmod, &hypot, &pow, &fmax, &fmin,
        &add, &sub, &mul, &div,
        &sinpi, &cospi, &tanpi, &atanpi, &average,
    };
    return ops[value % ops.size()];
}

const fn<double> *Functions::_op3(int value)
{
    std::array<const fn<double> *, 6> ops = {
        &fma, &interp, &add3, &sub3, &mul3, &div3,
    };
    return ops[value % ops.size()];
}

const fn<double> *Functions::register_fn(const fn<double> *f)
{
    if (!f) {
        return f;
    }
    if (!_pointers.count(f)) {
        _registered.push_back(f);
        _pointers[f] = _registered.size() - 1;
    }
    const char *name = f->info.name();
    if (name && *name && !_functions.count(name)) {
        _functions[name] = f;
    }
    return f;
}

void Functions::destroy_fn(const fn<double> *f)
{
    std::map<const fn<double> *, size_t>::iterator i = _pointers.find(f);
    if (i != _pointers.end()) {
        size_t index = (*i).second;
        _pointers.erase(i);
        _registered[index] = nullptr;
        _remove_function(f->info.name(), f);
        if (f->info.heap()) {
            delete f;
        }
    }
}

void Functions::_remove_function(const char *name, const fn<double> *f)
{
    std::map<const char *, const fn<double> *>::iterator i = _functions.find(name);
    if (i != _functions.end() && (!f || (*i).second == f)) { _functions.erase(i); }
}

/*
std::string Functions::format(const std::string &x, const fn<double> *fn)
{
    if (!fn) {
        return x;
    }
    size_t count = fn->composite_count();
    size_t arity = fn->arity();
    std::string result;
    if (count == 0) {
        char buffer[1024];
        format_vars(buffer, fn->info.format(), x.c_str(), nullptr, nullptr);
        std::string result;

        if (arity == 1) {
            
        }
    }
    std::string vs[8];
    for (size_t i = 0; i < count; i++) {
        
        
    }
    
    
    std::ostringstream oss;
    std::string s("x");  // start
    for (std::vector<const fn<double> *>::const_iterator i = list.begin(), e = list.end(); i != e; ++i) {
        const char *format = (*i)->info.format();
        oss << (*i)->info.format() << " ";
    }
    return oss.str();
}

std::string Functions::format(const std::vector<const fn<double> *> &list)
{
    std::ostringstream oss;
    std::string s("x");  // start
    for (std::vector<const fn<double> *>::const_iterator i = list.begin(), e = list.end(); i != e; ++i) {
        const char *format = (*i)->info.format();
        oss << (*i)->info.format() << " ";
    }
    return oss.str();
}
 */

const fn<double> *Functions::vm_function(State &state)
{
    printf("making function --------\n");
    std::string s("x");
    while (!state.done()) {
        int code = *state.p++;
        if (!code) {
            continue;
        }
        const fn<double> *fn = op(state);
        if (fn) {
            state.push(fn);
        }
    }

    printf("function: y=%s\n", s.c_str());
    //std::string sx = _format(state.queue[0]);
    //printf("function-format: y=%s\n", sx.c_str());
    printf("len %lu\n", state.queue.size());
    printf("end making function --------\n");
    const fn<double> *fr = new fn_list<double>(state.queue);
    std::string sy = fr->format();
    char buffer[64 * 1024];
    format_vars(buffer, sy.c_str(), "x");
    sy = buffer;
    printf("function-format-whole: y=%s\n", sy.c_str());
    for (size_t i = 0; i < state.queue.size(); i++) {
        const fn<double> *fr = state.queue[i];
        std::string sy = _format(fr);
        printf("function-format: y=%s\n", sy.c_str());
    }
    return fr;
}
/*
FunctionRepr Functions::op2(int value, const FunctionRepr &fr0, const FunctionRepr &fr1)
{
    static std::array<const func_double_t *, 9> ops {
        &add, &sub, &mul, &div, &sinpi, &cospi,
        &average, &hypot, &pow
    };
    static std::array<const char *, 9> format_str {
        "($A+$B)", "($A-$B)", "$A*$B", "$A/$B", "\\sin(\\pi*$A*$B)", "\\cos(\\pi*$A*$B)",
        "($A+$B)*0.5", "\\sqrt{$A^{2}+$B^{2}}", "$A^{$B}",// "max($A,$B)", "min($A,$B)",
    };
    FunctionRepr ret;
    int opn = value % ops.size();
    value /= ops.size();
    if (!value) {
        return ret;
    }
    const func_double_t *op = ops[opn];
    char y[12] = {0}, buffer[1024] = {0};
    sprintf(y, "%lu", value);
    format_vars(buffer, format_str[opn], s.c_str(), y, "");
    std::string ss(buffer);
    double vs[] = { (double)value };
    return name_function_t(ss, new fn_bind<double, 2, 1>(op, vs));
}*/

/*
Functions::func_double_t *Functions::create_vm_function(const std::array<int, 16> &definition)
{
    printf("making function --------\n");
    std::vector<const func_double_t *> queue;
    std::string s("x");
    for (size_t i = 0, sz = definition.size(); i < sz; i++) {
        size_t def = definition[i];
        if (!def) {
            continue;
        }
        name_function_t fn = op1(def, s);
        if (fn.second) {
            queue.push_back(fn.second);
            s = fn.first;
        } else {
            if (def > 55) {
                def -= 55;//func1s.size();
                name_function_t fn = op2(def, s);
                if (fn.second) {
                    s = fn.first;
                    queue.push_back(fn.second);
                }
            }
        }
    }
    printf("function: y=%s\n", s.c_str());
    printf("len %lu\n", queue.size());
    printf("end making function --------\n");
    return new fn_list<double>(queue);
}

*/
