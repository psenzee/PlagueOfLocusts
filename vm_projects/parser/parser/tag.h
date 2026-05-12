#pragma once

#include <iostream>
#include "types.h"

struct tag : public types
{
    Type _type;

    const char     *type_name()             const { return types::type_name(_type); }
    bool            is(Type type)           const { return _type == type; }
    bool            is_atom()               const { return types::is_atom(_type); }
    std::ostream   &print(std::ostream &os) const { return os << type_name(); }

    tag(Type type = types::NIL) : _type(type) {}
};

template <typename T> T       *cast(tag *t)          { return t && t->is(types::Type(T::TYPE)) ? static_cast<T *>(t) : nullptr; }
template <typename T> const T *cast(const tag *t)    { return t && t->is(types::Type(T::TYPE)) ? static_cast<const T *>(t) : nullptr; }
