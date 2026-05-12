#pragma once

#include <cstdint>
#include <string.h>
#include <string>
#include <string_view>
#include <span>

inline size_t      str_length(const char *s)        { return strlen(s); }
inline size_t      str_length(std::string_view s)   { return s.size(); }
inline size_t      str_length(const std::string &s) { return s.size(); }

inline const char *str_data(const char *s)          { return s; }
inline const char *str_data(std::string_view s)     { return s.data(); }
inline const char *str_data(const std::string &s)   { return s.data(); }

template <typename T> inline int char_to_int(T ch) { return static_cast<int>(static_cast<unsigned char>(ch)); }

// note: not suitable for utf8
inline void str_tolower(std::string &s) { std::transform(s.begin(), s.end(), s.begin(), [](uint8_t c){ return std::tolower(char_to_int(c)); }); }
inline void str_toupper(std::string &s) { std::transform(s.begin(), s.end(), s.begin(), [](uint8_t c){ return std::toupper(char_to_int(c)); }); }

template <typename T, typename U> inline int char_compare_cs(T a, U b) { return char_to_int(a) - char_to_int(b); }
template <typename T, typename U> inline int char_compare_ci(T a, U b) { return std::tolower(char_to_int(a)) - std::tolower(char_to_int(b)); }

template <typename T, typename U> inline int str_compare_ci(const T &t, const U &u)
{
    size_t tsz = str_length(t), usz = str_length(u);
    int result = strncasecmp(str_data(t), str_data(u), std::min(tsz, usz));
    return result == 0 ? (int(tsz) - int(usz)) : result;
}

template <typename T, typename U> inline int str_compare_cs(const T &t, const U &u)
{
    size_t tsz = str_length(t), usz = str_length(u);
    int result = strncmp(str_data(t), str_data(u), std::min(tsz, usz));
    return result == 0 ? (int(tsz) - int(usz)) : result;
}

template <typename T, typename ListT> int str_find_ci(const T &s, const ListT &list)
{
    int i = 0;
    for (auto e : list) {
        if (str_compare_ci(s, e) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}
