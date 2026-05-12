#include <iostream>
#include <cmath>
#include <array>
#include <vector>

#include "sv0_opcodes.h"

namespace sv0
{

template <typename T>
struct stack
{
    T *_base, *_back, *_;
    stack(T *data, size_t count): _base(data), _back(_base + count), _(_back) {}
    
    const T &top()                       const { return *_; }
    T       &top()                             { return *_; }
    const T &at(size_t n)                const { return _[n]; }
    T       &at(size_t n)                      { return _[n]; }
    bool     eq()                        const { return _[0] == _[1]; }
    bool     eq(size_t n)                const { return _[n] == _[n + 1]; }
    void     set(const T &v, size_t n)   const { _[n] = v; }
    void     set(const T &v)             const { *_ = v; }
    size_t   size()                      const { return size_t(_back - _); }
    void     push(const T &v)                  { *(--_) = v; }
    void     push(const T &v, size_t n)        { for (size_t i = 0; i < n; i++) *(--_) = v; } // a faster way to do this or let the compiler handle it?
    void     drop()                            { ++_; }
    void     drop(size_t n)                    { _ += n; }
    const T &pop()                             { return *_++; }
    void     swap()                            { T t(*_); *_ = _[1]; _[1] = t; }
    void     swap(size_t n)                    { T t(_[n]); _[n] = _[n + 1]; _[n + 1] = t; }
    void     dupe()                            { *(--_) = *_; }
    void     dupe(size_t n)                    { for (size_t i = 0; i < n; i++) *(--_) = *_; }
    void     over()                            { *(--_) = _[1]; }
    bool     empty()                     const { return _ >= _back; }
};

struct env: public opcodes
{
    stack<val>                            _s;
    const code_t                         *_ip;
    std::ostream                         &_os;
    
    env(const code_t *ip, val *data, size_t count): _ip(ip), _s(data, count), _os(std::cout) {}
    
    size_t run()
    {
        if (!_ip) {
            return 0;
        }
        while (true) {
            code_t code = *_ip++;
            if (code < 0) {
                // push an immediate constant
                _s.push(val(-code));
                continue;
            }
            switch (code) {
            // stack
            case DROP:    _s.drop();                             break;
            case DUPE:    _s.dupe();                             break;
            case SWAP:    _s.swap();                             break;
            case OVER:    _s.over();                             break;
                               
            case AND:     _s.predi_and();                        break;
            case OR:      _s.predi_or();                         break;
            case NOT:     _s.predi_not();                        break;
            case BOOL:    _s.predi_bool();                       break;
                
            case PRTOP:   _o.top().print(_os) << "\n";         break;
            case PRINT:   _o.print(_os) << "\n";               break;

            case HALT:    _ip--;                                 return 1;
            }
        }
        return 0;
    }
    
    void          set_ip(const code_t *ip)       { _ip = ip; }
    const code_t *get_ip()                 const { return _ip; }
};

}
