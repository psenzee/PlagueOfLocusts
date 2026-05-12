#pragma once

#include <vector>
#include <algorithm>

template <typename T>
struct range_set
{
    typedef range_set<T>                    self_t;
    typedef std::pair<T, T>                 range_t;
    typedef std::vector<range_t>            list_t;
    typedef typename list_t::const_iterator const_iterator_t;

    inline range_set() : _normalized(true) {}

    inline void add(const range_t &range)
    {
        range_t r(std::min(range.first, range.second), std::max(range.first, range.second));
        _ranges.push_back(r);
        _normalized = false;
    }
    
    inline const_iterator_t begin() const { _normalize(); return _ranges.begin(); }
    inline const_iterator_t end()   const { _normalize(); return _ranges.end(); }

private:

    list_t _ranges;
    bool   _normalized;

    struct _comparer { inline bool operator()(const range_t &a, const range_t &b) const { return a.first != b.first ? a.first < b.first : a.second < b.second; } };

    void _normalize()
    {
        if (_normalized)
            return;
        std::sort(_ranges.begin(), _ranges.end(), _comparer());
        list_t list;
        for (auto i = _ranges.begin(), e = _ranges.end(); i != e; ++i) {
            range_t r(*i);
            for (auto j = i + 1; j != e; ++j, ++i) {
                range_t q(*j);
                if (q.first > r.second + 1) break;
                r.second = std::max(r.second, q.second);
            }
            list.push_back(r);
        }
        _ranges = list;
        _normalized = true;
    }
};
