#include <iostream>
#include <cmath>
#include <array>
#include <vector>

#include "types.h"
#include "function.h"
#include "functions.h"
#include "operands.h"
#include "opcodes.h"

struct env: public opcodes
{
    static constexpr const size_t CALLS_COUNT = 128;
    
    operands                              _ops;
    stack_n<const code_t *, CALLS_COUNT>  _calls;
    const functions                      &_functions;
    const code_t                         *_ip;
    std::ostream                         &_os;
    
    env(const functions &f) : _functions(f), _os(std::cout) {}
    
    void          set_ip(const code_t *ip)       { _ip = ip; }
    const code_t *get_ip()                 const { return _ip; }
    
    void          call_native(const function &f) { f.fptr(this); }
    void          call_vm(const function &f)     { _calls.push(_ip); set_ip(f.fip.ptr); }
    void          ret(const function &f)         { set_ip(_calls.pop()); }
    
    void          call(uint32_t fid)
    {
        const function &f = _functions.get(fid);
        if (f.type == function::Type::NFUNC) { call_native(f); }
        else { call_vm(f); }
    }
    
    size_t run();
};

struct mvm: public opcodes
{
    env *_env;

    mvm(size_t stack_count, const code_t *data);

    size_t run(std::ostream &os);
};
