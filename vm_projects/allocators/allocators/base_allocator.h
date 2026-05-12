#pragma once

#include <iostream>
#include <cstdint>
#include <cstddef>

struct base_allocator
{
    uint32_t count, allocated, freed, item_size;

    inline base_allocator(uint32_t item_size) : count(0), allocated(0), freed(0), item_size(item_size) {}

    std::ostream &print_statistics(std::ostream &os) const;
    std::ostream &dump_statistics(std::ostream &os) const;
};
