#pragma once

#include <iostream>
#include <sstream>

//template <typename T, typename U = double> inline T lerp(const T &a, const T &b, U t) { return (1 - t) * a + t * b; }
//template <typename T>                      inline T clamp(T a, T min, T max)          { return a < min ? min : (a > max ? max : a); }

template <typename T>        inline std::ostream &print(std::ostream &os, const T *b, const T *e, const char *delimiter = " ") { auto s(b); while (b != e) os << (b != s ? delimiter : "") << *b++; return os; }
template <typename Iterator> inline std::ostream &print(std::ostream &os, Iterator b, Iterator e, const char *delimiter = " ") { auto s(b); while (b != e) os << (b != s ? delimiter : "") << *b++; return os; }

inline std::string filename(const char *path, const char *base_file, const char *extension, int appendable = -1)
{
    std::ostringstream s;
    s << path << "/" << base_file;
    if (appendable != -1)
        s << "_" << appendable;
    if (extension) {
        s << "." << extension;
    }
    return s.str();
}
