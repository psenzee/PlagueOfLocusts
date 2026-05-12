#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <ctime>
#include <queue>
#include <random>
#include <algorithm>
#include <cstring>
#include <bitset>
#include "bitsets.h"
#include "rule.h"
#include "grid.h"
#include "patch.h"
#include "identifier_table.h"

namespace wave {

template <size_t N>
struct rules
{
    typedef rules<N>                          rules_t;
    typedef rule<N>                           rule_t;
    typedef std::bitset<N>                    options_t;
    typedef std::array<size_t, N>             bit_list_t;
    typedef grid::grid<options_t>             grid_t;
    typedef grid::readable<options_t>         readable_t;
    typedef typename rule_t::patch_t          patch_t;
    typedef identifier_table<N>               table_t;
    
    std::vector<rule_t>    _rules;
    table_t                _table;
    options_t              _max_entropy;
    double                 _weight_total;
    std::vector<size_t>    _ordered;
    std::vector<options_t> _distribution;

    inline rules() : _weight_total(0.0) { _rules.resize(N); }
    
    inline const options_t &get_max_entropy() const { return _max_entropy; }
    
    inline const options_t &removable_bits_from_random(size_t random) const { return _distribution[random % _distribution.size()]; }

    options_t train(const uint8_t *map, size_t width, size_t height)
    {
        _rules.resize(N);
        for (point2i p(1, 1); p[1] < height - 1; p[1]++) {
            for (p[0] = 1; p[0] < width - 1; p[0]++) {
                size_t at = p[1] * width + p[0];
                _rules[map[at]].add_example(_table, map, width, p);
            }
        }
        for (const rule_t &r : _rules)
            _max_entropy |= r.get_max_entropy();
        auto *tm = training_map(_table, map, width, height);
        for (point2i p(1, 1); p[1] < height - 1; p[1]++) {
            for (p[0] = 1; p[0] < width - 1; p[0]++) {
                auto pat = patch(tm->get(p));
                or_patch(_table, pat, *tm, p);
            }
        }
        delete tm;
        if (_ordered.size() == 0) {
            for (size_t i = 0; i < N; i++) {
                _ordered.push_back(i);
            }
        }
        update_weights();
      //set_diagonals(_max_entropy);
        return _max_entropy;
    }
    
    patch_t patch(const options_t &options) const
    {
        // combine rule patches
        patch_t patch;
        bitset_enumerator en(options);
        while (en.has()) {
            const auto r = _rules[en.next()];
            patch.combine(r._patch);
        }
        return patch;
    }
    
    std::ostream &print(std::ostream &os) const
    {
        for (const auto &r : _rules) {
            if (r._identifier)
                r.print(os) << std::endl;
        }
        os << "weight total " << _weight_total << std::endl;
        for (size_t id : _ordered) {
            const auto &r = _rules[id];
            if (!r._identifier)
                continue;
            double w = r._weight / _weight_total;
            os << "#" << std::setw(3) << r._identifier << " norm weight " << std::fixed << std::setw(10) << std::setprecision(6) << w << " " << (1.0 / w) << std::endl;
        }
        return os;
    }
    
    void set_diagonals(const options_t &options)
    {
        for (auto &rule : _rules) {
            rule.set_diagonals(options);
        }
    }
    
private:
    
    struct weight_comparator
    {
        const rules_t &_rules;
        
        inline weight_comparator(const rules_t &rules) : _rules(rules) {}
        
        inline bool operator()(const size_t &a, const size_t &b) const
        {
            if (a == b) {
                return false;
            }
            double wa = _rules._rules[a]._weight, wb = _rules._rules[b]._weight;
            return wa > wb; // descending
        }
    };
    
    void update_weights()
    {
        weight_comparator c(*this);
        std::sort(_ordered.begin(), _ordered.end(), c);
        double max_inverted = 0, min_inverted = double(~uint32_t(0));
        for (size_t id : _ordered) {
            const auto &r = _rules[id];
            if (r._identifier) {
                double inv_w = 1.0 / (r._weight / _weight_total);
                max_inverted = std::max(inv_w, max_inverted);
                min_inverted = std::min(inv_w, min_inverted);
            }
        }
        max_inverted /= min_inverted;
        size_t mi = size_t(ceil(max_inverted));
        std::cout << "distribution size " << mi << std::endl;
        _distribution.resize(mi);
        for (size_t id : _ordered) {
            const auto &r = _rules[id];
            if (!r._identifier)
                continue;
            double inv_w = 1.0 / (r._weight / _weight_total);
            double w = inv_w / min_inverted;
            //std::cout << "id " << size_t(w) << " count " << _distribution.size() << std::endl;
            _distribution[size_t(w)].set(id);
        }
        options_t last;
        for (int i = int(_distribution.size()) - 1; i >= 0; --i) {
            if (last.none()) {
                last = _distribution[i];
            }
            if (_distribution[i].none()) {
                _distribution[i] = last;
            }
            last = _distribution[i];
            //std::cout << "i " << i << " " << _distribution[i] << std::endl;
        }
    }
    
    grid::basic<options_t> *training_map(const table_t &table, const uint8_t *map, size_t width, size_t height)
    {
        grid::basic<options_t> *grid = new grid::basic<options_t>(size2u { width, height });
        for (point2i p { 0, 0 }; p[1] < height; p[1]++) {
            for (p[0] = 0; p[0] < width; p[0]++) {
                size_t id = map[p[1] * width + p[0]];
                grid->set(p, table.get(id));
            }
        }
        return grid;
    }
    
    void add_weight(size_t id, double weight) { _rules[id].add_weight(weight); _weight_total += weight; }

    void or_patch(const table_t &lookup, patch_t &patch, const readable_t &grid, const point2i &p, double weight_multiplier = 1.0)
    {
        point2i r(p - patch_t::CENTER);
        size_t at = 0;
        for (point2i q { 0, 0 }; q[1] < patch.size().h; q[1]++) {
            for (q[0] = 0; q[0] < patch.size().w; q[0]++) {
                auto options = grid.get(r + q);
                auto list = list_from_bitset(options);
                auto count = options.count();
                double weight = (1.0 / count) * weight_multiplier;
                for (size_t i = 0; i < count; i++) {
                    size_t id = list[i];
                    add_weight(id, weight);
                    _rules[id]._patch._data[at] |= lookup.get(id);
                }
                at++;
            }
        }
    }
};

}
