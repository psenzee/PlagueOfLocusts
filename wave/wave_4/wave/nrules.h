#pragma once

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <bitset>
#include "nrule.h"

namespace wave {

template <std::size_t N, typename IndexType>
struct nrules
{
    typedef nrule<N, IndexType>      rule_t;
    typedef std::bitset<N>           bitset_t;
    typedef std::array<IndexType, N> bit_list_t;
    
    std::array<rule_t, N>   _rules;
    histogram<N>            _counts;
    
    nrules()
    {
        for (IndexType i = 0; i < _rules.size(); ++i) {
            _rules[i].set_key(i);
        }
    }

    template <typename T> void sample(const grid::readable<T> &grid)
    {
        auto sz = grid.size();
        for (point2i p(1, 1); p.y < sz.h - 1; ++p.y) {
            for (p.x = 0; p.x < sz.w - 1; ++p.x) {
                IndexType v = grid.get(p);
                if (v < N) {
                    _counts.increment(v);
                    _rules[v].sample(grid, p);
                }
            }
        }
    }
    
    bitset_t applicable_rules(const grid::readable<bitset_t> &grid, const point2i &at)
    {
        auto list = list_from_bitset<IndexType>(grid.get(at));
        bitset_t result(false);
        for (const auto &item: list) {
            const auto &rule = _rules[item];
            if (rule.applicable_count(grid, at)) {
                result.set(item);
            }
        }
        return result;
    }
    
    void clear_under(double threshold_ratio, std::size_t threshold_count)
    {
        for (auto &rule: _rules) {
            rule.clear_under(threshold_ratio, threshold_count);
        }
    }
    
    std::ostream &print(std::ostream &os) const
    {
        os << "total counts: ";
        _counts.print(os) << "\n";
        for (std::size_t i = 0; i < _rules.size(); ++i) {
            if (_rules[i].get_count()) {
                _rules[i].print(os);
            }
        }
        return os;
    }
};

}
