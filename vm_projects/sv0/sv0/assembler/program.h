#pragma once

#include <string>
#include "code_stream.h"

struct program
{
    code_stream data;
    std::string bytecode;
};
