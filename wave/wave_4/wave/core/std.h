#pragma once

#include <iostream>
#include <sstream>
#include <string_view>
#include <string>
#include <array>
#include <filesystem>

#define ARRAY_COUNT(ARRAY) (sizeof((ARRAY)) / sizeof((ARRAY)[0]))

template <typename T>                inline std::ostream &print(std::ostream &os, const T *b, const T *e, const char *delimiter = " ") { auto s(b); while (b != e) os << (b != s ? delimiter : "") << *b++; return os; }
template <typename Iterator>         inline std::ostream &print(std::ostream &os, Iterator b, Iterator e, const char *delimiter = " ") { auto s(b); while (b != e) os << (b != s ? delimiter : "") << *b++; return os; }
template <typename T, std::size_t N> inline std::ostream &print(std::ostream &os, const std::array<T, N> &array, const char *delimiter = " ") { return print(os, array.begin(), array.end()); }

// paths

template <typename RNG> inline double rand_0_1(RNG &rng)
{
    return rng() / (double(rng.max()) - double(rng.min()));
}

namespace
{
inline std::string terminate_path(std::string_view path)
{
    if (path.empty() || path.ends_with("/")) {
        return std::string(path);
    }
    std::string p(path);
    return p + "/";
}
}

inline std::filesystem::path path_replace_stem(std::filesystem::path path, std::string_view stem)
{
    if (path.extension().empty()) {
        path.replace_filename(stem);
        return path;
    }
    return path.replace_filename(std::string(stem) + path.extension().string());;
}

template <typename T>
std::filesystem::path path_append_stem(std::filesystem::path path, const T &append, std::string_view delimiter = "_")
{
    std::ostringstream s;
    s << path.stem().string() << delimiter << append;
    return path_replace_stem(path, s.str());
}

inline std::filesystem::path filename_append_id(std::string_view path, std::string_view base_file, std::string_view extension, int appendable = -1)
{
    std::filesystem::path fp(terminate_path(path));
    std::string fn(base_file);
    fn += ".";
    fn += extension;
    fp.replace_filename(fn);
    if (appendable == -1) {
        return fp;
    }
    std::cout << "filename " << fp << "\n";
    return path_append_stem(fp, appendable, "_");
}
