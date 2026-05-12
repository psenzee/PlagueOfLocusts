#include <string_view>
#include <iostream>
#include <array>
#include <string>
#include "tinylisp.h"
#include "elements.h"
#include "lex.h"
#include "lex_numeric.h"
#include "printer.h"
#include "reader.h"

elem_nil nil0;

void read_lispesq()
{
    typedef std::string_view::const_iterator iter_t;
    std::string_view s("(anom sdfkj as d () \"hello\" 10 ?a 11 3.234e-10 der (quote a b c +) @symbol (quote (hello there you)) (quote kjl) 'paul (\"from\" . the_other_side))");
    std::cout << std::endl << s;
    iter_t i = s.begin(), e = s.end();
    element_allocator pool;
    read_context r(pool, s);
    uint64_t a = (r.read_expr(i, e));
    std::cout << std::endl << "#" << std::hex << a << std::endl;
    std::cout << "sizeof tag " << sizeof(tag) << std::endl;
    std::cout << "sizeof ptr " << sizeof(void *) << std::endl;
    printer p(pool);
    p.print(std::cout << std::endl, a);

    std::cout << std::endl << std::endl << "--- tinylisp ----" << std::endl;
    std::cout << std::endl << s << std::endl;
    tl::reader tlr(s);
    tl::printer pr;
    pr.print(std::cout << std::endl, tlr.read()) << std::endl;
    
    std::string_view text("(def fn (lambda (a b c) (+ a b c)))");
    tl::reader program(text);
    tl::tree_evaluator tev;
    std::string_view text1("(list a b c d e f g h)");
    std::cout << std::endl << text1 << std::endl;
    tl::reader pr1(text1);
    auto read1 = pr1.read();
    tl::compile_evaluator cev;
    tl::pair *ex = cev.list_macro(tl::cast<tl::pair>(read1));
    pr.print(std::cout << std::endl << "list macro: ", ex) << std::endl;

    std::string_view text2("(list (pair? '(a b c d e)) (atom? '(a b c d e)) (length '(a b c d e)) (+ (* 2 5) (* (- 10 (/ 234 2)) (/ 235 2.) ) ) )");
    std::cout << std::endl << text2 << std::endl;
    tl::reader pr2(text2);
    auto read = pr2.read();
    auto result = tev.eval(read, nullptr);
    pr.print(std::cout << std::endl, result) << std::endl;
    tl::compile_context cc;
    cev.bytecode_text(read, nullptr, cc);
    cc.print(std::cout);
}
