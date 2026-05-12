#include "binary_stream.h"
#include "format.h"
#include "serialize.h"
#include <string>

std::size_t write_format(binary_out_stream &s, std::string_view fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::string formatted = format::format_args(fmt.data(), args);
    va_end(args);
    return s.write(formatted.data(), formatted.size()) == formatted.size();
}
