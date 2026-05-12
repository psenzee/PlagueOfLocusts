#include <iostream>
#include <string_view>
#include "json_parser.h"

void json_parse_test()
{
    std::string data("{\"able\":[\"a\",\"b\"],true:\"yes\"}");
    json_parser<json_parser_delegate, std::string::const_iterator> parser;
    parser.parse(data.begin(), data.end());
    
    std::cout << std::endl << "expecting identifier error: ";
    data = "{\"able\":[a,\"b\"],true:\"yes\"}"; // should get an identifier error
    parser.parse(data.begin(), data.end());
    
    std::cout << std::endl;
}
