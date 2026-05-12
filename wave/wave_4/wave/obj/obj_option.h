#pragma once

struct obj_option
{
    bool        position,
                normal,
                uv,
                smooth,
                reverse;
    std::size_t precision;

    inline obj_option(bool position = true, bool normal = true, bool uv = true, bool smooth = true, bool reverse = false, std::size_t precision = 3) :
        position(position), normal(normal), uv(uv), smooth(smooth), reverse(reverse), precision(precision) {}
};
