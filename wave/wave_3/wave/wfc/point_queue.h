#pragma once

#include <queue>
#include <algorithm>

namespace wave {

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

}
