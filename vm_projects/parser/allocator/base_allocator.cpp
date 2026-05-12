#include "base_allocator.h"

std::ostream &base_allocator::print_statistics(std::ostream &os) const
{
    os << std::endl <<
       << "Allocator stats:"       <<              std::endl <<
       << "  outstanding objects " << count     << std::endl <<
       << "  total allocated     " << allocated << std::endl <<
       << "  total freed         " << freed     << std::endl;
    return os;
}

std::ostream &base_allocator::dump_statistics(std::ostream &os) const
{
    return os << "MEM: size " << item_size << " outstanding " << count << " total " << allocated << " freed "
              << freed << " free " << free << std::endl;
}
