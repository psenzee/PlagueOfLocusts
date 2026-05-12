#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <algorithm>
#include <bitset>
#include "std.h"
#include "bitsets.h"

template <std::size_t N, typename IndexType = uint16_t>
struct histogram
{
    typedef IndexType num_t;

    typedef std::array<uint32_t, N> count_list_t;
    typedef std::bitset<N>          bitset_t;
    typedef std::pair<num_t, num_t> pair_t;
    typedef std::array<pair_t, N>   pair_list_t;

    constexpr histogram() : _counts({ 0 }), _total(0), _sorted({ pair_t({ 0, 0 }) }), _finalized(true) {}

    inline void         reset()                               { _counts = { 0 }; _total = 0; _finalized = true; }
    inline std::size_t  count(num_t v)                        { return _counts[v]; }
    inline double       ratio(num_t v)                  const { _finalize(); return _total ? (double(_counts[v]) / _total) : 0.; }
    inline void         increment(num_t v)                    { _counts[v]++; _total++; _finalized = false; }
    inline void         increment(const bitset_t &bits)       { increment(list_from_bitset<num_t>(_bits)); }
    inline void         decrement(num_t v)                    { _counts[v]--; _total--; _finalized = false; }
    inline void         decrement(const bitset_t &bits)       { decrement(list_from_bitset<num_t>(_bits)); }
    inline void         clear(num_t v)                        { _total -= _counts[v]; _counts[v] = 0; _finalized = false; }
    inline void         clear(const bitset_t &bits)           { clear(list_from_bitset<num_t>(_bits)); }
    inline count_list_t counts()                        const { return _counts; }
    inline std::size_t  total()                         const { return _total; }
    inline bitset_t     bits()                          const { _finalize(); return _bits; }
    inline pair_list_t  sorted()                        const { _finalize(); return _sorted; }
    
    template <typename T> inline void increment(const std::array<T, N> &list) { for (const auto &i: list) { if (i < N) { increment(i); } } }
    template <typename T> inline void decrement(const std::array<T, N> &list) { for (const auto &i: list) { if (i < N) { decrement(i); } } }
    template <typename T> inline void clear(const std::array<T, N> &list)     { for (const auto &i: list) { if (i < N) { clear(i); } } }
    
    template <typename T> inline std::array<T, N> bit_list() const
    {
        std::array<T, N> list;
        num_t n = 0;
        for (num_t i = 0; i < _counts.size(); ++i) {
            if (_counts[i] != 0) {
                list[n++] = i;
            }
        }
        for (; n < _counts.size(); ++n) {
            list[n] = N;
        }
        return list;
    }

    template <typename RNG> inline std::size_t random_weighted(RNG &rng) const
    {
        _finalize();
        if (!_total) {
            return 0;
        }
        double sum = 0, target = rand_0_1(rng);
        for (num_t i = 0; i < _sorted.size() - 1; ++i) {
            const auto &pair = _sorted[i];
            sum += double(pair.second) / _total;
            if (target < sum || pair.second == 0) {
                return pair.first;
            }
        }
        return _sorted.last().first;
    }
    
    void clear_under(double threshold_ratio, std::size_t threshold_count)
    {
        std::vector<num_t> list;
        for (num_t i = 0; i < _counts.size(); ++i) {
            if (_counts[i] && (ratio(i) < threshold_ratio || _counts[i] < threshold_count)) {
                list.push_back(i);
            }
        }
        for (const auto &index: list) {
            clear(index);
        }
        _finalize();
    }
    
    std::ostream &print(std::ostream &os) const
    {
        _finalize();
        if (!total()) {
            return os << "[empty]";
        }
        os << "[" << std::fixed << std::setw(2) << _total << "]";
        for (num_t i = 0; i < _sorted.size(); ++i) {
            const auto &pair = _sorted[i];
            if (pair.second) {
                os << " (" << std::fixed << std::setw(3) << pair.first << ")"
                   << " " << std::fixed << std::setprecision(2) << ratio(pair.first);
            }
        }
        return os;
    }

private:
    
    count_list_t        _counts;
    std::size_t         _total;

    mutable bitset_t    _bits;
    mutable pair_list_t _sorted;
    mutable bool        _finalized;
    
    void _finalize() const
    {
        if (!_finalized) {
            for (num_t i = 0; i < N; i++) {
                _sorted[i] = pair_t(i, _counts[i]);
            }
            std::sort(_sorted.begin(), _sorted.end(), [](const auto &a, const auto &b) {
                if (a.second == b.second) {
                    return a.first > b.first;
                }
                return a.second > b.second;
            });
            _bits = bitset_from_list(bit_list<num_t>());
            _finalized = true;
        }
    }
};
