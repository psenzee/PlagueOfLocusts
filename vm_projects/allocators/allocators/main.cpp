//
//  main.cpp
//  allocators
//
//  Created by paul on 7/22/24.
//

#include "base_allocator.h"
#include "page_list.h"
#include "pool_allocator.h"
#include <iostream>
#include <cmath>

int test_allocator()
{
    page_list pola(256, 16);
    std::vector<char *> pointers;
    for (size_t i = 0; i < 250; i++) {
        char *p = reinterpret_cast<char *>(pola.allocate());
        snprintf(p, 8, "hello 1");
        pointers.push_back(p);
    }
    for (auto p : pointers)
        pola.deallocate(p);
    pointers.clear();
    // create and delete at once
    for (size_t i = 0; i < 250; i++) {
        char *p = reinterpret_cast<char *>(pola.allocate());
        snprintf(p, 8, "hello 2");
        pola.deallocate(p);
    }
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}

extern void test_svm();

int main(int argc, const char * argv[])
{
    //test_allocator();
    test_svm();
}
