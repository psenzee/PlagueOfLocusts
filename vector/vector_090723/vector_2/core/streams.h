#pragma once

#include <iostream>

template <typename T0, typename T1> inline std::ostream &operator<<(std::ostream &os, const std::pair<T0, T1> &p) { return os << p.first << ":" << p.second; }

template <typename Vector>
std::ostream &__stream_out(std::ostream &os, const Vector &data, size_t max_count)
{
    bool first = true;
    os << "[";
    size_t count = 0;
    for (auto item : data) {
        if (max_count == 0 || count < max_count) {
            if (!first) os << " ";
            first = false;
            os << item;
        } else if (max_count != 0 && count >= max_count && data.size() > max_count) {
            os << " ...";
            break;
        }
        count++;
    }
    return os << "]";
}

template <typename Value>           std::ostream &_stream_out(std::ostream &os, const std::vector<Value>   &data, size_t max_count = 0) { return __stream_out(os, data, max_count); }
template <typename Value, size_t N> std::ostream &_stream_out(std::ostream &os, const std::array<Value, N> &data, size_t max_count = 0) { return __stream_out(os, data, max_count); }

template <typename Value>           inline std::ostream &operator<<(std::ostream &os, const std::vector<Value> &data)   { return _stream_out(os, data); }
template <typename Value, size_t N> inline std::ostream &operator<<(std::ostream &os, const std::array<Value, N> &data) { return _stream_out(os, data); }

template <typename Value>
struct valid_t
{
    Value data;
    bool  valid;

    inline valid_t() : valid(false) {}
    inline valid_t &operator=(const Value &v) { if (&v != &data) { data = v; valid = true; } return *this; }
    inline       Value &operator*()       { return data; }
    inline const Value &operator*() const { return data; }
    
    inline void invalidate() { valid = false; }
    
    inline std::ostream &print(std::ostream &os) const
    {
        if (!valid) return os << "[uncalculated]";
        return os << data;
    }
};

template <typename Value> inline std::ostream &operator<<(std::ostream &os, const valid_t<Value> &v) { return v.print(os); }
