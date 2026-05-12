#pragma once

#include <iostream>
#include <iomanip>

#include "stl.h"

template <typename Container>
std::ostream &print_range(std::ostream &os, const Container &c, size_t count = 10, size_t start = 0) const
{
    if (start > 0) os << "..." << std::endl;
    for (auto i = c.begin() + start, e = std::min(i + count, c.end()); i != e; ++i)
        os << (*i) << std::endl;
    if (start + count < c.size()) os << "..." << std::endl << std::endl;
    return os;
}
