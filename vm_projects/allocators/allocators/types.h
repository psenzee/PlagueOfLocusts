#pragma once

#include <cstddef>
#include <cstdint>

enum       TypeId       : uint16_t
{
    SHL  = 4,
    
    N    = 0,          I     = 1,          F     = 2,          L     = 3,
    
    N_   = N << SHL,   I_    = I << SHL,   F_    = F << SHL,   L_    = L << SHL,
    
    NN   = N_ | N,     NI    = N_ | I,     NF    = N_ | F,     NL   = N_ | L,
    IN   = I_ | N,     II    = I_ | I,     IF    = I_ | F,     IL   = I_ | L,
    FN   = F_ | N,     FI    = F_ | I,     FF    = F_ | F,     FL   = F_ | L,
    LN   = L_ | N,     LI    = L_ | I,     LF    = L_ | F,     LL   = L_ | L,
    
    NN_  = NN << SHL,  NI_   = NI << SHL,  NF_   = NF << SHL,  NL_  = NL << SHL,
    IN_  = IN << SHL,  II_   = II << SHL,  IF_   = IF << SHL,  IL_  = IL << SHL,
    FN_  = FN << SHL,  FI_   = FI << SHL,  FF_   = FF << SHL,  FL_  = FL << SHL,
    LN_  = LN << SHL,  LI_   = LI << SHL,  LF_   = LF << SHL,  LL_  = LL << SHL,

    NNN  = NN_ | N,    NIN   = NI_ | N,    NFN   = NF_ | N,    NLN  = NL_ | N,
    INN  = IN_ | N,    IIN   = II_ | N,    IFN   = IF_ | N,    ILN  = IL_ | N,
    FNN  = FN_ | N,    FIN   = FI_ | N,    FFN   = FF_ | N,    FLN  = FL_ | N,
    LNN  = LN_ | N,    LIN   = LI_ | N,    LFN   = LF_ | N,    LLN  = LL_ | N,
    
    NNI  = NN_ | I,    NII   = NI_ | I,    NFI   = NF_ | I,    NLI  = NL_ | I,
    INI  = IN_ | I,    III   = II_ | I,    IFI   = IF_ | I,    ILI  = IL_ | I,
    FNI  = FN_ | I,    FII   = FI_ | I,    FFI   = FF_ | I,    FLI  = FL_ | I,
    LNI  = LN_ | I,    LII   = LI_ | I,    LFI   = LF_ | I,    LLI  = LL_ | I,
    
    NNF  = NN_ | F,    NIF   = NI_ | F,    NFF   = NF_ | F,    NLF  = NL_ | F,
    INF  = IN_ | F,    IIF   = II_ | F,    IFF   = IF_ | F,    ILF  = IL_ | F,
    FNF  = FN_ | F,    FIF   = FI_ | F,    FFF   = FF_ | F,    FLF  = FL_ | F,
    LNF  = LN_ | F,    LIF   = LI_ | F,    LFF   = LF_ | F,    LLF  = LL_ | F,
    
    NNL  = NN_ | L,    NIL   = NI_ | L,    NFL   = NF_ | L,    NLL  = NL_ | L,
    INL  = IN_ | L,    IIL   = II_ | L,    IFL   = IF_ | L,    ILL  = IL_ | L,
    FNL  = FN_ | L,    FIL   = FI_ | L,    FFL   = FF_ | L,    FLL  = FL_ | L,
    LNL  = LN_ | L,    LIL   = LI_ | L,    LFL   = LF_ | L,    LLL  = LL_ | L,
};

enum class Type         : uint8_t  { NONE = TypeId::N, INT = TypeId::I, FLOAT = TypeId::F, FUNC = TypeId::L };
enum       TypeMask     : uint8_t  { NONE = 1 << N, INT = 1 << I, FLOAT = 1 << F, FUNC = 1 << L };

struct function;

inline Type type_for(const int64_t &)  { return Type::INT; }
inline Type type_for(const float &)    { return Type::FLOAT; }
inline Type type_for(const function *) { return Type::FUNC; }
inline Type type_for(const function &) { return Type::FUNC; }

template <typename T> struct type_traits
{
    static constexpr const Type type = Type::NONE;
};

template <> struct type_traits<float>
{
    static constexpr const Type type = Type::FLOAT;
};

template <> struct type_traits<int64_t>
{
    static constexpr const Type type = Type::INT;
};

template <> struct type_traits<function *>
{
    static constexpr const Type type = Type::FUNC;
};

template <> struct type_traits<const function *>
{
    static constexpr const Type type = Type::FUNC;
};

typedef int16_t code_t;
