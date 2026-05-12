//
//  main.cpp
//  wave
//
//  Created by paul on 12/27/23.
//

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
#include "map_data.h"
#include "grid.h"
#include "rules.h"

namespace wave {

inline int point_rand(const point &p, int seed = 0)
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
    
    inline bool operator()(const point &a, const point &b) const
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
    
    inline bool operator()(const point &a, const point &b) const
    {
        if (&a == &b || a == b) {
            return false;
        }
        size_t ca = _grid.get(a).count(), cb = _grid.get(b).count();
        return ca == cb ? a < b : ca < cb;
    }
};

template <size_t N>
struct context
{
    typedef bit_array<N>      options_t;
    typedef base_grid<options_t>   grid_t;
    typedef base_grid<bool>   bool_grid_t;

    grid_t      &grid;
    bool_grid_t  resolve;
    options_t    max_entropy;

    context(const options_t &max_entropy, grid_t &grid) : max_entropy(max_entropy), grid(grid), resolve(grid.size(), false) {}
};

template <size_t N, bool RESOLVED_ASC = true, bool COUNT_ASC = true>
struct point_count_comparator
{
    typedef context<N> context_t;
    
    const context_t &_context;
    
    inline point_count_comparator(const context_t &context) : _context(context) {}
    
    inline bool operator()(const point &a, const point &b) const
    {
        if (&a == &b || a == b)
            return false;
        bool ra = _context.resolve(a), rb = _context.resolve(b);
        if (ra != rb) return (RESOLVED_ASC ? ra < rb : ra > rb);
        size_t ca = _context.grid.get(a).count(), cb = _context.grid.get(b).count();
        if (ca != cb) return (COUNT_ASC ? ca < cb : ca > cb);
        int _a = point_rand(a), _b = point_rand(b);
        if (_a != _b) return _a < _b;
        return a < b;
    }
};

template <size_t N>
struct ordered_point_queue
{
    typedef bit_array<N>             options_t;
    typedef std::deque<point>        deque_t;
    typedef std::vector<point>       vector_t;
    typedef context<N>               context_t;
    typedef base_grid<bool>          bool_grid_t;
    
    deque_t      _list;
    vector_t     _max_entropy_list;
    bool_grid_t  _in_list;
    context_t   &_context;

    ordered_point_queue(context_t &context) : _context(context)
    {
        _in_list.init(_context.grid.size(), false);
        _initialize();
        _full_sort();
    }
    
    inline bool has_unresolved() const
    {
        return !_list.empty() && _context.resolve(_list.front());
    }
    
    inline point pop_unresolved()
    {
        auto p = _list.front();
        _list.pop_front();
        _in_list.set(p, false);
        return p;
    }
    
    inline point pop_resolved()
    {
        auto p = _list.back();
        _list.pop_back();
        _in_list.set(p, false);
        return p;
    }
    
    inline void push_unresolved(const point &p)
    {
        _push_front(p);
    }
    
    inline void sort()
    {
        point_count_comparator<N> c(_context);
        std::sort(_list.begin(), _list.end(), c);
    }
    
private:
    
    inline void _push_front(const point &p)
    {
        if (!_in_list.get(p))
            _list.push_front(p);
        _in_list.set(p, true);
    }
    
    inline void _push_back(const point &p)
    {
        if (!_in_list.get(p))
            _list.push_back(p);
        _in_list.set(p, true);
    }

    void _initialize()
    {
        size sz = _context.grid.size();
        for (point p(0, 0); p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                _context.resolve.set(p, 0);
                if (_context.grid(p) != _context.max_entropy) {
                    _push_back(p);
                } else {
                    _max_entropy_list.push_back(p);
                }
            }
        }
    }
    
    void _full_sort()
    {
        point_count_comparator<N> c(_context);
        std::sort(_list.begin(), _list.end(), c);
    }
};

struct point_queue
{
    struct elem
    {
        point  point;
        size_t count;
        size_t rand;
        
        inline bool operator< (const elem &o) const { return count != o.count ? count < o.count : (rand != o.rand ? rand < o.rand : point < o.point); }
        inline bool operator> (const elem &o) const { return count != o.count ? count > o.count : (rand != o.rand ? rand > o.rand : point > o.point); }
        inline bool operator==(const elem &o) const { return point == o.point && count == o.count && rand == o.rand; }
        inline bool operator<=(const elem &o) const { return !(*this > o); }
        inline bool operator>=(const elem &o) const { return !(*this < o); }
    };
    
    typedef std::priority_queue< elem, std::vector<elem>, std::greater<elem> > priority_queue_t;
    
    inline void        push(const elem &e)                                       { if (e.count > 1) _queue.push(e); }
    inline void        push(const point &p, size_t count, size_t rand = 0)       { push(elem { p, count, rand }); }
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
    typedef std::deque<point>        points_t;
    typedef rules<N>                 rules_t;
    typedef std::vector<point>       paths_t;
    typedef patcher<options_t, 3, 3> patched_t;
    typedef patch<3, 3>              patch_t;
    
    grid_t         &_grid;
    point_queue     _points;
    std::mt19937    _mt;
    
    inline availability_list(grid_t &grid, std::random_device &rd) : _grid(grid), _mt(rd()) {}
    
    void initialize_with_grid()
    {
        struct size sz = _grid.size();
        for (point p(0, 0); p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                enqueue(p);
            }
        }
    }
    
    void enqueue(const point &p)
    {
        _points.push(p, _grid.get(p).count(), _mt());
    }
    
    size_t size() const { return _points.size(); }
    
    bool empty()
    {
        _clear_finished();
        return _points.empty();
    }

    bool next(point &p)
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
    typedef std::deque<point>              points_t;
    typedef rules<N>                       rules_t;
    typedef patcher<options_t, 3, 3>       patched_t;
    typedef patch<3, 3>                    patch_t;
    
    grid_t                 &_grid;
    const rules_t          &_rules;
    options_t               _max_entropy;
    std::mt19937            _mt;

    bool_grid               _resolve;
    std::deque<point>       _points;
    
    state(grid_t &grid, const rules_t &rules, std::random_device &rd, const point &initial_point = point(0, 0))
        : _grid(grid), _rules(rules), _resolve(_grid.size()), _mt(rd()) {}

    bool reduce(const point &p)
    {
        return _reduce(p, options_t(true));
    }
    
    bool process(const point &q)
    {
        if (!_process(q)) {
            return false;
        }
        point p;
        while ((p = grid_utils<bool>::find_set_point(_resolve)).x != -1) {
            if (!_process(p)) {
                std::cout << "!process" << std::endl;
            }
        }
        return false;
    }

private:

    bool can_apply_one(grid_t &grid, const point &p, const patch_t &patch)
    {
        // test if any intersections are zero
        for (size_t i = 0; i < point::MAX_DIRECTIONS; i++) {
            auto a = p.direction_offset(i), b = patch.CENTER.direction_offset(i);
            if (grid_utils<options_t>::inside(grid, a)) {
                if ((grid.get(a) & patch.get(b)).count() == 0)
                    return false;
            }
        }
        return true;
    }
    
    void apply_one(grid_t &grid, const point &p, const patch_t &patch)
    {
        for (size_t i = 0; i < point::MAX_DIRECTIONS; i++) {
            auto a = p.direction_offset(i), b = patch.CENTER.direction_offset(i);
            if (grid_utils<options_t>::inside(grid, a)) {
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

    bool _process(const point &p)
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
    
    bool _evaluate(const point &p, options_t options)
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

    bool _reduce(const point &p, options_t options)
    {
        if (_evaluate(p, options)) {
            return true;
        }
        // reduce the options by a different bit each time and reevaluate
        options_t o(options & _grid.get(p));
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

    options_t _valid_options(const point &p, const options_t &options)
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
    point p(0, 0);
    os << "[" << std::endl << std::endl;
    size sz = grid.size();
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

bit_array<1024> random_bit_array(std::mt19937 &mt, size_t count)
{
    bit_array<1024> ba;
    for (size_t i = 0; i < count; i++)
        ba.set(mt() % 1024);
    return ba;
}

void test_fns()
{
    uint64_t test[] = { 0b0001000010000001000000100000100111001000111111110000100010101010ull };
    std::cout << "dd " << ffsll(0) << std::endl;
    std::cout << "test #0 " << bit_find_next_set_bit<uint64_t>(test[0], 0) << std::endl;
    std::cout << "test #1 " << bit_find_next_set_bit<uint64_t>(test[0], 1) << std::endl;
    std::cout << "test #2 " << bit_find_next_set_bit<uint64_t>(test[0], 2) << std::endl;
    std::mt19937 mt;
    for (size_t i = 0; i < 128; i++) {
        bit_array<1024> ba(random_bit_array(mt, mt() % 256));
        if (ba.empty()) continue;
        //for (size_t k = 0; k< 512; k++) {
            ssize_t k = mt() % 512, a = ba.find_next_set(k), b = bit_find_next_set_bit(ba.data(), 16, k);
            if (a != b) {
                std::cout << "fns fail! " << a << "!=" << b << " k " << k << " count " << ba.count() << " (" << i << ")" << std::endl;
                std::cout << " bits ";
                ba.print_bin(std::cout) << std::endl;
                const uint64_t *data = ba.data();
                bit_find_next_set_bit(data, 16, k);
            }
        //}
    }
}

int main(int argc, const char * argv[])
{
    time_t t;
    srand((unsigned int)time(&t));
    
    test_fns();

    wave::base_grid<wave::options_t> grid(wave::size(50, 50));
    wave::rules<256> r;
    
    //r.train(map100x256_1, 100, 256);
    //r.train(map100x100_2, 100, 100);
    //r.train(map100x100_3, 100, 100);
    //r.train(map100x100_4, 100, 100);
    //r.train(map25x25_rivers_paths, 25, 25);
    //auto opt = r.train(map25x25_0, 25, 25);
    auto opt = r.train(map100x50_0, 100, 50);
    //auto opt = r.train(map100x256_0, 100, 256);
    wave::grid_utils<wave::options_t>::clear(grid, opt);
    r.print(std::cout);

    wave::point q;
    std::random_device rd;
    wave::availability_list<256> list(grid, rd);
    list.initialize_with_grid();
    
    std::mt19937 mt(rd());
    wave::state<256> st(grid, r, rd);
    std::vector<wave::point> list_;
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
            std::cout << "size " << list.size() << "/" << list_.size() << std::endl;
        }
    } while (!list_.empty());
    print_final(grid, std::cout, true);
    std::cout << std::endl;
    print_final(grid, std::cout, false);
    //wave::grid_utils<wave::options_t>::print(std::cout, grid);

    return 0;
}
