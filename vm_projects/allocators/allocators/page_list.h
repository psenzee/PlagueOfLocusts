#pragma once

#include "page.h"

#include <cstring>
#include <cstdio>
#include <vector>

class page_list
{
    typedef std::vector<page *> list_t;

    list_t   _pages;
    page    *_current;

public:

    uint32_t items_per_page, item_size;

    page_list(uint32_t items_per_page, uint32_t item_size)
        : _current(nullptr), items_per_page(items_per_page), item_size(item_size) {}
    ~page_list() { for (auto page : _pages) if (page) delete page; _pages.clear(); }

    void *allocate()
    {
        if (!_current || _current->is_full()) {
            _pages.push_back(_current = new page(items_per_page, item_size));
        }
        return _current->allocate();
    }
    void deallocate(void *u) { _current->deallocate(u); }
    size_t get_page_count() const { return _pages.size(); }
};
