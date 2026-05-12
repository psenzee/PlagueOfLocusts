#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <deque>
#include <ctime>
#include <queue>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <bitset>

#include "point_queue.h"
#include "grid.h"
#include "rules.h"
#include "patch.h"
#include "patcher.h"
#include "std_array.h"
#include "color.h"

namespace wave {

template <size_t N>
struct availability_list
{
    typedef std::bitset<N>              options_t;
    typedef grid::grid<options_t>       grid_t;
    typedef grid::basic<options_t>      basic_t;
    typedef std::deque<point2i>         points_t;
    typedef rules<N>                    rules_t;
    typedef std::vector<point2i>        paths_t;
    typedef patcher<options_t, 3, 3>    patched_t;
    typedef patch<3, 3, N>              patch_t;
    
    grid_t         &_grid;
    point_queue     _points;
    std::mt19937    _mt;
    
    inline availability_list(grid_t &grid, const std::mt19937 &mt) : _grid(grid), _mt(mt) {}
    
    void initialize_with_grid()
    {
        size2u sz(_grid.size());
        for (point2i p { 0, 0 }; p.y < sz.h; p.y++) {
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

}
