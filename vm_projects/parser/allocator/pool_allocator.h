#pragma once

#include <cstring>
#include <cstdio>
#include <vector>
#include <new>
#include <exception>

#include "base_allocator.h"
#include "page_allocator.h"

class pool_allocator : public base_allocator
{
    struct free_item
    {
        int   magic;
        item *next;
    };

    free_item *m_free;
    enum  { FREE_MAGIC = 0x0A11F0EE };

    static constexpr size_t get_item_size(size_t item_size) { return sizeof(free_item) > item_size ? sizeof(free_item) : item_size; }

    page_allocator *allocator;

public:

    inline pool_allocator(size_t page_size, size_t item_size)
        : allocator(0), m_free(0), count(0), free(0), allocated(0), freed(0), item_size(get_item_size(item_size))
    {
        allocator = new page_allocator(page_size / item_size, get_item_size(item_size));
    }
    inline ~pool_allocator() { delete allocator; }

    void  *create();
    void   destroy(void *u);

    void   print_statistics() const;
    void   dump_statistics(FILE *file) const;
    size_t get_page_count() const;
};

void *PoolAllocator::Create()
{
    void *item = (void *)m_free;
    if (m_free) { m_free = m_free->next; free--; }
    else item = allocator->Create();
    count++;
    allocated++;
    return item;
}

void PoolAllocator::Destroy(void *u)
{
    Item *item = (Item *)u;
    if (item->magic == FREE_MAGIC)
    {
#ifdef WIN32
        throw std::exception("already deleted!!");
#else
        throw std::exception();
#endif
    }
    item->magic = FREE_MAGIC;
    item->next  = m_free;
    m_free      = item;
    count--;
    free++;
    freed++;
}

void PoolAllocator::PrintStatistics() const
{
    printf("\n"
            "Allocator stats:        \n"
            "  outstanding objects %lu\n"
            "  total allocated     %lu\n"
            "  total freed         %lu\n"
            "  free list count     %lu\n", count, allocated, freed, free);
}

size_t PoolAllocator::GetPageCount() const
{
    return allocator->GetPageCount();
}

void PoolAllocator::DumpStatistics(FILE *file) const
{
    fprintf(file, "MEM: size %3lu outstanding %5lu total %5lu freed %5lu free %5lu | pages %5lu\n", allocator->item_size, count, allocated, freed, free, allocator->GetPageCount());
}
