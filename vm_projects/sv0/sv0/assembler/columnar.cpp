#include "columnar.h"

#include <iostream>
#include <sstream>
#include <iomanip>

std::ostream &columnar::column::write(std::ostream &os) const
{
    if (fill && width != 0) {
        os << std::setw(std::abs(width));
    }
    return os << (width < 0 ? std::left : std::right) << stream.str();
}

void columnar::column::clear()
{
    stream = std::ostringstream();
}

void columnar::_initialize(std::span<const int> widths)
{
    for (size_t i = 0, sz = widths.size(); i < sz; ++i) {
        if (widths[i]) { add_column(widths[i], i + 1 < sz && widths[i + 1] != 0); }
    }
}

columnar::columnar(std::span<const int> widths)
{
    _initialize(widths);
}

void columnar::add_column(int width, bool fill)
{
    _columns.push_back(column(width));
}

void columnar::set_column(size_t col, int width, bool fill)
{
    column &c = _columns.at(col); c.width = width; c.fill = fill;
}

void columnar::reset_columns()
{
    _columns.clear();
}

std::ostringstream &columnar::stream(size_t column)
{
    return _columns[column].stream;
}

void columnar::clear()
{
    for (auto &col : _columns) {
        col.clear();
    }
}

std::ostream &columnar::write(std::ostream &os) const
{
    for (const auto &col : _columns) {
        col.write(os);
    }
    return os;
}
