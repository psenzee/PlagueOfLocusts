#pragma once

#include <cstring>
#include <cstdio>
#include <vector>
#include <new>
#include <exception>

#include "base_allocator.h"
#include "page_list.h"

class pool_allocator : public base_allocator
{
    struct free_item { int magic; free_item *next; };
    
    free_item *m_free;
    enum  { FREE_MAGIC = 0x0A11F0EE };
    
    static constexpr uint32_t get_item_size(uint32_t item_size)
        { return sizeof(free_item) > item_size ? sizeof(free_item) : item_size; }
    
    page_list *allocator;
    size_t     free_count;
    
public:
    
    inline pool_allocator(uint32_t page_size, uint32_t item_size_)
        : base_allocator(get_item_size(item_size_)), allocator(nullptr), m_free(nullptr), free_count(0)
    {
        allocator = new page_list(page_size / item_size, item_size);
    }
    inline ~pool_allocator() { delete allocator; }
    
    inline void *create()
    {
        void *item = (void *)m_free;
        if (m_free) { m_free = m_free->next; free_count--; }
        else item = allocator->allocate();
        count++;
        allocated++;
        return item;
    }

    inline void destroy(void *u)
    {
        allocator->deallocate(u);
        free_item *item = (free_item *)u;
        if (item->magic == FREE_MAGIC) {
            throw std::exception();
        }
        item->magic = FREE_MAGIC;
        item->next  = m_free;
        m_free      = item;
        count--;
        free_count++;
        freed++;
    }

    inline size_t get_page_count() const { return allocator->get_page_count(); }
};
