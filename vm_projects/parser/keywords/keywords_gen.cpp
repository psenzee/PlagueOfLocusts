#include "keywords_gen.h"
/*
void test_reader(std::string_view tst)
{
    identifier_reader<std::string_view::const_iterator> ri;
    auto r = ri.read(tst.begin(), tst.end());
    std::string_view sv(tst.begin(), r.first);
    std::cout << (r.first - tst.begin()) << " " << r.second << " " << sv << std::endl;
}*/

void keywords_gen()
{
    keywords kw;
    /*
    kw.add_keyword("true",    "ID_TRUE");
    kw.add_keyword("trunk",   "ID_TRUNK");
    kw.add_keyword("truer",   "ID_TRUER");
    kw.add_keyword("truery",  "ID_TRUERY");
    kw.add_keyword("trueryo", "ID_TRUERYO");
    kw.add_keyword("truerya", "ID_TRUERYA");
    kw.add_keyword("false",   "ID_FALSE");
    kw.add_keyword("f",       "ID_FALSE");
    kw.add_keyword("null",    "ID_NULL");
    kw.add_keyword("none",    "ID_NONE");
    */
    kw.add_keyword("true",    "ID_TRUE");
    kw.add_keyword("false",   "ID_FALSE");
    kw.add_keyword("null",    "ID_NULL");
    kw.add_keyword("{",       "ID_OP_BRACE");
    kw.add_keyword("}",       "ID_CL_BRACE");
    kw.add_keyword("[",       "ID_OP_BRACKET");
    kw.add_keyword("]",       "ID_CL_BRACKET");
    kw.add_keyword("(",       "ID_OP_PAREN");
    kw.add_keyword(")",       "ID_CL_PAREN");
    kw.add_keyword(",",       "ID_COMMA");
    kw.add_keyword(":",       "ID_COLON");
    kw.add_keyword("//",      "ID_LINE_COMMENT");
    kw.add_keyword("/*",      "ID_O_COMMENT");
    kw.add_keyword("*/",      "ID_C_COMMENT");
    kw.add_keyword("->",      "ID_DEREF");
    kw.add_keyword("<<=",     "ID_LSHIFT_ASSIGN");
    kw.add_keyword(">>=",     "ID_RSHIFT_ASSIGN");
    kw.add_keyword("<<",      "ID_LSHIFT");
    kw.add_keyword(">>",      "ID_RSHIFT");
    kw.add_keyword("<",       "ID_LT");
    kw.add_keyword(">",       "ID_GT");
    kw.add_keyword("<=",      "ID_LTE");
    kw.add_keyword(">=",      "ID_GTE");
    kw.add_keyword("<=>",     "ID_SPACESHIP");
    kw.add_keyword("=",       "ID_ASSIGN");
    kw.add_keyword("==",      "ID_EQUAL");
    kw.print(std::cout);

    std::cout << std::endl << std::endl;
    
    std::cout << kw.generate("identifier_reader") << std::endl;
    /*
    std::string tst("true trunk truer truery f ");
    
    test_reader(tst);
    test_reader("truery");
    test_reader("f");
     */
}
