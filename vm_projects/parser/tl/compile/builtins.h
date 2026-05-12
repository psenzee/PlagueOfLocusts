#pragma once

#include <array>
#include "symbols.h"
#include "functions.h"

namespace tl
{
std::array<function_call *, symbols::_MAX_SYMBOLS> construct_builtins();
}
