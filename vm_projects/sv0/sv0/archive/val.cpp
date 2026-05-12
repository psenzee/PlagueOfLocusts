#include "val.h"

std::ostream &val::print(std::ostream &os) const
{
    switch (type) {
    case Type::INT:   return os << i;
    case Type::FLOAT: return os << f;
    case Type::FUNC:  return os << func;
    case Type::NIL:
    default:          return os << "nil";
    }
}
