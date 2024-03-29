#include "std_string.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <string.h>

char *create_format_buffer_args(const char *fmt, va_list args)
{
    int    result = -1;
    size_t fmtsize = strlen(fmt), 
           length = fmtsize > 128 ? (fmtsize * 2) : 256;
    char  *buffer = 0;
    while (result == -1)
    {
        if (buffer)
            delete [] buffer;
        buffer = new char [length + 2];
        memset(buffer, 0, length + 2);
        result = vsnprintf(buffer, length, fmt, args);
        length *= 2;
    }
    return buffer;
}

char *create_format_buffer(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char *sz = create_format_buffer_args(fmt, args);
    va_end(args);
    return sz;
}

std::string format_args(const char *fmt, va_list args)
{
    char *buffer = create_format_buffer_args(fmt, args);
    std::string s(buffer ? buffer : "");
    if (buffer) delete [] buffer;
    buffer = 0;
    return s;
}

std::string format(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::string s = format_args(fmt, args);
    va_end(args);
    return s;
}
