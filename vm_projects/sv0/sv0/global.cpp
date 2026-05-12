#include "global.h"

#include <cmath>

#include "nfunc_f_f.h"

global::global(std::ostream &os) : _os(os)
{
    _map.set(key(std::string_view("sin")), tag(new nfunc_f_f(sinf)));
}
    
