#include "slices_finder.h"

#include <iostream>

#include "simple_timer.h"
#include "container.h"

void test_slices_finder(size_t count)
{
    constexpr size_t COUNT = 1024;
    typedef container<COUNT, float> container_t;
    typedef slices< slice<float, uint32_t> > slices_t;

    simple_timer t;
    std::cout << std::endl << std::endl << "slices finder prebuilding" << std::endl;
    container_t store;
    create_test_container(store, count);
    slices< slice<float, uint32_t> > index(COUNT);
    std::vector<float> v;
    v.resize(COUNT);
    for (size_t i = 0; i < count; i++) {
        std::copy(store.at(i).begin(), store.at(i).end(), v.begin());
        index.add_invalidate(v);
    }
    index.index();
    t.print_elapsed("slices finder prebuilding complete");

    slices_finder<container_t, slices_t> finder(store, index);
    vector_generator<float_t> generator(COUNT);
    generator.vector(v);
    container_t::vector_t array;
    std::copy(v.begin(), v.end(), array.begin());
    simple_timer tf;
    finder.set_search(array);
    finder.process();
    tf.print_elapsed("slices finder find complete");
}
