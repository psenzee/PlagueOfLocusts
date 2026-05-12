#pragma once

#include "base_evaluator.h"

namespace tl
{

struct environment
{
    pair *expr;
    pair *frames;
    pair *globals;
};

struct tree_evaluator : public base_evaluator
{
    tree_evaluator() : base_evaluator() {}
    
    struct frame
    {
        pair                   *expr;
        size_t                  count;
        std::array<elem *, 64>  fn_args;

        frame(pair *expr) : count(1), expr(expr), fn_args({ head(expr), nullptr }) {}

        void            push(elem *e)             { fn_args[count++] = e; }
        void            set_fn(elem *fn)          { fn_args[0] = fn; }
        elem           *uneval_fn()               { return fn_args[0]; }
        function_call  *fn()                      { return cast<function_call>(fn_args[0]); }
        elem          **args()                    { return (fn_args.data() + 1); }
        elem           *call(pair *environ)       { function_call *f = fn(); return f ? f->call(args()) : nullptr; }
    };

    elem *eval(elem *expr, pair *environ)
    {
        if (!expr || expr->_type == elem::NIL) {
            error();
            return nullptr;
        }
        elem::Type type = expr->_type;
        if (type != elem::PAIR) {
            return type == elem::SYMBOL ? eval_symbol(static_cast<symbol *>(expr), environ) : expr;
        }

        pair *pexpr = static_cast<pair *>(expr);
        elem *first = pexpr->head; // if we evaluate at the end, it gives us the possibility of multiple dispatch
        if (!first) {              // a nil head is an error, so we don't even have to check tail
            error();
            return nullptr;
        }
        frame fr(pexpr);

        pair   *rest = cast<pair>(pexpr->tail);
        symbol *sym  = cast<symbol>(first);

        // special forms - quote
        if (sym && sym->id == symbols::QUOTE) {
            return rest->head;
        }

        pair *p = rest;
        size_t n = 0;
        while (p) {
            fr.push(eval(head(p), environ));
            p = tail_pair(p);
            n++;
        }

        // special forms - list
        if (sym && sym->id == symbols::LIST) {
            return list(fr.args(), n);
        }

        fr.set_fn(eval(fr.uneval_fn(), environ));
        return fr.call(environ);
    }
};

}
