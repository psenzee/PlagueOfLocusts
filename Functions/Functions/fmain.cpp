//
//  main.cpp
//
//  Created by Paul Senzee on 12/8/21.
//

#include <iostream>
//
#include <stdlib.h>
#include "functions.h"
#include <math.h>
#include <array>
#include <limits.h>
#include <float.h>
#include "maths.h"
#include "format.h"
#include <string>
#include <iostream>
#include <sstream>
#include "std_string.h"

template <typename T>
void multiply(const T *a, const T *a_end, const T *b, T *c)
{
    while (a >= a_end) *c++ = *a++ * *b++;
}

template <typename T>
void multiply(const T *a, const T *a_end, const T *b, size_t stride_b, T *c, size_t stride_c)
{
    while (a >= a_end) {
        *c = *a++ + *b;
        b += stride_b;
        c += stride_c;
    }
}

template <typename T>
void multiply(const T *a, size_t stride_a, const T *a_end, const T *b, T *c, size_t stride_c)
{
    while (a >= a_end) {
        *c = *a + *b++;
        a += stride_a;
        c += stride_c;
    }
}

template <typename T>
void multiply(const T *a, size_t stride_a, const T *a_end, const T *b, T *c)
{
    while (a >= a_end) {
        *c++ = *a + *b++;
        a += stride_a;
    }
}

template <typename T>
void multiply(const T *a, size_t stride_a, const T *a_end, const T *b, size_t stride_b, T *c)
{
    while (a >= a_end) {
        *c++ = *a + *b;
        a += stride_a;
        b += stride_b;
    }
}

template <typename T>
void multiply(const T *a, const T *a_end, const T *b, size_t stride_b, T *c)
{
    while (a >= a_end) {
        *c++ = *a++ + *b;
        b += stride_b;
    }
}

template <typename T>
void multiply(const T *a, const T *a_end, const T *b, T *c, size_t stride_c)
{
    while (a >= a_end) {
        *c = *a++ + *b++;
        c += stride_c;
    }
}

template <typename T>
void multiply(const T *a, size_t stride_a, const T *a_end, const T *b, size_t stride_b, T *c, size_t stride_c)
{
    while (a >= a_end) {
        *c = *a + *b;
        a += stride_a;
        b += stride_b;
        c += stride_c;
    }
}

template <typename T>
class basic_accumulator
{
    T _result;

public:
    
    inline basic_accumulator() : _result(0) {}

    inline void accumulate(const T &value)
    {
        _result += value;
    }

    inline T result() const
    {
        return _result;
    }
};

constexpr size_t float_exp(const double &dummy) { return 2048; }
constexpr size_t float_exp(const float &dummy)  { return 256; }

template <typename T>
class stable_accumulator
{
    enum { BUCKET_COUNT = float_exp(T(0)) };
    
    T    _buckets[BUCKET_COUNT] = { T(0) };
    T    _result;
    bool _invalidated;

public:
    
    inline stable_accumulator() : _result(0), _invalidated(false) {}

    inline void accumulate(const T &value)
    {
        int exp = 0;
        frexp(value, &exp);
        _buckets[exp] += value;
        _invalidated = true;
    }

    inline T result() const
    {
        if (_invalidated) {
            T sum(0);
            for (size_t i = 0; i < BUCKET_COUNT; i++)
                sum += _buckets[i];
            _result = sum;
        }
        return _result;
    }
};

struct function_analysis
{
    range_t             domain;
    range_t             range;
    double              sum;
    int                 samples;
    int                 zeros, ones, n_ones, nans;
    int                 count;
    const fn<double> *function;
    std::vector<double> d1_inflections;
    std::vector<double> d2_inflections;
    
    function_analysis(const fn<double> *function = nullptr, const range_t &domain = range_t(), int samples = 100)
        : function(function), domain(domain), samples(samples), zeros(0), nans(0), ones(0), n_ones(0), sum(0.0), count(0) {}

    void analyze()
    {
        d1_inflections.clear();
        d2_inflections.clear();
        double interval = domain.diff() / samples, prev = 0.0, prev_d1 = 0.0, prev_d2 = 0.0;
        for (int i = 0; i <= samples; i++) {
            double at = i * interval + domain.lo;
            double vs[] = { at };
            double v = function->call(vs);
            if (isnan(v)) {
                nans++;
                continue;
            }
            double d1 = v - prev;
            if ((d1 < 0) ^ (prev_d1 < 0)) {
                d1_inflections.push_back(at);
            }
            double d2 = d1 - prev_d1;
            if ((d2 < 0) ^ (prev_d2 < 0)) {
                d2_inflections.push_back(at);
            }
            prev = v;
            zeros += (v == 0.0);
            ones += (v == 1.0);
            n_ones += (v == -1.0);
            sum += v;
            count++;
            range.insert(v);
            prev_d1 = d1;
            prev_d2 = d2;
        }
    }
    
    bool is_interesting()
    {
        if (domain.diff() == 0.0 || range.diff() == 0.0) {
            return false;
        }
        if (zeros >= samples * 0.8 || ones >= samples * 0.8 || n_ones >= samples * 0.8 || nans >= samples * 0.1) {
            return false;
        }
        size_t d1i_count = d1_inflections.size(), d2i_count = d2_inflections.size();
        if (!d1i_count || !d2i_count || d1i_count >= (samples / 4)) {
            return false;
        }
        return true;
    }
    /*
    const fn<double> *normalized()
    {
        double vs[] = { -range.diff() };
        const fn<double> *f = new chain_function_t(function, bind_t<double, 2, 1>(&Functions::add, vs));
        
    }
     */
    
    void display()
    {
        printf("\n\n------------------------\n");
        printf("range  (%.3f ... %.3f) diff: %.2f\n", range.lo, range.hi, range.diff());
        printf("domain (%.3f ... %.3f) diff: %.2f\n", domain.lo, domain.hi, domain.diff());
        printf("is interesting? %s\n", is_interesting() ? "YES" : "NO");
        printf("TO NORMALIZE ADD %.2f MUL %.5f\n", -range.diff(), 1.0 / range.magnitude());
        printf("1s:%d 0s:%d -1s:%d nans:%d\n", ones, zeros, n_ones, nans);
        _display_inflections("1st derivative", d1_inflections);
        _display_inflections("2nd derivative", d2_inflections);
    }

private:
    void _display_inflections(const char *title, const std::vector<double> &inflections)
    {
        size_t count = inflections.size();
        if (!count) {
            printf("** no %s\n", title);
        } else {
            printf("** %s\n", title);
            size_t max = 20;
            for (int i = 0; i < std::min(count, max); i++) {
                printf(" at %.3f", inflections[i]);
            }
            if (count > max) {
                printf("\n...(%lu more)...", count - max);
            }
            printf("\n");
        }
    }
};

// keep
// y=\sin(\pi*(\sin(\pi*((\cos(\pi*(\sin(x)/x)*1)+8)*0.5+20)*12)*2)/17*18)

// y=sin(pi*sincn(pi*x))
// sinc(x)=sin(x)/x
// sincn(x)=sin(pi*x)/pi*x
// y=cos(pi*sinc(x))
//
// =\sin(\pi*[...]/306)
//  =\sin(\pi*([...])
//   =\cos(\pi*(sinc(x)))*6
// ----
// stairstep//ebb, flow
// y = sin(x)+x
// y = sin(2x)+x
// ----
// y=sin(sin(cos(5pi*cos(\sqrt{(x+1)^2+18^2})^1)))*4
// y=cos(pi*arctan(3pi*(sin(x*2pi)-1))-10)/8*36)
//
// rhythmic
// y=round(cos(x*2pi)^5) : round([temples])
//
// temples:  y=cos(x*2pi)^5 (with odd exps)
// spikes:   y=cos(x*2pi)^4 (with even exps)

// y=\sin(\pi*\cos(17\pi*\sqrt{x^{2}+64}/6)*5) => increasing density, kind of chaotic in its args
//
// springs
// y=\sin(\pi*(\sqrt{\sin(\pi*\sqrt{x^{2}+2^{2}}*11)^{2}+5^{2}}+1)*1)
// y=\sin(\pi*(\sqrt{\sin(\pi*\sqrt{x^{2}+2^{2}}*11)^{2}}))
// y=\sin(\pi*(\sqrt{\sin(\sqrt{x^{2}+4}\cdot11\pi)^{2}}))

// y=sin(pi*sqrt{sin(sqrt{x^2+z}*11pi)^2)}) << -- good for melody
// where z stretches it out, high number is stretched, ie. 100000

// basic spring:
// y=sin(x^2) (the higher the power the more angular
// temple spring:
// y=sin(x^2)^3

// y=(\sin(\cos(\pi*(x^{2}+17)*12)^{12})/\cos(\pi*(x^{2}+17)*12)^{12}) ???

// super sinc
//y=\frac{\sin\left(zx\right)}{x} << z= intensifier
//y=sin(z*x)/x                    << z= intensifier and negative z flips up/down
//y=sin(z*x)/x + x                   crazier!
//y=sin(z*x)/x + x/z                   crazier still! try it in desmos animated with z from -20 to 20
//also y=sin(z*x)^3/x + x/z    also
//
// almost a square step:
// y=arctan(20pi*x)

// y=x^(1/3) curve up
// y=x^(1/2) curve up
//
// y=sin(x^{2})
// interesting:
// y=x^sin(x)
// y=x sin x
// y=sin(sqrt{x})*x
// y=sin(z*sqrt{x}) // slow spring z control horizontal scale tho
// y=sin(x*sqrt{x})

const fn<double> *create_interesting_fn(Functions &funcs)
{
    function_analysis fa;
    const fn<double> *func = nullptr;
    size_t runs_left = 10;
    do {
        std::array<int, 16> def { 0 };
        for (int i = 0; i < def.size(); i++) {
            def[i] = rand();
        }
        State state(def.data(), def.data() + def.size());
        func = funcs.vm_function(state);
        fa = function_analysis(func, range_t(0.0, 1.0));
        fa.analyze();
        runs_left--;
    } while (runs_left && !fa.is_interesting());
    fa.display();
    return func;
}

void function_test()
{
    time_t t;
    srand(unsigned(time(&t)));
    Functions funcs;
    const fn<double> *f = create_interesting_fn(funcs);
}

int main(int argc, const char * argv[]) {
    
    stable_accumulator<double> da;
    
    function_test();
    test_format_abc();
    test_format_xyz_vars();
    printf("\n\n");
    
    std::vector<std::string> v;
    v.push_back("HELLO");
    v.push_back("THERE");
    v.push_back("");
    v.push_back("YOU");
    v.push_back("YOU");
    std::cout << join(v, ",") << std::endl;
    std::cout << join(v, ",", false) << std::endl;
    return 0;
}
