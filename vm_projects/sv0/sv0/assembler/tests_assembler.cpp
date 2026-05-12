#include "tests_assembler.h"

#include <iostream>
#include <iomanip>
#include <string_view>
#include "assembler.h"
#include "disassembler.h"

void test_assembler()
{
    std::string_view data(
        "        4 const            \n"
        "        1000               \n"
        "        -23                \n"
        "        i16 2000           \n"
        "        234.3422           \n"
        "        $9                 \n"
        ":label0                    \n"
        "        64.11              \n"
        "        add                \n"
        "        compare            \n"
        "        j<0 label0         \n"
        "        jmp label1         \n"
        ":label1                    \n"
        "        $3                 \n"
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
}
