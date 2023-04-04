#pragma once

#include <array>
#include <string>
#include <iostream>
#include <sstream>

template <typename T>
std::ostringstream &join(std::ostringstream &stream, const T &v, const std::string &delimiter, bool include_empty = true)
{
    typename T::const_iterator first = v.end();
    for (typename T::const_iterator i = v.begin(), e = v.end(); i != e; ++i) {
        if (include_empty || !(*i).empty()) {
            if (first == v.end()) {
                first = i;
            } else if (!delimiter.empty()) {
                stream << delimiter;
            }
            stream << *i;
        }
    }
    return stream;
}

template <typename T>
std::string join(const T &v, const std::string &delimiter, bool include_empty = true)
{
    std::ostringstream oss;
    return join(oss, v, delimiter, include_empty).str();
}

char        *create_format_buffer_args(const char *fmt, va_list args);
char        *create_format_buffer(const char *fmt, ...);
std::string  format_args(const char *fmt, va_list args);
std::string  format(const char *fmt, ...);
