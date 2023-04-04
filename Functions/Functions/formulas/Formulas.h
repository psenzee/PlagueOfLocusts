#pragma once

#include "Formula.h"

class Formulas
{
public:
    static range_t range(const Formula *formula, size_t samples = 1000);

    static const Formula *identity();
    static const Formula *constant(double constant);
    static const Formula *const_pi();
    static const Formula *const_e();
    static const Formula *const_0();
    static const Formula *const_1();
    static const Formula *const_neg1();
    static const Formula *normalize(const Formula *f);

    static const Formula *clamp(const Formula *f, const range_t &range);
    static const Formula *add(const Formula *a, const Formula *b);
    static const Formula *add(const Formula **f, size_t sz);
    static const Formula *mul(const Formula *a, const Formula *b);
    static const Formula *mul(const Formula **f, size_t sz);
    static const Formula *div(const Formula *a, const Formula *b);
    static const Formula *sub(const Formula *a, const Formula *b);
    static const Formula *sin(const Formula *a = nullptr);
    static const Formula *sinh(const Formula *a = nullptr);
    static const Formula *sinc(const Formula *a = nullptr);
    static const Formula *sincn(const Formula *a = nullptr);
    static const Formula *cos(const Formula *a = nullptr);
    static const Formula *cosh(const Formula *a = nullptr);
    static const Formula *tan(const Formula *a = nullptr);
    static const Formula *tanh(const Formula *a = nullptr);
    static const Formula *sqrt(const Formula *a = nullptr);

    static const Formula *hardLimit(const Formula *formula);
};
