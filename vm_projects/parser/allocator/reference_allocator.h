#pragma once

#include <iostream>
#include <cstdio>

#include "base_allocator.h"

struct reference_allocator : public base_allocator
{
    inline reference_allocator(size_t, size_t item_size) : base_allocator(item_size) {}

    inline void *create()         { allocated++; count++; return malloc(item_size); }
    inline void  destroy(void *u) { freed++; count--; free(u); }
};
