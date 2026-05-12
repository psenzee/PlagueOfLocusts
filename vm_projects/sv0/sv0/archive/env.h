#pragma once

#include <iostream>
#include <cmath>
#include <memory.h>

#include "opcodes.h"
#include "stack.h"
#include "tag.h"
#include "object.h"
#include "pair.h"
#include "tag_ops.h"
#include "global.h"
#include "stack_objects.h"

struct env
{
    stack<tag>                            _s;
    stack_objects                         _objs;
    tag                                  *_locals;
    const code_t                         *_ip;
    global                               *_global;
    
    env(const code_t *ip, tag *data, size_t count, tag *locals, global *global) : _ip(ip), _s(data, count), _locals(locals), _global(global), _objs(_s) {}
    
    size_t run()
    {
        if (!_ip) {
            return 0;
        }
        while (true) {
            uint32_t u  = static_cast<uint32_t>(*_ip++);
            Code     oc = static_cast<Code>(u);
            switch (oc) {
                // stack
                case Code::I8:              read_push<int8_t>();                                   break;
                case Code::I16:             read_push<int16_t>();                                  break;
                case Code::I32:             read_push<int32_t>();                                  break;
                case Code::U8:              read_push<uint8_t>();                                  break;
                case Code::U16:             read_push<uint16_t>();                                 break;
                case Code::U32:             read_push<uint32_t>();                                 break;
                case Code::F32:             read_push<float>();                                    break;
                case Code::CONST:           read_push<uint16_t>();                                 break;
                case Code::LOCAL:           local();                                               break;
                case Code::SET:             set();                                                 break;
                case Code::NIL:             push_nil();                                            break;

                case Code::DROP:            _s.drop();                                             break;
                case Code::DUPE:            _s.dupe();                                             break;
                case Code::SWAP:            _s.swap();                                             break;
                case Code::OVER:            _s.over();                                             break;
                    
                case Code::ADD:             _s.at(1) += _s.top(); _s.drop();                       break;
                case Code::SUB:             _s.at(1) -= _s.top(); _s.drop();                       break;
                case Code::MUL:             _s.at(1) *= _s.top(); _s.drop();                       break;
                case Code::DIV:             _s.at(1) /= _s.top(); _s.drop();                       break;
                    
                case Code::COMPARE:         _s.at(1) -= _s.top(); _s.drop();                       break;
                    
                case Code::BIT_AND:         _s.at(1) &= _s.top(); _s.drop();                       break;
                case Code::BIT_OR:          _s.at(1) |= _s.top(); _s.drop();                       break;
                case Code::BIT_XOR:         _s.at(1) ^= _s.top(); _s.drop();                       break;
                case Code::BIT_NOT:         _s.top() = ~_s.top();                                  break;
                    
                case Code::INC:             _s.top()++;                                            break;
                case Code::DEC:             _s.top()--;                                            break;
                    
                case Code::NOT:             _s.top() =  !_s.top();                                 break;
                case Code::BOOL:            _s.top() = !!_s.top();                                 break;
                    
                case Code::PAIR:            _objs.repw_pair();                                     break;
                case Code::HEAD:            _objs.repw_head();                                     break;
                case Code::TAIL:            _objs.repw_tail();                                     break;
                case Code::LENGTH:          _objs.repw_length();                                   break;
                    
                case Code::LIST:            _objs.repw_list(read<uint16_t>());                     break;
                case Code::LISTR:           _objs.repw_listr(read<uint16_t>());                    break;

                case Code::PRTOP:           _s.top().print(stream_out()) << "\n";                  break;
                case Code::PRINT:           print(stream_out(), _s) << "\n";                       break;
                    
                case Code::JMP:             branch(true);                                          break;
                case Code::JLTZ:            branch(_s.pop().lt_zero());                            break;
                case Code::JLTEZ:           branch(_s.pop().lte_zero());                           break;
                case Code::JGTZ:            branch(_s.pop().gt_zero());                            break;
                case Code::JGTEZ:           branch(_s.pop().gte_zero());                           break;
                case Code::JZ:              branch(_s.pop().is_zero());                            break;
                case Code::JNZ:             branch(_s.pop().is_not_zero());                        break;
                    
                case Code::HALT:            _ip--;                                                 return 1;
                case Code::ERROR:
                default:                    std::cerr << "bad opcode error : "
                                                      << opcodes::name(u) << "/" << u << "\n";     return 0;
            }
        }
        return 0;
    }
    
    void                       set_ip(const code_t *ip)       { _ip = ip; }
    const code_t              *get_ip()                 const { return _ip; }
    std::ostream              &stream_out()                   { return _global->stream_out(); }
    
private:

    void                       local()                        { local(read<int16_t>()); }
    void                       local(uint16_t id)             { _s.top() = _locals[id]; }
    void                       set()                          { set(read<int16_t>()); }
    void                       set(uint16_t id)               { _locals[id] = _s.pop(); }
    template <typename T> T    read()                         { const code_t *p = _ip; _ip += sizeof(T); return *reinterpret_cast<const T *>(p); }
    template <typename T> void push(const T &t)               { _s.push().set(t); }
    template <typename T> void read_push()                    { _s.push().set(read<T>()); }
    void                       push_nil()                     { _s.push(tag::nil); }
    void                       branch(bool condition)         { int16_t to = read<int16_t>(); if (condition) _ip += to; }

    std::ostream              &print(std::ostream &os, const stack<tag> &s) const
    {
        for (auto v = s._back; v >= s._; --v) {
            v->print(os << " ");
        }
        return os;
    }
};
