#pragma once

#include <iostream>
#include <string_view>

struct position
{
    uint32_t column, line;

    position() : column(1), line(1) {}

    void new_line()             { line++; column = 1; }
    void next(size_t count = 1) { column += count; }

    std::ostream &print(std::ostream &os) const { return os << "line " << line << " column " << column; }
    
    template <typename IterT> static position from_iterators(IterT begin, IterT current)
    {
        position p;
        for (auto i = begin; i < current; ++i) {
            int ch = *i;
            if (ch == '\n') {
                p.new_line();
                continue;
            }
            switch (ch & 0xf0) {
                case 0xc0:
                case 0xd0: i += 1; break;
                case 0xe0: i += 2; break;
                case 0xf0: i += 3; break;
                default: break;
            }
            p.next();
        }
        return p;
    }
};

inline std::ostream &operator<<(std::ostream &os, const position &pos) { return pos.print(os); }
