#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <deque>
#include <vector>
#include "strs.h"
#include "elems.h"
#include "printer.h"

namespace tl
{

struct constants
{
    std::vector<elem *>     list;
    std::deque<std::string> text;

    int add(elem *e)
    {
        if (is_nil(e)) {
            return -1;
        }
        int id = find(e);
        if (id == -1) {
            pair *p = cast<pair>(e);
            std::ostringstream oss;
            printer pr;
            const char *str = ".CLST";
            switch (e->_type) {
                case elem::FLOAT:  str = ".CFLT"; break;
                case elem::INT:    str = ".CINT"; break;
                case elem::SYMBOL: str = ".CSYM"; break;
                case elem::STRING: str = ".CSTR"; break;
                default: break;
            }
            oss << str << " #" << std::setw(3);
            if (p) {
                int h = add(p->head), t = add(p->tail);
                id = append(p);
                oss << id << " [" << std::setw(4) << h;
                if (t != -1) {
                    oss << " " << std::setw(4) << t;
                } else {
                    oss << "     ";
                }
                pr.print(oss << "]  ; ", e);
            } else {
                id = append(e);
                pr.print(oss << id << "  " << std::setw(4), e);
            }
            text.push_back(oss.str());
        }
        return id;
    }

    std::ostream &print(std::ostream &os)
    {
        for (const auto &line : text) {
            os << line << std::endl;
        }
        return os;
    }

    int append(elem *e)
    {
        int id = static_cast<int>(list.size());
        list.push_back(e);
        return id;
    }

    int find(elem *e)
    {
        for (int i = 0; i != list.size(); ++i) {
            if (equals(list[i], e)) {
                return i;
            }
        }
        return -1;
    }
};

}
