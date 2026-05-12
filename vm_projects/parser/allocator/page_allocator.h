#pragma once

#include "page.h"

#include <cstring>
#include <cstdio>
#include <new>
#include <vector>
#include <exception>

class page_allocator
{
    typedef std::vector<page *> page_list;

    page_list   _pages;
    page       *_current;

public:

    size_t pages, items_per_page, item_size;

    inline page_allocator(size_t items_per_page, size_t item_size)
        : _current(0), pages(0), items_per_page(items_per_page), item_size(item_size) {}
    inline ~page_allocator() { for (auto page : _pages) if (page) delete page; _pages.clear(); }

    inline void *create()
    {
        if (!_current || _current->full()) { pages++; _pages.push_back(_current = new page(items_per_page, item_size)); }
        return _current->create();
    }
    inline size_t get_page_count() const { return _pages.size(); }
};
