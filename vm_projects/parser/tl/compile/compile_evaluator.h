#pragma once

#include <string_view>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <array>
#include <string>
#include <cctype>
#include <algorithm>
#include <string_view>
#include <deque>
#include <vector>
#include "strs.h"
#include "elems.h"
#include "reader.h"
#include "printer.h"
#include "functions.h"
#include "constants.h"
#include "builtins.h"
#include "compile_context.h"

namespace tl
{

struct compile_evaluator : public base_evaluator
{
    compile_evaluator() : base_evaluator() {}

    elem *bytecode_text(elem *expr, pair *environ, compile_context &cc)
    {
        std::deque<elem *> stack;
        stack.push_back(expr);
        printer pr;
        
        while (!stack.empty()) {
            elem *expr = stack.back();
            stack.pop_back();
            std::ostringstream oss;
            if (!expr || expr->_type == elem::NIL) {
                cc.assembly.push_front("PUSHNIL");
                continue;
            }
            elem::Type type = expr->_type;
            if (type != elem::PAIR) {
                elem *value = expr;
                if (type == elem::SYMBOL) {
                    symbol *sym = static_cast<symbol *>(expr);
                    switch (sym->id) {
                    case symbols::LIST:
                        continue;
                    }
                    sym->print(oss << "; symbol ");
                    value = eval_symbol(static_cast<symbol *>(expr), environ);
                    if ((expr && !value) || expr == value) {
                        cc.assembly.push_front("SYMBOL " + sym->value + " ???");
                    } else {
                        stack.push_back(value);
                        continue;
                    }
                } else if (type == elem::FLOAT) {
                    cc.push(value);
                    continue;
                } else if (type == elem::INT) {
                    cc.push(value);
                    continue;
                } else if (type == elem::FUNCTION) {
                    function_call *fc = static_cast<function_call *>(value);
                    if (!fc->_bytecode.empty()) {
                        fc->print_bytecode(oss);
                        cc.assembly.push_front(oss.str());
                        continue;
                    }
                    fc->print(oss << "PUSHFN ") << std::endl << "CALL";
                    cc.assembly.push_front(oss.str());
                } else {
                    //value->print(oss << "PUSH ");
                    cc.push(value);
                }
                cc.assembly.push_front(oss.str());
                continue;
            }

            pair *pexpr = static_cast<pair *>(expr);
            pexpr = list_macro(pexpr); // this is optional -- do we want to do 
            elem *first = pexpr->head; // if we evaluate at the end, it gives us the possibility of multiple dispatch
            
            if (!first) {              // a nil head is an error, so we don't even have to check tail
                error();
                return nullptr;
            }
            
            pair   *rest = cast<pair>(pexpr->tail);
            symbol *sym  = cast<symbol>(first);
            
            // special forms - quote
            if (sym && sym->id == symbols::QUOTE) {
                auto value = rest->head;
                pr.print(oss << "PUSHQ #" << cc.consts.add(value) << " ; ", value);
                cc.assembly.push_front(oss.str());
                continue;
            }
            
            if (sym && sym->id == symbols::LIST) {
                std::ostringstream oss;
                size_t len = length(rest);
                oss << "LIST_" << length(rest);
                cc.assembly.push_front(oss.str());
                oss.str(std::string());
                oss << "START_LIST_" << length(rest);
                stack.push_back(new symbol(oss.str()));
            }

            pair *p = rest;
            while (p) {
                stack.push_back(head(p));
                p = tail_pair(p);
            }
            stack.push_back(first);
        }
        return nullptr;
    }
};

}
