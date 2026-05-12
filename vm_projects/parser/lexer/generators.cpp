#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

void generate_pow_table_explicit()
{
    for (size_t i = 0; i < 64; i++) {
        printf("    1");
        for (size_t j = 0; j < i; j++) {
            printf("0");
        }
        printf(".,\n");
    }
}

void generate_pow_table_implicit()
{
    for (int i = 0; i < 256; i++) {
        if (i % 8 == 0) {
            std::cout << std::endl << "    ";
        }
        int v = i - 128;
        std::cout << "1e" << v << ", ";
    }
}

