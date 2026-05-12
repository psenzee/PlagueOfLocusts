#include "tag_op.h"
#include "functions.h"

std::ostream &tag_op::print(std::ostream &os) const
{
    switch (type) {
    case Type::NONE:        return os << "nil";
    case Type::INT:         return os << i;
    case Type::FLOAT:       return os << f;
    case Type::FUNC:        return func->print(os);
    default:                return os << "?";
    }
}
