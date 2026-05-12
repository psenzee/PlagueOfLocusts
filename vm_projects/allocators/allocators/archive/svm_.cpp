#include "svm.h"

std::ostream &svm::print_stack(std::ostream &os) const
{
    for (auto p = _; p < _back; p++)
        p->print(os) << " ";
    return os;
}

svm::svm(size_t stack_count, const int16_t *data) : _ip(data)
{
    _stack = new item_t [stack_count];
    _back = _ = _stack + stack_count;
}

size_t svm::run(std::ostream &os)
{
    if (!_ip) {
        return 0;
    }
    item_t t;
    while (true) {
        int16_t code = *_ip++;
        switch (code) {
        // stack
        case _DROP:   ++_;                                           break;
        case _DUPE:   *--_ = *_;                                     break;
        case _SWAP:   t = _[0]; _[0] = _[1]; _[1] = t;               break;
        case _TUCK:   t = _[0]; _[0] = _[1]; _[1] = _[2]; _[2] = t;  break;
        case _NIP:    t = _[2]; _[2] = _[1]; _[1] = _[0]; _[0] = t;  break;
        case _OVER:   *--_ = _[1];                                   break;
                
        // conversion
        case _F2I:    _->seti(static_cast<int32_t>(_->f));           break;
        case _I2F:    _->setf(static_cast<float>(_->i));             break;
        case _AND:    _[1].i = (_[1].i && _[0].i); ++_;              break;
        case _OR:     _[1].i = (_[1].i || _[0].i); ++_;              break;
        case _XOR:    _[1].i = (!!_[1].i ^ !!_[0].i); ++_;           break;
        case _NOT:    _[0].i =  !_[0].i;                             break;
        case _BOOL:   _[0].i = !!_[0].i;                             break;
                
        // debug
        case _PRTOP:  _[0].print(os);                                break;
        case _PRINT:  print_stack(os);                               break;

        // integer
        case _IPUSH0: (--_)->seti(0);                                break;
        case _IPUSH1: (--_)->seti(1);                                break;
        case _INEG:   _[0].i = -_[0].i;                              break;
        case _IADD:   _[1].i += _[0].i; ++_;                         break;
        case _ISUB:   _[1].i -= _[0].i; ++_;                         break;
        case _IMUL:   _[1].i *= _[0].i; ++_;                         break;
        case _IDIV:   _[1].i /= _[0].i; ++_;                         break;
        case _IMOD:   _[1].i %= _[0].i; ++_;                         break;
        case _IAND:   _[1].i &= _[0].i; ++_;                         break;
        case _IOR:    _[1].i |= _[0].i; ++_;                         break;
        case _IXOR:   _[1].i ^= _[0].i; ++_;                         break;
        case _INOT:   _[0].i = ~_[0].i;                              break;
        case _IEQ:    _[1].i = (_[1].i == _[0].i); ++_;              break;
        case _IGT:    _[1].i = (_[1].i >  _[0].i); ++_;              break;
        case _IGTE:   _[1].i = (_[1].i >= _[0].i); ++_;              break;
        case _ILT:    _[1].i = (_[1].i <  _[0].i); ++_;              break;
        case _ILTE:   _[1].i = (_[1].i <= _[0].i); ++_;              break;
        case _ISHR1:  _[0].i >>= 1;                                  break;
        case _ISHL1:  _[0].i <<= 1;                                  break;
                
        // float
        case _FPUSH0: (--_)->setf(0.);                               break;
        case _FPUSH1: (--_)->setf(1.);                               break;
        case _FNEG:   _[0].f = -_[0].f;                              break;
        case _FADD:   _[1].f += _[0].f; ++_;                         break;
        case _FSUB:   _[1].f -= _[0].f; ++_;                         break;
        case _FMUL:   _[1].f *= _[0].f; ++_;                         break;
        case _FDIV:   _[1].f /= _[0].f; ++_;                         break;
        case _FEQ:    _[1].seti(_[1].f == _[0].f); ++_;              break;
        case _FGT:    _[1].seti(_[1].f >  _[0].f); ++_;              break;
        case _FGTE:   _[1].seti(_[1].f >= _[0].f); ++_;              break;
        case _FLT:    _[1].seti(_[1].f <  _[0].f); ++_;              break;
        case _FLTE:   _[1].seti(_[1].f <= _[0].f); ++_;              break;

        case _FMOD:   _fff(std::fmod);                               break;
        case _FMIN:   _fff(std::fmin);                               break;
        case _FMAX:   _fff(std::fmax);                               break;
        case _FSQRT:  _ff(std::sqrt);                                break;
        case _FCBRT:  _ff(std::cbrt);                                break;
        case _FABS:   _ff(std::fabs);                                break;
        case _FSIN:   _ff(std::sin);                                 break;
        case _FCOS:   _ff(std::cos);                                 break;
        case _FTAN:   _ff(std::tan);                                 break;
        case _FSINH:  _ff(std::sinh);                                break;
        case _FCOSH:  _ff(std::cosh);                                break;
        case _FTANH:  _ff(std::tanh);                                break;
        case _FASIN:  _ff(std::asin);                                break;
        case _FACOS:  _ff(std::acos);                                break;
        case _FATAN:  _ff(std::atan);                                break;
        case _FASINH: _ff(std::asinh);                               break;
        case _FACOSH: _ff(std::acosh);                               break;
        case _FATANH: _ff(std::atanh);                               break;
        case _FHYPOT: _fff(std::hypot);                              break;
        case _FPOW:   _fff(std::pow);                                break;
        case _FMA:    _ffff(std::fma);                               break;
        case _FEXP:   _ff(std::exp);                                 break;
        case _FEXP2:  _ff(std::exp2);                                break;
        case _FDIM:   _fff(std::fdim);                               break;

        // flow
        case _HALT:   _ip--;                                         return 1;
        }
    }
    return 0;
}

std::ostream &svm::item_t::print(std::ostream &os) const
{
    switch (type) {
    case TYPE_INT:   os << i; break;
    case TYPE_FLOAT: os << f; break;
    default:                  break;
    }
    return os;
}

void test_svm()
{
    const int16_t data[] = {
        svm::_IPUSH1,
        svm::_DUPE,
        svm::_IADD,
        svm::_DUPE,
        svm::_DUPE,
        svm::_IMUL,
        svm::_DUPE,
        svm::_IMUL,
        svm::_SWAP,
        svm::_IDIV,
        svm::_I2F,
        svm::_FSIN,
        svm::_PRINT,
        svm::_HALT
    };
    svm vm(100, data);
    vm.run(std::cout);
}
