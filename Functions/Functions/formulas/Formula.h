#pragma once

#include "maths.h"

struct Formula
{
    Formula();
    virtual ~Formula() = 0;
    virtual double at(double t) const = 0;
};
