#include <array>
#include <algorithm>

template <typename T> inline std::pair<T, T> range_intersect(const std::pair<T, T> &a, const std::pair<T, T> &b) { return std::pair<T, T>(std::max(a.first, b.first), std::min(a.second, b.second)); }
template <typename T> inline std::pair<T, T> range_union    (const std::pair<T, T> &a, const std::pair<T, T> &b) { return std::pair<T, T>(std::min(a.first, b.first), std::max(a.second, b.second)); }
template <typename T> inline auto            range_size     (const std::pair<T, T> &a) { return a.second - a.first; }
