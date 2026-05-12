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
#include "bits.h"
#include "bit_array.h"
#include "grid.h"
#include "patch.h"

namespace wave {

template <size_t N>
struct identifier_table
{
    typedef bit_array<N>             options_t;
    typedef std::array<options_t, N> data_t;
    
    data_t _data;
    
    inline identifier_table() : _data({{ 0 }}), _is_special({ 0 }) { _init(); }
    
    inline const options_t &get(size_t index) const { return _data[index]; }
    
    inline bool is_special(size_t index) const { return _is_special[index]; }
    
private:
    
    struct special_t
    {
        size_t id;
        size_t include[N];
    };
    
    constexpr static const special_t _specials[] = {
        { 112, { 4, 5, 6, 43, 44, 45, 0 } },                                  // grass, swamp, farm & small brush
        { 113, { 6, 8, 9, 10, 0 } },                                          // trees & brush
        { 114, { 14, 15, 0 } },                                               // foothills
        { 115, { 11, 12, 13, 0 } },                                           // mountains
        { 116, { 114, 115, 0 } },                                             // mountains and foothills
        { 117, { 112, 113, 116, 0 } },                                        // general terrain types
        { 118, { 32, 33, 34, 35, 36, 37, 38, 0 } },                           // path types
    };
    
    bit_array<N> _is_special;
    
    void _init()
    {
        for (size_t i = 0; i < N; i++) {
            _data[i] = false;
            _data[i].set(i);
        }
        if (true) {
            size_t count = sizeof(_specials) / sizeof(_specials[0]);
            for (size_t i = 0; i != count; i++) {
                const special_t &s = _specials[i];
                options_t op(false);
                _is_special.set(i);
                for (size_t j = 0; s.include[j]; j++)
                    op |= get(s.include[j]);
                _data[s.id] = op;
                std::cout << "identifer " << s.id << " count " << op.count() << std::endl;
            }
        }
    }
};

template <size_t N>
struct rule
{
    typedef patch<3, 3>              patch_t;
    typedef bit_array<N>             options_t;
    typedef base_grid<options_t>     grid_t;
    typedef readable_grid<options_t> readable_grid_t;
    typedef identifier_table<N>      table_t;
    
    size_t    _identifier;
    patch_t   _patch;
    options_t _max_entropy;
    double    _weight;
    
    inline rule() : _identifier(0), _max_entropy(0), _weight(0.0) { options_t o; _patch.clear(o); }
    
    inline rule(const rule &r) : _identifier(r._identifier), _patch(r._patch) {}
    
    inline const options_t &get_max_entropy() const { return _max_entropy; }
    
    inline void add_weight(double weight) { _weight += weight; }
    
    std::ostream &print(std::ostream &os) const
    {
        os << "#" << _identifier << ": ";
        for (size_t i = 0; i < point2i::MAX_DIRECTIONS; i++)
            _patch.get(_patch.CENTER.direction_offset(i)).print(os) << " ";
        return os << "weight: " << _weight << std::endl;
    }

    void add_example(const table_t &lookup, const uint8_t *map, size_t stride, const point2i &p)
    {
        _identifier = map[p.y * stride + p.x];
        or_patch(lookup, _patch, map, stride, p);
    }

    void set_diagonals(const options_t &options)
    {
        size_t indices[] = { point2i::NORTHWEST, point2i::NORTHEAST, point2i::SOUTHEAST, point2i::SOUTHWEST };
        for (size_t i = 0; i < 4; i++)
            _patch.set(_patch.CENTER.direction_offset(indices[i]), options);
    }

private:

    void or_patch(const table_t &lookup, patch_t &patch, const uint8_t *map, size_t stride, const point2i &p)
    {
        point2i r(p - patch_t::CENTER);
        size_t at = 0;
        auto center = lookup.get(map[r.y * stride + r.x]);
        for (point2i q(0, 0); q.y < patch.size().h; q.y++) {
            for (q.x = 0; q.x < patch.size().w; q.x++) {
                uint8_t id = map[(q.y + r.y) * stride + (q.x + r.x)];
                auto options = lookup.get(id);
                _max_entropy |= options;
                _patch._data[at++] |= options;
            }
        }
    }

};

template <size_t N>
struct rules
{
    typedef rules<N>                          rules_t;
    typedef rule<N>                           rule_t;
    typedef bit_array<N>                      options_t;
    typedef typename bit_array<N>::bit_list_t bit_list_t;
    typedef base_grid<options_t>              grid_t;
    typedef readable_grid<options_t>          readable_grid_t;
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
        for (point2i p(1, 1); p.y < height - 1; p.y++) {
            for (p.x = 1; p.x < width - 1; p.x++) {
                size_t at = p.y * width + p.x;
                _rules[map[at]].add_example(_table, map, width, p);
            }
        }
        for (const rule_t &r : _rules)
            _max_entropy |= r.get_max_entropy();
        auto *tm = training_map(_table, map, width, height);
        for (point2i p(1, 1); p.y < height - 1; p.y++) {
            for (p.x = 1; p.x < width - 1; p.x++) {
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
        for (ssize_t i = 0; (i = options.find_next_set(i)) != -1; i++) {
            const auto r = _rules[i];
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
            if (last.empty()) {
                last = _distribution[i];
            }
            if (_distribution[i].empty()) {
                _distribution[i] = last;
            }
            last = _distribution[i];
            //std::cout << "i " << i << " " << _distribution[i] << std::endl;
        }
    }
    
    base_grid<options_t> *training_map(const table_t &table, const uint8_t *map, size_t width, size_t height)
    {
        base_grid<options_t> *grid = new base_grid<options_t>(size2u(width, height));
        for (point2i p(0, 0); p.y < height; p.y++) {
            for (p.x = 0; p.x < width; p.x++) {
                size_t id = map[p.y * width + p.x];
                grid->set(p, table.get(id));
            }
        }
        return grid;
    }
    
    void add_weight(size_t id, double weight) { _rules[id].add_weight(weight); _weight_total += weight; }

    void or_patch(const table_t &lookup, patch_t &patch, const readable_grid_t &grid, const point2i &p, double weight_multiplier = 1.0)
    {
        typedef typename options_t::bit_list_t list_t;
        point2i r(p - patch_t::CENTER);
        size_t at = 0;
        for (point2i q(0, 0); q.y < patch.size().h; q.y++) {
            for (q.x = 0; q.x < patch.size().w; q.x++) {
                auto options = grid.get(r + q);
                list_t list;
                size_t count = options.list_set_bits(list);
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
