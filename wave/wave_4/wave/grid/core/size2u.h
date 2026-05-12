#pragma once

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <array>

typedef std::array<std::size_t, 2> size2u;

std::size_t area(const size2u &s) { return s[0] * s[1]; }

