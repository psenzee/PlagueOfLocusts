#include "types.h"
#include "mvm.h"

typedef float (*ff_fp)(float);
typedef float (*fff_fp)(float, float);
typedef float (*ffff_fp)(float, float, float);
/*
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

*/
