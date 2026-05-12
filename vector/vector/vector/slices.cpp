#include "slices.h"

#include <iostream>

#include "simple_timer.h"
#include "generators.h"

void test_slices(size_t dims, size_t count)
{
    std::cout << std::endl << std::endl;
    typedef float float_t;
    simple_timer t;
    slices< slice<float_t, uint32_t> > index(dims);
    std::vector<float_t> v;
    std::cout << "generating " << count << " vectors of dimension " << dims << std::endl;
    simple_timer ta;
    vector_generator<float_t> generator(dims);
    for (size_t i = 0; i < count; i++)
        index.add_invalidate(generator.vector());
    ta.print_elapsed("  index generate + invalidating add");
    simple_timer tc;
    index.index();
    tc.print_elapsed("  index index");
    t.print_elapsed("index total");
}
