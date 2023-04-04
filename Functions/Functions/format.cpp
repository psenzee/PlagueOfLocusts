#include "format.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

size_t count_format_args(const char *f, char symbol = '$', char lo = 'A', char hi = 'Z')
{
    size_t count = 0, range = hi + 1 - lo;
    while (f && *f) {
        if ((*f == '\\' || *f == symbol) && f[1] == symbol) {
            f++;
        } else if (*f == symbol) {
            unsigned v = f[1] - lo;
            if (v < range) {
                count = v + 1 > count ? v + 1 : count;
                f++;
            }
        }
        f++;
    }
    return count;
}

int format_vars_array(char *buffer, char symbol, char lo, char hi, const char *format, size_t argcount, const char **args)
{
    if (!format) {
        return sprintf(buffer, "%s", format);
    }
    size_t count = count_format_args(format, symbol, lo, hi);
    count = count > argcount ? argcount : count;
    const char *p = format;
    char *q = buffer;
    char c = 0;
    while ((c = *p)) {
        unsigned v = p[1] - lo;
        if ((c == '\\' || c == symbol) && p[1] == symbol) {
            *q++ = *++p;
        } else if (c == symbol && v < count) {
            p++; q += sprintf(q, "%s", args[v]);
        } else {
            *q++ = c;
        }
        p++;
        *q = 0;
    }
    return int(q - buffer);
}

int format_vars_array(char *buffer, const char *format, size_t argcount, const char **args)
{
    return format_vars_array(buffer, '$', 'A', 'Z', format, argcount, args);
}

int format_va_list(char *buffer, char symbol, char lo, char hi, const char *format, va_list args)
{
    if (!format) {
        return sprintf(buffer, "%s", format);
    }
    const int MAX = 32;
    const char *argcc[MAX] = { nullptr };
    size_t count = count_format_args(format, symbol, lo, hi);
    count = count > MAX ? MAX : count;
    for (size_t i = 0, sz = count; i < sz; i++) {
        argcc[i] = va_arg(args, const char *);
    }
    return format_vars_array(buffer, symbol, lo, hi, format, count, argcc);
}

int format_vars_config(char *buffer, char symbol, char lo, char hi, const char *format, ...)
{
    if (!format) {
        return sprintf(buffer, "%s", format);
    }
    va_list args;
    va_start(args, format);
    int count = format_va_list(buffer, symbol, lo, hi, format, args);
    va_end(args);
    return count;
}

int format_vars(char *buffer, const char *format, ...)
{
    if (!format) {
        return sprintf(buffer, "%s", format);
    }
    va_list args;
    va_start(args, format);
    int count = format_va_list(buffer, '$', 'A', 'Z', format, args);
    va_end(args);
    return count;
}

void print_format_abc(const char *fmt, const char *x, const char *y, const char *z)
{
    char buffer[64 * 1024];
    format_vars(buffer, fmt, x, y, z);
    printf("%s", buffer);
}

void print_format_xyz_config(const char *fmt, const char *x, const char *y, const char *z)
{
    char buffer[64 * 1024];
    format_vars_config(buffer, '$', 'X', 'Z', fmt, x, y, z);
    printf("%s", buffer);
}

void test_format_abc()
{
    printf("\n");
    print_format_abc("a$Acde$Bgh$Cjklm ", "b", "f", "i");
    print_format_abc("$A/$A/$B/$C ", "abc", "def", "ghi");
    print_format_abc("$$ ", "abc", "def", "ghi");
    print_format_abc("\\$ ", "abc", "def", "ghi");
    print_format_abc("$", "abc", "def", "ghi");
    print_format_abc("", "abc", "def", "ghi");
}

void test_format_xyz_vars()
{
    printf("\n");
    print_format_xyz_config("a$Xcde$Ygh$Zjklm ", "b", "f", "i");
    print_format_xyz_config("$X/$X/$Y/$Z ", "abc", "def", "ghi");
    print_format_xyz_config("$$ ", "abc", "def", "ghi");
    print_format_xyz_config("\\$ ", "abc", "def", "ghi");
    print_format_xyz_config("$", "abc", "def", "ghi");
    print_format_xyz_config("", "abc", "def", "ghi");
}
