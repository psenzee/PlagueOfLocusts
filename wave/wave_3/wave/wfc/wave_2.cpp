#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <deque>
#include <ctime>
#include <queue>
#include <random>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <bitset>
#include "map_data.h"
#include "grid.h"
#include "rules.h"
#include "patch.h"
#include "state.h"
#include "availability_list.h"
#include "std_array.h"
#include "print_final.h"

namespace wave {
}

int wave_main(int argc, const char * argv[])
{
    //std::random_device rd;
    std::mt19937 mt(100);
    
    typedef std::bitset<256> options_t;

    grid::basic<options_t> grid(size2u(50, 50));
    wave::rules<256> r;

    r.train(map100x256_0, 100, 100);
//  r.train(map100x256_1, 100, 256);
//  r.train(map100x100_2, 100, 100);
//  r.train(map100x100_3, 100, 100); // this one causes failure -- all zeros
//  r.train(map100x100_4, 100, 100);
 // r.train(map25x25_rivers_paths, 25, 25);
//  r.train(map25x25_0, 25, 25);
//  r.train(map100x50_0, 100, 50);
    grid::clear(grid, r.get_max_entropy());
    r.print(std::cout);

    point2i q;
    wave::availability_list<256> list(grid, mt);
    list.initialize_with_grid();
    
    wave::state<256> st(grid, r, mt);
    std::vector<point2i> list_;
    do {
        for (const auto &p : list_) {
            list.enqueue(p);
        }
        list_.clear();
        size_t count_popped = 0, count_pushed = 0;
        while (list.next(q)) {
            count_popped++;
            if (!st.reduce(q)) {
                continue;
            }
            st.process(q);
            if (grid.get(q).count() > 1) {
                count_pushed++;
                list_.push_back(q);
            }
            //std::cout << "size " << list.size() << "/" << list_.size() << std::endl;
        }
    } while (!list_.empty());
    wave::print_final(grid, std::cout, true);
    std::cout << std::endl;
    wave::print_final(grid, std::cout, false);

    return 0;
}
