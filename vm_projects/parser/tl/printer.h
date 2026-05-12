#pragma once

#include <string_view>
#include <iostream>
#include <sstream>
#include <array>
#include <string>
#include <cctype>
#include <algorithm>
#include <string_view>
#include "strs.h"
#include "elems.h"
#include "utils.h"

namespace tl
{
typedef std::string_view::const_iterator iter_t;

struct printer
{
    std::ostream &print(std::ostream &os, elem *e, bool has_tail = false) const
    {
        if (!e) {
            return os << "#nil";
        }
        if (e->is_atom()) {
            return e->print(os);
        }
        auto *p = cast<pair>(e);
        if (p) {
            os << (has_tail ? "" : "(");
            print(os, head(p), false);
            auto *t = tail(p);
            if (t) {
                print(os << (is_atom(t) ? " . " : " "), t, true);
            }
            return os << (has_tail ? "" : ")");
        }
        return os << "#nil";
    }
};

}
