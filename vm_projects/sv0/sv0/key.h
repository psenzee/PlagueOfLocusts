#pragma once

#include <string>
#include <string_view>
#include <compare>

struct key
{
    size_t      hash;
    std::string data;

    key(const std::string &data, size_t hash) : data(data), hash(hash) {}
    key(std::string_view data, size_t hash)   : data(data), hash(hash) {}
    key(const std::string &data)              : data(data), hash(hasher(data)) {}
    key(std::string_view data)                : data(data), hash(hasher(data)) {}

    // Copy constructor
    key(const key &other) : hash(other.hash), data(other.data) {}

    // Move constructor
    key(key&& other) noexcept : hash(other.hash), data(std::move(other.data)) {}

    // Copy assignment operator
    key& operator=(const key& other)
    {
        if (this != &other) {
            hash = other.hash;
            data = other.data;
        }
        return *this;
    }

    // Move assignment operator
    key& operator=(key&& other) noexcept
    {
        if (this != &other) {
            hash = other.hash;
            data = std::move(other.data);
        }
        return *this;
    }

    // Defaulted spaceship operator
    auto operator<=>(const key&) const = default;
    
    template <typename T>
    static size_t hasher(const T &s)
    {
        std::hash<T> hasher;
        return hasher(s);
    }
};

// Hash specialization for std::hash
namespace std {
    template<>
    struct hash<key>
    {
        std::size_t operator()(const key &k) const noexcept
        {
            return k.hash;  // Return the precomputed hash member
        }
    };
}
