//
//  main.cpp
//  sv0
//
//  Created by paul on 8/2/24.
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string_view>
#include <limits>
#include "tag.h"
#include "assembler.h"
#include "disassembler.h"
#include "tests_assembler.h"
#include "vm.h"

void test_assemble_and_run()
{
    std::string_view data(R"STR(

                const    4
                push.i16 1000
                push.i8  -23
                push.i16 2000
                push.f   234.3422
                add
                add
                pair
                pair
                print
                set      0
                print
                push.f   64.11
        :label0
                push.f   10.043
                print
                sub
                dupe
                j>0      label0
                local    0
                dupe
                dupe
                tail
                swap
                head
                print
                push.nil
                pair
                print
                push.f32 3.141592653589793
                pair
                print
                push.u8 1
                push.u8 2
                push.u8 3
                push.u8 4
                push.u8 5
                push.u8 6
                print
                list 6
                print
                push.f32 3.141592653589793
                print
                over ; this actually pushes the sin() function to the stack; temporary
                print
                call 1
                print
)STR"
    );

    std::cout << "\n" << data << "\n\n";
    assembler asmb;
    program pr;
    token_iterator_t ti(data.begin(), data.end());
    if (asmb.assemble(ti, pr)) {
        std::cout << "\n\n";
        print_hex(std::cout, pr.bytecode, ' ');
        std::cout << "\n";
    }

    disassembler ds(pr.bytecode);
    std::cout << "\n\ndisassembly:\n";
    ds.disassemble(std::cout);
    std::cout << "\n";

    tag *stack_data = new tag [1024];
    std::array<tag, 16> locals;
    global gl(std::cout);
    //env env_a(reinterpret_cast<const code_t *>(pr.bytecode.data()), stack_data, 1024, locals.data(), &gl);
    //env_a.run();
    frame f(reinterpret_cast<const code_t *>(pr.bytecode.data()), stack_data, 1024, locals.data());
    vm vm_(&f, &gl);
    vm_.run();
}

int main(int argc, const char * argv[])
{
    tag t, u, v;
    t = 43LL;
    //u = 47LL;
    u = 47.f;
    //v = t + u;
    v = t;
    v += u;
    v.print_raw(std::cout) << std::endl;
    v.print(std::cout) << std::endl;
    
    //test_assembler();
    test_assemble_and_run();

    return 0;
}
