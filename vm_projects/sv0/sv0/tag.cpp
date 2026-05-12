#include "tag.h"

#include <iostream>
#include <iomanip>

#include "object.h"
#include "pair.h"
#include "func.h"

const tag tag::nil;

pair *tag::_pair(object *o) { return o && o->type() == object::PAIR ? dynamic_cast<pair *>(o) : nullptr; }
func *tag::_func(object *o) { return o && o->type() == object::FUNC ? dynamic_cast<func *>(o) : nullptr; }

std::ostream &tag::print_raw(std::ostream &os) const
{
    return os << "T" << std::hex << p << std::dec;
}

std::ostream &tag::print(std::ostream &os) const
{
    switch (type()) {
    case tag::INT:    return os << std::dec << as_int();
    case tag::FLOAT:  return os << std::dec << as_float();
    case tag::OBJECT:
        {
            object *obj = safe_object();
            return obj ? obj->print(os) : (os << "nil");
        }
    default:          return print_raw(os);
    }
    return os;
}
