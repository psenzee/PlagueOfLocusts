#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <span>

class columnar
{
public:
    
    columnar(std::span<const int> widths);

    template <size_t N> columnar(const std::array<int, N> &widths) { _initialize(std::span { widths }); }

    void                add_column(int width, bool fill = true);
    void                set_column(size_t col, int width, bool fill);
    void                reset_columns();
    std::ostringstream &stream(size_t column);
    void                clear();
    std::ostream       &write(std::ostream &os) const;
    
private:

    struct column
    {
        std::ostringstream stream;
        int                width;
        bool               fill;
        
        column(int width, bool fill = true) : width(width), fill(fill) {}
        
        std::ostream &write(std::ostream &os) const;
        void clear();
    };
    
    std::vector<column> _columns;
    
    void _initialize(std::span<const int> widths);
};
