//
//  main.cpp
//  parser
//
//  Created by paul on 5/18/24.
//

#include <iostream>
#include <cctype>
#include <array>
#include <string_view>
#include <clocale>
#include <ctime>
#include "reader.h"
#include "position.h"
#include "chars_utf8.h"
#include "char_classes_generator.h"
#include "lex_stack.h"
#include "lex.h"
#include "lex_numeric.h"
#include "keywords_gen.h"
#include "json_parser.h"
#include "dct.h"
#include "stock.h"

int matrix_test();

typedef std::string_view::const_iterator sv_iter_t;

void read_lispesq();

void generate_pow_table_implicit();

void json_parse_test();

int main(int argc, const char * argv[])
{
    matrix_test();
    
    keywords_gen();
    
    generate_pow_table_implicit();
    
    dct_test();
    
    char_classes_generator ccg;
    ccg.generate(std::cout);
    //generate_identifier_list();
    test_read_space();
    test_read_number();

    json_parse_test();

    read_lispesq();
    
    time_t tt;
    srand(time(&tt));
    stock::test_stock();
    
    // insert code here...
    std::cout << "\n";
    return 0;
}
