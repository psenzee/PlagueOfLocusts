#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <deque>
#include <ctime>
#include <queue>
#include <random>
#include <algorithm>
#include <cstring>
#include <ctime>
#include "bits.h"
#include "bit_array.h"
#include "map_data.h"
#include "grid.h"
#include "base_grid.h"
#include "bool_grid.h"
#include "rules.h"
#include "patch.h"
#include "std_array.h"
#include "std.h"
#include "color.h"

#include "bit_container.h"
#include "bit_array_core.h"

namespace wave {

typedef bit_array<256> options_t;

inline int point_rand(const point2i &p, int seed = 0)
{
    return (std::minstd_rand(p.x * 65535 + p.y + seed))();
}

template <size_t N>
struct point_options_count_desc_comparator
{
    typedef bit_array<N>    options_t;
    typedef grid<options_t> grid_t;
    
    const grid_t &_grid;
    
    inline point_options_count_desc_comparator(const grid_t &grid) : _grid(grid) {}
    
    inline bool operator()(const point2i &a, const point2i &b) const
    {
        if (&a == &b || a == b) {
            return false;
        }
        size_t ca = _grid.get(a).count(), cb = _grid.get(b).count();
        return ca == cb ? a > b : ca > cb;
    }
};

template <size_t N>
struct point_options_count_asc_comparator
{
    typedef bit_array<N>    options_t;
    typedef grid<options_t> grid_t;
    
    const grid_t &_grid;
    
    inline point_options_count_asc_comparator(const grid_t &grid) : _grid(grid) {}
    
    inline bool operator()(const point2i &a, const point2i &b) const
    {
        if (&a == &b || a == b) {
            return false;
        }
        size_t ca = _grid.get(a).count(), cb = _grid.get(b).count();
        return ca == cb ? a < b : ca < cb;
    }
};

struct point_queue
{
    struct elem
    {
        point2i point;
        size_t  count;
        size_t  rand;
        
        inline bool operator< (const elem &o) const { return count != o.count ? count < o.count : (rand != o.rand ? rand < o.rand : point < o.point); }
        inline bool operator> (const elem &o) const { return count != o.count ? count > o.count : (rand != o.rand ? rand > o.rand : point > o.point); }
        inline bool operator==(const elem &o) const { return point == o.point && count == o.count && rand == o.rand; }
        inline bool operator<=(const elem &o) const { return !(*this > o); }
        inline bool operator>=(const elem &o) const { return !(*this < o); }
    };
    
    typedef std::priority_queue< elem, std::vector<elem>, std::greater<elem> > priority_queue_t;
    
    inline void        push(const elem &e)                                       { if (e.count > 1) _queue.push(e); }
    inline void        push(const point2i &p, size_t count, size_t rand = 0)     { push(elem { p, count, rand }); }
    inline elem        pop()                                                     { elem e = _queue.top(); _queue.pop(); return e; }
    inline const elem &top()                                               const { return _queue.top(); }
    inline bool        empty()                                             const { return _queue.empty(); }
    inline size_t      size()                                              const { return _queue.size(); }
    
private:

    priority_queue_t _queue;
};

template <size_t N>
struct availability_list
{
    typedef bit_array<N>             options_t;
    typedef grid<options_t>          grid_t;
    typedef base_grid<options_t>     base_grid_t;
    typedef std::deque<point2i>      points_t;
    typedef rules<N>                 rules_t;
    typedef std::vector<point2i>     paths_t;
    typedef patcher<options_t, 3, 3> patched_t;
    typedef patch<3, 3>              patch_t;
    
    grid_t         &_grid;
    point_queue     _points;
    std::mt19937    _mt;
    
    inline availability_list(grid_t &grid, const std::mt19937 &mt) : _grid(grid), _mt(mt) {}
    
    void initialize_with_grid()
    {
        size2u sz(_grid.size());
        for (point2i p(0, 0); p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                enqueue(p);
            }
        }
    }
    
    void enqueue(const point2i &p)
    {
        _points.push(p, _grid.get(p).count(), _mt());
    }
    
    size_t size() const { return _points.size(); }
    
    bool empty()
    {
        _clear_finished();
        return _points.empty();
    }

    bool next(point2i &p)
    {
        if (empty()) {
            return false;
        }
        p = _points.pop().point;
        return true;
    }
private:

    void _clear_finished()
    {
        while (!_points.empty()) {
            auto e = _points.top();
            size_t count = _grid.get(e.point).count();
            if (e.count == count && count > 1)
                break;
            _points.pop();
            if (count > 1) {
                // here requeue any that have had their count updated
                enqueue(e.point);
            }
        }
    }
};

template <size_t N>
struct state
{
    typedef bit_array<N>                   options_t;
    typedef typename options_t::bit_list_t bit_list_t;
    typedef grid<options_t>                grid_t;
    typedef base_grid<options_t>           base_grid_t;
    typedef std::deque<point2i>            points_t;
    typedef rules<N>                       rules_t;
    typedef patcher<options_t, 3, 3>       patched_t;
    typedef patch<3, 3>                    patch_t;
    
    grid_t                 &_grid;
    const rules_t          &_rules;
    options_t               _max_entropy;
    std::mt19937            _mt;

    bool_grid               _resolve;
    std::deque<point2i>     _points;
    
    state(grid_t &grid, const rules_t &rules, const std::mt19937 &mt, const point2i &initial_point = point2i(0, 0))
        : _grid(grid), _rules(rules), _resolve(_grid.size()), _mt(mt) {}

    bool reduce(const point2i &p)
    {
        return _reduce(p, options_t(true));
    }
    
    bool process(const point2i &q)
    {
        if (!_process(q)) {
            return false;
        }
        point2i p;
        while ((p = find_first_set_point(_resolve)).x != -1) {
            if (!_process(p)) {
                std::cout << "!process" << std::endl;
            }
        }
        return false;
    }

private:

    bool can_apply_one(grid_t &grid, const point2i &p, const patch_t &patch)
    {
        // test if any intersections are zero
        for (size_t i = 0; i < point2i::MAX_DIRECTIONS; i++) {
            auto a = p.direction_offset(i), b = patch.CENTER.direction_offset(i);
            if (inside(grid, a)) {
                if ((grid.get(a) & patch.get(b)).count() == 0)
                    return false;
            }
        }
        return true;
    }
    
    void apply_one(grid_t &grid, const point2i &p, const patch_t &patch)
    {
        for (size_t i = 0; i < point2i::MAX_DIRECTIONS; i++) {
            auto a = p.direction_offset(i), b = patch.CENTER.direction_offset(i);
            if (inside(grid, a)) {
                auto prev = grid.get(a), value = prev & patch.get(b);
                grid.set(a, value);
                if (p == a || value.count() <= 1) {
                    _resolve.set(a, false);
                } else if (value != prev && value.count() > 1) {
                    _resolve.set(a, true);
                }
            }
        }
    }

    bool _process(const point2i &p)
    {
        //std::cout << "p " << p << std::endl;
        _resolve.set(p, false);
        options_t primary(_grid.get(p));
        if (primary == _rules.get_max_entropy()) {
            return true;
        }
        size_t count = primary.count();
        if (!count) {
            return false;
        }
        primary = _valid_options(p, primary);
        if (primary.empty()) {
            std::cout << "failed" << std::endl;
            return false;
        }
        apply_one(_grid, p, _rules.patch(primary));
        return true;
    }
    
    bool _evaluate(const point2i &p, options_t options)
    {
        options_t o = _grid.get(p);
        size_t count = o.count();
        if (count <= 1) {
            return (count == 1);
        }
        options_t out = _valid_options(p, o & options);
        count = out.count();
        if (count != 0 && (count == 1 || out != (o & options) || o != (o & options))) {
            _grid.set(p, out);
            _resolve.set(p, false); // should this be here?
            return true;
        }
        return false;
    }

    bool _reduce(const point2i &p, options_t options)
    {
        if (_evaluate(p, options)) {
            return true;
        }

        // remove randomly from the least likely bits first
        options_t o(options & _grid.get(p));
        
        for (size_t tries = 16; tries > 0; ) {
            options_t r(_rules.removable_bits_from_random(_mt()));
            options_t nr = ~r & o;
            if (nr != o) {
                if (_evaluate(p, nr)) {
                    return true;
                }
                tries--;
            }
        }

        // failing that..
        // reduce the options by a different bit each time and reevaluate
        bit_list_t arr;
        size_t count = shuffled_bit_list(o, arr, _mt);
        for (size_t i = 0; i < count; i++) {
            auto copy = arr;
            copy[i] = arr[c - 1]; // just replace the ith value with the last
            if (_evaluate(p, options_t(copy, count - 1))) {
                return true;
            }
        }
        return false;
    }

    options_t _valid_options(const point2i &p, const options_t &options)
    {
        // select only the bits that result in a valid state
        bit_list_t list;
        size_t count = options.list_set_bits(list);
        options_t out;
        for (size_t i = 0; i < count; i++) {
            options_t bits;
            bits.set(list[i]);
            auto patch = _rules.patch(bits);
            if (can_apply_one(_grid, p, patch))
                out |= bits;
        }
        return out;
    }
};

void print_final(const grid<options_t> &grid, std::ostream &os, bool bracket = true)
{
    point2i p(0, 0);
    os << "[" << std::endl << std::endl;
    size2u sz(grid.size());
    int pad = bracket ? 5 : 3;
    for (p.y = 0; p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++) {
            auto o = grid.get(p);
            auto count = o.count();
            if (count == 1) {
                auto value = o.find_next_set();
                os << std::fixed << std::setw(pad) <<
                std::setfill(' ') << value << ",";
            } else {
                if (bracket) {
                    os << std::fixed << "[" << std::setw(pad + 2) <<
                    std::setfill(' ') << count << "],";
                } else {
                    os << std::fixed << std::setw(pad) <<
                    std::setfill(' ') << 0 << ",";
                }
            }
        }
        os << std::endl;
    }
    os << std::endl << "]";
}

}

int wave_main(int argc, const char * argv[])
{
    //std::random_device rd;
    std::mt19937 mt(100);

    wave::base_grid<wave::options_t> grid(size2u(50, 50));
    wave::rules<256> r;

    r.train(map100x256_0, 100, 100);
//  r.train(map100x256_1, 100, 256);
//  r.train(map100x100_2, 100, 100);
//  r.train(map100x100_3, 100, 100); // this one causes failure -- all zeros
//  r.train(map100x100_4, 100, 100);
 // r.train(map25x25_rivers_paths, 25, 25);
//  r.train(map25x25_0, 25, 25);
//  r.train(map100x50_0, 100, 50);
    wave::clear(grid, r.get_max_entropy());
    r.print(std::cout);

    point2i q;
    wave::availability_list<256> list(grid, mt);
    list.initialize_with_grid();
    
    wave::state<256> st(grid, r, mt);
    std::vector<point2i> list_;
    do {
        for (const auto &p : list_) {
            list.enqueue(p);
        }
        list_.clear();
        size_t count_popped = 0, count_pushed = 0;
        while (list.next(q)) {
            count_popped++;
            if (!st.reduce(q)) {
                continue;
            }
            st.process(q);
            if (grid.get(q).count() > 1) {
                count_pushed++;
                list_.push_back(q);
            }
            //std::cout << "size " << list.size() << "/" << list_.size() << std::endl;
        }
    } while (!list_.empty());
    print_final(grid, std::cout, true);
    std::cout << std::endl;
    print_final(grid, std::cout, false);

    return 0;
}
