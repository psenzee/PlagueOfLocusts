#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <ctime>
#include <queue>
#include <random>
#include <algorithm>
#include <cstring>
#include "bits.h"
#include "bit_array.h"
#include "map_data.h"
#include "grid.h"
#include "rules.h"

bit_array<1024> random_bit_array(std::mt19937 &mt, size_t count)
{
    bit_array<1024> ba;
    for (size_t i = 0; i < count; i++)
        ba.set(mt() % 1024);
    return ba;
}

void test_bit_find_next_set_bit()
{
    uint64_t test[] = { 0b0001000010000001000000100000100111001000111111110000100010101010ull };
    std::cout << "dd " << ffsll(0) << std::endl;
    std::cout << "test #0 " << bit_find_next_set_bit<uint64_t>(test[0], 0) << std::endl;
    std::cout << "test #1 " << bit_find_next_set_bit<uint64_t>(test[0], 1) << std::endl;
    std::cout << "test #2 " << bit_find_next_set_bit<uint64_t>(test[0], 2) << std::endl;
    std::mt19937 mt;
    for (size_t i = 0; i < 128; i++) {
        bit_array<1024> ba(random_bit_array(mt, mt() % 256));
        if (ba.empty()) continue;
        //for (size_t k = 0; k< 512; k++) {
            ssize_t k = mt() % 512, a = ba.find_next_set(k), b = bit_find_next_set_bit(ba.data(), 16, k);
            if (a != b) {
                std::cout << "fns fail! " << a << "!=" << b << " k " << k << " count " << ba.count() << " (" << i << ")" << std::endl;
                std::cout << " bits ";
                ba.print_bin(std::cout) << std::endl;
                const uint64_t *data = ba.data();
                bit_find_next_set_bit(data, 16, k);
            }
        //}
    }
}
