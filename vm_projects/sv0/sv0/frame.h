#pragma once

#include <iostream>
#include <cmath>
#include <memory.h>

#include "stack.h"
#include "object.h"
#include "pair.h"
#include "stack_objects.h"
#include "opcodes.h"

struct frame : public object
{
    stack<tag>                            _s;
    tag                                  *_locals;
    const code_t                         *_ip;
    frame                                *_previous;
    
    frame(const code_t *ip, tag *data, size_t capacity, tag *locals, frame *previous = nullptr)
        : _ip(ip), _s(data, capacity), _locals(locals), _previous(previous) {}
    
    frame                     *child_frame()                       { auto *f = new frame(*this); f->_previous = this; f->_s = _s.substack(); return f; }
    Code                       next_op()                           { return static_cast<Code>(static_cast<uint32_t>(*_ip++)); }
    Type                       type()                        const { return FRAME; }
    void                       local()                             { local(read<int16_t>()); }
    void                       local(uint16_t id)                  { _s.top() = _locals[id]; }
    void                       set()                               { set(read<int16_t>()); }
    void                       set(uint16_t id)                    { _locals[id] = _s.pop(); }
    template <typename T> T    read()                              { const code_t *p = _ip; _ip += sizeof(T); return *reinterpret_cast<const T *>(p); }
    template <typename T> void push(const T &t)                    { _s.push().set(t); }
    template <typename T> void read_push()                         { _s.push().set(read<T>()); }
    void                       push_nil()                          { _s.push(tag::nil); }
    void                       branch(bool condition)              { int16_t to = read<int16_t>(); if (condition) _ip += to; }
    
    void                       error_opcode(Code oc)               { uint32_t u = static_cast<uint32_t>(oc); std::cerr << "opcode error : " << opcodes::name(u) << "/" << u << "\n"; }
    void                       error_nil_fn()                      { std::cerr << "nil function\n"; }
    
    std::ostream              &print_stack(std::ostream &os) const { return _s.print_stack(os); }
};
