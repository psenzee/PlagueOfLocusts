#pragma once

#include <iostream>
#include <cmath>
#include <memory.h>
#include "map.h"

struct global
{
    std::ostream &_os;
    
    map _map;
    
    global(std::ostream &os);
    
    std::ostream &stream_out() { return _os; }
};
