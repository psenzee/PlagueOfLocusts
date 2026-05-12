#pragma once

#include <cstddef>
#include <algorithm>
#include <vector>
#include <array>
#include "pairs.h"

typedef uint64_t page_id_t;

struct page_ids
{
    page_id_t page_id,
              page_size,
              prev_page_id,
              next_page_id;
    
    inline page_ids() : page_id(0), page_size(0), prev_page_id(0), next_page_id(0) {}
    inline page_ids(const page_ids &u) : page_id(u.page_id), page_size(u.page_size), prev_page_id(u.prev_page_id), next_page_id(u.next_page_id) {}
    
    inline bool      operator<(const page_ids &u) const { return page_id < u.page_id; }
};

template <typename T = uint64_t, typename CompareT = std::less<T> >
struct range
{
    typedef T                      value_t;
    typedef CompareT               compare_t;
    typedef range<T, CompareT>     self_t;

    value_t min, max;
    
    inline range() : min(std::numeric_limits<value_t>::max()), max(std::numeric_limits<value_t>::min()) {}
    inline range(const range &u) : min(u.min), max(u.max) {}
    
    inline self_t &insert(const value_t &id, const compare_t &c)       { if (c(id, min)) min = id; if (c(max, id)) max = id; return *this; }
    inline self_t &unite(const self_t &r, const compare_t &c)          { insert(r.min, c); insert(r.max, c); return *this; }
    inline bool    empty()                                       const { return max < min; }
};

template <typename T, size_t N, typename IdT = uint64_t, typename CompareIdT = std::less<uint64_t> >
struct page_header
{
    static constexpr size_t MAX_COUNT = N;
    
    typedef page_header<T, N, IdT, CompareIdT>  self_t;
    typedef IdT                                 id_t;
    typedef CompareIdT                          compare_id_t;
    typedef range<id_t, compare_id_t>           range_t;
    
    inline page_header() : _count(0), _sorted(0) {}
    inline page_header(const self_t &u) : _count(u._count), _sorted(u._sorted), _page_ids(u._page_ids), _range(u._range) {}
    
    inline self_t   &operator=(const self_t &u)               { _page_ids = u._page_ids; _count = u._count; _sorted = u._sorted; _range = u._range; return *this; }
    inline bool      operator<(const self_t &u)         const { return page_id() < u.page_id(); }

    inline page_id_t page_id()                          const { return _page_ids.page_id; }
    inline size_t    count()                            const { return _count; }
    inline size_t    sorted()                           const { return _sorted; }
    inline size_t    capacity()                         const { return MAX_COUNT; }
    inline bool      full()                             const { return _count == MAX_COUNT; }
    inline bool      too_small()                        const { return _count < (MAX_COUNT / 2); }
    inline bool      empty()                            const { return _count == 0; }
    inline float     fill_ratio()                       const { return float(_count) / MAX_COUNT; }
    
    inline self_t   &set_page_ids(const page_ids &ids)        { _page_ids = ids; return *this; }
    inline self_t   &set_count(uint32_t count)                { _count = count; return *this; }
    inline self_t   &set_sorted(uint32_t sorted)              { _sorted = sorted; return *this; }
    inline self_t   &set_range(const range_t &r)              { _range = r; return *this; }

private:

    page_ids   _page_ids;
    uint32_t   _count, _sorted;
    range_t    _range;
};

template <typename T, size_t N, typename IdT = uint64_t, typename CompareIdT = std::less<uint64_t> >
struct page
{
    typedef T                            value_type;
    typedef IdT                          id_t;
    typedef page<T, N, IdT, CompareIdT>  self_t;
    typedef std::pair<IdT, T>            pair_t;
    typedef CompareIdT                   compare_id_t;
    typedef std::array<pair_t, N>        data_t;
    typedef range<id_t, compare_id_t>    range_t;
    
    static constexpr size_t MAX_COUNT = N;
    
    inline page() : _count(0), _sorted(0) { _page_ids.page_size = sizeof(self_t); }

    inline page_id_t page_id()                          const { return _page_ids.page_id; }
    inline auto      begin()                            const { return _data.begin(); }
    inline auto      end()                              const { return std::min(_data.begin() + _count, _data.end()); }
    inline size_t    size()                             const { return _count; }
    inline size_t    capacity()                         const { return MAX_COUNT; }
    inline bool      full()                             const { return _data.end() == end(); }
    inline bool      too_small()                        const { return _count < (MAX_COUNT / 2); }
    inline bool      empty()                            const { return _count == 0; }
    inline float     fill_ratio()                       const { return float(_count) / MAX_COUNT; }

    inline bool      add(const id_t &id, const value_type &v, const compare_id_t &c) { return _add(id, v, c); }
    inline bool      add(const pair_t &p, const compare_id_t &c)                     { return _add(p, c); }
    inline bool      add(const id_t &id, const value_type &v)                        { compare_id_t c; return _add(id, v, c); }
    inline bool      add(const pair_t &p)                                            { compare_id_t c; return _add(p, c); }
    
    // TODO insert
    // TODO sort
    // TODO add bulk
    // TODO merge
    // TODO split
    
    inline const range_t  &range()    const { return _range; }
    inline const page_ids &page_ids() const { return _page_ids; }
    
private:

    page_ids   _page_ids;
    uint32_t   _count, _sorted;
    range_t    _range;
    data_t     _data;
    
    inline bool _add(const id_t &id, const value_type &v, const compare_id_t &c)
    {
        if (full()) return false;
        auto &at = _data[_count++];
        at.first = id;
        at.second = v;
        _range.insert(id, c);
        return true;
    }
    
    inline bool _add(const pair_t &p, const compare_id_t &c)
    {
        if (full()) return false;
        _data[_count++] = p;
        _range.insert(p.first, c);
        return true;
    }
};

template <typename T, size_t N, typename IdT = uint64_t, typename CompareIdT = std::less<uint64_t> >
struct page_handle
{
    static constexpr size_t MAX_COUNT = N;
    
    typedef page_handle<T, N, IdT, CompareIdT>  self_t;
    typedef page<T, N, IdT, CompareIdT>         page_t;
    typedef IdT                                 id_t;
    typedef CompareIdT                          compare_id_t;
    typedef range<id_t, compare_id_t>           range_t;
    
    inline page_handle(page_id_t id, page_t *page = 0) : _count(0), _page(page) { _page_ids.page_id = id; _page_ids.page_size = sizeof(page_t); }
    inline page_handle(const self_t &u) : _count(u._count), _page(u._page), _page_ids(u._page_ids) {}
    
    inline bool operator<(const self_t &u) const { return page_id() < u.page_id(); }

    inline page_id_t page_id()                          const { return _page_ids.page_id; }
    inline size_t    size()                             const { return _count; }
    inline size_t    capacity()                         const { return MAX_COUNT; }
    inline bool      full()                             const { return _count == MAX_COUNT; }
    inline bool      too_small()                        const { return _count < (MAX_COUNT / 2); }
    inline bool      empty()                            const { return _count == 0; }
    inline float     fill_ratio()                       const { return float(_count) / MAX_COUNT; }
    inline void      set_page(page_t *p)
    {
        _page = p;
        if (p) {
            _page_ids = p->page_ids();
            _range    = p->range();
            _count    = p->size();
        }
    }
    
private:

    page_ids   _page_ids;
    uint32_t   _count;
    range_t    _range;
    page_t    *_page;
};

template <typename T, size_t N, typename IdT = uint64_t, typename CompareIdT = std::less<uint64_t> >
struct page_registry
{
    typedef page_registry<T, N, IdT, CompareIdT> self_t;
    typedef page<T, N, IdT, CompareIdT>          page_t;
    typedef page_handle<T, N, IdT, CompareIdT>   page_handle_t;
    typedef std::vector<page_handle_t>           data_t;
    typedef typename data_t::iterator            iterator_t;
    typedef typename data_t::const_iterator      const_iterator_t;

    inline page_registry() : _sorted(true) {}
    
    inline self_t &insert(page_t *p)                 { return _insert(page_handle_t(p->page_id(), p)); }
    inline self_t &add(page_t *p)                    { return _add(page_handle_t(p->page_id(), p)); }

    inline page_t *get(page_id_t id)                 { return _find(id); }
    
private:

    data_t _data;
    bool   _sorted;
    
    inline self_t &_insert(const page_handle_t &ph)        { return _sorted ? _insert_sorted(ph) : _add(ph); }
    inline self_t &_add(const page_handle_t &ph)           { _data.push_back(ph); _sorted = false; return *this; }
    inline self_t &_add(iterator_t &i, iterator_t &e)      { _data.copy(i, e); _sorted = false; return *this; }
    
    inline self_t &_insert_sorted(const page_handle_t &ph)
    {
        auto i = std::upper_bound(_data.begin(), _data.end(), ph);
        _data.insert(i, ph);
        return *this;
    }
    
    inline page_handle_t *_find(page_id_t id)
    {
        _ensure_sorted();
        auto e = _data.end(), i = std::lower_bound(_data.begin(), e, page_handle_t(id, 0));
        return i == e ? 0 : &(*i);
    }
    
    void _ensure_sorted()
    {
        if (!_sorted) {
            std::sort(_data.begin(), _data.end());
            _sorted = true;
        }
    }
};
