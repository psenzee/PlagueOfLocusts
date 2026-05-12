#include "types.h"
#include "mvm.h"
/*
typedef float (*ff_fp)(float);
typedef float (*fff_fp)(float, float);
typedef float (*ffff_fp)(float, float, float);

inline void _ff   (env *e, ff_fp fn)   { v->_->setf(fn(v->_->f)); }
inline void _fff  (env *e, fff_fp fn)  { v->_[1].setf(fn(v->_[1].f, v->_[0].f));            ++v->_; }
inline void _ffff (env *e, ffff_fp fn) { v->_[2].setf(fn(v->_[2].f, v->_[1].f, v->_[0].f)); v->_ += 2; }

template <ff_fp Fn> constexpr const signature ff_signature(const char *name)
{
    return signature { tag_type::FUNCN, name, [](svm *v, env *e) -> void { _ff(v, e, Fn); }, {}, 1, 1, { signature::TYPE_FLOAT }, { signature::TYPE_FLOAT }, 0 };
}

template <fff_fp Fn> constexpr const signature fff_signature(const char *name, bool commutative = false)
{
    return signature { tag_type::FUNCN, name, [](svm *v, env *e) -> void { _fff(v, e, Fn); }, {}, 2, 1, { signature::TYPE_FLOAT, signature::TYPE_FLOAT }, { signature::TYPE_FLOAT }, uint16_t(commutative ? signature::FLAG_COMMUTATIVE : 0) };
}

template <ffff_fp Fn> constexpr const signature ffff_signature(const char *name, bool commutative = false)
{
    return signature { tag_type::FUNCN, name, [](svm *v, env *e) -> void { _ffff(v, e, Fn); }, {}, 3, 1, { signature::TYPE_FLOAT, signature::TYPE_FLOAT, signature::TYPE_FLOAT }, { signature::TYPE_FLOAT }, uint16_t(commutative ? signature::FLAG_COMMUTATIVE : 0) };
}

constexpr const std::array<signature, 32> signatures = {
    ff_signature<std::expf>   ("exp"),
    ff_signature<std::exp2f>  ("exp2"),
    ff_signature<std::fabsf>  ("abs"),
    ff_signature<std::sinf>   ("sin"),
    ff_signature<std::cosf>   ("cos"),
    ff_signature<std::cosf>   ("tan"),
    ff_signature<std::sinhf>  ("sinh"),
    ff_signature<std::coshf>  ("cosh"),
    ff_signature<std::coshf>  ("tanh"),
    ff_signature<std::asinf>  ("asin"),
    ff_signature<std::acosf>  ("acos"),
    ff_signature<std::acosf>  ("atan"),
    ff_signature<std::asinhf> ("asinh"),
    ff_signature<std::acoshf> ("acosh"),
    ff_signature<std::acoshf> ("atanh"),
    ff_signature<std::sqrtf>  ("sqrt"),
    ff_signature<std::cbrtf>  ("cbrt"),
    fff_signature<std::fmodf> ("fmod"),
    fff_signature<std::fdimf> ("fdim"),
    fff_signature<std::powf>  ("pow"),
    fff_signature<std::fmaxf> ("max", true),
    fff_signature<std::fminf> ("min", true),
    fff_signature<std::hypotf>("hypot"),
    ffff_signature<std::fmaf> ("fma")
};

std::ostream &print_code(std::ostream &os, const code_t *ptr)
{
    os << "[";
    for (const code_t *p = ptr; *p != opcodes::_RET; ++p) {
        if (*p < 0) { os << p; }
        else { os << opcodes::name((opcodes::Code)*p); }
        os << " ";
    }
    return os << "RET]";
}

std::ostream &signature::print(std::ostream &os) const
{
    if (type == tag_type::FUNCN) {
        return os << "fn/" << name << "#" << (void *)fptr;
    }
    os << "fp/" << name << "#";
    return print_code(os, fip.fip);
}

std::ostream &tag::print(std::ostream &os) const
{
    switch (type) {
    case tag::NONE:        return os << "nil";
    case tag::INT:         return os << i;
    case tag::FLOAT:       return os << f;
    case tag::OBJECT:      return obj->print(os);
    case tag::FUNCN:
    case tag::FUNC:
    {
        const signature &s = signatures[signature_id];
        return s.print(os);
    }
    default:               return os;
    }
}

std::ostream &svm::print_stack(std::ostream &os) const
{
    for (auto p = _; p < _back; p++)
        os << *p << " ";
    return os << std::endl;
}

svm::svm(size_t stack_count, const code_t *data) : _ip(data)
{
    _stack = new tag [stack_count];
    _back = _ = _stack + stack_count;
}

size_t svm::run(std::ostream &os)
{
    tag nil;
    nil.i = 0;
    nil.type = tag::NONE;
    if (!_ip) {
        return 0;
    }
    tag t;
    while (true) {
        int16_t code = *_ip++;
        if (code < 0) {
            // push an immediate constant
            (--_)->seti(-code);
            continue;
        }
        switch (code) {
        // stack
        case _DROP:    ++_;                                           break;
        case _DUPE:    *--_ = *_;                                     break;
        case _SWAP:    t = _[0]; _[0] = _[1]; _[1] = t;               break;
        case _OVER:    *--_ = _[1];                                   break;
                
        // s-expr
        case _PUSHNIL: *--_ = nil;                                    break;
        case _PAIR:    _[1].set(new pair(_[0], _[1])); ++_;           break;
        //case _HEAD:   _->set(head(_->e));                            break;
        //case _TAIL:   _->set(tail(_->e));                            break;

        // conversion
        case _F2I:     _->f2i();                                      break;
        case _I2F:     _->i2f();                                      break;
        case _AND:     _[1].i = (_[1].i && _[0].i); ++_;              break;
        case _OR:      _[1].i = (_[1].i || _[0].i); ++_;              break;
        case _NOT:     _->i =  !_->i;                                 break;
        case _BOOL:    _->i = !!_->i;                                 break;
                
        // debug
        case _PRTOP:   os << *_ << std::endl;                         break;
        case _PRINT:   print_stack(os);                               break;
                
        case _FUNC:    _->set(_->i, signatures[_->i]);                break;
        case _CALL:    t = *_++; t.fptr(this, nullptr);               break;

        // integer
        case _IPUSH0:  (--_)->seti(0);                                break;
        case _IPUSH1:  (--_)->seti(1);                                break;
        case _INEG:    _->i = -_->i;                                  break;
        case _INOT:    _->i = ~_->i;                                  break;
        case _ISHR1:   _->i >>= 1;                                    break;
        case _ISHL1:   _->i <<= 1;                                    break;
        case _IADD:    _[1].i += _[0].i; ++_;                         break;
        case _ISUB:    _[1].i -= _[0].i; ++_;                         break;
        case _IMUL:    _[1].i *= _[0].i; ++_;                         break;
        case _IDIV:    _[1].i /= _[0].i; ++_;                         break;
        case _IMOD:    _[1].i %= _[0].i; ++_;                         break;
        case _IAND:    _[1].i &= _[0].i; ++_;                         break;
        case _IOR:     _[1].i |= _[0].i; ++_;                         break;
        case _IXOR:    _[1].i ^= _[0].i; ++_;                         break;
        case _IEQ:     _[1].i = (_[1].i == _[0].i); ++_;              break;
        case _IGT:     _[1].i = (_[1].i >  _[0].i); ++_;              break;
        case _IGTE:    _[1].i = (_[1].i >= _[0].i); ++_;              break;
        case _ILT:     _[1].i = (_[1].i <  _[0].i); ++_;              break;
        case _ILTE:    _[1].i = (_[1].i <= _[0].i); ++_;              break;
                
        // float
        case _FPUSH0:  (--_)->setf(0.);                               break;
        case _FPUSH1:  (--_)->setf(1.);                               break;
        case _FNEG:    _->f = -_->f;                                  break;
        case _FADD:    _[1].f += _[0].f; ++_;                         break;
        case _FSUB:    _[1].f -= _[0].f; ++_;                         break;
        case _FMUL:    _[1].f *= _[0].f; ++_;                         break;
        case _FDIV:    _[1].f /= _[0].f; ++_;                         break;
        case _FEQ:     _[1].seti(_[1].f == _[0].f); ++_;              break;
        case _FGT:     _[1].seti(_[1].f >  _[0].f); ++_;              break;
        case _FGTE:    _[1].seti(_[1].f >= _[0].f); ++_;              break;
        case _FLT:     _[1].seti(_[1].f <  _[0].f); ++_;              break;
        case _FLTE:    _[1].seti(_[1].f <= _[0].f); ++_;              break;
        case _FSIN:    _ff(std::sin);                                 break;

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
        (code_t)-16,
        svm::_FUNC,
        svm::_PRINT,
        svm::_CALL,
        svm::_PUSHNIL,
        (code_t)-131,
        svm::_INEG,
        svm::_PRINT,
        svm::_PAIR,
        svm::_PRTOP,
        (code_t)-131,
        svm::_PAIR,
        svm::_PRINT,
        svm::_HALT
    };
    svm vm(100, data);
    vm.run(std::cout);
}
*/

size_t env::run()
{
    if (!get_ip()) {
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
        //case _OVER:    _ops.over();                             break;
                
        // s-expr
        //case _PUSHNIL: *--_ = nil;                                    break;
        //case _PAIR:    _[1].set(new pair(_[0], _[1])); ++_;           break;
        //case _HEAD:   _->set(head(_->e));                            break;
        //case _TAIL:   _->set(tail(_->e));                            break;

        // conversion
        case _F2I:     _ops.f2i();                                    break;
        case _I2F:     _ops.i2f();                                    break;
                /*
        case _AND:     _[1].i = (_[1].i && _[0].i); ++_;              break;
        case _OR:      _[1].i = (_[1].i || _[0].i); ++_;              break;
        case _NOT:     _->i =  !_->i;                                 break;
        case _BOOL:    _->i = !!_->i;                                 break;
                
        // debug
        case _PRTOP:   os << *_ << std::endl;                         break;
        case _PRINT:   print_stack(os);                               break;
                
        case _FUNC:    _->set(_->i, signatures[_->i]);                break;
        case _CALL:    t = *_++; t.fptr(this, nullptr);               break;

        // integer
        case _IPUSH0:  (--_)->seti(0);                                break;
        case _IPUSH1:  (--_)->seti(1);                                break;
        case _INEG:    _->i = -_->i;                                  break;
        case _INOT:    _->i = ~_->i;                                  break;
        case _ISHR1:   _->i >>= 1;                                    break;
        case _ISHL1:   _->i <<= 1;                                    break;
        case _IADD:    _[1].i += _[0].i; ++_;                         break;
        case _ISUB:    _[1].i -= _[0].i; ++_;                         break;
        case _IMUL:    _[1].i *= _[0].i; ++_;                         break;
        case _IDIV:    _[1].i /= _[0].i; ++_;                         break;
        case _IMOD:    _[1].i %= _[0].i; ++_;                         break;
        case _IAND:    _[1].i &= _[0].i; ++_;                         break;
        case _IOR:     _[1].i |= _[0].i; ++_;                         break;
        case _IXOR:    _[1].i ^= _[0].i; ++_;                         break;
        case _IEQ:     _[1].i = (_[1].i == _[0].i); ++_;              break;
        case _IGT:     _[1].i = (_[1].i >  _[0].i); ++_;              break;
        case _IGTE:    _[1].i = (_[1].i >= _[0].i); ++_;              break;
        case _ILT:     _[1].i = (_[1].i <  _[0].i); ++_;              break;
        case _ILTE:    _[1].i = (_[1].i <= _[0].i); ++_;              break;
                
        // float
        case _FPUSH0:  (--_)->setf(0.);                               break;
        case _FPUSH1:  (--_)->setf(1.);                               break;
        case _FNEG:    _->f = -_->f;                                  break;
        case _FADD:    _[1].f += _[0].f; ++_;                         break;
        case _FSUB:    _[1].f -= _[0].f; ++_;                         break;
        case _FMUL:    _[1].f *= _[0].f; ++_;                         break;
        case _FDIV:    _[1].f /= _[0].f; ++_;                         break;
        case _FEQ:     _[1].seti(_[1].f == _[0].f); ++_;              break;
        case _FGT:     _[1].seti(_[1].f >  _[0].f); ++_;              break;
        case _FGTE:    _[1].seti(_[1].f >= _[0].f); ++_;              break;
        case _FLT:     _[1].seti(_[1].f <  _[0].f); ++_;              break;
        case _FLTE:    _[1].seti(_[1].f <= _[0].f); ++_;              break;
        case _FSIN:    _ff(std::sin);                                 break;
*/
        // flow
        case _HALT:    _ip--;                                         return 1;
        }
    }
    return 0;
}
