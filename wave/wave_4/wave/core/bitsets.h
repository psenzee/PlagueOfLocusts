#pragma once

#include <iostream>
#include <array>
#include <bitset>
#include <random>

template <std::size_t N>
struct bitset_enumerator
{
    typedef std::bitset<N> bitset_t;
    
    constexpr bitset_enumerator(const bitset_t &u) : _data(u), _i(0) { _run(); }

    constexpr bool        has()  const { return _i < N; }
    constexpr std::size_t next()       { auto r = _i; if (_i < N) { _i++; _run(); } return r; }

public:

    bitset_t    _data;
    std::size_t _i;
    
    constexpr void _run() { for (; _i < N && !_data.test(_i); _i++) ; }
};

template <std::size_t N> bool bitset_is_superset(const std::bitset<N> &superset, const std::bitset<N> &subset)
{
    return (superset & subset) == subset;
}

template <std::size_t N, typename RNG>
std::bitset<N> bitset_init_random(RNG &rng)
{
    std::bitset<N> data(0);
    for (std::size_t i = 0; i < N; i++) {
        if (rng() & 1) {
            data.set(i);
        }
    }
    return data;
}

template <std::size_t N>
std::size_t bitset_first(const std::bitset<N> &v)
{
    for (std::size_t i = 0; i < N; ++i) {
        if (v.test(i)) {
            return i;
        }
    }
    return N;
}

template <std::size_t N>
std::size_t bitset_last(const std::bitset<N> &v)
{
    for (ssize_t i = N - 1; i >= 0; --i) {
        if (v.test(i)) {
            return i;
        }
    }
    return N;
}

template <std::size_t N>
std::ostream &bitset_print(std::ostream &os, const std::bitset<N> &v)
{
    auto en = bitset_enumerator(v);
    bool first = true;
    while (en.has()) {
        os << (!first ? " " : "") << en.next();
        first = false;
    }
    return os;
}

template <std::size_t N, typename ListType = uint16_t>
std::ostream &bit_list_print(std::ostream &os, const std::array<ListType, N> &v)
{
    bool first = true;
    for (const auto &i: v) {
        if (i >= N) {
            break;
        }
        os << (!first ? " " : "") << i;
        first = false;
    }
    return os;
}

namespace {
template <std::size_t N, typename ListType = uint16_t>
auto _bit_list_normalize(std::array<ListType, N> &list)
{
    return std::partition(list.begin(), list.end(), [](const auto &v) { return v < N; });
}
}

template <std::size_t N, typename ListType = uint16_t>
void bit_list_normal_form(std::array<ListType, N> &list)
{
    auto b = list.begin(), e = list.end();
    std::sort(b, (e = _bit_list_normalize(list)));
    for (auto i = std::unique(b, e); i != e; ++i) {
        *i = N;
    }
}

template <std::size_t N, typename RNG, typename ListType = uint16_t>
void bit_list_shuffle(std::array<ListType, N> &list, RNG &rng)
{
    std::shuffle(list.begin(), list.end(), rng);
    _bit_list_normalize(list);
}

template <std::size_t N, typename ListType = uint16_t>
std::size_t bit_list_count(const std::array<ListType, N> &list)
{
    for (std::size_t i = 0; i < N; i++) {
        if (list[i] >= N) {
            return i;
        }
    }
    return N;
}

template <std::size_t N, typename ListType = uint16_t>
void bit_list_remove_nth(std::array<ListType, N> &list, std::size_t n)
{
    list[n] = N;
    _bit_list_normalize(list);
}

template <std::size_t N, typename RNG, typename ListType = uint16_t>
void bit_list_remove_random(std::array<ListType, N> &list, RNG &rng)
{
    bit_list_remove_nth(list, rng() % bit_list_count(list));
}

template <std::size_t N, typename ListType = uint16_t>
void bit_list_insert(std::array<ListType, N> &list, std::size_t i)
{
    list[bit_list_count(list)] = i;
    _bit_list_normalize(list);
}

template <std::size_t N, typename ListType = uint16_t>
std::array<ListType, N> list_from_bitset(const std::bitset<N> &set)
{
    std::array<ListType, N> list;
    std::size_t n = 0;
    for (std::size_t i = 0; i < N; i++) {
        if (set.test(i)) {
            list[n++] = i;
        }
    }
    for (std::size_t i = n; i < N; i++) {
        list[i] = N;
    }
    return list;
}

template <std::size_t N, typename ListType = uint16_t>
std::bitset<N> bitset_from_list(const std::array<ListType, N> &list)
{
    std::bitset<N> set(false);
    for (auto i: list) {
        if (i < N && !set.test(i)) {
            set.set(i);
        }
    }
    return set;
}

int bitset_test_main();
