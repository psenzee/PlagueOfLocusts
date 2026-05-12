#include "types.h"
#include "mvm.h"

size_t env::run()
{
    if (!_ip) {
        return 0;
    }
    while (true) {
        int16_t code = *_ip++;
        if (code < 0) {
            // push an immediate constant
            _ops.pushi(-code);
            continue;
        }
        switch (code) {
        // stack
        case _DROP:    _ops.drop();                             break;
        case _DUPE:    _ops.dupe();                             break;
        case _SWAP:    _ops.swap();                             break;
        case _OVER:    _ops.over();                             break;

        // conversion
        case _F2I:     _ops.f2i();                              break;
        case _I2F:     _ops.i2f();                              break;
        
        case _AND:     _ops.predi_and();                        break;
        case _OR:      _ops.predi_or();                        break;
        case _NOT:     _ops.predi_not();                        break;
        case _BOOL:    _ops.predi_bool();                       break;

        // debug
        case _PRTOP:   _ops.at(0).print(_os) << "\n";                break;
        case _PRINT:   _ops.print(_os) << "\n";                      break;
        /*
        case _FUNC:    _->set(_->i, signatures[_->i]);                break;
        case _CALL:    t = *_++; t.fptr(this, nullptr);               break;
        */
        // integer
        case _IPUSH0:  _ops.pushi(0);                                 break;
        case _IPUSH1:  _ops.pushi(1);                                 break;
        case _INEG:    _ops.ineg();                                   break;
        case _INOT:    _ops.inot();                                   break;
        case _ISHR1:   _ops.ishr(1);                                  break;
        case _ISHL1:   _ops.ishl(1);                                  break;
        case _IADD:    _ops.iadd();                                   break;
        case _ISUB:    _ops.isub();                                   break;
        case _IMUL:    _ops.imul();                                   break;
        case _IDIV:    _ops.idiv();                                   break;
        case _IMOD:    _ops.imod();                                   break;
        case _IAND:    _ops.iand();                                   break;
        case _IOR:     _ops.ior();                                    break;
        case _IXOR:    _ops.ixor();                                   break;
        case _IEQ:     _ops.predi_eq();                               break;
        case _IGT:     _ops.predi_gt();                               break;
        case _IGTE:    _ops.predi_gte();                              break;
        case _ILT:     _ops.predi_lt();                               break;
        case _ILTE:    _ops.predi_lte();                              break;
                
        // float
        case _FPUSH0:  _ops.pushf(0.f);                               break;
        case _FPUSH1:  _ops.pushf(1.f);                               break;
        case _FNEG:    _ops.fneg();                                   break;
        case _FADD:    _ops.fadd();                                   break;
        case _FSUB:    _ops.fsub();                                   break;
        case _FMUL:    _ops.fmul();                                   break;
        case _FDIV:    _ops.fdiv();                                   break;
        case _FEQ:     _ops.predf_eq();                               break;
        case _FGT:     _ops.predf_gt();                               break;
        case _FGTE:    _ops.predf_gte();                              break;
        case _FLT:     _ops.predf_lt();                               break;
        case _FLTE:    _ops.predf_lte();                              break;
      //case _FSIN:    _ff(std::sin);                                 break;

        // flow
        case _HALT:    _ip--;                                         return 1;
        }
    }
    return 0;
}


void test_svm()
{
    const code_t data[] = {
        (code_t)-131,
        env::_DUPE,
        env::_IADD,
        env::_DUPE,
        env::_DUPE,
        env::_IMUL,
        env::_DUPE,
        env::_IMUL,
        env::_SWAP,
        env::_IDIV,
        env::_I2F,
        env::_FSIN,
        (code_t)-16,
        env::_FUNC,
        env::_PRINT,
        env::_CALL,
        env::_PUSHNIL,
        (code_t)-131,
        env::_INEG,
        env::_PRINT,
        env::_PAIR,
        env::_PRTOP,
        (code_t)-131,
        env::_PAIR,
        env::_PRINT,
        env::_HALT
    };
    functions f;
    env vm(f);
    vm.set_ip(data);
    vm.run();
}
