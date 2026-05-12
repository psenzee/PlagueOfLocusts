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
#include "frame.h"
#include "func.h"

struct vm
{
    frame  *_frame;
    global *_global;
    
    vm(frame *f, global *g) : _frame(f), _global(g) {}
    
    size_t run()
    {
        if (!_frame || !_frame->_ip) {
            return 0;
        }
        stack<tag> &s = _frame->_s;
        stack_objects  objs(s);
        while (true) {
            Code oc = _frame->next_op();
            switch (oc) {
            // stack
            case Code::I8:              _frame->read_push<int8_t>();                          break;
            case Code::I16:             _frame->read_push<int16_t>();                         break;
            case Code::I32:             _frame->read_push<int32_t>();                         break;
            case Code::U8:              _frame->read_push<uint8_t>();                         break;
            case Code::U16:             _frame->read_push<uint16_t>();                        break;
            case Code::U32:             _frame->read_push<uint32_t>();                        break;
            case Code::F32:             _frame->read_push<float>();                           break;
            case Code::CONST:           _frame->read_push<uint16_t>();                        break;
            case Code::LOCAL:           _frame->local();                                      break;
            case Code::SET:             _frame->set();                                        break;
            case Code::NIL:             _frame->push_nil();                                   break;

            case Code::DROP:            s.drop();                                             break;
            case Code::DUPE:            s.dupe();                                             break;
            case Code::SWAP:            s.swap();                                             break;
          //case Code::OVER:            s.over();                                             break;
                case Code::OVER:            {
                    const tag *t = _global->_map.get(key(std::string_view("sin")));
                    if (t) s.push(*t);
                }
                break;
                
            case Code::ADD:             s.at(1) += s.top(); s.drop();                         break;
            case Code::SUB:             s.at(1) -= s.top(); s.drop();                         break;
            case Code::MUL:             s.at(1) *= s.top(); s.drop();                         break;
            case Code::DIV:             s.at(1) /= s.top(); s.drop();                         break;
                
            case Code::COMPARE:         s.at(1) -= s.top(); s.drop();                         break;
                
            case Code::BIT_AND:         s.at(1) &= s.top(); s.drop();                         break;
            case Code::BIT_OR:          s.at(1) |= s.top(); s.drop();                         break;
            case Code::BIT_XOR:         s.at(1) ^= s.top(); s.drop();                         break;
            case Code::BIT_NOT:         s.top() = ~s.top();                                   break;
                
            case Code::INC:             s.top()++;                                            break;
            case Code::DEC:             s.top()--;                                            break;
                
            case Code::NOT:             s.top() =  !s.top();                                  break;
            case Code::BOOL:            s.top() = !!s.top();                                  break;
                
            case Code::PAIR:            objs.repw_pair();                                     break;
            case Code::HEAD:            objs.repw_head();                                     break;
            case Code::TAIL:            objs.repw_tail();                                     break;
            case Code::LENGTH:          objs.repw_length();                                   break;
            case Code::LIST:            objs.repw_list(_frame->read<uint16_t>());             break;
            case Code::LISTR:           objs.repw_listr(_frame->read<uint16_t>());            break;

            case Code::PRTOP:           s.top().print(_global->stream_out()) << "\n";         break;
            case Code::PRINT:           _frame->print_stack(_global->stream_out()) << "\n";   break;
                    
            case Code::CALL:            call(_frame->read<uint16_t>());                       break;
                
            case Code::JMP:             _frame->branch(true);                                 break;
            case Code::JLTZ:            _frame->branch(s.pop().lt_zero());                    break;
            case Code::JLTEZ:           _frame->branch(s.pop().lte_zero());                   break;
            case Code::JGTZ:            _frame->branch(s.pop().gt_zero());                    break;
            case Code::JGTEZ:           _frame->branch(s.pop().gte_zero());                   break;
            case Code::JZ:              _frame->branch(s.pop().is_zero());                    break;
            case Code::JNZ:             _frame->branch(s.pop().is_not_zero());                break;
                
            case Code::HALT:            _frame->_ip--;                                        return 1;
            case Code::ERROR:
            default:                    _frame->error_opcode(oc);                             return 0;
            }
        }
        return 0;
    }
    
    void call(uint8_t arg_count)
    {
        auto tag = _frame->_s.pop();
        auto *fn = tag.safe_func();
        if (!fn) {
            // error
            return;
        }
        fn->call(this);
    }
};
