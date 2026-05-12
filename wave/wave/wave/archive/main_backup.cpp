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
#include <random>
#include "bits.h"
#include "map_data.h"
#include "grid.h"

namespace wave {

template <size_t N>
struct rule
{
    typedef patch<3, 3>           patch_t;
    typedef base_grid<options_t>  grid_t;

    size_t identifier;
    patch_t _patch;

    inline rule() : identifier(0) { options_t o; _patch.clear(o); }

    rule(const rule &r) : identifier(r.identifier), _patch(r._patch) {}
    
    bool test_apply(const point &p, grid_t &grid) const
    {
        // resolve neighbors as an intersection of the existing possible tiles and
        // the possible tiles specified in this rule
        
        // first test if any intersections are zero
        for (size_t i = 0; i < point::MAX_DIRECTIONS; i++) {
            auto q = p.direction_offset(i);
            size_t sz = (grid.get(q) & _patch.get(_patch.CENTER.direction_offset(i))).count();
            if (sz == 0) {
                return false;
            }
        }
        return true;
    }

    bool apply(const point &p, grid_t &grid) const
    {
        // resolve neighbors as an intersection of the existing possible tiles and
        // the possible tiles specified in this rule
        
        // first test if any intersections are zero
        if (!test_apply(p, grid)) {
            return false;
        }
        // we're good, so resolve this point to a single tile
        options_t a;
        a.set(identifier);
        grid.set(p, a);
        // now resolve neighbors
        for (size_t i = 0; i < point::MAX_DIRECTIONS; i++) {
            auto q = p.direction_offset(i);
            grid.set(q, grid.get(q) & _patch.get(point(1, 1).direction_offset(i)));
        }
        return true;
    }

    std::ostream &print(std::ostream &os) const
    {
        os << "#" << identifier << ": ";
        for (size_t i = 0; i < point::MAX_DIRECTIONS; i++)
            _patch.get(_patch.CENTER.direction_offset(i)).print(os) << " ";
        return os << std::endl;
    }
    
    void add_example(const uint8_t *map, size_t stride, const point &p)
    {
        identifier = map[p.y * stride + p.x];
        _patch.or_from(map, stride, p);
    }
};

template <size_t N>
struct rules
{
    typedef rule<N>                  rule_t;
    typedef base_grid<options_t>     grid_t;
    typedef typename rule_t::patch_t patch_t;

    std::vector<rule_t> _rules;
    
    inline rules() { _rules.resize(N); }
    
    options_t train(const uint8_t *map, size_t width, size_t height)
    {
        _rules.resize(N);
        point p(0, 0);
        for (p.y = 1; p.y < height - 1; p.y++) {
            for (p.x = 1; p.x < width - 1; p.x++) {
                size_t at = p.y * width + p.x;
                _rules[map[at]].add_example(map, width, p);
            }
        }
        options_t op;
        size_t i = 0;
        for (const auto &v : _rules) {
            if (v.identifier != 0)
                op.set(i);
            i++;
        }
        return op;
    }
    
    patch_t patch(const options_t &options) const
    {
        // combine rule patches
        patch_t patch;
        for (ssize_t i = 0; (i = options.next_set(i)) != -1; i++) {
            const auto r = _rules[i];
            patch.combine(r._patch);
        }
        return patch;
    }
    
    std::ostream &print(std::ostream &os) const
    {
        for (const auto &r : _rules) {
            if (r.identifier)
                r.print(os) << std::endl;
        }
        return os;
    }
};

struct point_options_count_desc_comparator
{
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

struct point_options_count_asc_comparator
{
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
struct state
{
    typedef grid<options_t>          grid_t;
    typedef base_grid<options_t>     base_grid_t;
    typedef std::deque<point>        points_t;
    typedef rules<N>                 rules_t;
    typedef std::vector<point>       paths_t;
    typedef patcher<options_t, 3, 3> patched_t;
    typedef patch<3, 3>              patch_t;
    
    paths_t                 _paths;
    grid<options_t>        &_grid;
    const rules_t          &_rules;
    std::deque<patched_t *> _patchers;
    options_t               _max_entropy;
    base_grid<bool>         _visited;
    
    state(grid_t &grid, const rules_t &rules, const point &initial_point) : _grid(grid), _rules(rules), _visited(_grid.size(), false)
    {
        options_t entropy;
        size_t w = grid.size().w, h = grid.size().w;
        for (point p(0, 0); p.y < h; p.y++)
            for (p.x = 0; p.x < w; p.x++)
                entropy |= grid.get(p);
        _max_entropy = entropy;
        init_stack();
    }
    
    inline grid_t &push(const point &p)
    {
        grid_t *grid = _patchers.back();
        _patchers.push_back(new patched_t(*grid, p));
        return *_patchers.back();
    }
    
    inline void pop()
    {
        grid_t *grid = _patchers.back();
        _patchers.pop_back();
        delete grid;
    }
    
    inline grid_t &top()
    {
        return *_patchers.back();
    }
    
    inline bool path_contains(const point &p) const
    {
        for (const auto &q : _paths)
            if (p == q)
                return true;
        return false;
    }
    
    void init_stack()
    {
        _patchers.push_back(new patched_t(_grid, point(0, 0)));
    }
    
    bool can_apply_one(grid_t &grid, const point &p, const patch_t &patch)
    {
        // test if any intersections are zero
        for (size_t i = 0; i < point::MAX_DIRECTIONS; i++) {
            if (!path_contains(p)) {
                auto a = p.direction_offset(i), b = patch.CENTER.direction_offset(i);
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
            grid.set(a, grid.get(a) & patch.get(b));
        }
    }
    
    void resolve()
    {
        for (auto p : _patchers)
            p->resolve();
        for (auto p : _patchers)
            delete p;
        init_stack();
    }
    
    bool apply(const point &p, const options_t &o)
    {
        if (path_contains(p)) { // already in process
            return true;
        }
        grid_t &grid = top();
        options_t primary(grid.get(p) & o);
        if (primary == _max_entropy) {
            return true;
        }
        size_t count = primary.count();
        if (!count) {
            std::cout << "F";
            return false;
        }
        auto patch = _rules.patch(primary);
        if (!can_apply_one(grid, p, patch)) {
            std::cout << "F";
            return false;
        }
        std::vector<point> points;
        if (!next_points(grid, points, p, patch)) {
            std::cout << "F";
            return false;
        }
        std::cout << points.size();
        if (points.size() == 0) {
            // if there are no points, we are free to do a simple apply
            apply_one(grid, p, patch);
            return true;
        }
        // push a patch on to the stack
        grid = push(p);
        _paths.push_back(p);
        apply_one(grid, p, patch);
        for (const point &q : points) {
            std::cout << ">";
            if (!apply(q, grid.get(q))) {
                pop();
                _paths.pop_back();
                std::cout << "F";
                return false;
            }
        }
        _paths.pop_back();
        return true;
    }
    
    std::vector<point> _stack;
    
    void start(const point &p)
    {
        _stack.push_back(p);
    }
    
    bool process()
    {
        if (_stack.empty()) {
            std::cout << "f";
            return false;
        }
        point_options_count_desc_comparator c(_grid);
        std::sort(_stack.begin(), _stack.end(), c);
        point p(_stack.back());
        std::cout << p << " ";
        _stack.pop_back();
        if (_visited.get(p)) {
            std::cout << "t";
            return true;
        }
        _visited.set(p, true);
        options_t primary(_grid.get(p));
        if (primary == _max_entropy) {
            std::cout << "t";
            return true;
        }
        size_t count = primary.count();
        std::cout << "{" << count << "}";
        if (!count) {
            std::cout << "f";
            return false;
        }
        auto patch = _rules.patch(primary);
        if (!can_apply_one(_grid, p, patch)) {
            std::cout << "f";
            return false;
        }
        if (!next_points_(_grid, _stack, p, patch)) {
            std::cout << "f";
            return false;
        }
        apply_one(_grid, p, patch);
        std::cout << "[" << _stack.size() << "]";
        return true;
    }
    
    bool next_points_(grid_t &grid, std::vector<point> &points, const point &p, const patch_t &patch)
    {
        for (size_t i = 0; i < point::MAX_DIRECTIONS; i++) {
            auto pa = p.direction_offset(i), pb = patch.CENTER.direction_offset(i);
            if (pa != p && !_visited.get(pa)) {
                auto value = grid.get(pa) & patch.get(pb);
                if (value != _max_entropy) {
                    if (value.count() == 0) {
                        return false;
                    }
                    points.push_back(pa);
                }
            }
        }
        return true;
    }
    
    bool next_points(grid_t &grid, std::vector<point> &points, const point &p, const patch_t &patch)
    {
        for (size_t i = 0; i < point::MAX_DIRECTIONS; i++) {
            auto pa = p.direction_offset(i), pb = patch.CENTER.direction_offset(i);
            if (pa != p && !path_contains(pa)) { // this is kind of a question mark
                auto value = grid.get(pa) & patch.get(pb);
                if (value != _max_entropy) {
                    if (value.count() == 0) {
                        return false;
                    }
                    points.push_back(pa);
                }
            }
        }
        point_options_count_desc_comparator c(grid);
        std::sort(points.begin(), points.end(), c);
        return true;
    }
    
    void print_final(std::ostream &os)
    {
        point p(0, 0);
        os << "[" << std::endl << std::endl;
        size sz = _grid.size();
        for (p.y = 0; p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                auto o = _grid.get(p);
                auto value = o.next_set();
                os << std::fixed << std::setw(3) <<
                      std::setfill(' ') << value << ",";
            }
            os << std::endl;
        }
        os << std::endl << "]";
    }
};

template <size_t N>
struct resolver
{
    typedef grid<options_t>       grid_t;
    typedef base_grid<options_t>  base_grid_t;
    typedef std::deque<point>     points_t;
    typedef rules<N>              rules_t;
    typedef std::vector<point>    paths_t;
    typedef patcher<options_t, 3, 3> patched_t;
    typedef patch<3, 3>           patch_t;

    const rules_t     &_rules;
    base_grid_t       &_grid;
    base_grid<bool>    _completed;
    points_t           _points;
    std::random_device _random_device;
    size_t             _message_count;
    
    inline resolver(const rules_t &rules, base_grid_t &grid) : _rules(rules), _grid(grid), _completed(grid.size(), false), _message_count(0) { _init(); }
    
    bool can_apply_one(grid_t &grid, const point &p, const patch_t &patch)
    {
        // test if any intersections are zero
        for (size_t i = 0; i < point::MAX_DIRECTIONS; i++) {
            auto a = p.direction_offset(i), b = patch.CENTER.direction_offset(i);
            if ((grid.get(a) & patch.get(b)).count() == 0)
                return false;
        }
        return true;
    }
    
    void apply_one(grid_t &grid, const point &p, const patch_t &patch)
    {
        for (size_t i = 0; i < point::MAX_DIRECTIONS; i++) {
            auto a = p.direction_offset(i), b = patch.CENTER.direction_offset(i);
            grid.set(a, grid.get(a) & patch.get(b));
        }
    }
    
    void directions_(std::vector<point> &points, const point &p, const patch_t &patch)
    {
        /*
        points.clear();
        for (size_t i = 0, sz = point::MAX_DIRECTIONS; i < sz; i++) {
            auto q = p.direction_offset(i), pq = patch_t::CENTER.direction_offset(i);
            auto a = patch.get(pq), b = grid.get(q);
            a &= b;
            size_t count = a.count();
            if (count == 0) {
                return false;
            }
            if (count >= 1 && a != b && !path_contains(path, q) &&
                grid_utils<options_t>::inside(grid, q)) {
                points.push_back(q);
            }
        }
        return true;
         */
    }
    
    inline static bool path_contains(const paths_t &path, const point &p)
    {
        for (paths_t::const_iterator i = path.begin(), e = path.end(); i != e; ++i) {
            if (p == *i) {
                return true;
            }
        }
        return false;
    }
    
    enum Resolved { RESULT_FALSE = 0, RESULT_TRUE = 1, RESULT_DONE = 2 };
    
    Resolved try_apply(const point &p, grid_t &grid, const options_t &o, paths_t &path, std::deque<patched_t *> &patches, size_t depth = 0)
    {
        if (path_contains(path, p)) {
            std::cout << "d";
            return RESULT_DONE;
        }
        options_t primary(grid.get(p) & o);
        size_t count = primary.count();
        if (!count) {
            return RESULT_FALSE;
        }
        auto patch = _rules.patch(primary);
        std::vector<point> points;
        for (size_t i = 0, sz = point::MAX_DIRECTIONS; i < sz; i++) {
            auto q = p.direction_offset(i), pq = patch_t::CENTER.direction_offset(i);
            auto a = patch.get(pq), b = grid.get(q);
            a &= b;
            size_t count = a.count();
            if (count == 0) {
                return RESULT_FALSE;
            }
            if (count >= 1 && a != b && !path_contains(path, q) &&
                grid_utils<options_t>::inside(grid, q)) {
                points.push_back(q);
            }
        }
        if (points.empty()) {
            std::cout << "z";
            return RESULT_TRUE;
        }
        patched_t *patched = new patched_t(grid, p);
        // TODO set 'patch' now that it's removed from patched->patch()
        patched->set(p, primary);
        Resolved r = RESULT_TRUE;
        path.push_back(p);
        patches.push_back(patched);
        for (const auto &q : points) {
            auto a = patched->get(q);
            std::cout << "-";
            if (try_apply(q, *patched, a, path, patches, depth + 1) == RESULT_FALSE) {
                r = RESULT_FALSE;
                break;
            }
        }
        path.pop_back();
        if (r != RESULT_FALSE) {
            std::cout << ".";
            if (depth == 0) {
                for (auto patch : patches)
                    patch->resolve();
                for (auto patch : patches)
                    delete patch;
            }
        } else {
            delete patches.back();
            patches.pop_back();
            std::cout << "/";
        }
        return r;
    }

    bool next()
    {
        if (_points.empty())
            return false;
        _sort();
        point p(_pop_fewest_random());
        options_t o = _grid.get(p);
        options_t::index_array_t list;
        size_t lsz = o.list_set(list);
        if (lsz == 0) {
            std::cout << "ZERO!" << std::endl;
            return false;
        }
        std::mt19937 randomizer(_random_device());
        options_t opa;
        opa.set(list[randomizer() % lsz]);
        auto patch = _rules.patch(opa);
        if (can_apply_one(_grid, p, patch)) {
            patched_t patched(_grid, p);
            apply_one(patched, p, patch);
            patched.resolve();
            o = patched.get(p);
            if (o.count() > 1) {
                _points.push_back(p);
            }
        }
        return _points.size() != 0;
        
        paths_t path;
        std::deque<patched_t *> patches;
        Resolved r = try_apply(p, _grid, opa, path, patches, 0);
        if (r == RESULT_FALSE) {
            // remove and requeue
            _grid.set(p, ~opa & _grid.get(p));
            _points.push_back(p);
            return true;
        }
        return _points.size() != 0;
    }
    
    void print_final(std::ostream &os)
    {
        point p(0, 0);
        os << "[" << std::endl << std::endl;
        size sz = _grid.size();
        for (p.y = 0; p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                auto o = _grid.get(p);
                auto value = o.next_set();
                os << std::fixed << std::setw(3) <<
                      std::setfill(' ') << value << ",";
            }
            os << std::endl;
        }
        os << std::endl << "]";
    }

private:
    
    void print_list()
    {
        for (size_t i = 0; i < 100; i++) {
            const point &p = _points[i];
            std::cout << " x " << p.x << " y " << p.y << std::endl;
        }
    }
    
    std::ostream &message(std::ostream &os)
    {
        return os << " (" << _message_count++ << ")" << std::endl;
    }
    
    point _pop_fewest_random()
    {
        while (!_points.empty() && _grid.get(_points.back()).count() < 2)
            _points.pop_back();
        if (_points.empty())
            return point(-1, -1);
        point p(_points.back());
        const options_t &o = _grid.get(p);
        size_t c = o.count(), range = 0;
        for (points_t::const_reverse_iterator i = _points.rbegin(), e = _points.rend(); i != e; ++i, ++range)
            if (_grid.get(*i).count() > c)
                break;
        size_t sz = _points.size();
        p = _points[sz - range];
        std::swap(_points[sz - (rand() % range) - 1], _points[sz - 1]);
        p = _points.back();
        _points.pop_back();
        return p;
    }

    void _init()
    {
        _points.resize(_grid.size().product());
        point p(0, 0);
        size_t i = 0;
        size sz = _grid.size();
        for (p.y = 0; p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                _points[i++] = p;
            }
        }
    }
    
    struct comparator
    {
        const grid_t &_grid;
        
        inline comparator(const grid_t &grid) : _grid(grid) {}
        
        inline bool operator()(const point &a, const point &b) const
        {
            if (&a == &b || a == b) {
                return false;
            }
            size_t ca = _grid.get(a).count(), cb = _grid.get(b).count();
            return ca == cb ? a > b : ca > cb;
        }
    };

    void _sort()
    {
        comparator c(_grid);
        std::sort(_points.begin(), _points.end(), c);
    }
};

}

int main(int argc, const char * argv[])
{
    time_t t;
    srand((unsigned int)time(&t));

    wave::base_grid<wave::options_t> grid(wave::size(50, 50));
    wave::rules<256> r;
    r.train(map100x256_0, 100, 256);
    r.train(map100x256_1, 100, 256);
    r.train(map100x100_2, 100, 100);
    r.train(map100x100_3, 100, 100);
    auto opt = r.train(map100x100_4, 100, 100);
    wave::grid_utils<wave::options_t>::clear(grid, opt);
    r.print(std::cout);
    wave::resolver resolver(r, grid);

    wave::point q(10, 10);
    wave::state<256> st(grid, r, q);
    wave::options_t ops;
    ops.set(1);
    grid.set(q, ops);
    st.start(q);
    while (st.process()) {
        std::cout << "*";
    }
    
    //state.apply(wave::point(10, 10), ops);
    //state.resolve();
    st.print_final(std::cout);
    while (resolver.next()) {
        //std::cout << "resolving " << std::endl;
    }
    resolver.print_final(std::cout);
    wave::grid_utils<wave::options_t>::print(std::cout, grid);
    return 0;
}
