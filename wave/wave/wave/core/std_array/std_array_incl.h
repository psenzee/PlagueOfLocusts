#define A1  std::array<_T, 1>
#define A2  std::array<_T, 2>
#define A3  std::array<_T, 3>
#define A4  std::array<_T, 4>
#define A5  std::array<_T, 5>
#define A6  std::array<_T, 6>
#define A7  std::array<_T, 7>
#define A8  std::array<_T, 8>
#define A9  std::array<_T, 9>
#define A10 std::array<_T, 10>
#define A11 std::array<_T, 11>
#define A12 std::array<_T, 12>
#define A13 std::array<_T, 13>
#define A14 std::array<_T, 14>
#define A15 std::array<_T, 15>
#define A16 std::array<_T, 16>


#if _T_FLOAT == 1
template <> inline A1  invert        (const A1  &a)               { return A1 ({ _T(_T(1) / a[0]) }); }
template <> inline A2  invert        (const A2  &a)               { return A2 ({ _T(_T(1) / a[0]), _T(_T(1) / a[1]) }); }
template <> inline A3  invert        (const A3  &a)               { return A3 ({ _T(_T(1) / a[0]), _T(_T(1) / a[1]), _T(_T(1) / a[2]) }); }
template <> inline A4  invert        (const A4  &a)               { return A4 ({ _T(_T(1) / a[0]), _T(_T(1) / a[1]), _T(_T(1) / a[2]), _T(_T(1) / a[3]) }); }
template <> inline A5  invert        (const A5  &a)               { return A5 ({ _T(_T(1) / a[0]), _T(_T(1) / a[1]), _T(_T(1) / a[2]), _T(_T(1) / a[3]), _T(_T(1) / a[4]) }); }
template <> inline A6  invert        (const A6  &a)               { return A6 ({ _T(_T(1) / a[0]), _T(_T(1) / a[1]), _T(_T(1) / a[2]), _T(_T(1) / a[3]), _T(_T(1) / a[4]), _T(_T(1) / a[5]) }); }
template <> inline A7  invert        (const A7  &a)               { return A7 ({ _T(_T(1) / a[0]), _T(_T(1) / a[1]), _T(_T(1) / a[2]), _T(_T(1) / a[3]), _T(_T(1) / a[4]), _T(_T(1) / a[5]), _T(_T(1) / a[6]) }); }
template <> inline A8  invert        (const A8  &a)               { return A8 ({ _T(_T(1) / a[0]), _T(_T(1) / a[1]), _T(_T(1) / a[2]), _T(_T(1) / a[3]), _T(_T(1) / a[4]), _T(_T(1) / a[5]), _T(_T(1) / a[6]), _T(_T(1) / a[7]) }); }
template <> inline A9  invert        (const A9  &a)               { return A9 ({ _T(_T(1) / a[0]), _T(_T(1) / a[1]), _T(_T(1) / a[2]), _T(_T(1) / a[3]), _T(_T(1) / a[4]), _T(_T(1) / a[5]), _T(_T(1) / a[6]), _T(_T(1) / a[7]), _T(_T(1) / a[8]) }); }
template <> inline A10 invert        (const A10 &a)               { return A10({ _T(_T(1) / a[0]), _T(_T(1) / a[1]), _T(_T(1) / a[2]), _T(_T(1) / a[3]), _T(_T(1) / a[4]), _T(_T(1) / a[5]), _T(_T(1) / a[6]), _T(_T(1) / a[7]), _T(_T(1) / a[8]), _T(_T(1) / a[9]) }); }
template <> inline A11 invert        (const A11 &a)               { return A11({ _T(_T(1) / a[0]), _T(_T(1) / a[1]), _T(_T(1) / a[2]), _T(_T(1) / a[3]), _T(_T(1) / a[4]), _T(_T(1) / a[5]), _T(_T(1) / a[6]), _T(_T(1) / a[7]), _T(_T(1) / a[8]), _T(_T(1) / a[9]), _T(_T(1) / a[10]) }); }
template <> inline A12 invert        (const A12 &a)               { return A12({ _T(_T(1) / a[0]), _T(_T(1) / a[1]), _T(_T(1) / a[2]), _T(_T(1) / a[3]), _T(_T(1) / a[4]), _T(_T(1) / a[5]), _T(_T(1) / a[6]), _T(_T(1) / a[7]), _T(_T(1) / a[8]), _T(_T(1) / a[9]), _T(_T(1) / a[10]), _T(_T(1) / a[11]) }); }
template <> inline A13 invert        (const A13 &a)               { return A13({ _T(_T(1) / a[0]), _T(_T(1) / a[1]), _T(_T(1) / a[2]), _T(_T(1) / a[3]), _T(_T(1) / a[4]), _T(_T(1) / a[5]), _T(_T(1) / a[6]), _T(_T(1) / a[7]), _T(_T(1) / a[8]), _T(_T(1) / a[9]), _T(_T(1) / a[10]), _T(_T(1) / a[11]), _T(_T(1) / a[12]) }); }
template <> inline A14 invert        (const A14 &a)               { return A14({ _T(_T(1) / a[0]), _T(_T(1) / a[1]), _T(_T(1) / a[2]), _T(_T(1) / a[3]), _T(_T(1) / a[4]), _T(_T(1) / a[5]), _T(_T(1) / a[6]), _T(_T(1) / a[7]), _T(_T(1) / a[8]), _T(_T(1) / a[9]), _T(_T(1) / a[10]), _T(_T(1) / a[11]), _T(_T(1) / a[12]), _T(_T(1) / a[13]) }); }
template <> inline A15 invert        (const A15 &a)               { return A15({ _T(_T(1) / a[0]), _T(_T(1) / a[1]), _T(_T(1) / a[2]), _T(_T(1) / a[3]), _T(_T(1) / a[4]), _T(_T(1) / a[5]), _T(_T(1) / a[6]), _T(_T(1) / a[7]), _T(_T(1) / a[8]), _T(_T(1) / a[9]), _T(_T(1) / a[10]), _T(_T(1) / a[11]), _T(_T(1) / a[12]), _T(_T(1) / a[13]), _T(_T(1) / a[14]) }); }
template <> inline A16 invert        (const A16 &a)               { return A16({ _T(_T(1) / a[0]), _T(_T(1) / a[1]), _T(_T(1) / a[2]), _T(_T(1) / a[3]), _T(_T(1) / a[4]), _T(_T(1) / a[5]), _T(_T(1) / a[6]), _T(_T(1) / a[7]), _T(_T(1) / a[8]), _T(_T(1) / a[9]), _T(_T(1) / a[10]), _T(_T(1) / a[11]), _T(_T(1) / a[12]), _T(_T(1) / a[13]), _T(_T(1) / a[14]), _T(_T(1) / a[15]) }); }

template <> inline A1  sqrt          (const A1  &a)               { return A1 ({ _T(std::sqrt(a[0])) }); }
template <> inline A2  sqrt          (const A2  &a)               { return A2 ({ _T(std::sqrt(a[0])), _T(std::sqrt(a[1])) }); }
template <> inline A3  sqrt          (const A3  &a)               { return A3 ({ _T(std::sqrt(a[0])), _T(std::sqrt(a[1])), _T(std::sqrt(a[2])) }); }
template <> inline A4  sqrt          (const A4  &a)               { return A4 ({ _T(std::sqrt(a[0])), _T(std::sqrt(a[1])), _T(std::sqrt(a[2])), _T(std::sqrt(a[3])) }); }
template <> inline A5  sqrt          (const A5  &a)               { return A5 ({ _T(std::sqrt(a[0])), _T(std::sqrt(a[1])), _T(std::sqrt(a[2])), _T(std::sqrt(a[3])), _T(std::sqrt(a[4])) }); }
template <> inline A6  sqrt          (const A6  &a)               { return A6 ({ _T(std::sqrt(a[0])), _T(std::sqrt(a[1])), _T(std::sqrt(a[2])), _T(std::sqrt(a[3])), _T(std::sqrt(a[4])), _T(std::sqrt(a[5])) }); }
template <> inline A7  sqrt          (const A7  &a)               { return A7 ({ _T(std::sqrt(a[0])), _T(std::sqrt(a[1])), _T(std::sqrt(a[2])), _T(std::sqrt(a[3])), _T(std::sqrt(a[4])), _T(std::sqrt(a[5])), _T(std::sqrt(a[6])) }); }
template <> inline A8  sqrt          (const A8  &a)               { return A8 ({ _T(std::sqrt(a[0])), _T(std::sqrt(a[1])), _T(std::sqrt(a[2])), _T(std::sqrt(a[3])), _T(std::sqrt(a[4])), _T(std::sqrt(a[5])), _T(std::sqrt(a[6])), _T(std::sqrt(a[7])) }); }
template <> inline A9  sqrt          (const A9  &a)               { return A9 ({ _T(std::sqrt(a[0])), _T(std::sqrt(a[1])), _T(std::sqrt(a[2])), _T(std::sqrt(a[3])), _T(std::sqrt(a[4])), _T(std::sqrt(a[5])), _T(std::sqrt(a[6])), _T(std::sqrt(a[7])), _T(std::sqrt(a[8])) }); }
template <> inline A10 sqrt          (const A10 &a)               { return A10({ _T(std::sqrt(a[0])), _T(std::sqrt(a[1])), _T(std::sqrt(a[2])), _T(std::sqrt(a[3])), _T(std::sqrt(a[4])), _T(std::sqrt(a[5])), _T(std::sqrt(a[6])), _T(std::sqrt(a[7])), _T(std::sqrt(a[8])), _T(std::sqrt(a[9])) }); }
template <> inline A11 sqrt          (const A11 &a)               { return A11({ _T(std::sqrt(a[0])), _T(std::sqrt(a[1])), _T(std::sqrt(a[2])), _T(std::sqrt(a[3])), _T(std::sqrt(a[4])), _T(std::sqrt(a[5])), _T(std::sqrt(a[6])), _T(std::sqrt(a[7])), _T(std::sqrt(a[8])), _T(std::sqrt(a[9])), _T(std::sqrt(a[10])) }); }
template <> inline A12 sqrt          (const A12 &a)               { return A12({ _T(std::sqrt(a[0])), _T(std::sqrt(a[1])), _T(std::sqrt(a[2])), _T(std::sqrt(a[3])), _T(std::sqrt(a[4])), _T(std::sqrt(a[5])), _T(std::sqrt(a[6])), _T(std::sqrt(a[7])), _T(std::sqrt(a[8])), _T(std::sqrt(a[9])), _T(std::sqrt(a[10])), _T(std::sqrt(a[11])) }); }
template <> inline A13 sqrt          (const A13 &a)               { return A13({ _T(std::sqrt(a[0])), _T(std::sqrt(a[1])), _T(std::sqrt(a[2])), _T(std::sqrt(a[3])), _T(std::sqrt(a[4])), _T(std::sqrt(a[5])), _T(std::sqrt(a[6])), _T(std::sqrt(a[7])), _T(std::sqrt(a[8])), _T(std::sqrt(a[9])), _T(std::sqrt(a[10])), _T(std::sqrt(a[11])), _T(std::sqrt(a[12])) }); }
template <> inline A14 sqrt          (const A14 &a)               { return A14({ _T(std::sqrt(a[0])), _T(std::sqrt(a[1])), _T(std::sqrt(a[2])), _T(std::sqrt(a[3])), _T(std::sqrt(a[4])), _T(std::sqrt(a[5])), _T(std::sqrt(a[6])), _T(std::sqrt(a[7])), _T(std::sqrt(a[8])), _T(std::sqrt(a[9])), _T(std::sqrt(a[10])), _T(std::sqrt(a[11])), _T(std::sqrt(a[12])), _T(std::sqrt(a[13])) }); }
template <> inline A15 sqrt          (const A15 &a)               { return A15({ _T(std::sqrt(a[0])), _T(std::sqrt(a[1])), _T(std::sqrt(a[2])), _T(std::sqrt(a[3])), _T(std::sqrt(a[4])), _T(std::sqrt(a[5])), _T(std::sqrt(a[6])), _T(std::sqrt(a[7])), _T(std::sqrt(a[8])), _T(std::sqrt(a[9])), _T(std::sqrt(a[10])), _T(std::sqrt(a[11])), _T(std::sqrt(a[12])), _T(std::sqrt(a[13])), _T(std::sqrt(a[14])) }); }
template <> inline A16 sqrt          (const A16 &a)               { return A16({ _T(std::sqrt(a[0])), _T(std::sqrt(a[1])), _T(std::sqrt(a[2])), _T(std::sqrt(a[3])), _T(std::sqrt(a[4])), _T(std::sqrt(a[5])), _T(std::sqrt(a[6])), _T(std::sqrt(a[7])), _T(std::sqrt(a[8])), _T(std::sqrt(a[9])), _T(std::sqrt(a[10])), _T(std::sqrt(a[11])), _T(std::sqrt(a[12])), _T(std::sqrt(a[13])), _T(std::sqrt(a[14])), _T(std::sqrt(a[15])) }); }

template <> inline A1  isqrt         (const A1  &a)               { return A1 ({ _T(_T(1) / std::sqrt(a[0])) }); }
template <> inline A2  isqrt         (const A2  &a)               { return A2 ({ _T(_T(1) / std::sqrt(a[0])), _T(_T(1) / std::sqrt(a[1])) }); }
template <> inline A3  isqrt         (const A3  &a)               { return A3 ({ _T(_T(1) / std::sqrt(a[0])), _T(_T(1) / std::sqrt(a[1])), _T(_T(1) / std::sqrt(a[2])) }); }
template <> inline A4  isqrt         (const A4  &a)               { return A4 ({ _T(_T(1) / std::sqrt(a[0])), _T(_T(1) / std::sqrt(a[1])), _T(_T(1) / std::sqrt(a[2])), _T(_T(1) / std::sqrt(a[3])) }); }
template <> inline A5  isqrt         (const A5  &a)               { return A5 ({ _T(_T(1) / std::sqrt(a[0])), _T(_T(1) / std::sqrt(a[1])), _T(_T(1) / std::sqrt(a[2])), _T(_T(1) / std::sqrt(a[3])), _T(_T(1) / std::sqrt(a[4])) }); }
template <> inline A6  isqrt         (const A6  &a)               { return A6 ({ _T(_T(1) / std::sqrt(a[0])), _T(_T(1) / std::sqrt(a[1])), _T(_T(1) / std::sqrt(a[2])), _T(_T(1) / std::sqrt(a[3])), _T(_T(1) / std::sqrt(a[4])), _T(_T(1) / std::sqrt(a[5])) }); }
template <> inline A7  isqrt         (const A7  &a)               { return A7 ({ _T(_T(1) / std::sqrt(a[0])), _T(_T(1) / std::sqrt(a[1])), _T(_T(1) / std::sqrt(a[2])), _T(_T(1) / std::sqrt(a[3])), _T(_T(1) / std::sqrt(a[4])), _T(_T(1) / std::sqrt(a[5])), _T(_T(1) / std::sqrt(a[6])) }); }
template <> inline A8  isqrt         (const A8  &a)               { return A8 ({ _T(_T(1) / std::sqrt(a[0])), _T(_T(1) / std::sqrt(a[1])), _T(_T(1) / std::sqrt(a[2])), _T(_T(1) / std::sqrt(a[3])), _T(_T(1) / std::sqrt(a[4])), _T(_T(1) / std::sqrt(a[5])), _T(_T(1) / std::sqrt(a[6])), _T(_T(1) / std::sqrt(a[7])) }); }
template <> inline A9  isqrt         (const A9  &a)               { return A9 ({ _T(_T(1) / std::sqrt(a[0])), _T(_T(1) / std::sqrt(a[1])), _T(_T(1) / std::sqrt(a[2])), _T(_T(1) / std::sqrt(a[3])), _T(_T(1) / std::sqrt(a[4])), _T(_T(1) / std::sqrt(a[5])), _T(_T(1) / std::sqrt(a[6])), _T(_T(1) / std::sqrt(a[7])), _T(_T(1) / std::sqrt(a[8])) }); }
template <> inline A10 isqrt         (const A10 &a)               { return A10({ _T(_T(1) / std::sqrt(a[0])), _T(_T(1) / std::sqrt(a[1])), _T(_T(1) / std::sqrt(a[2])), _T(_T(1) / std::sqrt(a[3])), _T(_T(1) / std::sqrt(a[4])), _T(_T(1) / std::sqrt(a[5])), _T(_T(1) / std::sqrt(a[6])), _T(_T(1) / std::sqrt(a[7])), _T(_T(1) / std::sqrt(a[8])), _T(_T(1) / std::sqrt(a[9])) }); }
template <> inline A11 isqrt         (const A11 &a)               { return A11({ _T(_T(1) / std::sqrt(a[0])), _T(_T(1) / std::sqrt(a[1])), _T(_T(1) / std::sqrt(a[2])), _T(_T(1) / std::sqrt(a[3])), _T(_T(1) / std::sqrt(a[4])), _T(_T(1) / std::sqrt(a[5])), _T(_T(1) / std::sqrt(a[6])), _T(_T(1) / std::sqrt(a[7])), _T(_T(1) / std::sqrt(a[8])), _T(_T(1) / std::sqrt(a[9])), _T(_T(1) / std::sqrt(a[10])) }); }
template <> inline A12 isqrt         (const A12 &a)               { return A12({ _T(_T(1) / std::sqrt(a[0])), _T(_T(1) / std::sqrt(a[1])), _T(_T(1) / std::sqrt(a[2])), _T(_T(1) / std::sqrt(a[3])), _T(_T(1) / std::sqrt(a[4])), _T(_T(1) / std::sqrt(a[5])), _T(_T(1) / std::sqrt(a[6])), _T(_T(1) / std::sqrt(a[7])), _T(_T(1) / std::sqrt(a[8])), _T(_T(1) / std::sqrt(a[9])), _T(_T(1) / std::sqrt(a[10])), _T(_T(1) / std::sqrt(a[11])) }); }
template <> inline A13 isqrt         (const A13 &a)               { return A13({ _T(_T(1) / std::sqrt(a[0])), _T(_T(1) / std::sqrt(a[1])), _T(_T(1) / std::sqrt(a[2])), _T(_T(1) / std::sqrt(a[3])), _T(_T(1) / std::sqrt(a[4])), _T(_T(1) / std::sqrt(a[5])), _T(_T(1) / std::sqrt(a[6])), _T(_T(1) / std::sqrt(a[7])), _T(_T(1) / std::sqrt(a[8])), _T(_T(1) / std::sqrt(a[9])), _T(_T(1) / std::sqrt(a[10])), _T(_T(1) / std::sqrt(a[11])), _T(_T(1) / std::sqrt(a[12])) }); }
template <> inline A14 isqrt         (const A14 &a)               { return A14({ _T(_T(1) / std::sqrt(a[0])), _T(_T(1) / std::sqrt(a[1])), _T(_T(1) / std::sqrt(a[2])), _T(_T(1) / std::sqrt(a[3])), _T(_T(1) / std::sqrt(a[4])), _T(_T(1) / std::sqrt(a[5])), _T(_T(1) / std::sqrt(a[6])), _T(_T(1) / std::sqrt(a[7])), _T(_T(1) / std::sqrt(a[8])), _T(_T(1) / std::sqrt(a[9])), _T(_T(1) / std::sqrt(a[10])), _T(_T(1) / std::sqrt(a[11])), _T(_T(1) / std::sqrt(a[12])), _T(_T(1) / std::sqrt(a[13])) }); }
template <> inline A15 isqrt         (const A15 &a)               { return A15({ _T(_T(1) / std::sqrt(a[0])), _T(_T(1) / std::sqrt(a[1])), _T(_T(1) / std::sqrt(a[2])), _T(_T(1) / std::sqrt(a[3])), _T(_T(1) / std::sqrt(a[4])), _T(_T(1) / std::sqrt(a[5])), _T(_T(1) / std::sqrt(a[6])), _T(_T(1) / std::sqrt(a[7])), _T(_T(1) / std::sqrt(a[8])), _T(_T(1) / std::sqrt(a[9])), _T(_T(1) / std::sqrt(a[10])), _T(_T(1) / std::sqrt(a[11])), _T(_T(1) / std::sqrt(a[12])), _T(_T(1) / std::sqrt(a[13])), _T(_T(1) / std::sqrt(a[14])) }); }
template <> inline A16 isqrt         (const A16 &a)               { return A16({ _T(_T(1) / std::sqrt(a[0])), _T(_T(1) / std::sqrt(a[1])), _T(_T(1) / std::sqrt(a[2])), _T(_T(1) / std::sqrt(a[3])), _T(_T(1) / std::sqrt(a[4])), _T(_T(1) / std::sqrt(a[5])), _T(_T(1) / std::sqrt(a[6])), _T(_T(1) / std::sqrt(a[7])), _T(_T(1) / std::sqrt(a[8])), _T(_T(1) / std::sqrt(a[9])), _T(_T(1) / std::sqrt(a[10])), _T(_T(1) / std::sqrt(a[11])), _T(_T(1) / std::sqrt(a[12])), _T(_T(1) / std::sqrt(a[13])), _T(_T(1) / std::sqrt(a[14])), _T(_T(1) / std::sqrt(a[15])) }); }

template <> inline A1  cbrt          (const A1  &a)               { return A1 ({ _T(std::cbrt(a[0])) }); }
template <> inline A2  cbrt          (const A2  &a)               { return A2 ({ _T(std::cbrt(a[0])), _T(std::cbrt(a[1])) }); }
template <> inline A3  cbrt          (const A3  &a)               { return A3 ({ _T(std::cbrt(a[0])), _T(std::cbrt(a[1])), _T(std::cbrt(a[2])) }); }
template <> inline A4  cbrt          (const A4  &a)               { return A4 ({ _T(std::cbrt(a[0])), _T(std::cbrt(a[1])), _T(std::cbrt(a[2])), _T(std::cbrt(a[3])) }); }
template <> inline A5  cbrt          (const A5  &a)               { return A5 ({ _T(std::cbrt(a[0])), _T(std::cbrt(a[1])), _T(std::cbrt(a[2])), _T(std::cbrt(a[3])), _T(std::cbrt(a[4])) }); }
template <> inline A6  cbrt          (const A6  &a)               { return A6 ({ _T(std::cbrt(a[0])), _T(std::cbrt(a[1])), _T(std::cbrt(a[2])), _T(std::cbrt(a[3])), _T(std::cbrt(a[4])), _T(std::cbrt(a[5])) }); }
template <> inline A7  cbrt          (const A7  &a)               { return A7 ({ _T(std::cbrt(a[0])), _T(std::cbrt(a[1])), _T(std::cbrt(a[2])), _T(std::cbrt(a[3])), _T(std::cbrt(a[4])), _T(std::cbrt(a[5])), _T(std::cbrt(a[6])) }); }
template <> inline A8  cbrt          (const A8  &a)               { return A8 ({ _T(std::cbrt(a[0])), _T(std::cbrt(a[1])), _T(std::cbrt(a[2])), _T(std::cbrt(a[3])), _T(std::cbrt(a[4])), _T(std::cbrt(a[5])), _T(std::cbrt(a[6])), _T(std::cbrt(a[7])) }); }
template <> inline A9  cbrt          (const A9  &a)               { return A9 ({ _T(std::cbrt(a[0])), _T(std::cbrt(a[1])), _T(std::cbrt(a[2])), _T(std::cbrt(a[3])), _T(std::cbrt(a[4])), _T(std::cbrt(a[5])), _T(std::cbrt(a[6])), _T(std::cbrt(a[7])), _T(std::cbrt(a[8])) }); }
template <> inline A10 cbrt          (const A10 &a)               { return A10({ _T(std::cbrt(a[0])), _T(std::cbrt(a[1])), _T(std::cbrt(a[2])), _T(std::cbrt(a[3])), _T(std::cbrt(a[4])), _T(std::cbrt(a[5])), _T(std::cbrt(a[6])), _T(std::cbrt(a[7])), _T(std::cbrt(a[8])), _T(std::cbrt(a[9])) }); }
template <> inline A11 cbrt          (const A11 &a)               { return A11({ _T(std::cbrt(a[0])), _T(std::cbrt(a[1])), _T(std::cbrt(a[2])), _T(std::cbrt(a[3])), _T(std::cbrt(a[4])), _T(std::cbrt(a[5])), _T(std::cbrt(a[6])), _T(std::cbrt(a[7])), _T(std::cbrt(a[8])), _T(std::cbrt(a[9])), _T(std::cbrt(a[10])) }); }
template <> inline A12 cbrt          (const A12 &a)               { return A12({ _T(std::cbrt(a[0])), _T(std::cbrt(a[1])), _T(std::cbrt(a[2])), _T(std::cbrt(a[3])), _T(std::cbrt(a[4])), _T(std::cbrt(a[5])), _T(std::cbrt(a[6])), _T(std::cbrt(a[7])), _T(std::cbrt(a[8])), _T(std::cbrt(a[9])), _T(std::cbrt(a[10])), _T(std::cbrt(a[11])) }); }
template <> inline A13 cbrt          (const A13 &a)               { return A13({ _T(std::cbrt(a[0])), _T(std::cbrt(a[1])), _T(std::cbrt(a[2])), _T(std::cbrt(a[3])), _T(std::cbrt(a[4])), _T(std::cbrt(a[5])), _T(std::cbrt(a[6])), _T(std::cbrt(a[7])), _T(std::cbrt(a[8])), _T(std::cbrt(a[9])), _T(std::cbrt(a[10])), _T(std::cbrt(a[11])), _T(std::cbrt(a[12])) }); }
template <> inline A14 cbrt          (const A14 &a)               { return A14({ _T(std::cbrt(a[0])), _T(std::cbrt(a[1])), _T(std::cbrt(a[2])), _T(std::cbrt(a[3])), _T(std::cbrt(a[4])), _T(std::cbrt(a[5])), _T(std::cbrt(a[6])), _T(std::cbrt(a[7])), _T(std::cbrt(a[8])), _T(std::cbrt(a[9])), _T(std::cbrt(a[10])), _T(std::cbrt(a[11])), _T(std::cbrt(a[12])), _T(std::cbrt(a[13])) }); }
template <> inline A15 cbrt          (const A15 &a)               { return A15({ _T(std::cbrt(a[0])), _T(std::cbrt(a[1])), _T(std::cbrt(a[2])), _T(std::cbrt(a[3])), _T(std::cbrt(a[4])), _T(std::cbrt(a[5])), _T(std::cbrt(a[6])), _T(std::cbrt(a[7])), _T(std::cbrt(a[8])), _T(std::cbrt(a[9])), _T(std::cbrt(a[10])), _T(std::cbrt(a[11])), _T(std::cbrt(a[12])), _T(std::cbrt(a[13])), _T(std::cbrt(a[14])) }); }
template <> inline A16 cbrt          (const A16 &a)               { return A16({ _T(std::cbrt(a[0])), _T(std::cbrt(a[1])), _T(std::cbrt(a[2])), _T(std::cbrt(a[3])), _T(std::cbrt(a[4])), _T(std::cbrt(a[5])), _T(std::cbrt(a[6])), _T(std::cbrt(a[7])), _T(std::cbrt(a[8])), _T(std::cbrt(a[9])), _T(std::cbrt(a[10])), _T(std::cbrt(a[11])), _T(std::cbrt(a[12])), _T(std::cbrt(a[13])), _T(std::cbrt(a[14])), _T(std::cbrt(a[15])) }); }

template <> inline A1  floor         (const A1  &a)               { return A1 ({ _T(std::floor(a[0])) }); }
template <> inline A2  floor         (const A2  &a)               { return A2 ({ _T(std::floor(a[0])), _T(std::floor(a[1])) }); }
template <> inline A3  floor         (const A3  &a)               { return A3 ({ _T(std::floor(a[0])), _T(std::floor(a[1])), _T(std::floor(a[2])) }); }
template <> inline A4  floor         (const A4  &a)               { return A4 ({ _T(std::floor(a[0])), _T(std::floor(a[1])), _T(std::floor(a[2])), _T(std::floor(a[3])) }); }
template <> inline A5  floor         (const A5  &a)               { return A5 ({ _T(std::floor(a[0])), _T(std::floor(a[1])), _T(std::floor(a[2])), _T(std::floor(a[3])), _T(std::floor(a[4])) }); }
template <> inline A6  floor         (const A6  &a)               { return A6 ({ _T(std::floor(a[0])), _T(std::floor(a[1])), _T(std::floor(a[2])), _T(std::floor(a[3])), _T(std::floor(a[4])), _T(std::floor(a[5])) }); }
template <> inline A7  floor         (const A7  &a)               { return A7 ({ _T(std::floor(a[0])), _T(std::floor(a[1])), _T(std::floor(a[2])), _T(std::floor(a[3])), _T(std::floor(a[4])), _T(std::floor(a[5])), _T(std::floor(a[6])) }); }
template <> inline A8  floor         (const A8  &a)               { return A8 ({ _T(std::floor(a[0])), _T(std::floor(a[1])), _T(std::floor(a[2])), _T(std::floor(a[3])), _T(std::floor(a[4])), _T(std::floor(a[5])), _T(std::floor(a[6])), _T(std::floor(a[7])) }); }
template <> inline A9  floor         (const A9  &a)               { return A9 ({ _T(std::floor(a[0])), _T(std::floor(a[1])), _T(std::floor(a[2])), _T(std::floor(a[3])), _T(std::floor(a[4])), _T(std::floor(a[5])), _T(std::floor(a[6])), _T(std::floor(a[7])), _T(std::floor(a[8])) }); }
template <> inline A10 floor         (const A10 &a)               { return A10({ _T(std::floor(a[0])), _T(std::floor(a[1])), _T(std::floor(a[2])), _T(std::floor(a[3])), _T(std::floor(a[4])), _T(std::floor(a[5])), _T(std::floor(a[6])), _T(std::floor(a[7])), _T(std::floor(a[8])), _T(std::floor(a[9])) }); }
template <> inline A11 floor         (const A11 &a)               { return A11({ _T(std::floor(a[0])), _T(std::floor(a[1])), _T(std::floor(a[2])), _T(std::floor(a[3])), _T(std::floor(a[4])), _T(std::floor(a[5])), _T(std::floor(a[6])), _T(std::floor(a[7])), _T(std::floor(a[8])), _T(std::floor(a[9])), _T(std::floor(a[10])) }); }
template <> inline A12 floor         (const A12 &a)               { return A12({ _T(std::floor(a[0])), _T(std::floor(a[1])), _T(std::floor(a[2])), _T(std::floor(a[3])), _T(std::floor(a[4])), _T(std::floor(a[5])), _T(std::floor(a[6])), _T(std::floor(a[7])), _T(std::floor(a[8])), _T(std::floor(a[9])), _T(std::floor(a[10])), _T(std::floor(a[11])) }); }
template <> inline A13 floor         (const A13 &a)               { return A13({ _T(std::floor(a[0])), _T(std::floor(a[1])), _T(std::floor(a[2])), _T(std::floor(a[3])), _T(std::floor(a[4])), _T(std::floor(a[5])), _T(std::floor(a[6])), _T(std::floor(a[7])), _T(std::floor(a[8])), _T(std::floor(a[9])), _T(std::floor(a[10])), _T(std::floor(a[11])), _T(std::floor(a[12])) }); }
template <> inline A14 floor         (const A14 &a)               { return A14({ _T(std::floor(a[0])), _T(std::floor(a[1])), _T(std::floor(a[2])), _T(std::floor(a[3])), _T(std::floor(a[4])), _T(std::floor(a[5])), _T(std::floor(a[6])), _T(std::floor(a[7])), _T(std::floor(a[8])), _T(std::floor(a[9])), _T(std::floor(a[10])), _T(std::floor(a[11])), _T(std::floor(a[12])), _T(std::floor(a[13])) }); }
template <> inline A15 floor         (const A15 &a)               { return A15({ _T(std::floor(a[0])), _T(std::floor(a[1])), _T(std::floor(a[2])), _T(std::floor(a[3])), _T(std::floor(a[4])), _T(std::floor(a[5])), _T(std::floor(a[6])), _T(std::floor(a[7])), _T(std::floor(a[8])), _T(std::floor(a[9])), _T(std::floor(a[10])), _T(std::floor(a[11])), _T(std::floor(a[12])), _T(std::floor(a[13])), _T(std::floor(a[14])) }); }
template <> inline A16 floor         (const A16 &a)               { return A16({ _T(std::floor(a[0])), _T(std::floor(a[1])), _T(std::floor(a[2])), _T(std::floor(a[3])), _T(std::floor(a[4])), _T(std::floor(a[5])), _T(std::floor(a[6])), _T(std::floor(a[7])), _T(std::floor(a[8])), _T(std::floor(a[9])), _T(std::floor(a[10])), _T(std::floor(a[11])), _T(std::floor(a[12])), _T(std::floor(a[13])), _T(std::floor(a[14])), _T(std::floor(a[15])) }); }

template <> inline A1  round         (const A1  &a)               { return A1 ({ _T(std::round(a[0])) }); }
template <> inline A2  round         (const A2  &a)               { return A2 ({ _T(std::round(a[0])), _T(std::round(a[1])) }); }
template <> inline A3  round         (const A3  &a)               { return A3 ({ _T(std::round(a[0])), _T(std::round(a[1])), _T(std::round(a[2])) }); }
template <> inline A4  round         (const A4  &a)               { return A4 ({ _T(std::round(a[0])), _T(std::round(a[1])), _T(std::round(a[2])), _T(std::round(a[3])) }); }
template <> inline A5  round         (const A5  &a)               { return A5 ({ _T(std::round(a[0])), _T(std::round(a[1])), _T(std::round(a[2])), _T(std::round(a[3])), _T(std::round(a[4])) }); }
template <> inline A6  round         (const A6  &a)               { return A6 ({ _T(std::round(a[0])), _T(std::round(a[1])), _T(std::round(a[2])), _T(std::round(a[3])), _T(std::round(a[4])), _T(std::round(a[5])) }); }
template <> inline A7  round         (const A7  &a)               { return A7 ({ _T(std::round(a[0])), _T(std::round(a[1])), _T(std::round(a[2])), _T(std::round(a[3])), _T(std::round(a[4])), _T(std::round(a[5])), _T(std::round(a[6])) }); }
template <> inline A8  round         (const A8  &a)               { return A8 ({ _T(std::round(a[0])), _T(std::round(a[1])), _T(std::round(a[2])), _T(std::round(a[3])), _T(std::round(a[4])), _T(std::round(a[5])), _T(std::round(a[6])), _T(std::round(a[7])) }); }
template <> inline A9  round         (const A9  &a)               { return A9 ({ _T(std::round(a[0])), _T(std::round(a[1])), _T(std::round(a[2])), _T(std::round(a[3])), _T(std::round(a[4])), _T(std::round(a[5])), _T(std::round(a[6])), _T(std::round(a[7])), _T(std::round(a[8])) }); }
template <> inline A10 round         (const A10 &a)               { return A10({ _T(std::round(a[0])), _T(std::round(a[1])), _T(std::round(a[2])), _T(std::round(a[3])), _T(std::round(a[4])), _T(std::round(a[5])), _T(std::round(a[6])), _T(std::round(a[7])), _T(std::round(a[8])), _T(std::round(a[9])) }); }
template <> inline A11 round         (const A11 &a)               { return A11({ _T(std::round(a[0])), _T(std::round(a[1])), _T(std::round(a[2])), _T(std::round(a[3])), _T(std::round(a[4])), _T(std::round(a[5])), _T(std::round(a[6])), _T(std::round(a[7])), _T(std::round(a[8])), _T(std::round(a[9])), _T(std::round(a[10])) }); }
template <> inline A12 round         (const A12 &a)               { return A12({ _T(std::round(a[0])), _T(std::round(a[1])), _T(std::round(a[2])), _T(std::round(a[3])), _T(std::round(a[4])), _T(std::round(a[5])), _T(std::round(a[6])), _T(std::round(a[7])), _T(std::round(a[8])), _T(std::round(a[9])), _T(std::round(a[10])), _T(std::round(a[11])) }); }
template <> inline A13 round         (const A13 &a)               { return A13({ _T(std::round(a[0])), _T(std::round(a[1])), _T(std::round(a[2])), _T(std::round(a[3])), _T(std::round(a[4])), _T(std::round(a[5])), _T(std::round(a[6])), _T(std::round(a[7])), _T(std::round(a[8])), _T(std::round(a[9])), _T(std::round(a[10])), _T(std::round(a[11])), _T(std::round(a[12])) }); }
template <> inline A14 round         (const A14 &a)               { return A14({ _T(std::round(a[0])), _T(std::round(a[1])), _T(std::round(a[2])), _T(std::round(a[3])), _T(std::round(a[4])), _T(std::round(a[5])), _T(std::round(a[6])), _T(std::round(a[7])), _T(std::round(a[8])), _T(std::round(a[9])), _T(std::round(a[10])), _T(std::round(a[11])), _T(std::round(a[12])), _T(std::round(a[13])) }); }
template <> inline A15 round         (const A15 &a)               { return A15({ _T(std::round(a[0])), _T(std::round(a[1])), _T(std::round(a[2])), _T(std::round(a[3])), _T(std::round(a[4])), _T(std::round(a[5])), _T(std::round(a[6])), _T(std::round(a[7])), _T(std::round(a[8])), _T(std::round(a[9])), _T(std::round(a[10])), _T(std::round(a[11])), _T(std::round(a[12])), _T(std::round(a[13])), _T(std::round(a[14])) }); }
template <> inline A16 round         (const A16 &a)               { return A16({ _T(std::round(a[0])), _T(std::round(a[1])), _T(std::round(a[2])), _T(std::round(a[3])), _T(std::round(a[4])), _T(std::round(a[5])), _T(std::round(a[6])), _T(std::round(a[7])), _T(std::round(a[8])), _T(std::round(a[9])), _T(std::round(a[10])), _T(std::round(a[11])), _T(std::round(a[12])), _T(std::round(a[13])), _T(std::round(a[14])), _T(std::round(a[15])) }); }

template <> inline A1  ceil          (const A1  &a)               { return A1 ({ _T(std::ceil(a[0])) }); }
template <> inline A2  ceil          (const A2  &a)               { return A2 ({ _T(std::ceil(a[0])), _T(std::ceil(a[1])) }); }
template <> inline A3  ceil          (const A3  &a)               { return A3 ({ _T(std::ceil(a[0])), _T(std::ceil(a[1])), _T(std::ceil(a[2])) }); }
template <> inline A4  ceil          (const A4  &a)               { return A4 ({ _T(std::ceil(a[0])), _T(std::ceil(a[1])), _T(std::ceil(a[2])), _T(std::ceil(a[3])) }); }
template <> inline A5  ceil          (const A5  &a)               { return A5 ({ _T(std::ceil(a[0])), _T(std::ceil(a[1])), _T(std::ceil(a[2])), _T(std::ceil(a[3])), _T(std::ceil(a[4])) }); }
template <> inline A6  ceil          (const A6  &a)               { return A6 ({ _T(std::ceil(a[0])), _T(std::ceil(a[1])), _T(std::ceil(a[2])), _T(std::ceil(a[3])), _T(std::ceil(a[4])), _T(std::ceil(a[5])) }); }
template <> inline A7  ceil          (const A7  &a)               { return A7 ({ _T(std::ceil(a[0])), _T(std::ceil(a[1])), _T(std::ceil(a[2])), _T(std::ceil(a[3])), _T(std::ceil(a[4])), _T(std::ceil(a[5])), _T(std::ceil(a[6])) }); }
template <> inline A8  ceil          (const A8  &a)               { return A8 ({ _T(std::ceil(a[0])), _T(std::ceil(a[1])), _T(std::ceil(a[2])), _T(std::ceil(a[3])), _T(std::ceil(a[4])), _T(std::ceil(a[5])), _T(std::ceil(a[6])), _T(std::ceil(a[7])) }); }
template <> inline A9  ceil          (const A9  &a)               { return A9 ({ _T(std::ceil(a[0])), _T(std::ceil(a[1])), _T(std::ceil(a[2])), _T(std::ceil(a[3])), _T(std::ceil(a[4])), _T(std::ceil(a[5])), _T(std::ceil(a[6])), _T(std::ceil(a[7])), _T(std::ceil(a[8])) }); }
template <> inline A10 ceil          (const A10 &a)               { return A10({ _T(std::ceil(a[0])), _T(std::ceil(a[1])), _T(std::ceil(a[2])), _T(std::ceil(a[3])), _T(std::ceil(a[4])), _T(std::ceil(a[5])), _T(std::ceil(a[6])), _T(std::ceil(a[7])), _T(std::ceil(a[8])), _T(std::ceil(a[9])) }); }
template <> inline A11 ceil          (const A11 &a)               { return A11({ _T(std::ceil(a[0])), _T(std::ceil(a[1])), _T(std::ceil(a[2])), _T(std::ceil(a[3])), _T(std::ceil(a[4])), _T(std::ceil(a[5])), _T(std::ceil(a[6])), _T(std::ceil(a[7])), _T(std::ceil(a[8])), _T(std::ceil(a[9])), _T(std::ceil(a[10])) }); }
template <> inline A12 ceil          (const A12 &a)               { return A12({ _T(std::ceil(a[0])), _T(std::ceil(a[1])), _T(std::ceil(a[2])), _T(std::ceil(a[3])), _T(std::ceil(a[4])), _T(std::ceil(a[5])), _T(std::ceil(a[6])), _T(std::ceil(a[7])), _T(std::ceil(a[8])), _T(std::ceil(a[9])), _T(std::ceil(a[10])), _T(std::ceil(a[11])) }); }
template <> inline A13 ceil          (const A13 &a)               { return A13({ _T(std::ceil(a[0])), _T(std::ceil(a[1])), _T(std::ceil(a[2])), _T(std::ceil(a[3])), _T(std::ceil(a[4])), _T(std::ceil(a[5])), _T(std::ceil(a[6])), _T(std::ceil(a[7])), _T(std::ceil(a[8])), _T(std::ceil(a[9])), _T(std::ceil(a[10])), _T(std::ceil(a[11])), _T(std::ceil(a[12])) }); }
template <> inline A14 ceil          (const A14 &a)               { return A14({ _T(std::ceil(a[0])), _T(std::ceil(a[1])), _T(std::ceil(a[2])), _T(std::ceil(a[3])), _T(std::ceil(a[4])), _T(std::ceil(a[5])), _T(std::ceil(a[6])), _T(std::ceil(a[7])), _T(std::ceil(a[8])), _T(std::ceil(a[9])), _T(std::ceil(a[10])), _T(std::ceil(a[11])), _T(std::ceil(a[12])), _T(std::ceil(a[13])) }); }
template <> inline A15 ceil          (const A15 &a)               { return A15({ _T(std::ceil(a[0])), _T(std::ceil(a[1])), _T(std::ceil(a[2])), _T(std::ceil(a[3])), _T(std::ceil(a[4])), _T(std::ceil(a[5])), _T(std::ceil(a[6])), _T(std::ceil(a[7])), _T(std::ceil(a[8])), _T(std::ceil(a[9])), _T(std::ceil(a[10])), _T(std::ceil(a[11])), _T(std::ceil(a[12])), _T(std::ceil(a[13])), _T(std::ceil(a[14])) }); }
template <> inline A16 ceil          (const A16 &a)               { return A16({ _T(std::ceil(a[0])), _T(std::ceil(a[1])), _T(std::ceil(a[2])), _T(std::ceil(a[3])), _T(std::ceil(a[4])), _T(std::ceil(a[5])), _T(std::ceil(a[6])), _T(std::ceil(a[7])), _T(std::ceil(a[8])), _T(std::ceil(a[9])), _T(std::ceil(a[10])), _T(std::ceil(a[11])), _T(std::ceil(a[12])), _T(std::ceil(a[13])), _T(std::ceil(a[14])), _T(std::ceil(a[15])) }); }

template <> inline A1  exp           (const A1  &a)               { return A1 ({ _T(std::exp(a[0])) }); }
template <> inline A2  exp           (const A2  &a)               { return A2 ({ _T(std::exp(a[0])), _T(std::exp(a[1])) }); }
template <> inline A3  exp           (const A3  &a)               { return A3 ({ _T(std::exp(a[0])), _T(std::exp(a[1])), _T(std::exp(a[2])) }); }
template <> inline A4  exp           (const A4  &a)               { return A4 ({ _T(std::exp(a[0])), _T(std::exp(a[1])), _T(std::exp(a[2])), _T(std::exp(a[3])) }); }
template <> inline A5  exp           (const A5  &a)               { return A5 ({ _T(std::exp(a[0])), _T(std::exp(a[1])), _T(std::exp(a[2])), _T(std::exp(a[3])), _T(std::exp(a[4])) }); }
template <> inline A6  exp           (const A6  &a)               { return A6 ({ _T(std::exp(a[0])), _T(std::exp(a[1])), _T(std::exp(a[2])), _T(std::exp(a[3])), _T(std::exp(a[4])), _T(std::exp(a[5])) }); }
template <> inline A7  exp           (const A7  &a)               { return A7 ({ _T(std::exp(a[0])), _T(std::exp(a[1])), _T(std::exp(a[2])), _T(std::exp(a[3])), _T(std::exp(a[4])), _T(std::exp(a[5])), _T(std::exp(a[6])) }); }
template <> inline A8  exp           (const A8  &a)               { return A8 ({ _T(std::exp(a[0])), _T(std::exp(a[1])), _T(std::exp(a[2])), _T(std::exp(a[3])), _T(std::exp(a[4])), _T(std::exp(a[5])), _T(std::exp(a[6])), _T(std::exp(a[7])) }); }
template <> inline A9  exp           (const A9  &a)               { return A9 ({ _T(std::exp(a[0])), _T(std::exp(a[1])), _T(std::exp(a[2])), _T(std::exp(a[3])), _T(std::exp(a[4])), _T(std::exp(a[5])), _T(std::exp(a[6])), _T(std::exp(a[7])), _T(std::exp(a[8])) }); }
template <> inline A10 exp           (const A10 &a)               { return A10({ _T(std::exp(a[0])), _T(std::exp(a[1])), _T(std::exp(a[2])), _T(std::exp(a[3])), _T(std::exp(a[4])), _T(std::exp(a[5])), _T(std::exp(a[6])), _T(std::exp(a[7])), _T(std::exp(a[8])), _T(std::exp(a[9])) }); }
template <> inline A11 exp           (const A11 &a)               { return A11({ _T(std::exp(a[0])), _T(std::exp(a[1])), _T(std::exp(a[2])), _T(std::exp(a[3])), _T(std::exp(a[4])), _T(std::exp(a[5])), _T(std::exp(a[6])), _T(std::exp(a[7])), _T(std::exp(a[8])), _T(std::exp(a[9])), _T(std::exp(a[10])) }); }
template <> inline A12 exp           (const A12 &a)               { return A12({ _T(std::exp(a[0])), _T(std::exp(a[1])), _T(std::exp(a[2])), _T(std::exp(a[3])), _T(std::exp(a[4])), _T(std::exp(a[5])), _T(std::exp(a[6])), _T(std::exp(a[7])), _T(std::exp(a[8])), _T(std::exp(a[9])), _T(std::exp(a[10])), _T(std::exp(a[11])) }); }
template <> inline A13 exp           (const A13 &a)               { return A13({ _T(std::exp(a[0])), _T(std::exp(a[1])), _T(std::exp(a[2])), _T(std::exp(a[3])), _T(std::exp(a[4])), _T(std::exp(a[5])), _T(std::exp(a[6])), _T(std::exp(a[7])), _T(std::exp(a[8])), _T(std::exp(a[9])), _T(std::exp(a[10])), _T(std::exp(a[11])), _T(std::exp(a[12])) }); }
template <> inline A14 exp           (const A14 &a)               { return A14({ _T(std::exp(a[0])), _T(std::exp(a[1])), _T(std::exp(a[2])), _T(std::exp(a[3])), _T(std::exp(a[4])), _T(std::exp(a[5])), _T(std::exp(a[6])), _T(std::exp(a[7])), _T(std::exp(a[8])), _T(std::exp(a[9])), _T(std::exp(a[10])), _T(std::exp(a[11])), _T(std::exp(a[12])), _T(std::exp(a[13])) }); }
template <> inline A15 exp           (const A15 &a)               { return A15({ _T(std::exp(a[0])), _T(std::exp(a[1])), _T(std::exp(a[2])), _T(std::exp(a[3])), _T(std::exp(a[4])), _T(std::exp(a[5])), _T(std::exp(a[6])), _T(std::exp(a[7])), _T(std::exp(a[8])), _T(std::exp(a[9])), _T(std::exp(a[10])), _T(std::exp(a[11])), _T(std::exp(a[12])), _T(std::exp(a[13])), _T(std::exp(a[14])) }); }
template <> inline A16 exp           (const A16 &a)               { return A16({ _T(std::exp(a[0])), _T(std::exp(a[1])), _T(std::exp(a[2])), _T(std::exp(a[3])), _T(std::exp(a[4])), _T(std::exp(a[5])), _T(std::exp(a[6])), _T(std::exp(a[7])), _T(std::exp(a[8])), _T(std::exp(a[9])), _T(std::exp(a[10])), _T(std::exp(a[11])), _T(std::exp(a[12])), _T(std::exp(a[13])), _T(std::exp(a[14])), _T(std::exp(a[15])) }); }

template <> inline A1  abs           (const A1  &a)               { return A1 ({ _T(std::fabs(a[0])) }); }
template <> inline A2  abs           (const A2  &a)               { return A2 ({ _T(std::fabs(a[0])), _T(std::fabs(a[1])) }); }
template <> inline A3  abs           (const A3  &a)               { return A3 ({ _T(std::fabs(a[0])), _T(std::fabs(a[1])), _T(std::fabs(a[2])) }); }
template <> inline A4  abs           (const A4  &a)               { return A4 ({ _T(std::fabs(a[0])), _T(std::fabs(a[1])), _T(std::fabs(a[2])), _T(std::fabs(a[3])) }); }
template <> inline A5  abs           (const A5  &a)               { return A5 ({ _T(std::fabs(a[0])), _T(std::fabs(a[1])), _T(std::fabs(a[2])), _T(std::fabs(a[3])), _T(std::fabs(a[4])) }); }
template <> inline A6  abs           (const A6  &a)               { return A6 ({ _T(std::fabs(a[0])), _T(std::fabs(a[1])), _T(std::fabs(a[2])), _T(std::fabs(a[3])), _T(std::fabs(a[4])), _T(std::fabs(a[5])) }); }
template <> inline A7  abs           (const A7  &a)               { return A7 ({ _T(std::fabs(a[0])), _T(std::fabs(a[1])), _T(std::fabs(a[2])), _T(std::fabs(a[3])), _T(std::fabs(a[4])), _T(std::fabs(a[5])), _T(std::fabs(a[6])) }); }
template <> inline A8  abs           (const A8  &a)               { return A8 ({ _T(std::fabs(a[0])), _T(std::fabs(a[1])), _T(std::fabs(a[2])), _T(std::fabs(a[3])), _T(std::fabs(a[4])), _T(std::fabs(a[5])), _T(std::fabs(a[6])), _T(std::fabs(a[7])) }); }
template <> inline A9  abs           (const A9  &a)               { return A9 ({ _T(std::fabs(a[0])), _T(std::fabs(a[1])), _T(std::fabs(a[2])), _T(std::fabs(a[3])), _T(std::fabs(a[4])), _T(std::fabs(a[5])), _T(std::fabs(a[6])), _T(std::fabs(a[7])), _T(std::fabs(a[8])) }); }
template <> inline A10 abs           (const A10 &a)               { return A10({ _T(std::fabs(a[0])), _T(std::fabs(a[1])), _T(std::fabs(a[2])), _T(std::fabs(a[3])), _T(std::fabs(a[4])), _T(std::fabs(a[5])), _T(std::fabs(a[6])), _T(std::fabs(a[7])), _T(std::fabs(a[8])), _T(std::fabs(a[9])) }); }
template <> inline A11 abs           (const A11 &a)               { return A11({ _T(std::fabs(a[0])), _T(std::fabs(a[1])), _T(std::fabs(a[2])), _T(std::fabs(a[3])), _T(std::fabs(a[4])), _T(std::fabs(a[5])), _T(std::fabs(a[6])), _T(std::fabs(a[7])), _T(std::fabs(a[8])), _T(std::fabs(a[9])), _T(std::fabs(a[10])) }); }
template <> inline A12 abs           (const A12 &a)               { return A12({ _T(std::fabs(a[0])), _T(std::fabs(a[1])), _T(std::fabs(a[2])), _T(std::fabs(a[3])), _T(std::fabs(a[4])), _T(std::fabs(a[5])), _T(std::fabs(a[6])), _T(std::fabs(a[7])), _T(std::fabs(a[8])), _T(std::fabs(a[9])), _T(std::fabs(a[10])), _T(std::fabs(a[11])) }); }
template <> inline A13 abs           (const A13 &a)               { return A13({ _T(std::fabs(a[0])), _T(std::fabs(a[1])), _T(std::fabs(a[2])), _T(std::fabs(a[3])), _T(std::fabs(a[4])), _T(std::fabs(a[5])), _T(std::fabs(a[6])), _T(std::fabs(a[7])), _T(std::fabs(a[8])), _T(std::fabs(a[9])), _T(std::fabs(a[10])), _T(std::fabs(a[11])), _T(std::fabs(a[12])) }); }
template <> inline A14 abs           (const A14 &a)               { return A14({ _T(std::fabs(a[0])), _T(std::fabs(a[1])), _T(std::fabs(a[2])), _T(std::fabs(a[3])), _T(std::fabs(a[4])), _T(std::fabs(a[5])), _T(std::fabs(a[6])), _T(std::fabs(a[7])), _T(std::fabs(a[8])), _T(std::fabs(a[9])), _T(std::fabs(a[10])), _T(std::fabs(a[11])), _T(std::fabs(a[12])), _T(std::fabs(a[13])) }); }
template <> inline A15 abs           (const A15 &a)               { return A15({ _T(std::fabs(a[0])), _T(std::fabs(a[1])), _T(std::fabs(a[2])), _T(std::fabs(a[3])), _T(std::fabs(a[4])), _T(std::fabs(a[5])), _T(std::fabs(a[6])), _T(std::fabs(a[7])), _T(std::fabs(a[8])), _T(std::fabs(a[9])), _T(std::fabs(a[10])), _T(std::fabs(a[11])), _T(std::fabs(a[12])), _T(std::fabs(a[13])), _T(std::fabs(a[14])) }); }
template <> inline A16 abs           (const A16 &a)               { return A16({ _T(std::fabs(a[0])), _T(std::fabs(a[1])), _T(std::fabs(a[2])), _T(std::fabs(a[3])), _T(std::fabs(a[4])), _T(std::fabs(a[5])), _T(std::fabs(a[6])), _T(std::fabs(a[7])), _T(std::fabs(a[8])), _T(std::fabs(a[9])), _T(std::fabs(a[10])), _T(std::fabs(a[11])), _T(std::fabs(a[12])), _T(std::fabs(a[13])), _T(std::fabs(a[14])), _T(std::fabs(a[15])) }); }
#endif

template <> inline A1  negative      (const A1  &a)               { return A1 ({ _T(-a[0]) }); }
template <> inline A2  negative      (const A2  &a)               { return A2 ({ _T(-a[0]), _T(-a[1]) }); }
template <> inline A3  negative      (const A3  &a)               { return A3 ({ _T(-a[0]), _T(-a[1]), _T(-a[2]) }); }
template <> inline A4  negative      (const A4  &a)               { return A4 ({ _T(-a[0]), _T(-a[1]), _T(-a[2]), _T(-a[3]) }); }
template <> inline A5  negative      (const A5  &a)               { return A5 ({ _T(-a[0]), _T(-a[1]), _T(-a[2]), _T(-a[3]), _T(-a[4]) }); }
template <> inline A6  negative      (const A6  &a)               { return A6 ({ _T(-a[0]), _T(-a[1]), _T(-a[2]), _T(-a[3]), _T(-a[4]), _T(-a[5]) }); }
template <> inline A7  negative      (const A7  &a)               { return A7 ({ _T(-a[0]), _T(-a[1]), _T(-a[2]), _T(-a[3]), _T(-a[4]), _T(-a[5]), _T(-a[6]) }); }
template <> inline A8  negative      (const A8  &a)               { return A8 ({ _T(-a[0]), _T(-a[1]), _T(-a[2]), _T(-a[3]), _T(-a[4]), _T(-a[5]), _T(-a[6]), _T(-a[7]) }); }
template <> inline A9  negative      (const A9  &a)               { return A9 ({ _T(-a[0]), _T(-a[1]), _T(-a[2]), _T(-a[3]), _T(-a[4]), _T(-a[5]), _T(-a[6]), _T(-a[7]), _T(-a[8]) }); }
template <> inline A10 negative      (const A10 &a)               { return A10({ _T(-a[0]), _T(-a[1]), _T(-a[2]), _T(-a[3]), _T(-a[4]), _T(-a[5]), _T(-a[6]), _T(-a[7]), _T(-a[8]), _T(-a[9]) }); }
template <> inline A11 negative      (const A11 &a)               { return A11({ _T(-a[0]), _T(-a[1]), _T(-a[2]), _T(-a[3]), _T(-a[4]), _T(-a[5]), _T(-a[6]), _T(-a[7]), _T(-a[8]), _T(-a[9]), _T(-a[10]) }); }
template <> inline A12 negative      (const A12 &a)               { return A12({ _T(-a[0]), _T(-a[1]), _T(-a[2]), _T(-a[3]), _T(-a[4]), _T(-a[5]), _T(-a[6]), _T(-a[7]), _T(-a[8]), _T(-a[9]), _T(-a[10]), _T(-a[11]) }); }
template <> inline A13 negative      (const A13 &a)               { return A13({ _T(-a[0]), _T(-a[1]), _T(-a[2]), _T(-a[3]), _T(-a[4]), _T(-a[5]), _T(-a[6]), _T(-a[7]), _T(-a[8]), _T(-a[9]), _T(-a[10]), _T(-a[11]), _T(-a[12]) }); }
template <> inline A14 negative      (const A14 &a)               { return A14({ _T(-a[0]), _T(-a[1]), _T(-a[2]), _T(-a[3]), _T(-a[4]), _T(-a[5]), _T(-a[6]), _T(-a[7]), _T(-a[8]), _T(-a[9]), _T(-a[10]), _T(-a[11]), _T(-a[12]), _T(-a[13]) }); }
template <> inline A15 negative      (const A15 &a)               { return A15({ _T(-a[0]), _T(-a[1]), _T(-a[2]), _T(-a[3]), _T(-a[4]), _T(-a[5]), _T(-a[6]), _T(-a[7]), _T(-a[8]), _T(-a[9]), _T(-a[10]), _T(-a[11]), _T(-a[12]), _T(-a[13]), _T(-a[14]) }); }
template <> inline A16 negative      (const A16 &a)               { return A16({ _T(-a[0]), _T(-a[1]), _T(-a[2]), _T(-a[3]), _T(-a[4]), _T(-a[5]), _T(-a[6]), _T(-a[7]), _T(-a[8]), _T(-a[9]), _T(-a[10]), _T(-a[11]), _T(-a[12]), _T(-a[13]), _T(-a[14]), _T(-a[15]) }); }

template <> inline A1  square        (const A1  &a)               { return A1 ({ _T(a[0] * a[0]) }); }
template <> inline A2  square        (const A2  &a)               { return A2 ({ _T(a[0] * a[0]), _T(a[1] * a[1]) }); }
template <> inline A3  square        (const A3  &a)               { return A3 ({ _T(a[0] * a[0]), _T(a[1] * a[1]), _T(a[2] * a[2]) }); }
template <> inline A4  square        (const A4  &a)               { return A4 ({ _T(a[0] * a[0]), _T(a[1] * a[1]), _T(a[2] * a[2]), _T(a[3] * a[3]) }); }
template <> inline A5  square        (const A5  &a)               { return A5 ({ _T(a[0] * a[0]), _T(a[1] * a[1]), _T(a[2] * a[2]), _T(a[3] * a[3]), _T(a[4] * a[4]) }); }
template <> inline A6  square        (const A6  &a)               { return A6 ({ _T(a[0] * a[0]), _T(a[1] * a[1]), _T(a[2] * a[2]), _T(a[3] * a[3]), _T(a[4] * a[4]), _T(a[5] * a[5]) }); }
template <> inline A7  square        (const A7  &a)               { return A7 ({ _T(a[0] * a[0]), _T(a[1] * a[1]), _T(a[2] * a[2]), _T(a[3] * a[3]), _T(a[4] * a[4]), _T(a[5] * a[5]), _T(a[6] * a[6]) }); }
template <> inline A8  square        (const A8  &a)               { return A8 ({ _T(a[0] * a[0]), _T(a[1] * a[1]), _T(a[2] * a[2]), _T(a[3] * a[3]), _T(a[4] * a[4]), _T(a[5] * a[5]), _T(a[6] * a[6]), _T(a[7] * a[7]) }); }
template <> inline A9  square        (const A9  &a)               { return A9 ({ _T(a[0] * a[0]), _T(a[1] * a[1]), _T(a[2] * a[2]), _T(a[3] * a[3]), _T(a[4] * a[4]), _T(a[5] * a[5]), _T(a[6] * a[6]), _T(a[7] * a[7]), _T(a[8] * a[8]) }); }
template <> inline A10 square        (const A10 &a)               { return A10({ _T(a[0] * a[0]), _T(a[1] * a[1]), _T(a[2] * a[2]), _T(a[3] * a[3]), _T(a[4] * a[4]), _T(a[5] * a[5]), _T(a[6] * a[6]), _T(a[7] * a[7]), _T(a[8] * a[8]), _T(a[9] * a[9]) }); }
template <> inline A11 square        (const A11 &a)               { return A11({ _T(a[0] * a[0]), _T(a[1] * a[1]), _T(a[2] * a[2]), _T(a[3] * a[3]), _T(a[4] * a[4]), _T(a[5] * a[5]), _T(a[6] * a[6]), _T(a[7] * a[7]), _T(a[8] * a[8]), _T(a[9] * a[9]), _T(a[10] * a[10]) }); }
template <> inline A12 square        (const A12 &a)               { return A12({ _T(a[0] * a[0]), _T(a[1] * a[1]), _T(a[2] * a[2]), _T(a[3] * a[3]), _T(a[4] * a[4]), _T(a[5] * a[5]), _T(a[6] * a[6]), _T(a[7] * a[7]), _T(a[8] * a[8]), _T(a[9] * a[9]), _T(a[10] * a[10]), _T(a[11] * a[11]) }); }
template <> inline A13 square        (const A13 &a)               { return A13({ _T(a[0] * a[0]), _T(a[1] * a[1]), _T(a[2] * a[2]), _T(a[3] * a[3]), _T(a[4] * a[4]), _T(a[5] * a[5]), _T(a[6] * a[6]), _T(a[7] * a[7]), _T(a[8] * a[8]), _T(a[9] * a[9]), _T(a[10] * a[10]), _T(a[11] * a[11]), _T(a[12] * a[12]) }); }
template <> inline A14 square        (const A14 &a)               { return A14({ _T(a[0] * a[0]), _T(a[1] * a[1]), _T(a[2] * a[2]), _T(a[3] * a[3]), _T(a[4] * a[4]), _T(a[5] * a[5]), _T(a[6] * a[6]), _T(a[7] * a[7]), _T(a[8] * a[8]), _T(a[9] * a[9]), _T(a[10] * a[10]), _T(a[11] * a[11]), _T(a[12] * a[12]), _T(a[13] * a[13]) }); }
template <> inline A15 square        (const A15 &a)               { return A15({ _T(a[0] * a[0]), _T(a[1] * a[1]), _T(a[2] * a[2]), _T(a[3] * a[3]), _T(a[4] * a[4]), _T(a[5] * a[5]), _T(a[6] * a[6]), _T(a[7] * a[7]), _T(a[8] * a[8]), _T(a[9] * a[9]), _T(a[10] * a[10]), _T(a[11] * a[11]), _T(a[12] * a[12]), _T(a[13] * a[13]), _T(a[14] * a[14]) }); }
template <> inline A16 square        (const A16 &a)               { return A16({ _T(a[0] * a[0]), _T(a[1] * a[1]), _T(a[2] * a[2]), _T(a[3] * a[3]), _T(a[4] * a[4]), _T(a[5] * a[5]), _T(a[6] * a[6]), _T(a[7] * a[7]), _T(a[8] * a[8]), _T(a[9] * a[9]), _T(a[10] * a[10]), _T(a[11] * a[11]), _T(a[12] * a[12]), _T(a[13] * a[13]), _T(a[14] * a[14]), _T(a[15] * a[15]) }); }

template <> inline A1  cube          (const A1  &a)               { return A1 ({ _T(a[0] * a[0] * a[0]) }); }
template <> inline A2  cube          (const A2  &a)               { return A2 ({ _T(a[0] * a[0] * a[0]), _T(a[1] * a[1] * a[1]) }); }
template <> inline A3  cube          (const A3  &a)               { return A3 ({ _T(a[0] * a[0] * a[0]), _T(a[1] * a[1] * a[1]), _T(a[2] * a[2] * a[2]) }); }
template <> inline A4  cube          (const A4  &a)               { return A4 ({ _T(a[0] * a[0] * a[0]), _T(a[1] * a[1] * a[1]), _T(a[2] * a[2] * a[2]), _T(a[3] * a[3] * a[3]) }); }
template <> inline A5  cube          (const A5  &a)               { return A5 ({ _T(a[0] * a[0] * a[0]), _T(a[1] * a[1] * a[1]), _T(a[2] * a[2] * a[2]), _T(a[3] * a[3] * a[3]), _T(a[4] * a[4] * a[4]) }); }
template <> inline A6  cube          (const A6  &a)               { return A6 ({ _T(a[0] * a[0] * a[0]), _T(a[1] * a[1] * a[1]), _T(a[2] * a[2] * a[2]), _T(a[3] * a[3] * a[3]), _T(a[4] * a[4] * a[4]), _T(a[5] * a[5] * a[5]) }); }
template <> inline A7  cube          (const A7  &a)               { return A7 ({ _T(a[0] * a[0] * a[0]), _T(a[1] * a[1] * a[1]), _T(a[2] * a[2] * a[2]), _T(a[3] * a[3] * a[3]), _T(a[4] * a[4] * a[4]), _T(a[5] * a[5] * a[5]), _T(a[6] * a[6] * a[6]) }); }
template <> inline A8  cube          (const A8  &a)               { return A8 ({ _T(a[0] * a[0] * a[0]), _T(a[1] * a[1] * a[1]), _T(a[2] * a[2] * a[2]), _T(a[3] * a[3] * a[3]), _T(a[4] * a[4] * a[4]), _T(a[5] * a[5] * a[5]), _T(a[6] * a[6] * a[6]), _T(a[7] * a[7] * a[7]) }); }
template <> inline A9  cube          (const A9  &a)               { return A9 ({ _T(a[0] * a[0] * a[0]), _T(a[1] * a[1] * a[1]), _T(a[2] * a[2] * a[2]), _T(a[3] * a[3] * a[3]), _T(a[4] * a[4] * a[4]), _T(a[5] * a[5] * a[5]), _T(a[6] * a[6] * a[6]), _T(a[7] * a[7] * a[7]), _T(a[8] * a[8] * a[8]) }); }
template <> inline A10 cube          (const A10 &a)               { return A10({ _T(a[0] * a[0] * a[0]), _T(a[1] * a[1] * a[1]), _T(a[2] * a[2] * a[2]), _T(a[3] * a[3] * a[3]), _T(a[4] * a[4] * a[4]), _T(a[5] * a[5] * a[5]), _T(a[6] * a[6] * a[6]), _T(a[7] * a[7] * a[7]), _T(a[8] * a[8] * a[8]), _T(a[9] * a[9] * a[9]) }); }
template <> inline A11 cube          (const A11 &a)               { return A11({ _T(a[0] * a[0] * a[0]), _T(a[1] * a[1] * a[1]), _T(a[2] * a[2] * a[2]), _T(a[3] * a[3] * a[3]), _T(a[4] * a[4] * a[4]), _T(a[5] * a[5] * a[5]), _T(a[6] * a[6] * a[6]), _T(a[7] * a[7] * a[7]), _T(a[8] * a[8] * a[8]), _T(a[9] * a[9] * a[9]), _T(a[10] * a[10] * a[10]) }); }
template <> inline A12 cube          (const A12 &a)               { return A12({ _T(a[0] * a[0] * a[0]), _T(a[1] * a[1] * a[1]), _T(a[2] * a[2] * a[2]), _T(a[3] * a[3] * a[3]), _T(a[4] * a[4] * a[4]), _T(a[5] * a[5] * a[5]), _T(a[6] * a[6] * a[6]), _T(a[7] * a[7] * a[7]), _T(a[8] * a[8] * a[8]), _T(a[9] * a[9] * a[9]), _T(a[10] * a[10] * a[10]), _T(a[11] * a[11] * a[11]) }); }
template <> inline A13 cube          (const A13 &a)               { return A13({ _T(a[0] * a[0] * a[0]), _T(a[1] * a[1] * a[1]), _T(a[2] * a[2] * a[2]), _T(a[3] * a[3] * a[3]), _T(a[4] * a[4] * a[4]), _T(a[5] * a[5] * a[5]), _T(a[6] * a[6] * a[6]), _T(a[7] * a[7] * a[7]), _T(a[8] * a[8] * a[8]), _T(a[9] * a[9] * a[9]), _T(a[10] * a[10] * a[10]), _T(a[11] * a[11] * a[11]), _T(a[12] * a[12] * a[12]) }); }
template <> inline A14 cube          (const A14 &a)               { return A14({ _T(a[0] * a[0] * a[0]), _T(a[1] * a[1] * a[1]), _T(a[2] * a[2] * a[2]), _T(a[3] * a[3] * a[3]), _T(a[4] * a[4] * a[4]), _T(a[5] * a[5] * a[5]), _T(a[6] * a[6] * a[6]), _T(a[7] * a[7] * a[7]), _T(a[8] * a[8] * a[8]), _T(a[9] * a[9] * a[9]), _T(a[10] * a[10] * a[10]), _T(a[11] * a[11] * a[11]), _T(a[12] * a[12] * a[12]), _T(a[13] * a[13] * a[13]) }); }
template <> inline A15 cube          (const A15 &a)               { return A15({ _T(a[0] * a[0] * a[0]), _T(a[1] * a[1] * a[1]), _T(a[2] * a[2] * a[2]), _T(a[3] * a[3] * a[3]), _T(a[4] * a[4] * a[4]), _T(a[5] * a[5] * a[5]), _T(a[6] * a[6] * a[6]), _T(a[7] * a[7] * a[7]), _T(a[8] * a[8] * a[8]), _T(a[9] * a[9] * a[9]), _T(a[10] * a[10] * a[10]), _T(a[11] * a[11] * a[11]), _T(a[12] * a[12] * a[12]), _T(a[13] * a[13] * a[13]), _T(a[14] * a[14] * a[14]) }); }
template <> inline A16 cube          (const A16 &a)               { return A16({ _T(a[0] * a[0] * a[0]), _T(a[1] * a[1] * a[1]), _T(a[2] * a[2] * a[2]), _T(a[3] * a[3] * a[3]), _T(a[4] * a[4] * a[4]), _T(a[5] * a[5] * a[5]), _T(a[6] * a[6] * a[6]), _T(a[7] * a[7] * a[7]), _T(a[8] * a[8] * a[8]), _T(a[9] * a[9] * a[9]), _T(a[10] * a[10] * a[10]), _T(a[11] * a[11] * a[11]), _T(a[12] * a[12] * a[12]), _T(a[13] * a[13] * a[13]), _T(a[14] * a[14] * a[14]), _T(a[15] * a[15] * a[15]) }); }

#if _T_INT == 1
template <> inline A1  complement    (const A1  &a)               { return A1 ({ _T(~a[0]) }); }
template <> inline A2  complement    (const A2  &a)               { return A2 ({ _T(~a[0]), _T(~a[1]) }); }
template <> inline A3  complement    (const A3  &a)               { return A3 ({ _T(~a[0]), _T(~a[1]), _T(~a[2]) }); }
template <> inline A4  complement    (const A4  &a)               { return A4 ({ _T(~a[0]), _T(~a[1]), _T(~a[2]), _T(~a[3]) }); }
template <> inline A5  complement    (const A5  &a)               { return A5 ({ _T(~a[0]), _T(~a[1]), _T(~a[2]), _T(~a[3]), _T(~a[4]) }); }
template <> inline A6  complement    (const A6  &a)               { return A6 ({ _T(~a[0]), _T(~a[1]), _T(~a[2]), _T(~a[3]), _T(~a[4]), _T(~a[5]) }); }
template <> inline A7  complement    (const A7  &a)               { return A7 ({ _T(~a[0]), _T(~a[1]), _T(~a[2]), _T(~a[3]), _T(~a[4]), _T(~a[5]), _T(~a[6]) }); }
template <> inline A8  complement    (const A8  &a)               { return A8 ({ _T(~a[0]), _T(~a[1]), _T(~a[2]), _T(~a[3]), _T(~a[4]), _T(~a[5]), _T(~a[6]), _T(~a[7]) }); }
template <> inline A9  complement    (const A9  &a)               { return A9 ({ _T(~a[0]), _T(~a[1]), _T(~a[2]), _T(~a[3]), _T(~a[4]), _T(~a[5]), _T(~a[6]), _T(~a[7]), _T(~a[8]) }); }
template <> inline A10 complement    (const A10 &a)               { return A10({ _T(~a[0]), _T(~a[1]), _T(~a[2]), _T(~a[3]), _T(~a[4]), _T(~a[5]), _T(~a[6]), _T(~a[7]), _T(~a[8]), _T(~a[9]) }); }
template <> inline A11 complement    (const A11 &a)               { return A11({ _T(~a[0]), _T(~a[1]), _T(~a[2]), _T(~a[3]), _T(~a[4]), _T(~a[5]), _T(~a[6]), _T(~a[7]), _T(~a[8]), _T(~a[9]), _T(~a[10]) }); }
template <> inline A12 complement    (const A12 &a)               { return A12({ _T(~a[0]), _T(~a[1]), _T(~a[2]), _T(~a[3]), _T(~a[4]), _T(~a[5]), _T(~a[6]), _T(~a[7]), _T(~a[8]), _T(~a[9]), _T(~a[10]), _T(~a[11]) }); }
template <> inline A13 complement    (const A13 &a)               { return A13({ _T(~a[0]), _T(~a[1]), _T(~a[2]), _T(~a[3]), _T(~a[4]), _T(~a[5]), _T(~a[6]), _T(~a[7]), _T(~a[8]), _T(~a[9]), _T(~a[10]), _T(~a[11]), _T(~a[12]) }); }
template <> inline A14 complement    (const A14 &a)               { return A14({ _T(~a[0]), _T(~a[1]), _T(~a[2]), _T(~a[3]), _T(~a[4]), _T(~a[5]), _T(~a[6]), _T(~a[7]), _T(~a[8]), _T(~a[9]), _T(~a[10]), _T(~a[11]), _T(~a[12]), _T(~a[13]) }); }
template <> inline A15 complement    (const A15 &a)               { return A15({ _T(~a[0]), _T(~a[1]), _T(~a[2]), _T(~a[3]), _T(~a[4]), _T(~a[5]), _T(~a[6]), _T(~a[7]), _T(~a[8]), _T(~a[9]), _T(~a[10]), _T(~a[11]), _T(~a[12]), _T(~a[13]), _T(~a[14]) }); }
template <> inline A16 complement    (const A16 &a)               { return A16({ _T(~a[0]), _T(~a[1]), _T(~a[2]), _T(~a[3]), _T(~a[4]), _T(~a[5]), _T(~a[6]), _T(~a[7]), _T(~a[8]), _T(~a[9]), _T(~a[10]), _T(~a[11]), _T(~a[12]), _T(~a[13]), _T(~a[14]), _T(~a[15]) }); }
#endif

template <> inline A1  pow           (const A1  &a, const A1  &b) { return A1 ({ _T(std::pow(a[0], b[0])) }); }
template <> inline A2  pow           (const A2  &a, const A2  &b) { return A2 ({ _T(std::pow(a[0], b[0])), _T(std::pow(a[1], b[1])) }); }
template <> inline A3  pow           (const A3  &a, const A3  &b) { return A3 ({ _T(std::pow(a[0], b[0])), _T(std::pow(a[1], b[1])), _T(std::pow(a[2], b[2])) }); }
template <> inline A4  pow           (const A4  &a, const A4  &b) { return A4 ({ _T(std::pow(a[0], b[0])), _T(std::pow(a[1], b[1])), _T(std::pow(a[2], b[2])), _T(std::pow(a[3], b[3])) }); }
template <> inline A5  pow           (const A5  &a, const A5  &b) { return A5 ({ _T(std::pow(a[0], b[0])), _T(std::pow(a[1], b[1])), _T(std::pow(a[2], b[2])), _T(std::pow(a[3], b[3])), _T(std::pow(a[4], b[4])) }); }
template <> inline A6  pow           (const A6  &a, const A6  &b) { return A6 ({ _T(std::pow(a[0], b[0])), _T(std::pow(a[1], b[1])), _T(std::pow(a[2], b[2])), _T(std::pow(a[3], b[3])), _T(std::pow(a[4], b[4])), _T(std::pow(a[5], b[5])) }); }
template <> inline A7  pow           (const A7  &a, const A7  &b) { return A7 ({ _T(std::pow(a[0], b[0])), _T(std::pow(a[1], b[1])), _T(std::pow(a[2], b[2])), _T(std::pow(a[3], b[3])), _T(std::pow(a[4], b[4])), _T(std::pow(a[5], b[5])), _T(std::pow(a[6], b[6])) }); }
template <> inline A8  pow           (const A8  &a, const A8  &b) { return A8 ({ _T(std::pow(a[0], b[0])), _T(std::pow(a[1], b[1])), _T(std::pow(a[2], b[2])), _T(std::pow(a[3], b[3])), _T(std::pow(a[4], b[4])), _T(std::pow(a[5], b[5])), _T(std::pow(a[6], b[6])), _T(std::pow(a[7], b[7])) }); }
template <> inline A9  pow           (const A9  &a, const A9  &b) { return A9 ({ _T(std::pow(a[0], b[0])), _T(std::pow(a[1], b[1])), _T(std::pow(a[2], b[2])), _T(std::pow(a[3], b[3])), _T(std::pow(a[4], b[4])), _T(std::pow(a[5], b[5])), _T(std::pow(a[6], b[6])), _T(std::pow(a[7], b[7])), _T(std::pow(a[8], b[8])) }); }
template <> inline A10 pow           (const A10 &a, const A10 &b) { return A10({ _T(std::pow(a[0], b[0])), _T(std::pow(a[1], b[1])), _T(std::pow(a[2], b[2])), _T(std::pow(a[3], b[3])), _T(std::pow(a[4], b[4])), _T(std::pow(a[5], b[5])), _T(std::pow(a[6], b[6])), _T(std::pow(a[7], b[7])), _T(std::pow(a[8], b[8])), _T(std::pow(a[9], b[9])) }); }
template <> inline A11 pow           (const A11 &a, const A11 &b) { return A11({ _T(std::pow(a[0], b[0])), _T(std::pow(a[1], b[1])), _T(std::pow(a[2], b[2])), _T(std::pow(a[3], b[3])), _T(std::pow(a[4], b[4])), _T(std::pow(a[5], b[5])), _T(std::pow(a[6], b[6])), _T(std::pow(a[7], b[7])), _T(std::pow(a[8], b[8])), _T(std::pow(a[9], b[9])), _T(std::pow(a[10], b[10])) }); }
template <> inline A12 pow           (const A12 &a, const A12 &b) { return A12({ _T(std::pow(a[0], b[0])), _T(std::pow(a[1], b[1])), _T(std::pow(a[2], b[2])), _T(std::pow(a[3], b[3])), _T(std::pow(a[4], b[4])), _T(std::pow(a[5], b[5])), _T(std::pow(a[6], b[6])), _T(std::pow(a[7], b[7])), _T(std::pow(a[8], b[8])), _T(std::pow(a[9], b[9])), _T(std::pow(a[10], b[10])), _T(std::pow(a[11], b[11])) }); }
template <> inline A13 pow           (const A13 &a, const A13 &b) { return A13({ _T(std::pow(a[0], b[0])), _T(std::pow(a[1], b[1])), _T(std::pow(a[2], b[2])), _T(std::pow(a[3], b[3])), _T(std::pow(a[4], b[4])), _T(std::pow(a[5], b[5])), _T(std::pow(a[6], b[6])), _T(std::pow(a[7], b[7])), _T(std::pow(a[8], b[8])), _T(std::pow(a[9], b[9])), _T(std::pow(a[10], b[10])), _T(std::pow(a[11], b[11])), _T(std::pow(a[12], b[12])) }); }
template <> inline A14 pow           (const A14 &a, const A14 &b) { return A14({ _T(std::pow(a[0], b[0])), _T(std::pow(a[1], b[1])), _T(std::pow(a[2], b[2])), _T(std::pow(a[3], b[3])), _T(std::pow(a[4], b[4])), _T(std::pow(a[5], b[5])), _T(std::pow(a[6], b[6])), _T(std::pow(a[7], b[7])), _T(std::pow(a[8], b[8])), _T(std::pow(a[9], b[9])), _T(std::pow(a[10], b[10])), _T(std::pow(a[11], b[11])), _T(std::pow(a[12], b[12])), _T(std::pow(a[13], b[13])) }); }
template <> inline A15 pow           (const A15 &a, const A15 &b) { return A15({ _T(std::pow(a[0], b[0])), _T(std::pow(a[1], b[1])), _T(std::pow(a[2], b[2])), _T(std::pow(a[3], b[3])), _T(std::pow(a[4], b[4])), _T(std::pow(a[5], b[5])), _T(std::pow(a[6], b[6])), _T(std::pow(a[7], b[7])), _T(std::pow(a[8], b[8])), _T(std::pow(a[9], b[9])), _T(std::pow(a[10], b[10])), _T(std::pow(a[11], b[11])), _T(std::pow(a[12], b[12])), _T(std::pow(a[13], b[13])), _T(std::pow(a[14], b[14])) }); }
template <> inline A16 pow           (const A16 &a, const A16 &b) { return A16({ _T(std::pow(a[0], b[0])), _T(std::pow(a[1], b[1])), _T(std::pow(a[2], b[2])), _T(std::pow(a[3], b[3])), _T(std::pow(a[4], b[4])), _T(std::pow(a[5], b[5])), _T(std::pow(a[6], b[6])), _T(std::pow(a[7], b[7])), _T(std::pow(a[8], b[8])), _T(std::pow(a[9], b[9])), _T(std::pow(a[10], b[10])), _T(std::pow(a[11], b[11])), _T(std::pow(a[12], b[12])), _T(std::pow(a[13], b[13])), _T(std::pow(a[14], b[14])), _T(std::pow(a[15], b[15])) }); }
template <> inline A1  subtract      (const A1  &a, const A1  &b) { return A1 ({ _T(a[0] - b[0]) }); }
template <> inline A2  subtract      (const A2  &a, const A2  &b) { return A2 ({ _T(a[0] - b[0]), _T(a[1] - b[1]) }); }
template <> inline A3  subtract      (const A3  &a, const A3  &b) { return A3 ({ _T(a[0] - b[0]), _T(a[1] - b[1]), _T(a[2] - b[2]) }); }
template <> inline A4  subtract      (const A4  &a, const A4  &b) { return A4 ({ _T(a[0] - b[0]), _T(a[1] - b[1]), _T(a[2] - b[2]), _T(a[3] - b[3]) }); }
template <> inline A5  subtract      (const A5  &a, const A5  &b) { return A5 ({ _T(a[0] - b[0]), _T(a[1] - b[1]), _T(a[2] - b[2]), _T(a[3] - b[3]), _T(a[4] - b[4]) }); }
template <> inline A6  subtract      (const A6  &a, const A6  &b) { return A6 ({ _T(a[0] - b[0]), _T(a[1] - b[1]), _T(a[2] - b[2]), _T(a[3] - b[3]), _T(a[4] - b[4]), _T(a[5] - b[5]) }); }
template <> inline A7  subtract      (const A7  &a, const A7  &b) { return A7 ({ _T(a[0] - b[0]), _T(a[1] - b[1]), _T(a[2] - b[2]), _T(a[3] - b[3]), _T(a[4] - b[4]), _T(a[5] - b[5]), _T(a[6] - b[6]) }); }
template <> inline A8  subtract      (const A8  &a, const A8  &b) { return A8 ({ _T(a[0] - b[0]), _T(a[1] - b[1]), _T(a[2] - b[2]), _T(a[3] - b[3]), _T(a[4] - b[4]), _T(a[5] - b[5]), _T(a[6] - b[6]), _T(a[7] - b[7]) }); }
template <> inline A9  subtract      (const A9  &a, const A9  &b) { return A9 ({ _T(a[0] - b[0]), _T(a[1] - b[1]), _T(a[2] - b[2]), _T(a[3] - b[3]), _T(a[4] - b[4]), _T(a[5] - b[5]), _T(a[6] - b[6]), _T(a[7] - b[7]), _T(a[8] - b[8]) }); }
template <> inline A10 subtract      (const A10 &a, const A10 &b) { return A10({ _T(a[0] - b[0]), _T(a[1] - b[1]), _T(a[2] - b[2]), _T(a[3] - b[3]), _T(a[4] - b[4]), _T(a[5] - b[5]), _T(a[6] - b[6]), _T(a[7] - b[7]), _T(a[8] - b[8]), _T(a[9] - b[9]) }); }
template <> inline A11 subtract      (const A11 &a, const A11 &b) { return A11({ _T(a[0] - b[0]), _T(a[1] - b[1]), _T(a[2] - b[2]), _T(a[3] - b[3]), _T(a[4] - b[4]), _T(a[5] - b[5]), _T(a[6] - b[6]), _T(a[7] - b[7]), _T(a[8] - b[8]), _T(a[9] - b[9]), _T(a[10] - b[10]) }); }
template <> inline A12 subtract      (const A12 &a, const A12 &b) { return A12({ _T(a[0] - b[0]), _T(a[1] - b[1]), _T(a[2] - b[2]), _T(a[3] - b[3]), _T(a[4] - b[4]), _T(a[5] - b[5]), _T(a[6] - b[6]), _T(a[7] - b[7]), _T(a[8] - b[8]), _T(a[9] - b[9]), _T(a[10] - b[10]), _T(a[11] - b[11]) }); }
template <> inline A13 subtract      (const A13 &a, const A13 &b) { return A13({ _T(a[0] - b[0]), _T(a[1] - b[1]), _T(a[2] - b[2]), _T(a[3] - b[3]), _T(a[4] - b[4]), _T(a[5] - b[5]), _T(a[6] - b[6]), _T(a[7] - b[7]), _T(a[8] - b[8]), _T(a[9] - b[9]), _T(a[10] - b[10]), _T(a[11] - b[11]), _T(a[12] - b[12]) }); }
template <> inline A14 subtract      (const A14 &a, const A14 &b) { return A14({ _T(a[0] - b[0]), _T(a[1] - b[1]), _T(a[2] - b[2]), _T(a[3] - b[3]), _T(a[4] - b[4]), _T(a[5] - b[5]), _T(a[6] - b[6]), _T(a[7] - b[7]), _T(a[8] - b[8]), _T(a[9] - b[9]), _T(a[10] - b[10]), _T(a[11] - b[11]), _T(a[12] - b[12]), _T(a[13] - b[13]) }); }
template <> inline A15 subtract      (const A15 &a, const A15 &b) { return A15({ _T(a[0] - b[0]), _T(a[1] - b[1]), _T(a[2] - b[2]), _T(a[3] - b[3]), _T(a[4] - b[4]), _T(a[5] - b[5]), _T(a[6] - b[6]), _T(a[7] - b[7]), _T(a[8] - b[8]), _T(a[9] - b[9]), _T(a[10] - b[10]), _T(a[11] - b[11]), _T(a[12] - b[12]), _T(a[13] - b[13]), _T(a[14] - b[14]) }); }
template <> inline A16 subtract      (const A16 &a, const A16 &b) { return A16({ _T(a[0] - b[0]), _T(a[1] - b[1]), _T(a[2] - b[2]), _T(a[3] - b[3]), _T(a[4] - b[4]), _T(a[5] - b[5]), _T(a[6] - b[6]), _T(a[7] - b[7]), _T(a[8] - b[8]), _T(a[9] - b[9]), _T(a[10] - b[10]), _T(a[11] - b[11]), _T(a[12] - b[12]), _T(a[13] - b[13]), _T(a[14] - b[14]), _T(a[15] - b[15]) }); }

template <> inline A1  add           (const A1  &a, const A1  &b) { return A1 ({ _T(a[0] + b[0]) }); }
template <> inline A2  add           (const A2  &a, const A2  &b) { return A2 ({ _T(a[0] + b[0]), _T(a[1] + b[1]) }); }
template <> inline A3  add           (const A3  &a, const A3  &b) { return A3 ({ _T(a[0] + b[0]), _T(a[1] + b[1]), _T(a[2] + b[2]) }); }
template <> inline A4  add           (const A4  &a, const A4  &b) { return A4 ({ _T(a[0] + b[0]), _T(a[1] + b[1]), _T(a[2] + b[2]), _T(a[3] + b[3]) }); }
template <> inline A5  add           (const A5  &a, const A5  &b) { return A5 ({ _T(a[0] + b[0]), _T(a[1] + b[1]), _T(a[2] + b[2]), _T(a[3] + b[3]), _T(a[4] + b[4]) }); }
template <> inline A6  add           (const A6  &a, const A6  &b) { return A6 ({ _T(a[0] + b[0]), _T(a[1] + b[1]), _T(a[2] + b[2]), _T(a[3] + b[3]), _T(a[4] + b[4]), _T(a[5] + b[5]) }); }
template <> inline A7  add           (const A7  &a, const A7  &b) { return A7 ({ _T(a[0] + b[0]), _T(a[1] + b[1]), _T(a[2] + b[2]), _T(a[3] + b[3]), _T(a[4] + b[4]), _T(a[5] + b[5]), _T(a[6] + b[6]) }); }
template <> inline A8  add           (const A8  &a, const A8  &b) { return A8 ({ _T(a[0] + b[0]), _T(a[1] + b[1]), _T(a[2] + b[2]), _T(a[3] + b[3]), _T(a[4] + b[4]), _T(a[5] + b[5]), _T(a[6] + b[6]), _T(a[7] + b[7]) }); }
template <> inline A9  add           (const A9  &a, const A9  &b) { return A9 ({ _T(a[0] + b[0]), _T(a[1] + b[1]), _T(a[2] + b[2]), _T(a[3] + b[3]), _T(a[4] + b[4]), _T(a[5] + b[5]), _T(a[6] + b[6]), _T(a[7] + b[7]), _T(a[8] + b[8]) }); }
template <> inline A10 add           (const A10 &a, const A10 &b) { return A10({ _T(a[0] + b[0]), _T(a[1] + b[1]), _T(a[2] + b[2]), _T(a[3] + b[3]), _T(a[4] + b[4]), _T(a[5] + b[5]), _T(a[6] + b[6]), _T(a[7] + b[7]), _T(a[8] + b[8]), _T(a[9] + b[9]) }); }
template <> inline A11 add           (const A11 &a, const A11 &b) { return A11({ _T(a[0] + b[0]), _T(a[1] + b[1]), _T(a[2] + b[2]), _T(a[3] + b[3]), _T(a[4] + b[4]), _T(a[5] + b[5]), _T(a[6] + b[6]), _T(a[7] + b[7]), _T(a[8] + b[8]), _T(a[9] + b[9]), _T(a[10] + b[10]) }); }
template <> inline A12 add           (const A12 &a, const A12 &b) { return A12({ _T(a[0] + b[0]), _T(a[1] + b[1]), _T(a[2] + b[2]), _T(a[3] + b[3]), _T(a[4] + b[4]), _T(a[5] + b[5]), _T(a[6] + b[6]), _T(a[7] + b[7]), _T(a[8] + b[8]), _T(a[9] + b[9]), _T(a[10] + b[10]), _T(a[11] + b[11]) }); }
template <> inline A13 add           (const A13 &a, const A13 &b) { return A13({ _T(a[0] + b[0]), _T(a[1] + b[1]), _T(a[2] + b[2]), _T(a[3] + b[3]), _T(a[4] + b[4]), _T(a[5] + b[5]), _T(a[6] + b[6]), _T(a[7] + b[7]), _T(a[8] + b[8]), _T(a[9] + b[9]), _T(a[10] + b[10]), _T(a[11] + b[11]), _T(a[12] + b[12]) }); }
template <> inline A14 add           (const A14 &a, const A14 &b) { return A14({ _T(a[0] + b[0]), _T(a[1] + b[1]), _T(a[2] + b[2]), _T(a[3] + b[3]), _T(a[4] + b[4]), _T(a[5] + b[5]), _T(a[6] + b[6]), _T(a[7] + b[7]), _T(a[8] + b[8]), _T(a[9] + b[9]), _T(a[10] + b[10]), _T(a[11] + b[11]), _T(a[12] + b[12]), _T(a[13] + b[13]) }); }
template <> inline A15 add           (const A15 &a, const A15 &b) { return A15({ _T(a[0] + b[0]), _T(a[1] + b[1]), _T(a[2] + b[2]), _T(a[3] + b[3]), _T(a[4] + b[4]), _T(a[5] + b[5]), _T(a[6] + b[6]), _T(a[7] + b[7]), _T(a[8] + b[8]), _T(a[9] + b[9]), _T(a[10] + b[10]), _T(a[11] + b[11]), _T(a[12] + b[12]), _T(a[13] + b[13]), _T(a[14] + b[14]) }); }
template <> inline A16 add           (const A16 &a, const A16 &b) { return A16({ _T(a[0] + b[0]), _T(a[1] + b[1]), _T(a[2] + b[2]), _T(a[3] + b[3]), _T(a[4] + b[4]), _T(a[5] + b[5]), _T(a[6] + b[6]), _T(a[7] + b[7]), _T(a[8] + b[8]), _T(a[9] + b[9]), _T(a[10] + b[10]), _T(a[11] + b[11]), _T(a[12] + b[12]), _T(a[13] + b[13]), _T(a[14] + b[14]), _T(a[15] + b[15]) }); }

template <> inline A1  multiply      (const A1  &a, const A1  &b) { return A1 ({ _T(a[0] * b[0]) }); }
template <> inline A2  multiply      (const A2  &a, const A2  &b) { return A2 ({ _T(a[0] * b[0]), _T(a[1] * b[1]) }); }
template <> inline A3  multiply      (const A3  &a, const A3  &b) { return A3 ({ _T(a[0] * b[0]), _T(a[1] * b[1]), _T(a[2] * b[2]) }); }
template <> inline A4  multiply      (const A4  &a, const A4  &b) { return A4 ({ _T(a[0] * b[0]), _T(a[1] * b[1]), _T(a[2] * b[2]), _T(a[3] * b[3]) }); }
template <> inline A5  multiply      (const A5  &a, const A5  &b) { return A5 ({ _T(a[0] * b[0]), _T(a[1] * b[1]), _T(a[2] * b[2]), _T(a[3] * b[3]), _T(a[4] * b[4]) }); }
template <> inline A6  multiply      (const A6  &a, const A6  &b) { return A6 ({ _T(a[0] * b[0]), _T(a[1] * b[1]), _T(a[2] * b[2]), _T(a[3] * b[3]), _T(a[4] * b[4]), _T(a[5] * b[5]) }); }
template <> inline A7  multiply      (const A7  &a, const A7  &b) { return A7 ({ _T(a[0] * b[0]), _T(a[1] * b[1]), _T(a[2] * b[2]), _T(a[3] * b[3]), _T(a[4] * b[4]), _T(a[5] * b[5]), _T(a[6] * b[6]) }); }
template <> inline A8  multiply      (const A8  &a, const A8  &b) { return A8 ({ _T(a[0] * b[0]), _T(a[1] * b[1]), _T(a[2] * b[2]), _T(a[3] * b[3]), _T(a[4] * b[4]), _T(a[5] * b[5]), _T(a[6] * b[6]), _T(a[7] * b[7]) }); }
template <> inline A9  multiply      (const A9  &a, const A9  &b) { return A9 ({ _T(a[0] * b[0]), _T(a[1] * b[1]), _T(a[2] * b[2]), _T(a[3] * b[3]), _T(a[4] * b[4]), _T(a[5] * b[5]), _T(a[6] * b[6]), _T(a[7] * b[7]), _T(a[8] * b[8]) }); }
template <> inline A10 multiply      (const A10 &a, const A10 &b) { return A10({ _T(a[0] * b[0]), _T(a[1] * b[1]), _T(a[2] * b[2]), _T(a[3] * b[3]), _T(a[4] * b[4]), _T(a[5] * b[5]), _T(a[6] * b[6]), _T(a[7] * b[7]), _T(a[8] * b[8]), _T(a[9] * b[9]) }); }
template <> inline A11 multiply      (const A11 &a, const A11 &b) { return A11({ _T(a[0] * b[0]), _T(a[1] * b[1]), _T(a[2] * b[2]), _T(a[3] * b[3]), _T(a[4] * b[4]), _T(a[5] * b[5]), _T(a[6] * b[6]), _T(a[7] * b[7]), _T(a[8] * b[8]), _T(a[9] * b[9]), _T(a[10] * b[10]) }); }
template <> inline A12 multiply      (const A12 &a, const A12 &b) { return A12({ _T(a[0] * b[0]), _T(a[1] * b[1]), _T(a[2] * b[2]), _T(a[3] * b[3]), _T(a[4] * b[4]), _T(a[5] * b[5]), _T(a[6] * b[6]), _T(a[7] * b[7]), _T(a[8] * b[8]), _T(a[9] * b[9]), _T(a[10] * b[10]), _T(a[11] * b[11]) }); }
template <> inline A13 multiply      (const A13 &a, const A13 &b) { return A13({ _T(a[0] * b[0]), _T(a[1] * b[1]), _T(a[2] * b[2]), _T(a[3] * b[3]), _T(a[4] * b[4]), _T(a[5] * b[5]), _T(a[6] * b[6]), _T(a[7] * b[7]), _T(a[8] * b[8]), _T(a[9] * b[9]), _T(a[10] * b[10]), _T(a[11] * b[11]), _T(a[12] * b[12]) }); }
template <> inline A14 multiply      (const A14 &a, const A14 &b) { return A14({ _T(a[0] * b[0]), _T(a[1] * b[1]), _T(a[2] * b[2]), _T(a[3] * b[3]), _T(a[4] * b[4]), _T(a[5] * b[5]), _T(a[6] * b[6]), _T(a[7] * b[7]), _T(a[8] * b[8]), _T(a[9] * b[9]), _T(a[10] * b[10]), _T(a[11] * b[11]), _T(a[12] * b[12]), _T(a[13] * b[13]) }); }
template <> inline A15 multiply      (const A15 &a, const A15 &b) { return A15({ _T(a[0] * b[0]), _T(a[1] * b[1]), _T(a[2] * b[2]), _T(a[3] * b[3]), _T(a[4] * b[4]), _T(a[5] * b[5]), _T(a[6] * b[6]), _T(a[7] * b[7]), _T(a[8] * b[8]), _T(a[9] * b[9]), _T(a[10] * b[10]), _T(a[11] * b[11]), _T(a[12] * b[12]), _T(a[13] * b[13]), _T(a[14] * b[14]) }); }
template <> inline A16 multiply      (const A16 &a, const A16 &b) { return A16({ _T(a[0] * b[0]), _T(a[1] * b[1]), _T(a[2] * b[2]), _T(a[3] * b[3]), _T(a[4] * b[4]), _T(a[5] * b[5]), _T(a[6] * b[6]), _T(a[7] * b[7]), _T(a[8] * b[8]), _T(a[9] * b[9]), _T(a[10] * b[10]), _T(a[11] * b[11]), _T(a[12] * b[12]), _T(a[13] * b[13]), _T(a[14] * b[14]), _T(a[15] * b[15]) }); }

template <> inline A1  divide        (const A1  &a, const A1  &b) { return A1 ({ _T(a[0] / b[0]) }); }
template <> inline A2  divide        (const A2  &a, const A2  &b) { return A2 ({ _T(a[0] / b[0]), _T(a[1] / b[1]) }); }
template <> inline A3  divide        (const A3  &a, const A3  &b) { return A3 ({ _T(a[0] / b[0]), _T(a[1] / b[1]), _T(a[2] / b[2]) }); }
template <> inline A4  divide        (const A4  &a, const A4  &b) { return A4 ({ _T(a[0] / b[0]), _T(a[1] / b[1]), _T(a[2] / b[2]), _T(a[3] / b[3]) }); }
template <> inline A5  divide        (const A5  &a, const A5  &b) { return A5 ({ _T(a[0] / b[0]), _T(a[1] / b[1]), _T(a[2] / b[2]), _T(a[3] / b[3]), _T(a[4] / b[4]) }); }
template <> inline A6  divide        (const A6  &a, const A6  &b) { return A6 ({ _T(a[0] / b[0]), _T(a[1] / b[1]), _T(a[2] / b[2]), _T(a[3] / b[3]), _T(a[4] / b[4]), _T(a[5] / b[5]) }); }
template <> inline A7  divide        (const A7  &a, const A7  &b) { return A7 ({ _T(a[0] / b[0]), _T(a[1] / b[1]), _T(a[2] / b[2]), _T(a[3] / b[3]), _T(a[4] / b[4]), _T(a[5] / b[5]), _T(a[6] / b[6]) }); }
template <> inline A8  divide        (const A8  &a, const A8  &b) { return A8 ({ _T(a[0] / b[0]), _T(a[1] / b[1]), _T(a[2] / b[2]), _T(a[3] / b[3]), _T(a[4] / b[4]), _T(a[5] / b[5]), _T(a[6] / b[6]), _T(a[7] / b[7]) }); }
template <> inline A9  divide        (const A9  &a, const A9  &b) { return A9 ({ _T(a[0] / b[0]), _T(a[1] / b[1]), _T(a[2] / b[2]), _T(a[3] / b[3]), _T(a[4] / b[4]), _T(a[5] / b[5]), _T(a[6] / b[6]), _T(a[7] / b[7]), _T(a[8] / b[8]) }); }
template <> inline A10 divide        (const A10 &a, const A10 &b) { return A10({ _T(a[0] / b[0]), _T(a[1] / b[1]), _T(a[2] / b[2]), _T(a[3] / b[3]), _T(a[4] / b[4]), _T(a[5] / b[5]), _T(a[6] / b[6]), _T(a[7] / b[7]), _T(a[8] / b[8]), _T(a[9] / b[9]) }); }
template <> inline A11 divide        (const A11 &a, const A11 &b) { return A11({ _T(a[0] / b[0]), _T(a[1] / b[1]), _T(a[2] / b[2]), _T(a[3] / b[3]), _T(a[4] / b[4]), _T(a[5] / b[5]), _T(a[6] / b[6]), _T(a[7] / b[7]), _T(a[8] / b[8]), _T(a[9] / b[9]), _T(a[10] / b[10]) }); }
template <> inline A12 divide        (const A12 &a, const A12 &b) { return A12({ _T(a[0] / b[0]), _T(a[1] / b[1]), _T(a[2] / b[2]), _T(a[3] / b[3]), _T(a[4] / b[4]), _T(a[5] / b[5]), _T(a[6] / b[6]), _T(a[7] / b[7]), _T(a[8] / b[8]), _T(a[9] / b[9]), _T(a[10] / b[10]), _T(a[11] / b[11]) }); }
template <> inline A13 divide        (const A13 &a, const A13 &b) { return A13({ _T(a[0] / b[0]), _T(a[1] / b[1]), _T(a[2] / b[2]), _T(a[3] / b[3]), _T(a[4] / b[4]), _T(a[5] / b[5]), _T(a[6] / b[6]), _T(a[7] / b[7]), _T(a[8] / b[8]), _T(a[9] / b[9]), _T(a[10] / b[10]), _T(a[11] / b[11]), _T(a[12] / b[12]) }); }
template <> inline A14 divide        (const A14 &a, const A14 &b) { return A14({ _T(a[0] / b[0]), _T(a[1] / b[1]), _T(a[2] / b[2]), _T(a[3] / b[3]), _T(a[4] / b[4]), _T(a[5] / b[5]), _T(a[6] / b[6]), _T(a[7] / b[7]), _T(a[8] / b[8]), _T(a[9] / b[9]), _T(a[10] / b[10]), _T(a[11] / b[11]), _T(a[12] / b[12]), _T(a[13] / b[13]) }); }
template <> inline A15 divide        (const A15 &a, const A15 &b) { return A15({ _T(a[0] / b[0]), _T(a[1] / b[1]), _T(a[2] / b[2]), _T(a[3] / b[3]), _T(a[4] / b[4]), _T(a[5] / b[5]), _T(a[6] / b[6]), _T(a[7] / b[7]), _T(a[8] / b[8]), _T(a[9] / b[9]), _T(a[10] / b[10]), _T(a[11] / b[11]), _T(a[12] / b[12]), _T(a[13] / b[13]), _T(a[14] / b[14]) }); }
template <> inline A16 divide        (const A16 &a, const A16 &b) { return A16({ _T(a[0] / b[0]), _T(a[1] / b[1]), _T(a[2] / b[2]), _T(a[3] / b[3]), _T(a[4] / b[4]), _T(a[5] / b[5]), _T(a[6] / b[6]), _T(a[7] / b[7]), _T(a[8] / b[8]), _T(a[9] / b[9]), _T(a[10] / b[10]), _T(a[11] / b[11]), _T(a[12] / b[12]), _T(a[13] / b[13]), _T(a[14] / b[14]), _T(a[15] / b[15]) }); }

#if _T_INT == 1
template <> inline A1  modulus       (const A1  &a, const A1  &b) { return A1 ({ _T(a[0] % b[0]) }); }
template <> inline A2  modulus       (const A2  &a, const A2  &b) { return A2 ({ _T(a[0] % b[0]), _T(a[1] % b[1]) }); }
template <> inline A3  modulus       (const A3  &a, const A3  &b) { return A3 ({ _T(a[0] % b[0]), _T(a[1] % b[1]), _T(a[2] % b[2]) }); }
template <> inline A4  modulus       (const A4  &a, const A4  &b) { return A4 ({ _T(a[0] % b[0]), _T(a[1] % b[1]), _T(a[2] % b[2]), _T(a[3] % b[3]) }); }
template <> inline A5  modulus       (const A5  &a, const A5  &b) { return A5 ({ _T(a[0] % b[0]), _T(a[1] % b[1]), _T(a[2] % b[2]), _T(a[3] % b[3]), _T(a[4] % b[4]) }); }
template <> inline A6  modulus       (const A6  &a, const A6  &b) { return A6 ({ _T(a[0] % b[0]), _T(a[1] % b[1]), _T(a[2] % b[2]), _T(a[3] % b[3]), _T(a[4] % b[4]), _T(a[5] % b[5]) }); }
template <> inline A7  modulus       (const A7  &a, const A7  &b) { return A7 ({ _T(a[0] % b[0]), _T(a[1] % b[1]), _T(a[2] % b[2]), _T(a[3] % b[3]), _T(a[4] % b[4]), _T(a[5] % b[5]), _T(a[6] % b[6]) }); }
template <> inline A8  modulus       (const A8  &a, const A8  &b) { return A8 ({ _T(a[0] % b[0]), _T(a[1] % b[1]), _T(a[2] % b[2]), _T(a[3] % b[3]), _T(a[4] % b[4]), _T(a[5] % b[5]), _T(a[6] % b[6]), _T(a[7] % b[7]) }); }
template <> inline A9  modulus       (const A9  &a, const A9  &b) { return A9 ({ _T(a[0] % b[0]), _T(a[1] % b[1]), _T(a[2] % b[2]), _T(a[3] % b[3]), _T(a[4] % b[4]), _T(a[5] % b[5]), _T(a[6] % b[6]), _T(a[7] % b[7]), _T(a[8] % b[8]) }); }
template <> inline A10 modulus       (const A10 &a, const A10 &b) { return A10({ _T(a[0] % b[0]), _T(a[1] % b[1]), _T(a[2] % b[2]), _T(a[3] % b[3]), _T(a[4] % b[4]), _T(a[5] % b[5]), _T(a[6] % b[6]), _T(a[7] % b[7]), _T(a[8] % b[8]), _T(a[9] % b[9]) }); }
template <> inline A11 modulus       (const A11 &a, const A11 &b) { return A11({ _T(a[0] % b[0]), _T(a[1] % b[1]), _T(a[2] % b[2]), _T(a[3] % b[3]), _T(a[4] % b[4]), _T(a[5] % b[5]), _T(a[6] % b[6]), _T(a[7] % b[7]), _T(a[8] % b[8]), _T(a[9] % b[9]), _T(a[10] % b[10]) }); }
template <> inline A12 modulus       (const A12 &a, const A12 &b) { return A12({ _T(a[0] % b[0]), _T(a[1] % b[1]), _T(a[2] % b[2]), _T(a[3] % b[3]), _T(a[4] % b[4]), _T(a[5] % b[5]), _T(a[6] % b[6]), _T(a[7] % b[7]), _T(a[8] % b[8]), _T(a[9] % b[9]), _T(a[10] % b[10]), _T(a[11] % b[11]) }); }
template <> inline A13 modulus       (const A13 &a, const A13 &b) { return A13({ _T(a[0] % b[0]), _T(a[1] % b[1]), _T(a[2] % b[2]), _T(a[3] % b[3]), _T(a[4] % b[4]), _T(a[5] % b[5]), _T(a[6] % b[6]), _T(a[7] % b[7]), _T(a[8] % b[8]), _T(a[9] % b[9]), _T(a[10] % b[10]), _T(a[11] % b[11]), _T(a[12] % b[12]) }); }
template <> inline A14 modulus       (const A14 &a, const A14 &b) { return A14({ _T(a[0] % b[0]), _T(a[1] % b[1]), _T(a[2] % b[2]), _T(a[3] % b[3]), _T(a[4] % b[4]), _T(a[5] % b[5]), _T(a[6] % b[6]), _T(a[7] % b[7]), _T(a[8] % b[8]), _T(a[9] % b[9]), _T(a[10] % b[10]), _T(a[11] % b[11]), _T(a[12] % b[12]), _T(a[13] % b[13]) }); }
template <> inline A15 modulus       (const A15 &a, const A15 &b) { return A15({ _T(a[0] % b[0]), _T(a[1] % b[1]), _T(a[2] % b[2]), _T(a[3] % b[3]), _T(a[4] % b[4]), _T(a[5] % b[5]), _T(a[6] % b[6]), _T(a[7] % b[7]), _T(a[8] % b[8]), _T(a[9] % b[9]), _T(a[10] % b[10]), _T(a[11] % b[11]), _T(a[12] % b[12]), _T(a[13] % b[13]), _T(a[14] % b[14]) }); }
template <> inline A16 modulus       (const A16 &a, const A16 &b) { return A16({ _T(a[0] % b[0]), _T(a[1] % b[1]), _T(a[2] % b[2]), _T(a[3] % b[3]), _T(a[4] % b[4]), _T(a[5] % b[5]), _T(a[6] % b[6]), _T(a[7] % b[7]), _T(a[8] % b[8]), _T(a[9] % b[9]), _T(a[10] % b[10]), _T(a[11] % b[11]), _T(a[12] % b[12]), _T(a[13] % b[13]), _T(a[14] % b[14]), _T(a[15] % b[15]) }); }

template <> inline A1  bit_and       (const A1  &a, const A1  &b) { return A1 ({ _T(a[0] & b[0]) }); }
template <> inline A2  bit_and       (const A2  &a, const A2  &b) { return A2 ({ _T(a[0] & b[0]), _T(a[1] & b[1]) }); }
template <> inline A3  bit_and       (const A3  &a, const A3  &b) { return A3 ({ _T(a[0] & b[0]), _T(a[1] & b[1]), _T(a[2] & b[2]) }); }
template <> inline A4  bit_and       (const A4  &a, const A4  &b) { return A4 ({ _T(a[0] & b[0]), _T(a[1] & b[1]), _T(a[2] & b[2]), _T(a[3] & b[3]) }); }
template <> inline A5  bit_and       (const A5  &a, const A5  &b) { return A5 ({ _T(a[0] & b[0]), _T(a[1] & b[1]), _T(a[2] & b[2]), _T(a[3] & b[3]), _T(a[4] & b[4]) }); }
template <> inline A6  bit_and       (const A6  &a, const A6  &b) { return A6 ({ _T(a[0] & b[0]), _T(a[1] & b[1]), _T(a[2] & b[2]), _T(a[3] & b[3]), _T(a[4] & b[4]), _T(a[5] & b[5]) }); }
template <> inline A7  bit_and       (const A7  &a, const A7  &b) { return A7 ({ _T(a[0] & b[0]), _T(a[1] & b[1]), _T(a[2] & b[2]), _T(a[3] & b[3]), _T(a[4] & b[4]), _T(a[5] & b[5]), _T(a[6] & b[6]) }); }
template <> inline A8  bit_and       (const A8  &a, const A8  &b) { return A8 ({ _T(a[0] & b[0]), _T(a[1] & b[1]), _T(a[2] & b[2]), _T(a[3] & b[3]), _T(a[4] & b[4]), _T(a[5] & b[5]), _T(a[6] & b[6]), _T(a[7] & b[7]) }); }
template <> inline A9  bit_and       (const A9  &a, const A9  &b) { return A9 ({ _T(a[0] & b[0]), _T(a[1] & b[1]), _T(a[2] & b[2]), _T(a[3] & b[3]), _T(a[4] & b[4]), _T(a[5] & b[5]), _T(a[6] & b[6]), _T(a[7] & b[7]), _T(a[8] & b[8]) }); }
template <> inline A10 bit_and       (const A10 &a, const A10 &b) { return A10({ _T(a[0] & b[0]), _T(a[1] & b[1]), _T(a[2] & b[2]), _T(a[3] & b[3]), _T(a[4] & b[4]), _T(a[5] & b[5]), _T(a[6] & b[6]), _T(a[7] & b[7]), _T(a[8] & b[8]), _T(a[9] & b[9]) }); }
template <> inline A11 bit_and       (const A11 &a, const A11 &b) { return A11({ _T(a[0] & b[0]), _T(a[1] & b[1]), _T(a[2] & b[2]), _T(a[3] & b[3]), _T(a[4] & b[4]), _T(a[5] & b[5]), _T(a[6] & b[6]), _T(a[7] & b[7]), _T(a[8] & b[8]), _T(a[9] & b[9]), _T(a[10] & b[10]) }); }
template <> inline A12 bit_and       (const A12 &a, const A12 &b) { return A12({ _T(a[0] & b[0]), _T(a[1] & b[1]), _T(a[2] & b[2]), _T(a[3] & b[3]), _T(a[4] & b[4]), _T(a[5] & b[5]), _T(a[6] & b[6]), _T(a[7] & b[7]), _T(a[8] & b[8]), _T(a[9] & b[9]), _T(a[10] & b[10]), _T(a[11] & b[11]) }); }
template <> inline A13 bit_and       (const A13 &a, const A13 &b) { return A13({ _T(a[0] & b[0]), _T(a[1] & b[1]), _T(a[2] & b[2]), _T(a[3] & b[3]), _T(a[4] & b[4]), _T(a[5] & b[5]), _T(a[6] & b[6]), _T(a[7] & b[7]), _T(a[8] & b[8]), _T(a[9] & b[9]), _T(a[10] & b[10]), _T(a[11] & b[11]), _T(a[12] & b[12]) }); }
template <> inline A14 bit_and       (const A14 &a, const A14 &b) { return A14({ _T(a[0] & b[0]), _T(a[1] & b[1]), _T(a[2] & b[2]), _T(a[3] & b[3]), _T(a[4] & b[4]), _T(a[5] & b[5]), _T(a[6] & b[6]), _T(a[7] & b[7]), _T(a[8] & b[8]), _T(a[9] & b[9]), _T(a[10] & b[10]), _T(a[11] & b[11]), _T(a[12] & b[12]), _T(a[13] & b[13]) }); }
template <> inline A15 bit_and       (const A15 &a, const A15 &b) { return A15({ _T(a[0] & b[0]), _T(a[1] & b[1]), _T(a[2] & b[2]), _T(a[3] & b[3]), _T(a[4] & b[4]), _T(a[5] & b[5]), _T(a[6] & b[6]), _T(a[7] & b[7]), _T(a[8] & b[8]), _T(a[9] & b[9]), _T(a[10] & b[10]), _T(a[11] & b[11]), _T(a[12] & b[12]), _T(a[13] & b[13]), _T(a[14] & b[14]) }); }
template <> inline A16 bit_and       (const A16 &a, const A16 &b) { return A16({ _T(a[0] & b[0]), _T(a[1] & b[1]), _T(a[2] & b[2]), _T(a[3] & b[3]), _T(a[4] & b[4]), _T(a[5] & b[5]), _T(a[6] & b[6]), _T(a[7] & b[7]), _T(a[8] & b[8]), _T(a[9] & b[9]), _T(a[10] & b[10]), _T(a[11] & b[11]), _T(a[12] & b[12]), _T(a[13] & b[13]), _T(a[14] & b[14]), _T(a[15] & b[15]) }); }

template <> inline A1  bit_or        (const A1  &a, const A1  &b) { return A1 ({ _T(a[0] | b[0]) }); }
template <> inline A2  bit_or        (const A2  &a, const A2  &b) { return A2 ({ _T(a[0] | b[0]), _T(a[1] | b[1]) }); }
template <> inline A3  bit_or        (const A3  &a, const A3  &b) { return A3 ({ _T(a[0] | b[0]), _T(a[1] | b[1]), _T(a[2] | b[2]) }); }
template <> inline A4  bit_or        (const A4  &a, const A4  &b) { return A4 ({ _T(a[0] | b[0]), _T(a[1] | b[1]), _T(a[2] | b[2]), _T(a[3] | b[3]) }); }
template <> inline A5  bit_or        (const A5  &a, const A5  &b) { return A5 ({ _T(a[0] | b[0]), _T(a[1] | b[1]), _T(a[2] | b[2]), _T(a[3] | b[3]), _T(a[4] | b[4]) }); }
template <> inline A6  bit_or        (const A6  &a, const A6  &b) { return A6 ({ _T(a[0] | b[0]), _T(a[1] | b[1]), _T(a[2] | b[2]), _T(a[3] | b[3]), _T(a[4] | b[4]), _T(a[5] | b[5]) }); }
template <> inline A7  bit_or        (const A7  &a, const A7  &b) { return A7 ({ _T(a[0] | b[0]), _T(a[1] | b[1]), _T(a[2] | b[2]), _T(a[3] | b[3]), _T(a[4] | b[4]), _T(a[5] | b[5]), _T(a[6] | b[6]) }); }
template <> inline A8  bit_or        (const A8  &a, const A8  &b) { return A8 ({ _T(a[0] | b[0]), _T(a[1] | b[1]), _T(a[2] | b[2]), _T(a[3] | b[3]), _T(a[4] | b[4]), _T(a[5] | b[5]), _T(a[6] | b[6]), _T(a[7] | b[7]) }); }
template <> inline A9  bit_or        (const A9  &a, const A9  &b) { return A9 ({ _T(a[0] | b[0]), _T(a[1] | b[1]), _T(a[2] | b[2]), _T(a[3] | b[3]), _T(a[4] | b[4]), _T(a[5] | b[5]), _T(a[6] | b[6]), _T(a[7] | b[7]), _T(a[8] | b[8]) }); }
template <> inline A10 bit_or        (const A10 &a, const A10 &b) { return A10({ _T(a[0] | b[0]), _T(a[1] | b[1]), _T(a[2] | b[2]), _T(a[3] | b[3]), _T(a[4] | b[4]), _T(a[5] | b[5]), _T(a[6] | b[6]), _T(a[7] | b[7]), _T(a[8] | b[8]), _T(a[9] | b[9]) }); }
template <> inline A11 bit_or        (const A11 &a, const A11 &b) { return A11({ _T(a[0] | b[0]), _T(a[1] | b[1]), _T(a[2] | b[2]), _T(a[3] | b[3]), _T(a[4] | b[4]), _T(a[5] | b[5]), _T(a[6] | b[6]), _T(a[7] | b[7]), _T(a[8] | b[8]), _T(a[9] | b[9]), _T(a[10] | b[10]) }); }
template <> inline A12 bit_or        (const A12 &a, const A12 &b) { return A12({ _T(a[0] | b[0]), _T(a[1] | b[1]), _T(a[2] | b[2]), _T(a[3] | b[3]), _T(a[4] | b[4]), _T(a[5] | b[5]), _T(a[6] | b[6]), _T(a[7] | b[7]), _T(a[8] | b[8]), _T(a[9] | b[9]), _T(a[10] | b[10]), _T(a[11] | b[11]) }); }
template <> inline A13 bit_or        (const A13 &a, const A13 &b) { return A13({ _T(a[0] | b[0]), _T(a[1] | b[1]), _T(a[2] | b[2]), _T(a[3] | b[3]), _T(a[4] | b[4]), _T(a[5] | b[5]), _T(a[6] | b[6]), _T(a[7] | b[7]), _T(a[8] | b[8]), _T(a[9] | b[9]), _T(a[10] | b[10]), _T(a[11] | b[11]), _T(a[12] | b[12]) }); }
template <> inline A14 bit_or        (const A14 &a, const A14 &b) { return A14({ _T(a[0] | b[0]), _T(a[1] | b[1]), _T(a[2] | b[2]), _T(a[3] | b[3]), _T(a[4] | b[4]), _T(a[5] | b[5]), _T(a[6] | b[6]), _T(a[7] | b[7]), _T(a[8] | b[8]), _T(a[9] | b[9]), _T(a[10] | b[10]), _T(a[11] | b[11]), _T(a[12] | b[12]), _T(a[13] | b[13]) }); }
template <> inline A15 bit_or        (const A15 &a, const A15 &b) { return A15({ _T(a[0] | b[0]), _T(a[1] | b[1]), _T(a[2] | b[2]), _T(a[3] | b[3]), _T(a[4] | b[4]), _T(a[5] | b[5]), _T(a[6] | b[6]), _T(a[7] | b[7]), _T(a[8] | b[8]), _T(a[9] | b[9]), _T(a[10] | b[10]), _T(a[11] | b[11]), _T(a[12] | b[12]), _T(a[13] | b[13]), _T(a[14] | b[14]) }); }
template <> inline A16 bit_or        (const A16 &a, const A16 &b) { return A16({ _T(a[0] | b[0]), _T(a[1] | b[1]), _T(a[2] | b[2]), _T(a[3] | b[3]), _T(a[4] | b[4]), _T(a[5] | b[5]), _T(a[6] | b[6]), _T(a[7] | b[7]), _T(a[8] | b[8]), _T(a[9] | b[9]), _T(a[10] | b[10]), _T(a[11] | b[11]), _T(a[12] | b[12]), _T(a[13] | b[13]), _T(a[14] | b[14]), _T(a[15] | b[15]) }); }

template <> inline A1  bit_xor       (const A1  &a, const A1  &b) { return A1 ({ _T(a[0] ^ b[0]) }); }
template <> inline A2  bit_xor       (const A2  &a, const A2  &b) { return A2 ({ _T(a[0] ^ b[0]), _T(a[1] ^ b[1]) }); }
template <> inline A3  bit_xor       (const A3  &a, const A3  &b) { return A3 ({ _T(a[0] ^ b[0]), _T(a[1] ^ b[1]), _T(a[2] ^ b[2]) }); }
template <> inline A4  bit_xor       (const A4  &a, const A4  &b) { return A4 ({ _T(a[0] ^ b[0]), _T(a[1] ^ b[1]), _T(a[2] ^ b[2]), _T(a[3] ^ b[3]) }); }
template <> inline A5  bit_xor       (const A5  &a, const A5  &b) { return A5 ({ _T(a[0] ^ b[0]), _T(a[1] ^ b[1]), _T(a[2] ^ b[2]), _T(a[3] ^ b[3]), _T(a[4] ^ b[4]) }); }
template <> inline A6  bit_xor       (const A6  &a, const A6  &b) { return A6 ({ _T(a[0] ^ b[0]), _T(a[1] ^ b[1]), _T(a[2] ^ b[2]), _T(a[3] ^ b[3]), _T(a[4] ^ b[4]), _T(a[5] ^ b[5]) }); }
template <> inline A7  bit_xor       (const A7  &a, const A7  &b) { return A7 ({ _T(a[0] ^ b[0]), _T(a[1] ^ b[1]), _T(a[2] ^ b[2]), _T(a[3] ^ b[3]), _T(a[4] ^ b[4]), _T(a[5] ^ b[5]), _T(a[6] ^ b[6]) }); }
template <> inline A8  bit_xor       (const A8  &a, const A8  &b) { return A8 ({ _T(a[0] ^ b[0]), _T(a[1] ^ b[1]), _T(a[2] ^ b[2]), _T(a[3] ^ b[3]), _T(a[4] ^ b[4]), _T(a[5] ^ b[5]), _T(a[6] ^ b[6]), _T(a[7] ^ b[7]) }); }
template <> inline A9  bit_xor       (const A9  &a, const A9  &b) { return A9 ({ _T(a[0] ^ b[0]), _T(a[1] ^ b[1]), _T(a[2] ^ b[2]), _T(a[3] ^ b[3]), _T(a[4] ^ b[4]), _T(a[5] ^ b[5]), _T(a[6] ^ b[6]), _T(a[7] ^ b[7]), _T(a[8] ^ b[8]) }); }
template <> inline A10 bit_xor       (const A10 &a, const A10 &b) { return A10({ _T(a[0] ^ b[0]), _T(a[1] ^ b[1]), _T(a[2] ^ b[2]), _T(a[3] ^ b[3]), _T(a[4] ^ b[4]), _T(a[5] ^ b[5]), _T(a[6] ^ b[6]), _T(a[7] ^ b[7]), _T(a[8] ^ b[8]), _T(a[9] ^ b[9]) }); }
template <> inline A11 bit_xor       (const A11 &a, const A11 &b) { return A11({ _T(a[0] ^ b[0]), _T(a[1] ^ b[1]), _T(a[2] ^ b[2]), _T(a[3] ^ b[3]), _T(a[4] ^ b[4]), _T(a[5] ^ b[5]), _T(a[6] ^ b[6]), _T(a[7] ^ b[7]), _T(a[8] ^ b[8]), _T(a[9] ^ b[9]), _T(a[10] ^ b[10]) }); }
template <> inline A12 bit_xor       (const A12 &a, const A12 &b) { return A12({ _T(a[0] ^ b[0]), _T(a[1] ^ b[1]), _T(a[2] ^ b[2]), _T(a[3] ^ b[3]), _T(a[4] ^ b[4]), _T(a[5] ^ b[5]), _T(a[6] ^ b[6]), _T(a[7] ^ b[7]), _T(a[8] ^ b[8]), _T(a[9] ^ b[9]), _T(a[10] ^ b[10]), _T(a[11] ^ b[11]) }); }
template <> inline A13 bit_xor       (const A13 &a, const A13 &b) { return A13({ _T(a[0] ^ b[0]), _T(a[1] ^ b[1]), _T(a[2] ^ b[2]), _T(a[3] ^ b[3]), _T(a[4] ^ b[4]), _T(a[5] ^ b[5]), _T(a[6] ^ b[6]), _T(a[7] ^ b[7]), _T(a[8] ^ b[8]), _T(a[9] ^ b[9]), _T(a[10] ^ b[10]), _T(a[11] ^ b[11]), _T(a[12] ^ b[12]) }); }
template <> inline A14 bit_xor       (const A14 &a, const A14 &b) { return A14({ _T(a[0] ^ b[0]), _T(a[1] ^ b[1]), _T(a[2] ^ b[2]), _T(a[3] ^ b[3]), _T(a[4] ^ b[4]), _T(a[5] ^ b[5]), _T(a[6] ^ b[6]), _T(a[7] ^ b[7]), _T(a[8] ^ b[8]), _T(a[9] ^ b[9]), _T(a[10] ^ b[10]), _T(a[11] ^ b[11]), _T(a[12] ^ b[12]), _T(a[13] ^ b[13]) }); }
template <> inline A15 bit_xor       (const A15 &a, const A15 &b) { return A15({ _T(a[0] ^ b[0]), _T(a[1] ^ b[1]), _T(a[2] ^ b[2]), _T(a[3] ^ b[3]), _T(a[4] ^ b[4]), _T(a[5] ^ b[5]), _T(a[6] ^ b[6]), _T(a[7] ^ b[7]), _T(a[8] ^ b[8]), _T(a[9] ^ b[9]), _T(a[10] ^ b[10]), _T(a[11] ^ b[11]), _T(a[12] ^ b[12]), _T(a[13] ^ b[13]), _T(a[14] ^ b[14]) }); }
template <> inline A16 bit_xor       (const A16 &a, const A16 &b) { return A16({ _T(a[0] ^ b[0]), _T(a[1] ^ b[1]), _T(a[2] ^ b[2]), _T(a[3] ^ b[3]), _T(a[4] ^ b[4]), _T(a[5] ^ b[5]), _T(a[6] ^ b[6]), _T(a[7] ^ b[7]), _T(a[8] ^ b[8]), _T(a[9] ^ b[9]), _T(a[10] ^ b[10]), _T(a[11] ^ b[11]), _T(a[12] ^ b[12]), _T(a[13] ^ b[13]), _T(a[14] ^ b[14]), _T(a[15] ^ b[15]) }); }
#endif

template <> inline A1  pow           (const A1  &a, _T b)         { return A1 ({ _T(std::pow(a[0], b)) }); }
template <> inline A2  pow           (const A2  &a, _T b)         { return A2 ({ _T(std::pow(a[0], b)), _T(std::pow(a[1], b)) }); }
template <> inline A3  pow           (const A3  &a, _T b)         { return A3 ({ _T(std::pow(a[0], b)), _T(std::pow(a[1], b)), _T(std::pow(a[2], b)) }); }
template <> inline A4  pow           (const A4  &a, _T b)         { return A4 ({ _T(std::pow(a[0], b)), _T(std::pow(a[1], b)), _T(std::pow(a[2], b)), _T(std::pow(a[3], b)) }); }
template <> inline A5  pow           (const A5  &a, _T b)         { return A5 ({ _T(std::pow(a[0], b)), _T(std::pow(a[1], b)), _T(std::pow(a[2], b)), _T(std::pow(a[3], b)), _T(std::pow(a[4], b)) }); }
template <> inline A6  pow           (const A6  &a, _T b)         { return A6 ({ _T(std::pow(a[0], b)), _T(std::pow(a[1], b)), _T(std::pow(a[2], b)), _T(std::pow(a[3], b)), _T(std::pow(a[4], b)), _T(std::pow(a[5], b)) }); }
template <> inline A7  pow           (const A7  &a, _T b)         { return A7 ({ _T(std::pow(a[0], b)), _T(std::pow(a[1], b)), _T(std::pow(a[2], b)), _T(std::pow(a[3], b)), _T(std::pow(a[4], b)), _T(std::pow(a[5], b)), _T(std::pow(a[6], b)) }); }
template <> inline A8  pow           (const A8  &a, _T b)         { return A8 ({ _T(std::pow(a[0], b)), _T(std::pow(a[1], b)), _T(std::pow(a[2], b)), _T(std::pow(a[3], b)), _T(std::pow(a[4], b)), _T(std::pow(a[5], b)), _T(std::pow(a[6], b)), _T(std::pow(a[7], b)) }); }
template <> inline A9  pow           (const A9  &a, _T b)         { return A9 ({ _T(std::pow(a[0], b)), _T(std::pow(a[1], b)), _T(std::pow(a[2], b)), _T(std::pow(a[3], b)), _T(std::pow(a[4], b)), _T(std::pow(a[5], b)), _T(std::pow(a[6], b)), _T(std::pow(a[7], b)), _T(std::pow(a[8], b)) }); }
template <> inline A10 pow           (const A10 &a, _T b)         { return A10({ _T(std::pow(a[0], b)), _T(std::pow(a[1], b)), _T(std::pow(a[2], b)), _T(std::pow(a[3], b)), _T(std::pow(a[4], b)), _T(std::pow(a[5], b)), _T(std::pow(a[6], b)), _T(std::pow(a[7], b)), _T(std::pow(a[8], b)), _T(std::pow(a[9], b)) }); }
template <> inline A11 pow           (const A11 &a, _T b)         { return A11({ _T(std::pow(a[0], b)), _T(std::pow(a[1], b)), _T(std::pow(a[2], b)), _T(std::pow(a[3], b)), _T(std::pow(a[4], b)), _T(std::pow(a[5], b)), _T(std::pow(a[6], b)), _T(std::pow(a[7], b)), _T(std::pow(a[8], b)), _T(std::pow(a[9], b)), _T(std::pow(a[10], b)) }); }
template <> inline A12 pow           (const A12 &a, _T b)         { return A12({ _T(std::pow(a[0], b)), _T(std::pow(a[1], b)), _T(std::pow(a[2], b)), _T(std::pow(a[3], b)), _T(std::pow(a[4], b)), _T(std::pow(a[5], b)), _T(std::pow(a[6], b)), _T(std::pow(a[7], b)), _T(std::pow(a[8], b)), _T(std::pow(a[9], b)), _T(std::pow(a[10], b)), _T(std::pow(a[11], b)) }); }
template <> inline A13 pow           (const A13 &a, _T b)         { return A13({ _T(std::pow(a[0], b)), _T(std::pow(a[1], b)), _T(std::pow(a[2], b)), _T(std::pow(a[3], b)), _T(std::pow(a[4], b)), _T(std::pow(a[5], b)), _T(std::pow(a[6], b)), _T(std::pow(a[7], b)), _T(std::pow(a[8], b)), _T(std::pow(a[9], b)), _T(std::pow(a[10], b)), _T(std::pow(a[11], b)), _T(std::pow(a[12], b)) }); }
template <> inline A14 pow           (const A14 &a, _T b)         { return A14({ _T(std::pow(a[0], b)), _T(std::pow(a[1], b)), _T(std::pow(a[2], b)), _T(std::pow(a[3], b)), _T(std::pow(a[4], b)), _T(std::pow(a[5], b)), _T(std::pow(a[6], b)), _T(std::pow(a[7], b)), _T(std::pow(a[8], b)), _T(std::pow(a[9], b)), _T(std::pow(a[10], b)), _T(std::pow(a[11], b)), _T(std::pow(a[12], b)), _T(std::pow(a[13], b)) }); }
template <> inline A15 pow           (const A15 &a, _T b)         { return A15({ _T(std::pow(a[0], b)), _T(std::pow(a[1], b)), _T(std::pow(a[2], b)), _T(std::pow(a[3], b)), _T(std::pow(a[4], b)), _T(std::pow(a[5], b)), _T(std::pow(a[6], b)), _T(std::pow(a[7], b)), _T(std::pow(a[8], b)), _T(std::pow(a[9], b)), _T(std::pow(a[10], b)), _T(std::pow(a[11], b)), _T(std::pow(a[12], b)), _T(std::pow(a[13], b)), _T(std::pow(a[14], b)) }); }
template <> inline A16 pow           (const A16 &a, _T b)         { return A16({ _T(std::pow(a[0], b)), _T(std::pow(a[1], b)), _T(std::pow(a[2], b)), _T(std::pow(a[3], b)), _T(std::pow(a[4], b)), _T(std::pow(a[5], b)), _T(std::pow(a[6], b)), _T(std::pow(a[7], b)), _T(std::pow(a[8], b)), _T(std::pow(a[9], b)), _T(std::pow(a[10], b)), _T(std::pow(a[11], b)), _T(std::pow(a[12], b)), _T(std::pow(a[13], b)), _T(std::pow(a[14], b)), _T(std::pow(a[15], b)) }); }
template <> inline A1  subtract      (const A1  &a, _T b)         { return A1 ({ _T(a[0] - b) }); }
template <> inline A2  subtract      (const A2  &a, _T b)         { return A2 ({ _T(a[0] - b), _T(a[1] - b) }); }
template <> inline A3  subtract      (const A3  &a, _T b)         { return A3 ({ _T(a[0] - b), _T(a[1] - b), _T(a[2] - b) }); }
template <> inline A4  subtract      (const A4  &a, _T b)         { return A4 ({ _T(a[0] - b), _T(a[1] - b), _T(a[2] - b), _T(a[3] - b) }); }
template <> inline A5  subtract      (const A5  &a, _T b)         { return A5 ({ _T(a[0] - b), _T(a[1] - b), _T(a[2] - b), _T(a[3] - b), _T(a[4] - b) }); }
template <> inline A6  subtract      (const A6  &a, _T b)         { return A6 ({ _T(a[0] - b), _T(a[1] - b), _T(a[2] - b), _T(a[3] - b), _T(a[4] - b), _T(a[5] - b) }); }
template <> inline A7  subtract      (const A7  &a, _T b)         { return A7 ({ _T(a[0] - b), _T(a[1] - b), _T(a[2] - b), _T(a[3] - b), _T(a[4] - b), _T(a[5] - b), _T(a[6] - b) }); }
template <> inline A8  subtract      (const A8  &a, _T b)         { return A8 ({ _T(a[0] - b), _T(a[1] - b), _T(a[2] - b), _T(a[3] - b), _T(a[4] - b), _T(a[5] - b), _T(a[6] - b), _T(a[7] - b) }); }
template <> inline A9  subtract      (const A9  &a, _T b)         { return A9 ({ _T(a[0] - b), _T(a[1] - b), _T(a[2] - b), _T(a[3] - b), _T(a[4] - b), _T(a[5] - b), _T(a[6] - b), _T(a[7] - b), _T(a[8] - b) }); }
template <> inline A10 subtract      (const A10 &a, _T b)         { return A10({ _T(a[0] - b), _T(a[1] - b), _T(a[2] - b), _T(a[3] - b), _T(a[4] - b), _T(a[5] - b), _T(a[6] - b), _T(a[7] - b), _T(a[8] - b), _T(a[9] - b) }); }
template <> inline A11 subtract      (const A11 &a, _T b)         { return A11({ _T(a[0] - b), _T(a[1] - b), _T(a[2] - b), _T(a[3] - b), _T(a[4] - b), _T(a[5] - b), _T(a[6] - b), _T(a[7] - b), _T(a[8] - b), _T(a[9] - b), _T(a[10] - b) }); }
template <> inline A12 subtract      (const A12 &a, _T b)         { return A12({ _T(a[0] - b), _T(a[1] - b), _T(a[2] - b), _T(a[3] - b), _T(a[4] - b), _T(a[5] - b), _T(a[6] - b), _T(a[7] - b), _T(a[8] - b), _T(a[9] - b), _T(a[10] - b), _T(a[11] - b) }); }
template <> inline A13 subtract      (const A13 &a, _T b)         { return A13({ _T(a[0] - b), _T(a[1] - b), _T(a[2] - b), _T(a[3] - b), _T(a[4] - b), _T(a[5] - b), _T(a[6] - b), _T(a[7] - b), _T(a[8] - b), _T(a[9] - b), _T(a[10] - b), _T(a[11] - b), _T(a[12] - b) }); }
template <> inline A14 subtract      (const A14 &a, _T b)         { return A14({ _T(a[0] - b), _T(a[1] - b), _T(a[2] - b), _T(a[3] - b), _T(a[4] - b), _T(a[5] - b), _T(a[6] - b), _T(a[7] - b), _T(a[8] - b), _T(a[9] - b), _T(a[10] - b), _T(a[11] - b), _T(a[12] - b), _T(a[13] - b) }); }
template <> inline A15 subtract      (const A15 &a, _T b)         { return A15({ _T(a[0] - b), _T(a[1] - b), _T(a[2] - b), _T(a[3] - b), _T(a[4] - b), _T(a[5] - b), _T(a[6] - b), _T(a[7] - b), _T(a[8] - b), _T(a[9] - b), _T(a[10] - b), _T(a[11] - b), _T(a[12] - b), _T(a[13] - b), _T(a[14] - b) }); }
template <> inline A16 subtract      (const A16 &a, _T b)         { return A16({ _T(a[0] - b), _T(a[1] - b), _T(a[2] - b), _T(a[3] - b), _T(a[4] - b), _T(a[5] - b), _T(a[6] - b), _T(a[7] - b), _T(a[8] - b), _T(a[9] - b), _T(a[10] - b), _T(a[11] - b), _T(a[12] - b), _T(a[13] - b), _T(a[14] - b), _T(a[15] - b) }); }

template <> inline A1  add           (const A1  &a, _T b)         { return A1 ({ _T(a[0] + b) }); }
template <> inline A2  add           (const A2  &a, _T b)         { return A2 ({ _T(a[0] + b), _T(a[1] + b) }); }
template <> inline A3  add           (const A3  &a, _T b)         { return A3 ({ _T(a[0] + b), _T(a[1] + b), _T(a[2] + b) }); }
template <> inline A4  add           (const A4  &a, _T b)         { return A4 ({ _T(a[0] + b), _T(a[1] + b), _T(a[2] + b), _T(a[3] + b) }); }
template <> inline A5  add           (const A5  &a, _T b)         { return A5 ({ _T(a[0] + b), _T(a[1] + b), _T(a[2] + b), _T(a[3] + b), _T(a[4] + b) }); }
template <> inline A6  add           (const A6  &a, _T b)         { return A6 ({ _T(a[0] + b), _T(a[1] + b), _T(a[2] + b), _T(a[3] + b), _T(a[4] + b), _T(a[5] + b) }); }
template <> inline A7  add           (const A7  &a, _T b)         { return A7 ({ _T(a[0] + b), _T(a[1] + b), _T(a[2] + b), _T(a[3] + b), _T(a[4] + b), _T(a[5] + b), _T(a[6] + b) }); }
template <> inline A8  add           (const A8  &a, _T b)         { return A8 ({ _T(a[0] + b), _T(a[1] + b), _T(a[2] + b), _T(a[3] + b), _T(a[4] + b), _T(a[5] + b), _T(a[6] + b), _T(a[7] + b) }); }
template <> inline A9  add           (const A9  &a, _T b)         { return A9 ({ _T(a[0] + b), _T(a[1] + b), _T(a[2] + b), _T(a[3] + b), _T(a[4] + b), _T(a[5] + b), _T(a[6] + b), _T(a[7] + b), _T(a[8] + b) }); }
template <> inline A10 add           (const A10 &a, _T b)         { return A10({ _T(a[0] + b), _T(a[1] + b), _T(a[2] + b), _T(a[3] + b), _T(a[4] + b), _T(a[5] + b), _T(a[6] + b), _T(a[7] + b), _T(a[8] + b), _T(a[9] + b) }); }
template <> inline A11 add           (const A11 &a, _T b)         { return A11({ _T(a[0] + b), _T(a[1] + b), _T(a[2] + b), _T(a[3] + b), _T(a[4] + b), _T(a[5] + b), _T(a[6] + b), _T(a[7] + b), _T(a[8] + b), _T(a[9] + b), _T(a[10] + b) }); }
template <> inline A12 add           (const A12 &a, _T b)         { return A12({ _T(a[0] + b), _T(a[1] + b), _T(a[2] + b), _T(a[3] + b), _T(a[4] + b), _T(a[5] + b), _T(a[6] + b), _T(a[7] + b), _T(a[8] + b), _T(a[9] + b), _T(a[10] + b), _T(a[11] + b) }); }
template <> inline A13 add           (const A13 &a, _T b)         { return A13({ _T(a[0] + b), _T(a[1] + b), _T(a[2] + b), _T(a[3] + b), _T(a[4] + b), _T(a[5] + b), _T(a[6] + b), _T(a[7] + b), _T(a[8] + b), _T(a[9] + b), _T(a[10] + b), _T(a[11] + b), _T(a[12] + b) }); }
template <> inline A14 add           (const A14 &a, _T b)         { return A14({ _T(a[0] + b), _T(a[1] + b), _T(a[2] + b), _T(a[3] + b), _T(a[4] + b), _T(a[5] + b), _T(a[6] + b), _T(a[7] + b), _T(a[8] + b), _T(a[9] + b), _T(a[10] + b), _T(a[11] + b), _T(a[12] + b), _T(a[13] + b) }); }
template <> inline A15 add           (const A15 &a, _T b)         { return A15({ _T(a[0] + b), _T(a[1] + b), _T(a[2] + b), _T(a[3] + b), _T(a[4] + b), _T(a[5] + b), _T(a[6] + b), _T(a[7] + b), _T(a[8] + b), _T(a[9] + b), _T(a[10] + b), _T(a[11] + b), _T(a[12] + b), _T(a[13] + b), _T(a[14] + b) }); }
template <> inline A16 add           (const A16 &a, _T b)         { return A16({ _T(a[0] + b), _T(a[1] + b), _T(a[2] + b), _T(a[3] + b), _T(a[4] + b), _T(a[5] + b), _T(a[6] + b), _T(a[7] + b), _T(a[8] + b), _T(a[9] + b), _T(a[10] + b), _T(a[11] + b), _T(a[12] + b), _T(a[13] + b), _T(a[14] + b), _T(a[15] + b) }); }

template <> inline A1  multiply      (const A1  &a, _T b)         { return A1 ({ _T(a[0] * b) }); }
template <> inline A2  multiply      (const A2  &a, _T b)         { return A2 ({ _T(a[0] * b), _T(a[1] * b) }); }
template <> inline A3  multiply      (const A3  &a, _T b)         { return A3 ({ _T(a[0] * b), _T(a[1] * b), _T(a[2] * b) }); }
template <> inline A4  multiply      (const A4  &a, _T b)         { return A4 ({ _T(a[0] * b), _T(a[1] * b), _T(a[2] * b), _T(a[3] * b) }); }
template <> inline A5  multiply      (const A5  &a, _T b)         { return A5 ({ _T(a[0] * b), _T(a[1] * b), _T(a[2] * b), _T(a[3] * b), _T(a[4] * b) }); }
template <> inline A6  multiply      (const A6  &a, _T b)         { return A6 ({ _T(a[0] * b), _T(a[1] * b), _T(a[2] * b), _T(a[3] * b), _T(a[4] * b), _T(a[5] * b) }); }
template <> inline A7  multiply      (const A7  &a, _T b)         { return A7 ({ _T(a[0] * b), _T(a[1] * b), _T(a[2] * b), _T(a[3] * b), _T(a[4] * b), _T(a[5] * b), _T(a[6] * b) }); }
template <> inline A8  multiply      (const A8  &a, _T b)         { return A8 ({ _T(a[0] * b), _T(a[1] * b), _T(a[2] * b), _T(a[3] * b), _T(a[4] * b), _T(a[5] * b), _T(a[6] * b), _T(a[7] * b) }); }
template <> inline A9  multiply      (const A9  &a, _T b)         { return A9 ({ _T(a[0] * b), _T(a[1] * b), _T(a[2] * b), _T(a[3] * b), _T(a[4] * b), _T(a[5] * b), _T(a[6] * b), _T(a[7] * b), _T(a[8] * b) }); }
template <> inline A10 multiply      (const A10 &a, _T b)         { return A10({ _T(a[0] * b), _T(a[1] * b), _T(a[2] * b), _T(a[3] * b), _T(a[4] * b), _T(a[5] * b), _T(a[6] * b), _T(a[7] * b), _T(a[8] * b), _T(a[9] * b) }); }
template <> inline A11 multiply      (const A11 &a, _T b)         { return A11({ _T(a[0] * b), _T(a[1] * b), _T(a[2] * b), _T(a[3] * b), _T(a[4] * b), _T(a[5] * b), _T(a[6] * b), _T(a[7] * b), _T(a[8] * b), _T(a[9] * b), _T(a[10] * b) }); }
template <> inline A12 multiply      (const A12 &a, _T b)         { return A12({ _T(a[0] * b), _T(a[1] * b), _T(a[2] * b), _T(a[3] * b), _T(a[4] * b), _T(a[5] * b), _T(a[6] * b), _T(a[7] * b), _T(a[8] * b), _T(a[9] * b), _T(a[10] * b), _T(a[11] * b) }); }
template <> inline A13 multiply      (const A13 &a, _T b)         { return A13({ _T(a[0] * b), _T(a[1] * b), _T(a[2] * b), _T(a[3] * b), _T(a[4] * b), _T(a[5] * b), _T(a[6] * b), _T(a[7] * b), _T(a[8] * b), _T(a[9] * b), _T(a[10] * b), _T(a[11] * b), _T(a[12] * b) }); }
template <> inline A14 multiply      (const A14 &a, _T b)         { return A14({ _T(a[0] * b), _T(a[1] * b), _T(a[2] * b), _T(a[3] * b), _T(a[4] * b), _T(a[5] * b), _T(a[6] * b), _T(a[7] * b), _T(a[8] * b), _T(a[9] * b), _T(a[10] * b), _T(a[11] * b), _T(a[12] * b), _T(a[13] * b) }); }
template <> inline A15 multiply      (const A15 &a, _T b)         { return A15({ _T(a[0] * b), _T(a[1] * b), _T(a[2] * b), _T(a[3] * b), _T(a[4] * b), _T(a[5] * b), _T(a[6] * b), _T(a[7] * b), _T(a[8] * b), _T(a[9] * b), _T(a[10] * b), _T(a[11] * b), _T(a[12] * b), _T(a[13] * b), _T(a[14] * b) }); }
template <> inline A16 multiply      (const A16 &a, _T b)         { return A16({ _T(a[0] * b), _T(a[1] * b), _T(a[2] * b), _T(a[3] * b), _T(a[4] * b), _T(a[5] * b), _T(a[6] * b), _T(a[7] * b), _T(a[8] * b), _T(a[9] * b), _T(a[10] * b), _T(a[11] * b), _T(a[12] * b), _T(a[13] * b), _T(a[14] * b), _T(a[15] * b) }); }

template <> inline A1  divide        (const A1  &a, _T b)         { return A1 ({ _T(a[0] / b) }); }
template <> inline A2  divide        (const A2  &a, _T b)         { return A2 ({ _T(a[0] / b), _T(a[1] / b) }); }
template <> inline A3  divide        (const A3  &a, _T b)         { return A3 ({ _T(a[0] / b), _T(a[1] / b), _T(a[2] / b) }); }
template <> inline A4  divide        (const A4  &a, _T b)         { return A4 ({ _T(a[0] / b), _T(a[1] / b), _T(a[2] / b), _T(a[3] / b) }); }
template <> inline A5  divide        (const A5  &a, _T b)         { return A5 ({ _T(a[0] / b), _T(a[1] / b), _T(a[2] / b), _T(a[3] / b), _T(a[4] / b) }); }
template <> inline A6  divide        (const A6  &a, _T b)         { return A6 ({ _T(a[0] / b), _T(a[1] / b), _T(a[2] / b), _T(a[3] / b), _T(a[4] / b), _T(a[5] / b) }); }
template <> inline A7  divide        (const A7  &a, _T b)         { return A7 ({ _T(a[0] / b), _T(a[1] / b), _T(a[2] / b), _T(a[3] / b), _T(a[4] / b), _T(a[5] / b), _T(a[6] / b) }); }
template <> inline A8  divide        (const A8  &a, _T b)         { return A8 ({ _T(a[0] / b), _T(a[1] / b), _T(a[2] / b), _T(a[3] / b), _T(a[4] / b), _T(a[5] / b), _T(a[6] / b), _T(a[7] / b) }); }
template <> inline A9  divide        (const A9  &a, _T b)         { return A9 ({ _T(a[0] / b), _T(a[1] / b), _T(a[2] / b), _T(a[3] / b), _T(a[4] / b), _T(a[5] / b), _T(a[6] / b), _T(a[7] / b), _T(a[8] / b) }); }
template <> inline A10 divide        (const A10 &a, _T b)         { return A10({ _T(a[0] / b), _T(a[1] / b), _T(a[2] / b), _T(a[3] / b), _T(a[4] / b), _T(a[5] / b), _T(a[6] / b), _T(a[7] / b), _T(a[8] / b), _T(a[9] / b) }); }
template <> inline A11 divide        (const A11 &a, _T b)         { return A11({ _T(a[0] / b), _T(a[1] / b), _T(a[2] / b), _T(a[3] / b), _T(a[4] / b), _T(a[5] / b), _T(a[6] / b), _T(a[7] / b), _T(a[8] / b), _T(a[9] / b), _T(a[10] / b) }); }
template <> inline A12 divide        (const A12 &a, _T b)         { return A12({ _T(a[0] / b), _T(a[1] / b), _T(a[2] / b), _T(a[3] / b), _T(a[4] / b), _T(a[5] / b), _T(a[6] / b), _T(a[7] / b), _T(a[8] / b), _T(a[9] / b), _T(a[10] / b), _T(a[11] / b) }); }
template <> inline A13 divide        (const A13 &a, _T b)         { return A13({ _T(a[0] / b), _T(a[1] / b), _T(a[2] / b), _T(a[3] / b), _T(a[4] / b), _T(a[5] / b), _T(a[6] / b), _T(a[7] / b), _T(a[8] / b), _T(a[9] / b), _T(a[10] / b), _T(a[11] / b), _T(a[12] / b) }); }
template <> inline A14 divide        (const A14 &a, _T b)         { return A14({ _T(a[0] / b), _T(a[1] / b), _T(a[2] / b), _T(a[3] / b), _T(a[4] / b), _T(a[5] / b), _T(a[6] / b), _T(a[7] / b), _T(a[8] / b), _T(a[9] / b), _T(a[10] / b), _T(a[11] / b), _T(a[12] / b), _T(a[13] / b) }); }
template <> inline A15 divide        (const A15 &a, _T b)         { return A15({ _T(a[0] / b), _T(a[1] / b), _T(a[2] / b), _T(a[3] / b), _T(a[4] / b), _T(a[5] / b), _T(a[6] / b), _T(a[7] / b), _T(a[8] / b), _T(a[9] / b), _T(a[10] / b), _T(a[11] / b), _T(a[12] / b), _T(a[13] / b), _T(a[14] / b) }); }
template <> inline A16 divide        (const A16 &a, _T b)         { return A16({ _T(a[0] / b), _T(a[1] / b), _T(a[2] / b), _T(a[3] / b), _T(a[4] / b), _T(a[5] / b), _T(a[6] / b), _T(a[7] / b), _T(a[8] / b), _T(a[9] / b), _T(a[10] / b), _T(a[11] / b), _T(a[12] / b), _T(a[13] / b), _T(a[14] / b), _T(a[15] / b) }); }

#if _T_INT == 1
template <> inline A1  modulus       (const A1  &a, _T b)         { return A1 ({ _T(a[0] % b) }); }
template <> inline A2  modulus       (const A2  &a, _T b)         { return A2 ({ _T(a[0] % b), _T(a[1] % b) }); }
template <> inline A3  modulus       (const A3  &a, _T b)         { return A3 ({ _T(a[0] % b), _T(a[1] % b), _T(a[2] % b) }); }
template <> inline A4  modulus       (const A4  &a, _T b)         { return A4 ({ _T(a[0] % b), _T(a[1] % b), _T(a[2] % b), _T(a[3] % b) }); }
template <> inline A5  modulus       (const A5  &a, _T b)         { return A5 ({ _T(a[0] % b), _T(a[1] % b), _T(a[2] % b), _T(a[3] % b), _T(a[4] % b) }); }
template <> inline A6  modulus       (const A6  &a, _T b)         { return A6 ({ _T(a[0] % b), _T(a[1] % b), _T(a[2] % b), _T(a[3] % b), _T(a[4] % b), _T(a[5] % b) }); }
template <> inline A7  modulus       (const A7  &a, _T b)         { return A7 ({ _T(a[0] % b), _T(a[1] % b), _T(a[2] % b), _T(a[3] % b), _T(a[4] % b), _T(a[5] % b), _T(a[6] % b) }); }
template <> inline A8  modulus       (const A8  &a, _T b)         { return A8 ({ _T(a[0] % b), _T(a[1] % b), _T(a[2] % b), _T(a[3] % b), _T(a[4] % b), _T(a[5] % b), _T(a[6] % b), _T(a[7] % b) }); }
template <> inline A9  modulus       (const A9  &a, _T b)         { return A9 ({ _T(a[0] % b), _T(a[1] % b), _T(a[2] % b), _T(a[3] % b), _T(a[4] % b), _T(a[5] % b), _T(a[6] % b), _T(a[7] % b), _T(a[8] % b) }); }
template <> inline A10 modulus       (const A10 &a, _T b)         { return A10({ _T(a[0] % b), _T(a[1] % b), _T(a[2] % b), _T(a[3] % b), _T(a[4] % b), _T(a[5] % b), _T(a[6] % b), _T(a[7] % b), _T(a[8] % b), _T(a[9] % b) }); }
template <> inline A11 modulus       (const A11 &a, _T b)         { return A11({ _T(a[0] % b), _T(a[1] % b), _T(a[2] % b), _T(a[3] % b), _T(a[4] % b), _T(a[5] % b), _T(a[6] % b), _T(a[7] % b), _T(a[8] % b), _T(a[9] % b), _T(a[10] % b) }); }
template <> inline A12 modulus       (const A12 &a, _T b)         { return A12({ _T(a[0] % b), _T(a[1] % b), _T(a[2] % b), _T(a[3] % b), _T(a[4] % b), _T(a[5] % b), _T(a[6] % b), _T(a[7] % b), _T(a[8] % b), _T(a[9] % b), _T(a[10] % b), _T(a[11] % b) }); }
template <> inline A13 modulus       (const A13 &a, _T b)         { return A13({ _T(a[0] % b), _T(a[1] % b), _T(a[2] % b), _T(a[3] % b), _T(a[4] % b), _T(a[5] % b), _T(a[6] % b), _T(a[7] % b), _T(a[8] % b), _T(a[9] % b), _T(a[10] % b), _T(a[11] % b), _T(a[12] % b) }); }
template <> inline A14 modulus       (const A14 &a, _T b)         { return A14({ _T(a[0] % b), _T(a[1] % b), _T(a[2] % b), _T(a[3] % b), _T(a[4] % b), _T(a[5] % b), _T(a[6] % b), _T(a[7] % b), _T(a[8] % b), _T(a[9] % b), _T(a[10] % b), _T(a[11] % b), _T(a[12] % b), _T(a[13] % b) }); }
template <> inline A15 modulus       (const A15 &a, _T b)         { return A15({ _T(a[0] % b), _T(a[1] % b), _T(a[2] % b), _T(a[3] % b), _T(a[4] % b), _T(a[5] % b), _T(a[6] % b), _T(a[7] % b), _T(a[8] % b), _T(a[9] % b), _T(a[10] % b), _T(a[11] % b), _T(a[12] % b), _T(a[13] % b), _T(a[14] % b) }); }
template <> inline A16 modulus       (const A16 &a, _T b)         { return A16({ _T(a[0] % b), _T(a[1] % b), _T(a[2] % b), _T(a[3] % b), _T(a[4] % b), _T(a[5] % b), _T(a[6] % b), _T(a[7] % b), _T(a[8] % b), _T(a[9] % b), _T(a[10] % b), _T(a[11] % b), _T(a[12] % b), _T(a[13] % b), _T(a[14] % b), _T(a[15] % b) }); }

template <> inline A1  bit_and       (const A1  &a, _T b)         { return A1 ({ _T(a[0] & b) }); }
template <> inline A2  bit_and       (const A2  &a, _T b)         { return A2 ({ _T(a[0] & b), _T(a[1] & b) }); }
template <> inline A3  bit_and       (const A3  &a, _T b)         { return A3 ({ _T(a[0] & b), _T(a[1] & b), _T(a[2] & b) }); }
template <> inline A4  bit_and       (const A4  &a, _T b)         { return A4 ({ _T(a[0] & b), _T(a[1] & b), _T(a[2] & b), _T(a[3] & b) }); }
template <> inline A5  bit_and       (const A5  &a, _T b)         { return A5 ({ _T(a[0] & b), _T(a[1] & b), _T(a[2] & b), _T(a[3] & b), _T(a[4] & b) }); }
template <> inline A6  bit_and       (const A6  &a, _T b)         { return A6 ({ _T(a[0] & b), _T(a[1] & b), _T(a[2] & b), _T(a[3] & b), _T(a[4] & b), _T(a[5] & b) }); }
template <> inline A7  bit_and       (const A7  &a, _T b)         { return A7 ({ _T(a[0] & b), _T(a[1] & b), _T(a[2] & b), _T(a[3] & b), _T(a[4] & b), _T(a[5] & b), _T(a[6] & b) }); }
template <> inline A8  bit_and       (const A8  &a, _T b)         { return A8 ({ _T(a[0] & b), _T(a[1] & b), _T(a[2] & b), _T(a[3] & b), _T(a[4] & b), _T(a[5] & b), _T(a[6] & b), _T(a[7] & b) }); }
template <> inline A9  bit_and       (const A9  &a, _T b)         { return A9 ({ _T(a[0] & b), _T(a[1] & b), _T(a[2] & b), _T(a[3] & b), _T(a[4] & b), _T(a[5] & b), _T(a[6] & b), _T(a[7] & b), _T(a[8] & b) }); }
template <> inline A10 bit_and       (const A10 &a, _T b)         { return A10({ _T(a[0] & b), _T(a[1] & b), _T(a[2] & b), _T(a[3] & b), _T(a[4] & b), _T(a[5] & b), _T(a[6] & b), _T(a[7] & b), _T(a[8] & b), _T(a[9] & b) }); }
template <> inline A11 bit_and       (const A11 &a, _T b)         { return A11({ _T(a[0] & b), _T(a[1] & b), _T(a[2] & b), _T(a[3] & b), _T(a[4] & b), _T(a[5] & b), _T(a[6] & b), _T(a[7] & b), _T(a[8] & b), _T(a[9] & b), _T(a[10] & b) }); }
template <> inline A12 bit_and       (const A12 &a, _T b)         { return A12({ _T(a[0] & b), _T(a[1] & b), _T(a[2] & b), _T(a[3] & b), _T(a[4] & b), _T(a[5] & b), _T(a[6] & b), _T(a[7] & b), _T(a[8] & b), _T(a[9] & b), _T(a[10] & b), _T(a[11] & b) }); }
template <> inline A13 bit_and       (const A13 &a, _T b)         { return A13({ _T(a[0] & b), _T(a[1] & b), _T(a[2] & b), _T(a[3] & b), _T(a[4] & b), _T(a[5] & b), _T(a[6] & b), _T(a[7] & b), _T(a[8] & b), _T(a[9] & b), _T(a[10] & b), _T(a[11] & b), _T(a[12] & b) }); }
template <> inline A14 bit_and       (const A14 &a, _T b)         { return A14({ _T(a[0] & b), _T(a[1] & b), _T(a[2] & b), _T(a[3] & b), _T(a[4] & b), _T(a[5] & b), _T(a[6] & b), _T(a[7] & b), _T(a[8] & b), _T(a[9] & b), _T(a[10] & b), _T(a[11] & b), _T(a[12] & b), _T(a[13] & b) }); }
template <> inline A15 bit_and       (const A15 &a, _T b)         { return A15({ _T(a[0] & b), _T(a[1] & b), _T(a[2] & b), _T(a[3] & b), _T(a[4] & b), _T(a[5] & b), _T(a[6] & b), _T(a[7] & b), _T(a[8] & b), _T(a[9] & b), _T(a[10] & b), _T(a[11] & b), _T(a[12] & b), _T(a[13] & b), _T(a[14] & b) }); }
template <> inline A16 bit_and       (const A16 &a, _T b)         { return A16({ _T(a[0] & b), _T(a[1] & b), _T(a[2] & b), _T(a[3] & b), _T(a[4] & b), _T(a[5] & b), _T(a[6] & b), _T(a[7] & b), _T(a[8] & b), _T(a[9] & b), _T(a[10] & b), _T(a[11] & b), _T(a[12] & b), _T(a[13] & b), _T(a[14] & b), _T(a[15] & b) }); }

template <> inline A1  bit_or        (const A1  &a, _T b)         { return A1 ({ _T(a[0] | b) }); }
template <> inline A2  bit_or        (const A2  &a, _T b)         { return A2 ({ _T(a[0] | b), _T(a[1] | b) }); }
template <> inline A3  bit_or        (const A3  &a, _T b)         { return A3 ({ _T(a[0] | b), _T(a[1] | b), _T(a[2] | b) }); }
template <> inline A4  bit_or        (const A4  &a, _T b)         { return A4 ({ _T(a[0] | b), _T(a[1] | b), _T(a[2] | b), _T(a[3] | b) }); }
template <> inline A5  bit_or        (const A5  &a, _T b)         { return A5 ({ _T(a[0] | b), _T(a[1] | b), _T(a[2] | b), _T(a[3] | b), _T(a[4] | b) }); }
template <> inline A6  bit_or        (const A6  &a, _T b)         { return A6 ({ _T(a[0] | b), _T(a[1] | b), _T(a[2] | b), _T(a[3] | b), _T(a[4] | b), _T(a[5] | b) }); }
template <> inline A7  bit_or        (const A7  &a, _T b)         { return A7 ({ _T(a[0] | b), _T(a[1] | b), _T(a[2] | b), _T(a[3] | b), _T(a[4] | b), _T(a[5] | b), _T(a[6] | b) }); }
template <> inline A8  bit_or        (const A8  &a, _T b)         { return A8 ({ _T(a[0] | b), _T(a[1] | b), _T(a[2] | b), _T(a[3] | b), _T(a[4] | b), _T(a[5] | b), _T(a[6] | b), _T(a[7] | b) }); }
template <> inline A9  bit_or        (const A9  &a, _T b)         { return A9 ({ _T(a[0] | b), _T(a[1] | b), _T(a[2] | b), _T(a[3] | b), _T(a[4] | b), _T(a[5] | b), _T(a[6] | b), _T(a[7] | b), _T(a[8] | b) }); }
template <> inline A10 bit_or        (const A10 &a, _T b)         { return A10({ _T(a[0] | b), _T(a[1] | b), _T(a[2] | b), _T(a[3] | b), _T(a[4] | b), _T(a[5] | b), _T(a[6] | b), _T(a[7] | b), _T(a[8] | b), _T(a[9] | b) }); }
template <> inline A11 bit_or        (const A11 &a, _T b)         { return A11({ _T(a[0] | b), _T(a[1] | b), _T(a[2] | b), _T(a[3] | b), _T(a[4] | b), _T(a[5] | b), _T(a[6] | b), _T(a[7] | b), _T(a[8] | b), _T(a[9] | b), _T(a[10] | b) }); }
template <> inline A12 bit_or        (const A12 &a, _T b)         { return A12({ _T(a[0] | b), _T(a[1] | b), _T(a[2] | b), _T(a[3] | b), _T(a[4] | b), _T(a[5] | b), _T(a[6] | b), _T(a[7] | b), _T(a[8] | b), _T(a[9] | b), _T(a[10] | b), _T(a[11] | b) }); }
template <> inline A13 bit_or        (const A13 &a, _T b)         { return A13({ _T(a[0] | b), _T(a[1] | b), _T(a[2] | b), _T(a[3] | b), _T(a[4] | b), _T(a[5] | b), _T(a[6] | b), _T(a[7] | b), _T(a[8] | b), _T(a[9] | b), _T(a[10] | b), _T(a[11] | b), _T(a[12] | b) }); }
template <> inline A14 bit_or        (const A14 &a, _T b)         { return A14({ _T(a[0] | b), _T(a[1] | b), _T(a[2] | b), _T(a[3] | b), _T(a[4] | b), _T(a[5] | b), _T(a[6] | b), _T(a[7] | b), _T(a[8] | b), _T(a[9] | b), _T(a[10] | b), _T(a[11] | b), _T(a[12] | b), _T(a[13] | b) }); }
template <> inline A15 bit_or        (const A15 &a, _T b)         { return A15({ _T(a[0] | b), _T(a[1] | b), _T(a[2] | b), _T(a[3] | b), _T(a[4] | b), _T(a[5] | b), _T(a[6] | b), _T(a[7] | b), _T(a[8] | b), _T(a[9] | b), _T(a[10] | b), _T(a[11] | b), _T(a[12] | b), _T(a[13] | b), _T(a[14] | b) }); }
template <> inline A16 bit_or        (const A16 &a, _T b)         { return A16({ _T(a[0] | b), _T(a[1] | b), _T(a[2] | b), _T(a[3] | b), _T(a[4] | b), _T(a[5] | b), _T(a[6] | b), _T(a[7] | b), _T(a[8] | b), _T(a[9] | b), _T(a[10] | b), _T(a[11] | b), _T(a[12] | b), _T(a[13] | b), _T(a[14] | b), _T(a[15] | b) }); }

template <> inline A1  bit_xor       (const A1  &a, _T b)         { return A1 ({ _T(a[0] ^ b) }); }
template <> inline A2  bit_xor       (const A2  &a, _T b)         { return A2 ({ _T(a[0] ^ b), _T(a[1] ^ b) }); }
template <> inline A3  bit_xor       (const A3  &a, _T b)         { return A3 ({ _T(a[0] ^ b), _T(a[1] ^ b), _T(a[2] ^ b) }); }
template <> inline A4  bit_xor       (const A4  &a, _T b)         { return A4 ({ _T(a[0] ^ b), _T(a[1] ^ b), _T(a[2] ^ b), _T(a[3] ^ b) }); }
template <> inline A5  bit_xor       (const A5  &a, _T b)         { return A5 ({ _T(a[0] ^ b), _T(a[1] ^ b), _T(a[2] ^ b), _T(a[3] ^ b), _T(a[4] ^ b) }); }
template <> inline A6  bit_xor       (const A6  &a, _T b)         { return A6 ({ _T(a[0] ^ b), _T(a[1] ^ b), _T(a[2] ^ b), _T(a[3] ^ b), _T(a[4] ^ b), _T(a[5] ^ b) }); }
template <> inline A7  bit_xor       (const A7  &a, _T b)         { return A7 ({ _T(a[0] ^ b), _T(a[1] ^ b), _T(a[2] ^ b), _T(a[3] ^ b), _T(a[4] ^ b), _T(a[5] ^ b), _T(a[6] ^ b) }); }
template <> inline A8  bit_xor       (const A8  &a, _T b)         { return A8 ({ _T(a[0] ^ b), _T(a[1] ^ b), _T(a[2] ^ b), _T(a[3] ^ b), _T(a[4] ^ b), _T(a[5] ^ b), _T(a[6] ^ b), _T(a[7] ^ b) }); }
template <> inline A9  bit_xor       (const A9  &a, _T b)         { return A9 ({ _T(a[0] ^ b), _T(a[1] ^ b), _T(a[2] ^ b), _T(a[3] ^ b), _T(a[4] ^ b), _T(a[5] ^ b), _T(a[6] ^ b), _T(a[7] ^ b), _T(a[8] ^ b) }); }
template <> inline A10 bit_xor       (const A10 &a, _T b)         { return A10({ _T(a[0] ^ b), _T(a[1] ^ b), _T(a[2] ^ b), _T(a[3] ^ b), _T(a[4] ^ b), _T(a[5] ^ b), _T(a[6] ^ b), _T(a[7] ^ b), _T(a[8] ^ b), _T(a[9] ^ b) }); }
template <> inline A11 bit_xor       (const A11 &a, _T b)         { return A11({ _T(a[0] ^ b), _T(a[1] ^ b), _T(a[2] ^ b), _T(a[3] ^ b), _T(a[4] ^ b), _T(a[5] ^ b), _T(a[6] ^ b), _T(a[7] ^ b), _T(a[8] ^ b), _T(a[9] ^ b), _T(a[10] ^ b) }); }
template <> inline A12 bit_xor       (const A12 &a, _T b)         { return A12({ _T(a[0] ^ b), _T(a[1] ^ b), _T(a[2] ^ b), _T(a[3] ^ b), _T(a[4] ^ b), _T(a[5] ^ b), _T(a[6] ^ b), _T(a[7] ^ b), _T(a[8] ^ b), _T(a[9] ^ b), _T(a[10] ^ b), _T(a[11] ^ b) }); }
template <> inline A13 bit_xor       (const A13 &a, _T b)         { return A13({ _T(a[0] ^ b), _T(a[1] ^ b), _T(a[2] ^ b), _T(a[3] ^ b), _T(a[4] ^ b), _T(a[5] ^ b), _T(a[6] ^ b), _T(a[7] ^ b), _T(a[8] ^ b), _T(a[9] ^ b), _T(a[10] ^ b), _T(a[11] ^ b), _T(a[12] ^ b) }); }
template <> inline A14 bit_xor       (const A14 &a, _T b)         { return A14({ _T(a[0] ^ b), _T(a[1] ^ b), _T(a[2] ^ b), _T(a[3] ^ b), _T(a[4] ^ b), _T(a[5] ^ b), _T(a[6] ^ b), _T(a[7] ^ b), _T(a[8] ^ b), _T(a[9] ^ b), _T(a[10] ^ b), _T(a[11] ^ b), _T(a[12] ^ b), _T(a[13] ^ b) }); }
template <> inline A15 bit_xor       (const A15 &a, _T b)         { return A15({ _T(a[0] ^ b), _T(a[1] ^ b), _T(a[2] ^ b), _T(a[3] ^ b), _T(a[4] ^ b), _T(a[5] ^ b), _T(a[6] ^ b), _T(a[7] ^ b), _T(a[8] ^ b), _T(a[9] ^ b), _T(a[10] ^ b), _T(a[11] ^ b), _T(a[12] ^ b), _T(a[13] ^ b), _T(a[14] ^ b) }); }
template <> inline A16 bit_xor       (const A16 &a, _T b)         { return A16({ _T(a[0] ^ b), _T(a[1] ^ b), _T(a[2] ^ b), _T(a[3] ^ b), _T(a[4] ^ b), _T(a[5] ^ b), _T(a[6] ^ b), _T(a[7] ^ b), _T(a[8] ^ b), _T(a[9] ^ b), _T(a[10] ^ b), _T(a[11] ^ b), _T(a[12] ^ b), _T(a[13] ^ b), _T(a[14] ^ b), _T(a[15] ^ b) }); }
#endif

template <> inline A1  pow           (_T a, const A1  &b)         { return A1 ({ _T(std::pow(a, b[0])) }); }
template <> inline A2  pow           (_T a, const A2  &b)         { return A2 ({ _T(std::pow(a, b[0])), _T(std::pow(a, b[1])) }); }
template <> inline A3  pow           (_T a, const A3  &b)         { return A3 ({ _T(std::pow(a, b[0])), _T(std::pow(a, b[1])), _T(std::pow(a, b[2])) }); }
template <> inline A4  pow           (_T a, const A4  &b)         { return A4 ({ _T(std::pow(a, b[0])), _T(std::pow(a, b[1])), _T(std::pow(a, b[2])), _T(std::pow(a, b[3])) }); }
template <> inline A5  pow           (_T a, const A5  &b)         { return A5 ({ _T(std::pow(a, b[0])), _T(std::pow(a, b[1])), _T(std::pow(a, b[2])), _T(std::pow(a, b[3])), _T(std::pow(a, b[4])) }); }
template <> inline A6  pow           (_T a, const A6  &b)         { return A6 ({ _T(std::pow(a, b[0])), _T(std::pow(a, b[1])), _T(std::pow(a, b[2])), _T(std::pow(a, b[3])), _T(std::pow(a, b[4])), _T(std::pow(a, b[5])) }); }
template <> inline A7  pow           (_T a, const A7  &b)         { return A7 ({ _T(std::pow(a, b[0])), _T(std::pow(a, b[1])), _T(std::pow(a, b[2])), _T(std::pow(a, b[3])), _T(std::pow(a, b[4])), _T(std::pow(a, b[5])), _T(std::pow(a, b[6])) }); }
template <> inline A8  pow           (_T a, const A8  &b)         { return A8 ({ _T(std::pow(a, b[0])), _T(std::pow(a, b[1])), _T(std::pow(a, b[2])), _T(std::pow(a, b[3])), _T(std::pow(a, b[4])), _T(std::pow(a, b[5])), _T(std::pow(a, b[6])), _T(std::pow(a, b[7])) }); }
template <> inline A9  pow           (_T a, const A9  &b)         { return A9 ({ _T(std::pow(a, b[0])), _T(std::pow(a, b[1])), _T(std::pow(a, b[2])), _T(std::pow(a, b[3])), _T(std::pow(a, b[4])), _T(std::pow(a, b[5])), _T(std::pow(a, b[6])), _T(std::pow(a, b[7])), _T(std::pow(a, b[8])) }); }
template <> inline A10 pow           (_T a, const A10 &b)         { return A10({ _T(std::pow(a, b[0])), _T(std::pow(a, b[1])), _T(std::pow(a, b[2])), _T(std::pow(a, b[3])), _T(std::pow(a, b[4])), _T(std::pow(a, b[5])), _T(std::pow(a, b[6])), _T(std::pow(a, b[7])), _T(std::pow(a, b[8])), _T(std::pow(a, b[9])) }); }
template <> inline A11 pow           (_T a, const A11 &b)         { return A11({ _T(std::pow(a, b[0])), _T(std::pow(a, b[1])), _T(std::pow(a, b[2])), _T(std::pow(a, b[3])), _T(std::pow(a, b[4])), _T(std::pow(a, b[5])), _T(std::pow(a, b[6])), _T(std::pow(a, b[7])), _T(std::pow(a, b[8])), _T(std::pow(a, b[9])), _T(std::pow(a, b[10])) }); }
template <> inline A12 pow           (_T a, const A12 &b)         { return A12({ _T(std::pow(a, b[0])), _T(std::pow(a, b[1])), _T(std::pow(a, b[2])), _T(std::pow(a, b[3])), _T(std::pow(a, b[4])), _T(std::pow(a, b[5])), _T(std::pow(a, b[6])), _T(std::pow(a, b[7])), _T(std::pow(a, b[8])), _T(std::pow(a, b[9])), _T(std::pow(a, b[10])), _T(std::pow(a, b[11])) }); }
template <> inline A13 pow           (_T a, const A13 &b)         { return A13({ _T(std::pow(a, b[0])), _T(std::pow(a, b[1])), _T(std::pow(a, b[2])), _T(std::pow(a, b[3])), _T(std::pow(a, b[4])), _T(std::pow(a, b[5])), _T(std::pow(a, b[6])), _T(std::pow(a, b[7])), _T(std::pow(a, b[8])), _T(std::pow(a, b[9])), _T(std::pow(a, b[10])), _T(std::pow(a, b[11])), _T(std::pow(a, b[12])) }); }
template <> inline A14 pow           (_T a, const A14 &b)         { return A14({ _T(std::pow(a, b[0])), _T(std::pow(a, b[1])), _T(std::pow(a, b[2])), _T(std::pow(a, b[3])), _T(std::pow(a, b[4])), _T(std::pow(a, b[5])), _T(std::pow(a, b[6])), _T(std::pow(a, b[7])), _T(std::pow(a, b[8])), _T(std::pow(a, b[9])), _T(std::pow(a, b[10])), _T(std::pow(a, b[11])), _T(std::pow(a, b[12])), _T(std::pow(a, b[13])) }); }
template <> inline A15 pow           (_T a, const A15 &b)         { return A15({ _T(std::pow(a, b[0])), _T(std::pow(a, b[1])), _T(std::pow(a, b[2])), _T(std::pow(a, b[3])), _T(std::pow(a, b[4])), _T(std::pow(a, b[5])), _T(std::pow(a, b[6])), _T(std::pow(a, b[7])), _T(std::pow(a, b[8])), _T(std::pow(a, b[9])), _T(std::pow(a, b[10])), _T(std::pow(a, b[11])), _T(std::pow(a, b[12])), _T(std::pow(a, b[13])), _T(std::pow(a, b[14])) }); }
template <> inline A16 pow           (_T a, const A16 &b)         { return A16({ _T(std::pow(a, b[0])), _T(std::pow(a, b[1])), _T(std::pow(a, b[2])), _T(std::pow(a, b[3])), _T(std::pow(a, b[4])), _T(std::pow(a, b[5])), _T(std::pow(a, b[6])), _T(std::pow(a, b[7])), _T(std::pow(a, b[8])), _T(std::pow(a, b[9])), _T(std::pow(a, b[10])), _T(std::pow(a, b[11])), _T(std::pow(a, b[12])), _T(std::pow(a, b[13])), _T(std::pow(a, b[14])), _T(std::pow(a, b[15])) }); }
template <> inline A1  subtract      (_T a, const A1  &b)         { return A1 ({ _T(a - b[0]) }); }
template <> inline A2  subtract      (_T a, const A2  &b)         { return A2 ({ _T(a - b[0]), _T(a - b[1]) }); }
template <> inline A3  subtract      (_T a, const A3  &b)         { return A3 ({ _T(a - b[0]), _T(a - b[1]), _T(a - b[2]) }); }
template <> inline A4  subtract      (_T a, const A4  &b)         { return A4 ({ _T(a - b[0]), _T(a - b[1]), _T(a - b[2]), _T(a - b[3]) }); }
template <> inline A5  subtract      (_T a, const A5  &b)         { return A5 ({ _T(a - b[0]), _T(a - b[1]), _T(a - b[2]), _T(a - b[3]), _T(a - b[4]) }); }
template <> inline A6  subtract      (_T a, const A6  &b)         { return A6 ({ _T(a - b[0]), _T(a - b[1]), _T(a - b[2]), _T(a - b[3]), _T(a - b[4]), _T(a - b[5]) }); }
template <> inline A7  subtract      (_T a, const A7  &b)         { return A7 ({ _T(a - b[0]), _T(a - b[1]), _T(a - b[2]), _T(a - b[3]), _T(a - b[4]), _T(a - b[5]), _T(a - b[6]) }); }
template <> inline A8  subtract      (_T a, const A8  &b)         { return A8 ({ _T(a - b[0]), _T(a - b[1]), _T(a - b[2]), _T(a - b[3]), _T(a - b[4]), _T(a - b[5]), _T(a - b[6]), _T(a - b[7]) }); }
template <> inline A9  subtract      (_T a, const A9  &b)         { return A9 ({ _T(a - b[0]), _T(a - b[1]), _T(a - b[2]), _T(a - b[3]), _T(a - b[4]), _T(a - b[5]), _T(a - b[6]), _T(a - b[7]), _T(a - b[8]) }); }
template <> inline A10 subtract      (_T a, const A10 &b)         { return A10({ _T(a - b[0]), _T(a - b[1]), _T(a - b[2]), _T(a - b[3]), _T(a - b[4]), _T(a - b[5]), _T(a - b[6]), _T(a - b[7]), _T(a - b[8]), _T(a - b[9]) }); }
template <> inline A11 subtract      (_T a, const A11 &b)         { return A11({ _T(a - b[0]), _T(a - b[1]), _T(a - b[2]), _T(a - b[3]), _T(a - b[4]), _T(a - b[5]), _T(a - b[6]), _T(a - b[7]), _T(a - b[8]), _T(a - b[9]), _T(a - b[10]) }); }
template <> inline A12 subtract      (_T a, const A12 &b)         { return A12({ _T(a - b[0]), _T(a - b[1]), _T(a - b[2]), _T(a - b[3]), _T(a - b[4]), _T(a - b[5]), _T(a - b[6]), _T(a - b[7]), _T(a - b[8]), _T(a - b[9]), _T(a - b[10]), _T(a - b[11]) }); }
template <> inline A13 subtract      (_T a, const A13 &b)         { return A13({ _T(a - b[0]), _T(a - b[1]), _T(a - b[2]), _T(a - b[3]), _T(a - b[4]), _T(a - b[5]), _T(a - b[6]), _T(a - b[7]), _T(a - b[8]), _T(a - b[9]), _T(a - b[10]), _T(a - b[11]), _T(a - b[12]) }); }
template <> inline A14 subtract      (_T a, const A14 &b)         { return A14({ _T(a - b[0]), _T(a - b[1]), _T(a - b[2]), _T(a - b[3]), _T(a - b[4]), _T(a - b[5]), _T(a - b[6]), _T(a - b[7]), _T(a - b[8]), _T(a - b[9]), _T(a - b[10]), _T(a - b[11]), _T(a - b[12]), _T(a - b[13]) }); }
template <> inline A15 subtract      (_T a, const A15 &b)         { return A15({ _T(a - b[0]), _T(a - b[1]), _T(a - b[2]), _T(a - b[3]), _T(a - b[4]), _T(a - b[5]), _T(a - b[6]), _T(a - b[7]), _T(a - b[8]), _T(a - b[9]), _T(a - b[10]), _T(a - b[11]), _T(a - b[12]), _T(a - b[13]), _T(a - b[14]) }); }
template <> inline A16 subtract      (_T a, const A16 &b)         { return A16({ _T(a - b[0]), _T(a - b[1]), _T(a - b[2]), _T(a - b[3]), _T(a - b[4]), _T(a - b[5]), _T(a - b[6]), _T(a - b[7]), _T(a - b[8]), _T(a - b[9]), _T(a - b[10]), _T(a - b[11]), _T(a - b[12]), _T(a - b[13]), _T(a - b[14]), _T(a - b[15]) }); }

template <> inline A1  add           (_T a, const A1  &b)         { return A1 ({ _T(a + b[0]) }); }
template <> inline A2  add           (_T a, const A2  &b)         { return A2 ({ _T(a + b[0]), _T(a + b[1]) }); }
template <> inline A3  add           (_T a, const A3  &b)         { return A3 ({ _T(a + b[0]), _T(a + b[1]), _T(a + b[2]) }); }
template <> inline A4  add           (_T a, const A4  &b)         { return A4 ({ _T(a + b[0]), _T(a + b[1]), _T(a + b[2]), _T(a + b[3]) }); }
template <> inline A5  add           (_T a, const A5  &b)         { return A5 ({ _T(a + b[0]), _T(a + b[1]), _T(a + b[2]), _T(a + b[3]), _T(a + b[4]) }); }
template <> inline A6  add           (_T a, const A6  &b)         { return A6 ({ _T(a + b[0]), _T(a + b[1]), _T(a + b[2]), _T(a + b[3]), _T(a + b[4]), _T(a + b[5]) }); }
template <> inline A7  add           (_T a, const A7  &b)         { return A7 ({ _T(a + b[0]), _T(a + b[1]), _T(a + b[2]), _T(a + b[3]), _T(a + b[4]), _T(a + b[5]), _T(a + b[6]) }); }
template <> inline A8  add           (_T a, const A8  &b)         { return A8 ({ _T(a + b[0]), _T(a + b[1]), _T(a + b[2]), _T(a + b[3]), _T(a + b[4]), _T(a + b[5]), _T(a + b[6]), _T(a + b[7]) }); }
template <> inline A9  add           (_T a, const A9  &b)         { return A9 ({ _T(a + b[0]), _T(a + b[1]), _T(a + b[2]), _T(a + b[3]), _T(a + b[4]), _T(a + b[5]), _T(a + b[6]), _T(a + b[7]), _T(a + b[8]) }); }
template <> inline A10 add           (_T a, const A10 &b)         { return A10({ _T(a + b[0]), _T(a + b[1]), _T(a + b[2]), _T(a + b[3]), _T(a + b[4]), _T(a + b[5]), _T(a + b[6]), _T(a + b[7]), _T(a + b[8]), _T(a + b[9]) }); }
template <> inline A11 add           (_T a, const A11 &b)         { return A11({ _T(a + b[0]), _T(a + b[1]), _T(a + b[2]), _T(a + b[3]), _T(a + b[4]), _T(a + b[5]), _T(a + b[6]), _T(a + b[7]), _T(a + b[8]), _T(a + b[9]), _T(a + b[10]) }); }
template <> inline A12 add           (_T a, const A12 &b)         { return A12({ _T(a + b[0]), _T(a + b[1]), _T(a + b[2]), _T(a + b[3]), _T(a + b[4]), _T(a + b[5]), _T(a + b[6]), _T(a + b[7]), _T(a + b[8]), _T(a + b[9]), _T(a + b[10]), _T(a + b[11]) }); }
template <> inline A13 add           (_T a, const A13 &b)         { return A13({ _T(a + b[0]), _T(a + b[1]), _T(a + b[2]), _T(a + b[3]), _T(a + b[4]), _T(a + b[5]), _T(a + b[6]), _T(a + b[7]), _T(a + b[8]), _T(a + b[9]), _T(a + b[10]), _T(a + b[11]), _T(a + b[12]) }); }
template <> inline A14 add           (_T a, const A14 &b)         { return A14({ _T(a + b[0]), _T(a + b[1]), _T(a + b[2]), _T(a + b[3]), _T(a + b[4]), _T(a + b[5]), _T(a + b[6]), _T(a + b[7]), _T(a + b[8]), _T(a + b[9]), _T(a + b[10]), _T(a + b[11]), _T(a + b[12]), _T(a + b[13]) }); }
template <> inline A15 add           (_T a, const A15 &b)         { return A15({ _T(a + b[0]), _T(a + b[1]), _T(a + b[2]), _T(a + b[3]), _T(a + b[4]), _T(a + b[5]), _T(a + b[6]), _T(a + b[7]), _T(a + b[8]), _T(a + b[9]), _T(a + b[10]), _T(a + b[11]), _T(a + b[12]), _T(a + b[13]), _T(a + b[14]) }); }
template <> inline A16 add           (_T a, const A16 &b)         { return A16({ _T(a + b[0]), _T(a + b[1]), _T(a + b[2]), _T(a + b[3]), _T(a + b[4]), _T(a + b[5]), _T(a + b[6]), _T(a + b[7]), _T(a + b[8]), _T(a + b[9]), _T(a + b[10]), _T(a + b[11]), _T(a + b[12]), _T(a + b[13]), _T(a + b[14]), _T(a + b[15]) }); }

template <> inline A1  multiply      (_T a, const A1  &b)         { return A1 ({ _T(a * b[0]) }); }
template <> inline A2  multiply      (_T a, const A2  &b)         { return A2 ({ _T(a * b[0]), _T(a * b[1]) }); }
template <> inline A3  multiply      (_T a, const A3  &b)         { return A3 ({ _T(a * b[0]), _T(a * b[1]), _T(a * b[2]) }); }
template <> inline A4  multiply      (_T a, const A4  &b)         { return A4 ({ _T(a * b[0]), _T(a * b[1]), _T(a * b[2]), _T(a * b[3]) }); }
template <> inline A5  multiply      (_T a, const A5  &b)         { return A5 ({ _T(a * b[0]), _T(a * b[1]), _T(a * b[2]), _T(a * b[3]), _T(a * b[4]) }); }
template <> inline A6  multiply      (_T a, const A6  &b)         { return A6 ({ _T(a * b[0]), _T(a * b[1]), _T(a * b[2]), _T(a * b[3]), _T(a * b[4]), _T(a * b[5]) }); }
template <> inline A7  multiply      (_T a, const A7  &b)         { return A7 ({ _T(a * b[0]), _T(a * b[1]), _T(a * b[2]), _T(a * b[3]), _T(a * b[4]), _T(a * b[5]), _T(a * b[6]) }); }
template <> inline A8  multiply      (_T a, const A8  &b)         { return A8 ({ _T(a * b[0]), _T(a * b[1]), _T(a * b[2]), _T(a * b[3]), _T(a * b[4]), _T(a * b[5]), _T(a * b[6]), _T(a * b[7]) }); }
template <> inline A9  multiply      (_T a, const A9  &b)         { return A9 ({ _T(a * b[0]), _T(a * b[1]), _T(a * b[2]), _T(a * b[3]), _T(a * b[4]), _T(a * b[5]), _T(a * b[6]), _T(a * b[7]), _T(a * b[8]) }); }
template <> inline A10 multiply      (_T a, const A10 &b)         { return A10({ _T(a * b[0]), _T(a * b[1]), _T(a * b[2]), _T(a * b[3]), _T(a * b[4]), _T(a * b[5]), _T(a * b[6]), _T(a * b[7]), _T(a * b[8]), _T(a * b[9]) }); }
template <> inline A11 multiply      (_T a, const A11 &b)         { return A11({ _T(a * b[0]), _T(a * b[1]), _T(a * b[2]), _T(a * b[3]), _T(a * b[4]), _T(a * b[5]), _T(a * b[6]), _T(a * b[7]), _T(a * b[8]), _T(a * b[9]), _T(a * b[10]) }); }
template <> inline A12 multiply      (_T a, const A12 &b)         { return A12({ _T(a * b[0]), _T(a * b[1]), _T(a * b[2]), _T(a * b[3]), _T(a * b[4]), _T(a * b[5]), _T(a * b[6]), _T(a * b[7]), _T(a * b[8]), _T(a * b[9]), _T(a * b[10]), _T(a * b[11]) }); }
template <> inline A13 multiply      (_T a, const A13 &b)         { return A13({ _T(a * b[0]), _T(a * b[1]), _T(a * b[2]), _T(a * b[3]), _T(a * b[4]), _T(a * b[5]), _T(a * b[6]), _T(a * b[7]), _T(a * b[8]), _T(a * b[9]), _T(a * b[10]), _T(a * b[11]), _T(a * b[12]) }); }
template <> inline A14 multiply      (_T a, const A14 &b)         { return A14({ _T(a * b[0]), _T(a * b[1]), _T(a * b[2]), _T(a * b[3]), _T(a * b[4]), _T(a * b[5]), _T(a * b[6]), _T(a * b[7]), _T(a * b[8]), _T(a * b[9]), _T(a * b[10]), _T(a * b[11]), _T(a * b[12]), _T(a * b[13]) }); }
template <> inline A15 multiply      (_T a, const A15 &b)         { return A15({ _T(a * b[0]), _T(a * b[1]), _T(a * b[2]), _T(a * b[3]), _T(a * b[4]), _T(a * b[5]), _T(a * b[6]), _T(a * b[7]), _T(a * b[8]), _T(a * b[9]), _T(a * b[10]), _T(a * b[11]), _T(a * b[12]), _T(a * b[13]), _T(a * b[14]) }); }
template <> inline A16 multiply      (_T a, const A16 &b)         { return A16({ _T(a * b[0]), _T(a * b[1]), _T(a * b[2]), _T(a * b[3]), _T(a * b[4]), _T(a * b[5]), _T(a * b[6]), _T(a * b[7]), _T(a * b[8]), _T(a * b[9]), _T(a * b[10]), _T(a * b[11]), _T(a * b[12]), _T(a * b[13]), _T(a * b[14]), _T(a * b[15]) }); }

template <> inline A1  divide        (_T a, const A1  &b)         { return A1 ({ _T(a / b[0]) }); }
template <> inline A2  divide        (_T a, const A2  &b)         { return A2 ({ _T(a / b[0]), _T(a / b[1]) }); }
template <> inline A3  divide        (_T a, const A3  &b)         { return A3 ({ _T(a / b[0]), _T(a / b[1]), _T(a / b[2]) }); }
template <> inline A4  divide        (_T a, const A4  &b)         { return A4 ({ _T(a / b[0]), _T(a / b[1]), _T(a / b[2]), _T(a / b[3]) }); }
template <> inline A5  divide        (_T a, const A5  &b)         { return A5 ({ _T(a / b[0]), _T(a / b[1]), _T(a / b[2]), _T(a / b[3]), _T(a / b[4]) }); }
template <> inline A6  divide        (_T a, const A6  &b)         { return A6 ({ _T(a / b[0]), _T(a / b[1]), _T(a / b[2]), _T(a / b[3]), _T(a / b[4]), _T(a / b[5]) }); }
template <> inline A7  divide        (_T a, const A7  &b)         { return A7 ({ _T(a / b[0]), _T(a / b[1]), _T(a / b[2]), _T(a / b[3]), _T(a / b[4]), _T(a / b[5]), _T(a / b[6]) }); }
template <> inline A8  divide        (_T a, const A8  &b)         { return A8 ({ _T(a / b[0]), _T(a / b[1]), _T(a / b[2]), _T(a / b[3]), _T(a / b[4]), _T(a / b[5]), _T(a / b[6]), _T(a / b[7]) }); }
template <> inline A9  divide        (_T a, const A9  &b)         { return A9 ({ _T(a / b[0]), _T(a / b[1]), _T(a / b[2]), _T(a / b[3]), _T(a / b[4]), _T(a / b[5]), _T(a / b[6]), _T(a / b[7]), _T(a / b[8]) }); }
template <> inline A10 divide        (_T a, const A10 &b)         { return A10({ _T(a / b[0]), _T(a / b[1]), _T(a / b[2]), _T(a / b[3]), _T(a / b[4]), _T(a / b[5]), _T(a / b[6]), _T(a / b[7]), _T(a / b[8]), _T(a / b[9]) }); }
template <> inline A11 divide        (_T a, const A11 &b)         { return A11({ _T(a / b[0]), _T(a / b[1]), _T(a / b[2]), _T(a / b[3]), _T(a / b[4]), _T(a / b[5]), _T(a / b[6]), _T(a / b[7]), _T(a / b[8]), _T(a / b[9]), _T(a / b[10]) }); }
template <> inline A12 divide        (_T a, const A12 &b)         { return A12({ _T(a / b[0]), _T(a / b[1]), _T(a / b[2]), _T(a / b[3]), _T(a / b[4]), _T(a / b[5]), _T(a / b[6]), _T(a / b[7]), _T(a / b[8]), _T(a / b[9]), _T(a / b[10]), _T(a / b[11]) }); }
template <> inline A13 divide        (_T a, const A13 &b)         { return A13({ _T(a / b[0]), _T(a / b[1]), _T(a / b[2]), _T(a / b[3]), _T(a / b[4]), _T(a / b[5]), _T(a / b[6]), _T(a / b[7]), _T(a / b[8]), _T(a / b[9]), _T(a / b[10]), _T(a / b[11]), _T(a / b[12]) }); }
template <> inline A14 divide        (_T a, const A14 &b)         { return A14({ _T(a / b[0]), _T(a / b[1]), _T(a / b[2]), _T(a / b[3]), _T(a / b[4]), _T(a / b[5]), _T(a / b[6]), _T(a / b[7]), _T(a / b[8]), _T(a / b[9]), _T(a / b[10]), _T(a / b[11]), _T(a / b[12]), _T(a / b[13]) }); }
template <> inline A15 divide        (_T a, const A15 &b)         { return A15({ _T(a / b[0]), _T(a / b[1]), _T(a / b[2]), _T(a / b[3]), _T(a / b[4]), _T(a / b[5]), _T(a / b[6]), _T(a / b[7]), _T(a / b[8]), _T(a / b[9]), _T(a / b[10]), _T(a / b[11]), _T(a / b[12]), _T(a / b[13]), _T(a / b[14]) }); }
template <> inline A16 divide        (_T a, const A16 &b)         { return A16({ _T(a / b[0]), _T(a / b[1]), _T(a / b[2]), _T(a / b[3]), _T(a / b[4]), _T(a / b[5]), _T(a / b[6]), _T(a / b[7]), _T(a / b[8]), _T(a / b[9]), _T(a / b[10]), _T(a / b[11]), _T(a / b[12]), _T(a / b[13]), _T(a / b[14]), _T(a / b[15]) }); }

#if _T_INT == 1
template <> inline A1  modulus       (_T a, const A1  &b)         { return A1 ({ _T(a % b[0]) }); }
template <> inline A2  modulus       (_T a, const A2  &b)         { return A2 ({ _T(a % b[0]), _T(a % b[1]) }); }
template <> inline A3  modulus       (_T a, const A3  &b)         { return A3 ({ _T(a % b[0]), _T(a % b[1]), _T(a % b[2]) }); }
template <> inline A4  modulus       (_T a, const A4  &b)         { return A4 ({ _T(a % b[0]), _T(a % b[1]), _T(a % b[2]), _T(a % b[3]) }); }
template <> inline A5  modulus       (_T a, const A5  &b)         { return A5 ({ _T(a % b[0]), _T(a % b[1]), _T(a % b[2]), _T(a % b[3]), _T(a % b[4]) }); }
template <> inline A6  modulus       (_T a, const A6  &b)         { return A6 ({ _T(a % b[0]), _T(a % b[1]), _T(a % b[2]), _T(a % b[3]), _T(a % b[4]), _T(a % b[5]) }); }
template <> inline A7  modulus       (_T a, const A7  &b)         { return A7 ({ _T(a % b[0]), _T(a % b[1]), _T(a % b[2]), _T(a % b[3]), _T(a % b[4]), _T(a % b[5]), _T(a % b[6]) }); }
template <> inline A8  modulus       (_T a, const A8  &b)         { return A8 ({ _T(a % b[0]), _T(a % b[1]), _T(a % b[2]), _T(a % b[3]), _T(a % b[4]), _T(a % b[5]), _T(a % b[6]), _T(a % b[7]) }); }
template <> inline A9  modulus       (_T a, const A9  &b)         { return A9 ({ _T(a % b[0]), _T(a % b[1]), _T(a % b[2]), _T(a % b[3]), _T(a % b[4]), _T(a % b[5]), _T(a % b[6]), _T(a % b[7]), _T(a % b[8]) }); }
template <> inline A10 modulus       (_T a, const A10 &b)         { return A10({ _T(a % b[0]), _T(a % b[1]), _T(a % b[2]), _T(a % b[3]), _T(a % b[4]), _T(a % b[5]), _T(a % b[6]), _T(a % b[7]), _T(a % b[8]), _T(a % b[9]) }); }
template <> inline A11 modulus       (_T a, const A11 &b)         { return A11({ _T(a % b[0]), _T(a % b[1]), _T(a % b[2]), _T(a % b[3]), _T(a % b[4]), _T(a % b[5]), _T(a % b[6]), _T(a % b[7]), _T(a % b[8]), _T(a % b[9]), _T(a % b[10]) }); }
template <> inline A12 modulus       (_T a, const A12 &b)         { return A12({ _T(a % b[0]), _T(a % b[1]), _T(a % b[2]), _T(a % b[3]), _T(a % b[4]), _T(a % b[5]), _T(a % b[6]), _T(a % b[7]), _T(a % b[8]), _T(a % b[9]), _T(a % b[10]), _T(a % b[11]) }); }
template <> inline A13 modulus       (_T a, const A13 &b)         { return A13({ _T(a % b[0]), _T(a % b[1]), _T(a % b[2]), _T(a % b[3]), _T(a % b[4]), _T(a % b[5]), _T(a % b[6]), _T(a % b[7]), _T(a % b[8]), _T(a % b[9]), _T(a % b[10]), _T(a % b[11]), _T(a % b[12]) }); }
template <> inline A14 modulus       (_T a, const A14 &b)         { return A14({ _T(a % b[0]), _T(a % b[1]), _T(a % b[2]), _T(a % b[3]), _T(a % b[4]), _T(a % b[5]), _T(a % b[6]), _T(a % b[7]), _T(a % b[8]), _T(a % b[9]), _T(a % b[10]), _T(a % b[11]), _T(a % b[12]), _T(a % b[13]) }); }
template <> inline A15 modulus       (_T a, const A15 &b)         { return A15({ _T(a % b[0]), _T(a % b[1]), _T(a % b[2]), _T(a % b[3]), _T(a % b[4]), _T(a % b[5]), _T(a % b[6]), _T(a % b[7]), _T(a % b[8]), _T(a % b[9]), _T(a % b[10]), _T(a % b[11]), _T(a % b[12]), _T(a % b[13]), _T(a % b[14]) }); }
template <> inline A16 modulus       (_T a, const A16 &b)         { return A16({ _T(a % b[0]), _T(a % b[1]), _T(a % b[2]), _T(a % b[3]), _T(a % b[4]), _T(a % b[5]), _T(a % b[6]), _T(a % b[7]), _T(a % b[8]), _T(a % b[9]), _T(a % b[10]), _T(a % b[11]), _T(a % b[12]), _T(a % b[13]), _T(a % b[14]), _T(a % b[15]) }); }

template <> inline A1  bit_and       (_T a, const A1  &b)         { return A1 ({ _T(a & b[0]) }); }
template <> inline A2  bit_and       (_T a, const A2  &b)         { return A2 ({ _T(a & b[0]), _T(a & b[1]) }); }
template <> inline A3  bit_and       (_T a, const A3  &b)         { return A3 ({ _T(a & b[0]), _T(a & b[1]), _T(a & b[2]) }); }
template <> inline A4  bit_and       (_T a, const A4  &b)         { return A4 ({ _T(a & b[0]), _T(a & b[1]), _T(a & b[2]), _T(a & b[3]) }); }
template <> inline A5  bit_and       (_T a, const A5  &b)         { return A5 ({ _T(a & b[0]), _T(a & b[1]), _T(a & b[2]), _T(a & b[3]), _T(a & b[4]) }); }
template <> inline A6  bit_and       (_T a, const A6  &b)         { return A6 ({ _T(a & b[0]), _T(a & b[1]), _T(a & b[2]), _T(a & b[3]), _T(a & b[4]), _T(a & b[5]) }); }
template <> inline A7  bit_and       (_T a, const A7  &b)         { return A7 ({ _T(a & b[0]), _T(a & b[1]), _T(a & b[2]), _T(a & b[3]), _T(a & b[4]), _T(a & b[5]), _T(a & b[6]) }); }
template <> inline A8  bit_and       (_T a, const A8  &b)         { return A8 ({ _T(a & b[0]), _T(a & b[1]), _T(a & b[2]), _T(a & b[3]), _T(a & b[4]), _T(a & b[5]), _T(a & b[6]), _T(a & b[7]) }); }
template <> inline A9  bit_and       (_T a, const A9  &b)         { return A9 ({ _T(a & b[0]), _T(a & b[1]), _T(a & b[2]), _T(a & b[3]), _T(a & b[4]), _T(a & b[5]), _T(a & b[6]), _T(a & b[7]), _T(a & b[8]) }); }
template <> inline A10 bit_and       (_T a, const A10 &b)         { return A10({ _T(a & b[0]), _T(a & b[1]), _T(a & b[2]), _T(a & b[3]), _T(a & b[4]), _T(a & b[5]), _T(a & b[6]), _T(a & b[7]), _T(a & b[8]), _T(a & b[9]) }); }
template <> inline A11 bit_and       (_T a, const A11 &b)         { return A11({ _T(a & b[0]), _T(a & b[1]), _T(a & b[2]), _T(a & b[3]), _T(a & b[4]), _T(a & b[5]), _T(a & b[6]), _T(a & b[7]), _T(a & b[8]), _T(a & b[9]), _T(a & b[10]) }); }
template <> inline A12 bit_and       (_T a, const A12 &b)         { return A12({ _T(a & b[0]), _T(a & b[1]), _T(a & b[2]), _T(a & b[3]), _T(a & b[4]), _T(a & b[5]), _T(a & b[6]), _T(a & b[7]), _T(a & b[8]), _T(a & b[9]), _T(a & b[10]), _T(a & b[11]) }); }
template <> inline A13 bit_and       (_T a, const A13 &b)         { return A13({ _T(a & b[0]), _T(a & b[1]), _T(a & b[2]), _T(a & b[3]), _T(a & b[4]), _T(a & b[5]), _T(a & b[6]), _T(a & b[7]), _T(a & b[8]), _T(a & b[9]), _T(a & b[10]), _T(a & b[11]), _T(a & b[12]) }); }
template <> inline A14 bit_and       (_T a, const A14 &b)         { return A14({ _T(a & b[0]), _T(a & b[1]), _T(a & b[2]), _T(a & b[3]), _T(a & b[4]), _T(a & b[5]), _T(a & b[6]), _T(a & b[7]), _T(a & b[8]), _T(a & b[9]), _T(a & b[10]), _T(a & b[11]), _T(a & b[12]), _T(a & b[13]) }); }
template <> inline A15 bit_and       (_T a, const A15 &b)         { return A15({ _T(a & b[0]), _T(a & b[1]), _T(a & b[2]), _T(a & b[3]), _T(a & b[4]), _T(a & b[5]), _T(a & b[6]), _T(a & b[7]), _T(a & b[8]), _T(a & b[9]), _T(a & b[10]), _T(a & b[11]), _T(a & b[12]), _T(a & b[13]), _T(a & b[14]) }); }
template <> inline A16 bit_and       (_T a, const A16 &b)         { return A16({ _T(a & b[0]), _T(a & b[1]), _T(a & b[2]), _T(a & b[3]), _T(a & b[4]), _T(a & b[5]), _T(a & b[6]), _T(a & b[7]), _T(a & b[8]), _T(a & b[9]), _T(a & b[10]), _T(a & b[11]), _T(a & b[12]), _T(a & b[13]), _T(a & b[14]), _T(a & b[15]) }); }

template <> inline A1  bit_or        (_T a, const A1  &b)         { return A1 ({ _T(a | b[0]) }); }
template <> inline A2  bit_or        (_T a, const A2  &b)         { return A2 ({ _T(a | b[0]), _T(a | b[1]) }); }
template <> inline A3  bit_or        (_T a, const A3  &b)         { return A3 ({ _T(a | b[0]), _T(a | b[1]), _T(a | b[2]) }); }
template <> inline A4  bit_or        (_T a, const A4  &b)         { return A4 ({ _T(a | b[0]), _T(a | b[1]), _T(a | b[2]), _T(a | b[3]) }); }
template <> inline A5  bit_or        (_T a, const A5  &b)         { return A5 ({ _T(a | b[0]), _T(a | b[1]), _T(a | b[2]), _T(a | b[3]), _T(a | b[4]) }); }
template <> inline A6  bit_or        (_T a, const A6  &b)         { return A6 ({ _T(a | b[0]), _T(a | b[1]), _T(a | b[2]), _T(a | b[3]), _T(a | b[4]), _T(a | b[5]) }); }
template <> inline A7  bit_or        (_T a, const A7  &b)         { return A7 ({ _T(a | b[0]), _T(a | b[1]), _T(a | b[2]), _T(a | b[3]), _T(a | b[4]), _T(a | b[5]), _T(a | b[6]) }); }
template <> inline A8  bit_or        (_T a, const A8  &b)         { return A8 ({ _T(a | b[0]), _T(a | b[1]), _T(a | b[2]), _T(a | b[3]), _T(a | b[4]), _T(a | b[5]), _T(a | b[6]), _T(a | b[7]) }); }
template <> inline A9  bit_or        (_T a, const A9  &b)         { return A9 ({ _T(a | b[0]), _T(a | b[1]), _T(a | b[2]), _T(a | b[3]), _T(a | b[4]), _T(a | b[5]), _T(a | b[6]), _T(a | b[7]), _T(a | b[8]) }); }
template <> inline A10 bit_or        (_T a, const A10 &b)         { return A10({ _T(a | b[0]), _T(a | b[1]), _T(a | b[2]), _T(a | b[3]), _T(a | b[4]), _T(a | b[5]), _T(a | b[6]), _T(a | b[7]), _T(a | b[8]), _T(a | b[9]) }); }
template <> inline A11 bit_or        (_T a, const A11 &b)         { return A11({ _T(a | b[0]), _T(a | b[1]), _T(a | b[2]), _T(a | b[3]), _T(a | b[4]), _T(a | b[5]), _T(a | b[6]), _T(a | b[7]), _T(a | b[8]), _T(a | b[9]), _T(a | b[10]) }); }
template <> inline A12 bit_or        (_T a, const A12 &b)         { return A12({ _T(a | b[0]), _T(a | b[1]), _T(a | b[2]), _T(a | b[3]), _T(a | b[4]), _T(a | b[5]), _T(a | b[6]), _T(a | b[7]), _T(a | b[8]), _T(a | b[9]), _T(a | b[10]), _T(a | b[11]) }); }
template <> inline A13 bit_or        (_T a, const A13 &b)         { return A13({ _T(a | b[0]), _T(a | b[1]), _T(a | b[2]), _T(a | b[3]), _T(a | b[4]), _T(a | b[5]), _T(a | b[6]), _T(a | b[7]), _T(a | b[8]), _T(a | b[9]), _T(a | b[10]), _T(a | b[11]), _T(a | b[12]) }); }
template <> inline A14 bit_or        (_T a, const A14 &b)         { return A14({ _T(a | b[0]), _T(a | b[1]), _T(a | b[2]), _T(a | b[3]), _T(a | b[4]), _T(a | b[5]), _T(a | b[6]), _T(a | b[7]), _T(a | b[8]), _T(a | b[9]), _T(a | b[10]), _T(a | b[11]), _T(a | b[12]), _T(a | b[13]) }); }
template <> inline A15 bit_or        (_T a, const A15 &b)         { return A15({ _T(a | b[0]), _T(a | b[1]), _T(a | b[2]), _T(a | b[3]), _T(a | b[4]), _T(a | b[5]), _T(a | b[6]), _T(a | b[7]), _T(a | b[8]), _T(a | b[9]), _T(a | b[10]), _T(a | b[11]), _T(a | b[12]), _T(a | b[13]), _T(a | b[14]) }); }
template <> inline A16 bit_or        (_T a, const A16 &b)         { return A16({ _T(a | b[0]), _T(a | b[1]), _T(a | b[2]), _T(a | b[3]), _T(a | b[4]), _T(a | b[5]), _T(a | b[6]), _T(a | b[7]), _T(a | b[8]), _T(a | b[9]), _T(a | b[10]), _T(a | b[11]), _T(a | b[12]), _T(a | b[13]), _T(a | b[14]), _T(a | b[15]) }); }

template <> inline A1  bit_xor       (_T a, const A1  &b)         { return A1 ({ _T(a ^ b[0]) }); }
template <> inline A2  bit_xor       (_T a, const A2  &b)         { return A2 ({ _T(a ^ b[0]), _T(a ^ b[1]) }); }
template <> inline A3  bit_xor       (_T a, const A3  &b)         { return A3 ({ _T(a ^ b[0]), _T(a ^ b[1]), _T(a ^ b[2]) }); }
template <> inline A4  bit_xor       (_T a, const A4  &b)         { return A4 ({ _T(a ^ b[0]), _T(a ^ b[1]), _T(a ^ b[2]), _T(a ^ b[3]) }); }
template <> inline A5  bit_xor       (_T a, const A5  &b)         { return A5 ({ _T(a ^ b[0]), _T(a ^ b[1]), _T(a ^ b[2]), _T(a ^ b[3]), _T(a ^ b[4]) }); }
template <> inline A6  bit_xor       (_T a, const A6  &b)         { return A6 ({ _T(a ^ b[0]), _T(a ^ b[1]), _T(a ^ b[2]), _T(a ^ b[3]), _T(a ^ b[4]), _T(a ^ b[5]) }); }
template <> inline A7  bit_xor       (_T a, const A7  &b)         { return A7 ({ _T(a ^ b[0]), _T(a ^ b[1]), _T(a ^ b[2]), _T(a ^ b[3]), _T(a ^ b[4]), _T(a ^ b[5]), _T(a ^ b[6]) }); }
template <> inline A8  bit_xor       (_T a, const A8  &b)         { return A8 ({ _T(a ^ b[0]), _T(a ^ b[1]), _T(a ^ b[2]), _T(a ^ b[3]), _T(a ^ b[4]), _T(a ^ b[5]), _T(a ^ b[6]), _T(a ^ b[7]) }); }
template <> inline A9  bit_xor       (_T a, const A9  &b)         { return A9 ({ _T(a ^ b[0]), _T(a ^ b[1]), _T(a ^ b[2]), _T(a ^ b[3]), _T(a ^ b[4]), _T(a ^ b[5]), _T(a ^ b[6]), _T(a ^ b[7]), _T(a ^ b[8]) }); }
template <> inline A10 bit_xor       (_T a, const A10 &b)         { return A10({ _T(a ^ b[0]), _T(a ^ b[1]), _T(a ^ b[2]), _T(a ^ b[3]), _T(a ^ b[4]), _T(a ^ b[5]), _T(a ^ b[6]), _T(a ^ b[7]), _T(a ^ b[8]), _T(a ^ b[9]) }); }
template <> inline A11 bit_xor       (_T a, const A11 &b)         { return A11({ _T(a ^ b[0]), _T(a ^ b[1]), _T(a ^ b[2]), _T(a ^ b[3]), _T(a ^ b[4]), _T(a ^ b[5]), _T(a ^ b[6]), _T(a ^ b[7]), _T(a ^ b[8]), _T(a ^ b[9]), _T(a ^ b[10]) }); }
template <> inline A12 bit_xor       (_T a, const A12 &b)         { return A12({ _T(a ^ b[0]), _T(a ^ b[1]), _T(a ^ b[2]), _T(a ^ b[3]), _T(a ^ b[4]), _T(a ^ b[5]), _T(a ^ b[6]), _T(a ^ b[7]), _T(a ^ b[8]), _T(a ^ b[9]), _T(a ^ b[10]), _T(a ^ b[11]) }); }
template <> inline A13 bit_xor       (_T a, const A13 &b)         { return A13({ _T(a ^ b[0]), _T(a ^ b[1]), _T(a ^ b[2]), _T(a ^ b[3]), _T(a ^ b[4]), _T(a ^ b[5]), _T(a ^ b[6]), _T(a ^ b[7]), _T(a ^ b[8]), _T(a ^ b[9]), _T(a ^ b[10]), _T(a ^ b[11]), _T(a ^ b[12]) }); }
template <> inline A14 bit_xor       (_T a, const A14 &b)         { return A14({ _T(a ^ b[0]), _T(a ^ b[1]), _T(a ^ b[2]), _T(a ^ b[3]), _T(a ^ b[4]), _T(a ^ b[5]), _T(a ^ b[6]), _T(a ^ b[7]), _T(a ^ b[8]), _T(a ^ b[9]), _T(a ^ b[10]), _T(a ^ b[11]), _T(a ^ b[12]), _T(a ^ b[13]) }); }
template <> inline A15 bit_xor       (_T a, const A15 &b)         { return A15({ _T(a ^ b[0]), _T(a ^ b[1]), _T(a ^ b[2]), _T(a ^ b[3]), _T(a ^ b[4]), _T(a ^ b[5]), _T(a ^ b[6]), _T(a ^ b[7]), _T(a ^ b[8]), _T(a ^ b[9]), _T(a ^ b[10]), _T(a ^ b[11]), _T(a ^ b[12]), _T(a ^ b[13]), _T(a ^ b[14]) }); }
template <> inline A16 bit_xor       (_T a, const A16 &b)         { return A16({ _T(a ^ b[0]), _T(a ^ b[1]), _T(a ^ b[2]), _T(a ^ b[3]), _T(a ^ b[4]), _T(a ^ b[5]), _T(a ^ b[6]), _T(a ^ b[7]), _T(a ^ b[8]), _T(a ^ b[9]), _T(a ^ b[10]), _T(a ^ b[11]), _T(a ^ b[12]), _T(a ^ b[13]), _T(a ^ b[14]), _T(a ^ b[15]) }); }

template <> inline A1  bit_shl       (const A1  &a, size_t b)     { return A1 ({ _T(a[0] << b) }); }
template <> inline A2  bit_shl       (const A2  &a, size_t b)     { return A2 ({ _T(a[0] << b), _T(a[1] << b) }); }
template <> inline A3  bit_shl       (const A3  &a, size_t b)     { return A3 ({ _T(a[0] << b), _T(a[1] << b), _T(a[2] << b) }); }
template <> inline A4  bit_shl       (const A4  &a, size_t b)     { return A4 ({ _T(a[0] << b), _T(a[1] << b), _T(a[2] << b), _T(a[3] << b) }); }
template <> inline A5  bit_shl       (const A5  &a, size_t b)     { return A5 ({ _T(a[0] << b), _T(a[1] << b), _T(a[2] << b), _T(a[3] << b), _T(a[4] << b) }); }
template <> inline A6  bit_shl       (const A6  &a, size_t b)     { return A6 ({ _T(a[0] << b), _T(a[1] << b), _T(a[2] << b), _T(a[3] << b), _T(a[4] << b), _T(a[5] << b) }); }
template <> inline A7  bit_shl       (const A7  &a, size_t b)     { return A7 ({ _T(a[0] << b), _T(a[1] << b), _T(a[2] << b), _T(a[3] << b), _T(a[4] << b), _T(a[5] << b), _T(a[6] << b) }); }
template <> inline A8  bit_shl       (const A8  &a, size_t b)     { return A8 ({ _T(a[0] << b), _T(a[1] << b), _T(a[2] << b), _T(a[3] << b), _T(a[4] << b), _T(a[5] << b), _T(a[6] << b), _T(a[7] << b) }); }
template <> inline A9  bit_shl       (const A9  &a, size_t b)     { return A9 ({ _T(a[0] << b), _T(a[1] << b), _T(a[2] << b), _T(a[3] << b), _T(a[4] << b), _T(a[5] << b), _T(a[6] << b), _T(a[7] << b), _T(a[8] << b) }); }
template <> inline A10 bit_shl       (const A10 &a, size_t b)     { return A10({ _T(a[0] << b), _T(a[1] << b), _T(a[2] << b), _T(a[3] << b), _T(a[4] << b), _T(a[5] << b), _T(a[6] << b), _T(a[7] << b), _T(a[8] << b), _T(a[9] << b) }); }
template <> inline A11 bit_shl       (const A11 &a, size_t b)     { return A11({ _T(a[0] << b), _T(a[1] << b), _T(a[2] << b), _T(a[3] << b), _T(a[4] << b), _T(a[5] << b), _T(a[6] << b), _T(a[7] << b), _T(a[8] << b), _T(a[9] << b), _T(a[10] << b) }); }
template <> inline A12 bit_shl       (const A12 &a, size_t b)     { return A12({ _T(a[0] << b), _T(a[1] << b), _T(a[2] << b), _T(a[3] << b), _T(a[4] << b), _T(a[5] << b), _T(a[6] << b), _T(a[7] << b), _T(a[8] << b), _T(a[9] << b), _T(a[10] << b), _T(a[11] << b) }); }
template <> inline A13 bit_shl       (const A13 &a, size_t b)     { return A13({ _T(a[0] << b), _T(a[1] << b), _T(a[2] << b), _T(a[3] << b), _T(a[4] << b), _T(a[5] << b), _T(a[6] << b), _T(a[7] << b), _T(a[8] << b), _T(a[9] << b), _T(a[10] << b), _T(a[11] << b), _T(a[12] << b) }); }
template <> inline A14 bit_shl       (const A14 &a, size_t b)     { return A14({ _T(a[0] << b), _T(a[1] << b), _T(a[2] << b), _T(a[3] << b), _T(a[4] << b), _T(a[5] << b), _T(a[6] << b), _T(a[7] << b), _T(a[8] << b), _T(a[9] << b), _T(a[10] << b), _T(a[11] << b), _T(a[12] << b), _T(a[13] << b) }); }
template <> inline A15 bit_shl       (const A15 &a, size_t b)     { return A15({ _T(a[0] << b), _T(a[1] << b), _T(a[2] << b), _T(a[3] << b), _T(a[4] << b), _T(a[5] << b), _T(a[6] << b), _T(a[7] << b), _T(a[8] << b), _T(a[9] << b), _T(a[10] << b), _T(a[11] << b), _T(a[12] << b), _T(a[13] << b), _T(a[14] << b) }); }
template <> inline A16 bit_shl       (const A16 &a, size_t b)     { return A16({ _T(a[0] << b), _T(a[1] << b), _T(a[2] << b), _T(a[3] << b), _T(a[4] << b), _T(a[5] << b), _T(a[6] << b), _T(a[7] << b), _T(a[8] << b), _T(a[9] << b), _T(a[10] << b), _T(a[11] << b), _T(a[12] << b), _T(a[13] << b), _T(a[14] << b), _T(a[15] << b) }); }

template <> inline A1  bit_shr       (const A1  &a, size_t b)     { return A1 ({ _T(a[0] >> b) }); }
template <> inline A2  bit_shr       (const A2  &a, size_t b)     { return A2 ({ _T(a[0] >> b), _T(a[1] >> b) }); }
template <> inline A3  bit_shr       (const A3  &a, size_t b)     { return A3 ({ _T(a[0] >> b), _T(a[1] >> b), _T(a[2] >> b) }); }
template <> inline A4  bit_shr       (const A4  &a, size_t b)     { return A4 ({ _T(a[0] >> b), _T(a[1] >> b), _T(a[2] >> b), _T(a[3] >> b) }); }
template <> inline A5  bit_shr       (const A5  &a, size_t b)     { return A5 ({ _T(a[0] >> b), _T(a[1] >> b), _T(a[2] >> b), _T(a[3] >> b), _T(a[4] >> b) }); }
template <> inline A6  bit_shr       (const A6  &a, size_t b)     { return A6 ({ _T(a[0] >> b), _T(a[1] >> b), _T(a[2] >> b), _T(a[3] >> b), _T(a[4] >> b), _T(a[5] >> b) }); }
template <> inline A7  bit_shr       (const A7  &a, size_t b)     { return A7 ({ _T(a[0] >> b), _T(a[1] >> b), _T(a[2] >> b), _T(a[3] >> b), _T(a[4] >> b), _T(a[5] >> b), _T(a[6] >> b) }); }
template <> inline A8  bit_shr       (const A8  &a, size_t b)     { return A8 ({ _T(a[0] >> b), _T(a[1] >> b), _T(a[2] >> b), _T(a[3] >> b), _T(a[4] >> b), _T(a[5] >> b), _T(a[6] >> b), _T(a[7] >> b) }); }
template <> inline A9  bit_shr       (const A9  &a, size_t b)     { return A9 ({ _T(a[0] >> b), _T(a[1] >> b), _T(a[2] >> b), _T(a[3] >> b), _T(a[4] >> b), _T(a[5] >> b), _T(a[6] >> b), _T(a[7] >> b), _T(a[8] >> b) }); }
template <> inline A10 bit_shr       (const A10 &a, size_t b)     { return A10({ _T(a[0] >> b), _T(a[1] >> b), _T(a[2] >> b), _T(a[3] >> b), _T(a[4] >> b), _T(a[5] >> b), _T(a[6] >> b), _T(a[7] >> b), _T(a[8] >> b), _T(a[9] >> b) }); }
template <> inline A11 bit_shr       (const A11 &a, size_t b)     { return A11({ _T(a[0] >> b), _T(a[1] >> b), _T(a[2] >> b), _T(a[3] >> b), _T(a[4] >> b), _T(a[5] >> b), _T(a[6] >> b), _T(a[7] >> b), _T(a[8] >> b), _T(a[9] >> b), _T(a[10] >> b) }); }
template <> inline A12 bit_shr       (const A12 &a, size_t b)     { return A12({ _T(a[0] >> b), _T(a[1] >> b), _T(a[2] >> b), _T(a[3] >> b), _T(a[4] >> b), _T(a[5] >> b), _T(a[6] >> b), _T(a[7] >> b), _T(a[8] >> b), _T(a[9] >> b), _T(a[10] >> b), _T(a[11] >> b) }); }
template <> inline A13 bit_shr       (const A13 &a, size_t b)     { return A13({ _T(a[0] >> b), _T(a[1] >> b), _T(a[2] >> b), _T(a[3] >> b), _T(a[4] >> b), _T(a[5] >> b), _T(a[6] >> b), _T(a[7] >> b), _T(a[8] >> b), _T(a[9] >> b), _T(a[10] >> b), _T(a[11] >> b), _T(a[12] >> b) }); }
template <> inline A14 bit_shr       (const A14 &a, size_t b)     { return A14({ _T(a[0] >> b), _T(a[1] >> b), _T(a[2] >> b), _T(a[3] >> b), _T(a[4] >> b), _T(a[5] >> b), _T(a[6] >> b), _T(a[7] >> b), _T(a[8] >> b), _T(a[9] >> b), _T(a[10] >> b), _T(a[11] >> b), _T(a[12] >> b), _T(a[13] >> b) }); }
template <> inline A15 bit_shr       (const A15 &a, size_t b)     { return A15({ _T(a[0] >> b), _T(a[1] >> b), _T(a[2] >> b), _T(a[3] >> b), _T(a[4] >> b), _T(a[5] >> b), _T(a[6] >> b), _T(a[7] >> b), _T(a[8] >> b), _T(a[9] >> b), _T(a[10] >> b), _T(a[11] >> b), _T(a[12] >> b), _T(a[13] >> b), _T(a[14] >> b) }); }
template <> inline A16 bit_shr       (const A16 &a, size_t b)     { return A16({ _T(a[0] >> b), _T(a[1] >> b), _T(a[2] >> b), _T(a[3] >> b), _T(a[4] >> b), _T(a[5] >> b), _T(a[6] >> b), _T(a[7] >> b), _T(a[8] >> b), _T(a[9] >> b), _T(a[10] >> b), _T(a[11] >> b), _T(a[12] >> b), _T(a[13] >> b), _T(a[14] >> b), _T(a[15] >> b) }); }
#endif

template <> inline A1  &in_place_sub (A1  &a, const A1  &b)       { a[0] -= b[0]; return a; }
template <> inline A2  &in_place_sub (A2  &a, const A2  &b)       { a[0] -= b[0]; a[1] -= b[1]; return a; }
template <> inline A3  &in_place_sub (A3  &a, const A3  &b)       { a[0] -= b[0]; a[1] -= b[1]; a[2] -= b[2]; return a; }
template <> inline A4  &in_place_sub (A4  &a, const A4  &b)       { a[0] -= b[0]; a[1] -= b[1]; a[2] -= b[2]; a[3] -= b[3]; return a; }
template <> inline A5  &in_place_sub (A5  &a, const A5  &b)       { a[0] -= b[0]; a[1] -= b[1]; a[2] -= b[2]; a[3] -= b[3]; a[4] -= b[4]; return a; }
template <> inline A6  &in_place_sub (A6  &a, const A6  &b)       { a[0] -= b[0]; a[1] -= b[1]; a[2] -= b[2]; a[3] -= b[3]; a[4] -= b[4]; a[5] -= b[5]; return a; }
template <> inline A7  &in_place_sub (A7  &a, const A7  &b)       { a[0] -= b[0]; a[1] -= b[1]; a[2] -= b[2]; a[3] -= b[3]; a[4] -= b[4]; a[5] -= b[5]; a[6] -= b[6]; return a; }
template <> inline A8  &in_place_sub (A8  &a, const A8  &b)       { a[0] -= b[0]; a[1] -= b[1]; a[2] -= b[2]; a[3] -= b[3]; a[4] -= b[4]; a[5] -= b[5]; a[6] -= b[6]; a[7] -= b[7]; return a; }
template <> inline A9  &in_place_sub (A9  &a, const A9  &b)       { a[0] -= b[0]; a[1] -= b[1]; a[2] -= b[2]; a[3] -= b[3]; a[4] -= b[4]; a[5] -= b[5]; a[6] -= b[6]; a[7] -= b[7]; a[8] -= b[8]; return a; }
template <> inline A10 &in_place_sub (A10 &a, const A10 &b)       { a[0] -= b[0]; a[1] -= b[1]; a[2] -= b[2]; a[3] -= b[3]; a[4] -= b[4]; a[5] -= b[5]; a[6] -= b[6]; a[7] -= b[7]; a[8] -= b[8]; a[9] -= b[9]; return a; }
template <> inline A11 &in_place_sub (A11 &a, const A11 &b)       { a[0] -= b[0]; a[1] -= b[1]; a[2] -= b[2]; a[3] -= b[3]; a[4] -= b[4]; a[5] -= b[5]; a[6] -= b[6]; a[7] -= b[7]; a[8] -= b[8]; a[9] -= b[9]; a[10] -= b[10]; return a; }
template <> inline A12 &in_place_sub (A12 &a, const A12 &b)       { a[0] -= b[0]; a[1] -= b[1]; a[2] -= b[2]; a[3] -= b[3]; a[4] -= b[4]; a[5] -= b[5]; a[6] -= b[6]; a[7] -= b[7]; a[8] -= b[8]; a[9] -= b[9]; a[10] -= b[10]; a[11] -= b[11]; return a; }
template <> inline A13 &in_place_sub (A13 &a, const A13 &b)       { a[0] -= b[0]; a[1] -= b[1]; a[2] -= b[2]; a[3] -= b[3]; a[4] -= b[4]; a[5] -= b[5]; a[6] -= b[6]; a[7] -= b[7]; a[8] -= b[8]; a[9] -= b[9]; a[10] -= b[10]; a[11] -= b[11]; a[12] -= b[12]; return a; }
template <> inline A14 &in_place_sub (A14 &a, const A14 &b)       { a[0] -= b[0]; a[1] -= b[1]; a[2] -= b[2]; a[3] -= b[3]; a[4] -= b[4]; a[5] -= b[5]; a[6] -= b[6]; a[7] -= b[7]; a[8] -= b[8]; a[9] -= b[9]; a[10] -= b[10]; a[11] -= b[11]; a[12] -= b[12]; a[13] -= b[13]; return a; }
template <> inline A15 &in_place_sub (A15 &a, const A15 &b)       { a[0] -= b[0]; a[1] -= b[1]; a[2] -= b[2]; a[3] -= b[3]; a[4] -= b[4]; a[5] -= b[5]; a[6] -= b[6]; a[7] -= b[7]; a[8] -= b[8]; a[9] -= b[9]; a[10] -= b[10]; a[11] -= b[11]; a[12] -= b[12]; a[13] -= b[13]; a[14] -= b[14]; return a; }
template <> inline A16 &in_place_sub (A16 &a, const A16 &b)       { a[0] -= b[0]; a[1] -= b[1]; a[2] -= b[2]; a[3] -= b[3]; a[4] -= b[4]; a[5] -= b[5]; a[6] -= b[6]; a[7] -= b[7]; a[8] -= b[8]; a[9] -= b[9]; a[10] -= b[10]; a[11] -= b[11]; a[12] -= b[12]; a[13] -= b[13]; a[14] -= b[14]; a[15] -= b[15]; return a; }

template <> inline A1  &in_place_add (A1  &a, const A1  &b)       { a[0] += b[0]; return a; }
template <> inline A2  &in_place_add (A2  &a, const A2  &b)       { a[0] += b[0]; a[1] += b[1]; return a; }
template <> inline A3  &in_place_add (A3  &a, const A3  &b)       { a[0] += b[0]; a[1] += b[1]; a[2] += b[2]; return a; }
template <> inline A4  &in_place_add (A4  &a, const A4  &b)       { a[0] += b[0]; a[1] += b[1]; a[2] += b[2]; a[3] += b[3]; return a; }
template <> inline A5  &in_place_add (A5  &a, const A5  &b)       { a[0] += b[0]; a[1] += b[1]; a[2] += b[2]; a[3] += b[3]; a[4] += b[4]; return a; }
template <> inline A6  &in_place_add (A6  &a, const A6  &b)       { a[0] += b[0]; a[1] += b[1]; a[2] += b[2]; a[3] += b[3]; a[4] += b[4]; a[5] += b[5]; return a; }
template <> inline A7  &in_place_add (A7  &a, const A7  &b)       { a[0] += b[0]; a[1] += b[1]; a[2] += b[2]; a[3] += b[3]; a[4] += b[4]; a[5] += b[5]; a[6] += b[6]; return a; }
template <> inline A8  &in_place_add (A8  &a, const A8  &b)       { a[0] += b[0]; a[1] += b[1]; a[2] += b[2]; a[3] += b[3]; a[4] += b[4]; a[5] += b[5]; a[6] += b[6]; a[7] += b[7]; return a; }
template <> inline A9  &in_place_add (A9  &a, const A9  &b)       { a[0] += b[0]; a[1] += b[1]; a[2] += b[2]; a[3] += b[3]; a[4] += b[4]; a[5] += b[5]; a[6] += b[6]; a[7] += b[7]; a[8] += b[8]; return a; }
template <> inline A10 &in_place_add (A10 &a, const A10 &b)       { a[0] += b[0]; a[1] += b[1]; a[2] += b[2]; a[3] += b[3]; a[4] += b[4]; a[5] += b[5]; a[6] += b[6]; a[7] += b[7]; a[8] += b[8]; a[9] += b[9]; return a; }
template <> inline A11 &in_place_add (A11 &a, const A11 &b)       { a[0] += b[0]; a[1] += b[1]; a[2] += b[2]; a[3] += b[3]; a[4] += b[4]; a[5] += b[5]; a[6] += b[6]; a[7] += b[7]; a[8] += b[8]; a[9] += b[9]; a[10] += b[10]; return a; }
template <> inline A12 &in_place_add (A12 &a, const A12 &b)       { a[0] += b[0]; a[1] += b[1]; a[2] += b[2]; a[3] += b[3]; a[4] += b[4]; a[5] += b[5]; a[6] += b[6]; a[7] += b[7]; a[8] += b[8]; a[9] += b[9]; a[10] += b[10]; a[11] += b[11]; return a; }
template <> inline A13 &in_place_add (A13 &a, const A13 &b)       { a[0] += b[0]; a[1] += b[1]; a[2] += b[2]; a[3] += b[3]; a[4] += b[4]; a[5] += b[5]; a[6] += b[6]; a[7] += b[7]; a[8] += b[8]; a[9] += b[9]; a[10] += b[10]; a[11] += b[11]; a[12] += b[12]; return a; }
template <> inline A14 &in_place_add (A14 &a, const A14 &b)       { a[0] += b[0]; a[1] += b[1]; a[2] += b[2]; a[3] += b[3]; a[4] += b[4]; a[5] += b[5]; a[6] += b[6]; a[7] += b[7]; a[8] += b[8]; a[9] += b[9]; a[10] += b[10]; a[11] += b[11]; a[12] += b[12]; a[13] += b[13]; return a; }
template <> inline A15 &in_place_add (A15 &a, const A15 &b)       { a[0] += b[0]; a[1] += b[1]; a[2] += b[2]; a[3] += b[3]; a[4] += b[4]; a[5] += b[5]; a[6] += b[6]; a[7] += b[7]; a[8] += b[8]; a[9] += b[9]; a[10] += b[10]; a[11] += b[11]; a[12] += b[12]; a[13] += b[13]; a[14] += b[14]; return a; }
template <> inline A16 &in_place_add (A16 &a, const A16 &b)       { a[0] += b[0]; a[1] += b[1]; a[2] += b[2]; a[3] += b[3]; a[4] += b[4]; a[5] += b[5]; a[6] += b[6]; a[7] += b[7]; a[8] += b[8]; a[9] += b[9]; a[10] += b[10]; a[11] += b[11]; a[12] += b[12]; a[13] += b[13]; a[14] += b[14]; a[15] += b[15]; return a; }

template <> inline A1  &in_place_mul (A1  &a, const A1  &b)       { a[0] *= b[0]; return a; }
template <> inline A2  &in_place_mul (A2  &a, const A2  &b)       { a[0] *= b[0]; a[1] *= b[1]; return a; }
template <> inline A3  &in_place_mul (A3  &a, const A3  &b)       { a[0] *= b[0]; a[1] *= b[1]; a[2] *= b[2]; return a; }
template <> inline A4  &in_place_mul (A4  &a, const A4  &b)       { a[0] *= b[0]; a[1] *= b[1]; a[2] *= b[2]; a[3] *= b[3]; return a; }
template <> inline A5  &in_place_mul (A5  &a, const A5  &b)       { a[0] *= b[0]; a[1] *= b[1]; a[2] *= b[2]; a[3] *= b[3]; a[4] *= b[4]; return a; }
template <> inline A6  &in_place_mul (A6  &a, const A6  &b)       { a[0] *= b[0]; a[1] *= b[1]; a[2] *= b[2]; a[3] *= b[3]; a[4] *= b[4]; a[5] *= b[5]; return a; }
template <> inline A7  &in_place_mul (A7  &a, const A7  &b)       { a[0] *= b[0]; a[1] *= b[1]; a[2] *= b[2]; a[3] *= b[3]; a[4] *= b[4]; a[5] *= b[5]; a[6] *= b[6]; return a; }
template <> inline A8  &in_place_mul (A8  &a, const A8  &b)       { a[0] *= b[0]; a[1] *= b[1]; a[2] *= b[2]; a[3] *= b[3]; a[4] *= b[4]; a[5] *= b[5]; a[6] *= b[6]; a[7] *= b[7]; return a; }
template <> inline A9  &in_place_mul (A9  &a, const A9  &b)       { a[0] *= b[0]; a[1] *= b[1]; a[2] *= b[2]; a[3] *= b[3]; a[4] *= b[4]; a[5] *= b[5]; a[6] *= b[6]; a[7] *= b[7]; a[8] *= b[8]; return a; }
template <> inline A10 &in_place_mul (A10 &a, const A10 &b)       { a[0] *= b[0]; a[1] *= b[1]; a[2] *= b[2]; a[3] *= b[3]; a[4] *= b[4]; a[5] *= b[5]; a[6] *= b[6]; a[7] *= b[7]; a[8] *= b[8]; a[9] *= b[9]; return a; }
template <> inline A11 &in_place_mul (A11 &a, const A11 &b)       { a[0] *= b[0]; a[1] *= b[1]; a[2] *= b[2]; a[3] *= b[3]; a[4] *= b[4]; a[5] *= b[5]; a[6] *= b[6]; a[7] *= b[7]; a[8] *= b[8]; a[9] *= b[9]; a[10] *= b[10]; return a; }
template <> inline A12 &in_place_mul (A12 &a, const A12 &b)       { a[0] *= b[0]; a[1] *= b[1]; a[2] *= b[2]; a[3] *= b[3]; a[4] *= b[4]; a[5] *= b[5]; a[6] *= b[6]; a[7] *= b[7]; a[8] *= b[8]; a[9] *= b[9]; a[10] *= b[10]; a[11] *= b[11]; return a; }
template <> inline A13 &in_place_mul (A13 &a, const A13 &b)       { a[0] *= b[0]; a[1] *= b[1]; a[2] *= b[2]; a[3] *= b[3]; a[4] *= b[4]; a[5] *= b[5]; a[6] *= b[6]; a[7] *= b[7]; a[8] *= b[8]; a[9] *= b[9]; a[10] *= b[10]; a[11] *= b[11]; a[12] *= b[12]; return a; }
template <> inline A14 &in_place_mul (A14 &a, const A14 &b)       { a[0] *= b[0]; a[1] *= b[1]; a[2] *= b[2]; a[3] *= b[3]; a[4] *= b[4]; a[5] *= b[5]; a[6] *= b[6]; a[7] *= b[7]; a[8] *= b[8]; a[9] *= b[9]; a[10] *= b[10]; a[11] *= b[11]; a[12] *= b[12]; a[13] *= b[13]; return a; }
template <> inline A15 &in_place_mul (A15 &a, const A15 &b)       { a[0] *= b[0]; a[1] *= b[1]; a[2] *= b[2]; a[3] *= b[3]; a[4] *= b[4]; a[5] *= b[5]; a[6] *= b[6]; a[7] *= b[7]; a[8] *= b[8]; a[9] *= b[9]; a[10] *= b[10]; a[11] *= b[11]; a[12] *= b[12]; a[13] *= b[13]; a[14] *= b[14]; return a; }
template <> inline A16 &in_place_mul (A16 &a, const A16 &b)       { a[0] *= b[0]; a[1] *= b[1]; a[2] *= b[2]; a[3] *= b[3]; a[4] *= b[4]; a[5] *= b[5]; a[6] *= b[6]; a[7] *= b[7]; a[8] *= b[8]; a[9] *= b[9]; a[10] *= b[10]; a[11] *= b[11]; a[12] *= b[12]; a[13] *= b[13]; a[14] *= b[14]; a[15] *= b[15]; return a; }

template <> inline A1  &in_place_div (A1  &a, const A1  &b)       { a[0] /= b[0]; return a; }
template <> inline A2  &in_place_div (A2  &a, const A2  &b)       { a[0] /= b[0]; a[1] /= b[1]; return a; }
template <> inline A3  &in_place_div (A3  &a, const A3  &b)       { a[0] /= b[0]; a[1] /= b[1]; a[2] /= b[2]; return a; }
template <> inline A4  &in_place_div (A4  &a, const A4  &b)       { a[0] /= b[0]; a[1] /= b[1]; a[2] /= b[2]; a[3] /= b[3]; return a; }
template <> inline A5  &in_place_div (A5  &a, const A5  &b)       { a[0] /= b[0]; a[1] /= b[1]; a[2] /= b[2]; a[3] /= b[3]; a[4] /= b[4]; return a; }
template <> inline A6  &in_place_div (A6  &a, const A6  &b)       { a[0] /= b[0]; a[1] /= b[1]; a[2] /= b[2]; a[3] /= b[3]; a[4] /= b[4]; a[5] /= b[5]; return a; }
template <> inline A7  &in_place_div (A7  &a, const A7  &b)       { a[0] /= b[0]; a[1] /= b[1]; a[2] /= b[2]; a[3] /= b[3]; a[4] /= b[4]; a[5] /= b[5]; a[6] /= b[6]; return a; }
template <> inline A8  &in_place_div (A8  &a, const A8  &b)       { a[0] /= b[0]; a[1] /= b[1]; a[2] /= b[2]; a[3] /= b[3]; a[4] /= b[4]; a[5] /= b[5]; a[6] /= b[6]; a[7] /= b[7]; return a; }
template <> inline A9  &in_place_div (A9  &a, const A9  &b)       { a[0] /= b[0]; a[1] /= b[1]; a[2] /= b[2]; a[3] /= b[3]; a[4] /= b[4]; a[5] /= b[5]; a[6] /= b[6]; a[7] /= b[7]; a[8] /= b[8]; return a; }
template <> inline A10 &in_place_div (A10 &a, const A10 &b)       { a[0] /= b[0]; a[1] /= b[1]; a[2] /= b[2]; a[3] /= b[3]; a[4] /= b[4]; a[5] /= b[5]; a[6] /= b[6]; a[7] /= b[7]; a[8] /= b[8]; a[9] /= b[9]; return a; }
template <> inline A11 &in_place_div (A11 &a, const A11 &b)       { a[0] /= b[0]; a[1] /= b[1]; a[2] /= b[2]; a[3] /= b[3]; a[4] /= b[4]; a[5] /= b[5]; a[6] /= b[6]; a[7] /= b[7]; a[8] /= b[8]; a[9] /= b[9]; a[10] /= b[10]; return a; }
template <> inline A12 &in_place_div (A12 &a, const A12 &b)       { a[0] /= b[0]; a[1] /= b[1]; a[2] /= b[2]; a[3] /= b[3]; a[4] /= b[4]; a[5] /= b[5]; a[6] /= b[6]; a[7] /= b[7]; a[8] /= b[8]; a[9] /= b[9]; a[10] /= b[10]; a[11] /= b[11]; return a; }
template <> inline A13 &in_place_div (A13 &a, const A13 &b)       { a[0] /= b[0]; a[1] /= b[1]; a[2] /= b[2]; a[3] /= b[3]; a[4] /= b[4]; a[5] /= b[5]; a[6] /= b[6]; a[7] /= b[7]; a[8] /= b[8]; a[9] /= b[9]; a[10] /= b[10]; a[11] /= b[11]; a[12] /= b[12]; return a; }
template <> inline A14 &in_place_div (A14 &a, const A14 &b)       { a[0] /= b[0]; a[1] /= b[1]; a[2] /= b[2]; a[3] /= b[3]; a[4] /= b[4]; a[5] /= b[5]; a[6] /= b[6]; a[7] /= b[7]; a[8] /= b[8]; a[9] /= b[9]; a[10] /= b[10]; a[11] /= b[11]; a[12] /= b[12]; a[13] /= b[13]; return a; }
template <> inline A15 &in_place_div (A15 &a, const A15 &b)       { a[0] /= b[0]; a[1] /= b[1]; a[2] /= b[2]; a[3] /= b[3]; a[4] /= b[4]; a[5] /= b[5]; a[6] /= b[6]; a[7] /= b[7]; a[8] /= b[8]; a[9] /= b[9]; a[10] /= b[10]; a[11] /= b[11]; a[12] /= b[12]; a[13] /= b[13]; a[14] /= b[14]; return a; }
template <> inline A16 &in_place_div (A16 &a, const A16 &b)       { a[0] /= b[0]; a[1] /= b[1]; a[2] /= b[2]; a[3] /= b[3]; a[4] /= b[4]; a[5] /= b[5]; a[6] /= b[6]; a[7] /= b[7]; a[8] /= b[8]; a[9] /= b[9]; a[10] /= b[10]; a[11] /= b[11]; a[12] /= b[12]; a[13] /= b[13]; a[14] /= b[14]; a[15] /= b[15]; return a; }

#if _T_INT == 1
template <> inline A1  &in_place_mod (A1  &a, const A1  &b)       { a[0] %= b[0]; return a; }
template <> inline A2  &in_place_mod (A2  &a, const A2  &b)       { a[0] %= b[0]; a[1] %= b[1]; return a; }
template <> inline A3  &in_place_mod (A3  &a, const A3  &b)       { a[0] %= b[0]; a[1] %= b[1]; a[2] %= b[2]; return a; }
template <> inline A4  &in_place_mod (A4  &a, const A4  &b)       { a[0] %= b[0]; a[1] %= b[1]; a[2] %= b[2]; a[3] %= b[3]; return a; }
template <> inline A5  &in_place_mod (A5  &a, const A5  &b)       { a[0] %= b[0]; a[1] %= b[1]; a[2] %= b[2]; a[3] %= b[3]; a[4] %= b[4]; return a; }
template <> inline A6  &in_place_mod (A6  &a, const A6  &b)       { a[0] %= b[0]; a[1] %= b[1]; a[2] %= b[2]; a[3] %= b[3]; a[4] %= b[4]; a[5] %= b[5]; return a; }
template <> inline A7  &in_place_mod (A7  &a, const A7  &b)       { a[0] %= b[0]; a[1] %= b[1]; a[2] %= b[2]; a[3] %= b[3]; a[4] %= b[4]; a[5] %= b[5]; a[6] %= b[6]; return a; }
template <> inline A8  &in_place_mod (A8  &a, const A8  &b)       { a[0] %= b[0]; a[1] %= b[1]; a[2] %= b[2]; a[3] %= b[3]; a[4] %= b[4]; a[5] %= b[5]; a[6] %= b[6]; a[7] %= b[7]; return a; }
template <> inline A9  &in_place_mod (A9  &a, const A9  &b)       { a[0] %= b[0]; a[1] %= b[1]; a[2] %= b[2]; a[3] %= b[3]; a[4] %= b[4]; a[5] %= b[5]; a[6] %= b[6]; a[7] %= b[7]; a[8] %= b[8]; return a; }
template <> inline A10 &in_place_mod (A10 &a, const A10 &b)       { a[0] %= b[0]; a[1] %= b[1]; a[2] %= b[2]; a[3] %= b[3]; a[4] %= b[4]; a[5] %= b[5]; a[6] %= b[6]; a[7] %= b[7]; a[8] %= b[8]; a[9] %= b[9]; return a; }
template <> inline A11 &in_place_mod (A11 &a, const A11 &b)       { a[0] %= b[0]; a[1] %= b[1]; a[2] %= b[2]; a[3] %= b[3]; a[4] %= b[4]; a[5] %= b[5]; a[6] %= b[6]; a[7] %= b[7]; a[8] %= b[8]; a[9] %= b[9]; a[10] %= b[10]; return a; }
template <> inline A12 &in_place_mod (A12 &a, const A12 &b)       { a[0] %= b[0]; a[1] %= b[1]; a[2] %= b[2]; a[3] %= b[3]; a[4] %= b[4]; a[5] %= b[5]; a[6] %= b[6]; a[7] %= b[7]; a[8] %= b[8]; a[9] %= b[9]; a[10] %= b[10]; a[11] %= b[11]; return a; }
template <> inline A13 &in_place_mod (A13 &a, const A13 &b)       { a[0] %= b[0]; a[1] %= b[1]; a[2] %= b[2]; a[3] %= b[3]; a[4] %= b[4]; a[5] %= b[5]; a[6] %= b[6]; a[7] %= b[7]; a[8] %= b[8]; a[9] %= b[9]; a[10] %= b[10]; a[11] %= b[11]; a[12] %= b[12]; return a; }
template <> inline A14 &in_place_mod (A14 &a, const A14 &b)       { a[0] %= b[0]; a[1] %= b[1]; a[2] %= b[2]; a[3] %= b[3]; a[4] %= b[4]; a[5] %= b[5]; a[6] %= b[6]; a[7] %= b[7]; a[8] %= b[8]; a[9] %= b[9]; a[10] %= b[10]; a[11] %= b[11]; a[12] %= b[12]; a[13] %= b[13]; return a; }
template <> inline A15 &in_place_mod (A15 &a, const A15 &b)       { a[0] %= b[0]; a[1] %= b[1]; a[2] %= b[2]; a[3] %= b[3]; a[4] %= b[4]; a[5] %= b[5]; a[6] %= b[6]; a[7] %= b[7]; a[8] %= b[8]; a[9] %= b[9]; a[10] %= b[10]; a[11] %= b[11]; a[12] %= b[12]; a[13] %= b[13]; a[14] %= b[14]; return a; }
template <> inline A16 &in_place_mod (A16 &a, const A16 &b)       { a[0] %= b[0]; a[1] %= b[1]; a[2] %= b[2]; a[3] %= b[3]; a[4] %= b[4]; a[5] %= b[5]; a[6] %= b[6]; a[7] %= b[7]; a[8] %= b[8]; a[9] %= b[9]; a[10] %= b[10]; a[11] %= b[11]; a[12] %= b[12]; a[13] %= b[13]; a[14] %= b[14]; a[15] %= b[15]; return a; }

template <> inline A1  &in_place_and (A1  &a, const A1  &b)       { a[0] &= b[0]; return a; }
template <> inline A2  &in_place_and (A2  &a, const A2  &b)       { a[0] &= b[0]; a[1] &= b[1]; return a; }
template <> inline A3  &in_place_and (A3  &a, const A3  &b)       { a[0] &= b[0]; a[1] &= b[1]; a[2] &= b[2]; return a; }
template <> inline A4  &in_place_and (A4  &a, const A4  &b)       { a[0] &= b[0]; a[1] &= b[1]; a[2] &= b[2]; a[3] &= b[3]; return a; }
template <> inline A5  &in_place_and (A5  &a, const A5  &b)       { a[0] &= b[0]; a[1] &= b[1]; a[2] &= b[2]; a[3] &= b[3]; a[4] &= b[4]; return a; }
template <> inline A6  &in_place_and (A6  &a, const A6  &b)       { a[0] &= b[0]; a[1] &= b[1]; a[2] &= b[2]; a[3] &= b[3]; a[4] &= b[4]; a[5] &= b[5]; return a; }
template <> inline A7  &in_place_and (A7  &a, const A7  &b)       { a[0] &= b[0]; a[1] &= b[1]; a[2] &= b[2]; a[3] &= b[3]; a[4] &= b[4]; a[5] &= b[5]; a[6] &= b[6]; return a; }
template <> inline A8  &in_place_and (A8  &a, const A8  &b)       { a[0] &= b[0]; a[1] &= b[1]; a[2] &= b[2]; a[3] &= b[3]; a[4] &= b[4]; a[5] &= b[5]; a[6] &= b[6]; a[7] &= b[7]; return a; }
template <> inline A9  &in_place_and (A9  &a, const A9  &b)       { a[0] &= b[0]; a[1] &= b[1]; a[2] &= b[2]; a[3] &= b[3]; a[4] &= b[4]; a[5] &= b[5]; a[6] &= b[6]; a[7] &= b[7]; a[8] &= b[8]; return a; }
template <> inline A10 &in_place_and (A10 &a, const A10 &b)       { a[0] &= b[0]; a[1] &= b[1]; a[2] &= b[2]; a[3] &= b[3]; a[4] &= b[4]; a[5] &= b[5]; a[6] &= b[6]; a[7] &= b[7]; a[8] &= b[8]; a[9] &= b[9]; return a; }
template <> inline A11 &in_place_and (A11 &a, const A11 &b)       { a[0] &= b[0]; a[1] &= b[1]; a[2] &= b[2]; a[3] &= b[3]; a[4] &= b[4]; a[5] &= b[5]; a[6] &= b[6]; a[7] &= b[7]; a[8] &= b[8]; a[9] &= b[9]; a[10] &= b[10]; return a; }
template <> inline A12 &in_place_and (A12 &a, const A12 &b)       { a[0] &= b[0]; a[1] &= b[1]; a[2] &= b[2]; a[3] &= b[3]; a[4] &= b[4]; a[5] &= b[5]; a[6] &= b[6]; a[7] &= b[7]; a[8] &= b[8]; a[9] &= b[9]; a[10] &= b[10]; a[11] &= b[11]; return a; }
template <> inline A13 &in_place_and (A13 &a, const A13 &b)       { a[0] &= b[0]; a[1] &= b[1]; a[2] &= b[2]; a[3] &= b[3]; a[4] &= b[4]; a[5] &= b[5]; a[6] &= b[6]; a[7] &= b[7]; a[8] &= b[8]; a[9] &= b[9]; a[10] &= b[10]; a[11] &= b[11]; a[12] &= b[12]; return a; }
template <> inline A14 &in_place_and (A14 &a, const A14 &b)       { a[0] &= b[0]; a[1] &= b[1]; a[2] &= b[2]; a[3] &= b[3]; a[4] &= b[4]; a[5] &= b[5]; a[6] &= b[6]; a[7] &= b[7]; a[8] &= b[8]; a[9] &= b[9]; a[10] &= b[10]; a[11] &= b[11]; a[12] &= b[12]; a[13] &= b[13]; return a; }
template <> inline A15 &in_place_and (A15 &a, const A15 &b)       { a[0] &= b[0]; a[1] &= b[1]; a[2] &= b[2]; a[3] &= b[3]; a[4] &= b[4]; a[5] &= b[5]; a[6] &= b[6]; a[7] &= b[7]; a[8] &= b[8]; a[9] &= b[9]; a[10] &= b[10]; a[11] &= b[11]; a[12] &= b[12]; a[13] &= b[13]; a[14] &= b[14]; return a; }
template <> inline A16 &in_place_and (A16 &a, const A16 &b)       { a[0] &= b[0]; a[1] &= b[1]; a[2] &= b[2]; a[3] &= b[3]; a[4] &= b[4]; a[5] &= b[5]; a[6] &= b[6]; a[7] &= b[7]; a[8] &= b[8]; a[9] &= b[9]; a[10] &= b[10]; a[11] &= b[11]; a[12] &= b[12]; a[13] &= b[13]; a[14] &= b[14]; a[15] &= b[15]; return a; }

template <> inline A1  &in_place_or  (A1  &a, const A1  &b)       { a[0] |= b[0]; return a; }
template <> inline A2  &in_place_or  (A2  &a, const A2  &b)       { a[0] |= b[0]; a[1] |= b[1]; return a; }
template <> inline A3  &in_place_or  (A3  &a, const A3  &b)       { a[0] |= b[0]; a[1] |= b[1]; a[2] |= b[2]; return a; }
template <> inline A4  &in_place_or  (A4  &a, const A4  &b)       { a[0] |= b[0]; a[1] |= b[1]; a[2] |= b[2]; a[3] |= b[3]; return a; }
template <> inline A5  &in_place_or  (A5  &a, const A5  &b)       { a[0] |= b[0]; a[1] |= b[1]; a[2] |= b[2]; a[3] |= b[3]; a[4] |= b[4]; return a; }
template <> inline A6  &in_place_or  (A6  &a, const A6  &b)       { a[0] |= b[0]; a[1] |= b[1]; a[2] |= b[2]; a[3] |= b[3]; a[4] |= b[4]; a[5] |= b[5]; return a; }
template <> inline A7  &in_place_or  (A7  &a, const A7  &b)       { a[0] |= b[0]; a[1] |= b[1]; a[2] |= b[2]; a[3] |= b[3]; a[4] |= b[4]; a[5] |= b[5]; a[6] |= b[6]; return a; }
template <> inline A8  &in_place_or  (A8  &a, const A8  &b)       { a[0] |= b[0]; a[1] |= b[1]; a[2] |= b[2]; a[3] |= b[3]; a[4] |= b[4]; a[5] |= b[5]; a[6] |= b[6]; a[7] |= b[7]; return a; }
template <> inline A9  &in_place_or  (A9  &a, const A9  &b)       { a[0] |= b[0]; a[1] |= b[1]; a[2] |= b[2]; a[3] |= b[3]; a[4] |= b[4]; a[5] |= b[5]; a[6] |= b[6]; a[7] |= b[7]; a[8] |= b[8]; return a; }
template <> inline A10 &in_place_or  (A10 &a, const A10 &b)       { a[0] |= b[0]; a[1] |= b[1]; a[2] |= b[2]; a[3] |= b[3]; a[4] |= b[4]; a[5] |= b[5]; a[6] |= b[6]; a[7] |= b[7]; a[8] |= b[8]; a[9] |= b[9]; return a; }
template <> inline A11 &in_place_or  (A11 &a, const A11 &b)       { a[0] |= b[0]; a[1] |= b[1]; a[2] |= b[2]; a[3] |= b[3]; a[4] |= b[4]; a[5] |= b[5]; a[6] |= b[6]; a[7] |= b[7]; a[8] |= b[8]; a[9] |= b[9]; a[10] |= b[10]; return a; }
template <> inline A12 &in_place_or  (A12 &a, const A12 &b)       { a[0] |= b[0]; a[1] |= b[1]; a[2] |= b[2]; a[3] |= b[3]; a[4] |= b[4]; a[5] |= b[5]; a[6] |= b[6]; a[7] |= b[7]; a[8] |= b[8]; a[9] |= b[9]; a[10] |= b[10]; a[11] |= b[11]; return a; }
template <> inline A13 &in_place_or  (A13 &a, const A13 &b)       { a[0] |= b[0]; a[1] |= b[1]; a[2] |= b[2]; a[3] |= b[3]; a[4] |= b[4]; a[5] |= b[5]; a[6] |= b[6]; a[7] |= b[7]; a[8] |= b[8]; a[9] |= b[9]; a[10] |= b[10]; a[11] |= b[11]; a[12] |= b[12]; return a; }
template <> inline A14 &in_place_or  (A14 &a, const A14 &b)       { a[0] |= b[0]; a[1] |= b[1]; a[2] |= b[2]; a[3] |= b[3]; a[4] |= b[4]; a[5] |= b[5]; a[6] |= b[6]; a[7] |= b[7]; a[8] |= b[8]; a[9] |= b[9]; a[10] |= b[10]; a[11] |= b[11]; a[12] |= b[12]; a[13] |= b[13]; return a; }
template <> inline A15 &in_place_or  (A15 &a, const A15 &b)       { a[0] |= b[0]; a[1] |= b[1]; a[2] |= b[2]; a[3] |= b[3]; a[4] |= b[4]; a[5] |= b[5]; a[6] |= b[6]; a[7] |= b[7]; a[8] |= b[8]; a[9] |= b[9]; a[10] |= b[10]; a[11] |= b[11]; a[12] |= b[12]; a[13] |= b[13]; a[14] |= b[14]; return a; }
template <> inline A16 &in_place_or  (A16 &a, const A16 &b)       { a[0] |= b[0]; a[1] |= b[1]; a[2] |= b[2]; a[3] |= b[3]; a[4] |= b[4]; a[5] |= b[5]; a[6] |= b[6]; a[7] |= b[7]; a[8] |= b[8]; a[9] |= b[9]; a[10] |= b[10]; a[11] |= b[11]; a[12] |= b[12]; a[13] |= b[13]; a[14] |= b[14]; a[15] |= b[15]; return a; }

template <> inline A1  &in_place_xor (A1  &a, const A1  &b)       { a[0] ^= b[0]; return a; }
template <> inline A2  &in_place_xor (A2  &a, const A2  &b)       { a[0] ^= b[0]; a[1] ^= b[1]; return a; }
template <> inline A3  &in_place_xor (A3  &a, const A3  &b)       { a[0] ^= b[0]; a[1] ^= b[1]; a[2] ^= b[2]; return a; }
template <> inline A4  &in_place_xor (A4  &a, const A4  &b)       { a[0] ^= b[0]; a[1] ^= b[1]; a[2] ^= b[2]; a[3] ^= b[3]; return a; }
template <> inline A5  &in_place_xor (A5  &a, const A5  &b)       { a[0] ^= b[0]; a[1] ^= b[1]; a[2] ^= b[2]; a[3] ^= b[3]; a[4] ^= b[4]; return a; }
template <> inline A6  &in_place_xor (A6  &a, const A6  &b)       { a[0] ^= b[0]; a[1] ^= b[1]; a[2] ^= b[2]; a[3] ^= b[3]; a[4] ^= b[4]; a[5] ^= b[5]; return a; }
template <> inline A7  &in_place_xor (A7  &a, const A7  &b)       { a[0] ^= b[0]; a[1] ^= b[1]; a[2] ^= b[2]; a[3] ^= b[3]; a[4] ^= b[4]; a[5] ^= b[5]; a[6] ^= b[6]; return a; }
template <> inline A8  &in_place_xor (A8  &a, const A8  &b)       { a[0] ^= b[0]; a[1] ^= b[1]; a[2] ^= b[2]; a[3] ^= b[3]; a[4] ^= b[4]; a[5] ^= b[5]; a[6] ^= b[6]; a[7] ^= b[7]; return a; }
template <> inline A9  &in_place_xor (A9  &a, const A9  &b)       { a[0] ^= b[0]; a[1] ^= b[1]; a[2] ^= b[2]; a[3] ^= b[3]; a[4] ^= b[4]; a[5] ^= b[5]; a[6] ^= b[6]; a[7] ^= b[7]; a[8] ^= b[8]; return a; }
template <> inline A10 &in_place_xor (A10 &a, const A10 &b)       { a[0] ^= b[0]; a[1] ^= b[1]; a[2] ^= b[2]; a[3] ^= b[3]; a[4] ^= b[4]; a[5] ^= b[5]; a[6] ^= b[6]; a[7] ^= b[7]; a[8] ^= b[8]; a[9] ^= b[9]; return a; }
template <> inline A11 &in_place_xor (A11 &a, const A11 &b)       { a[0] ^= b[0]; a[1] ^= b[1]; a[2] ^= b[2]; a[3] ^= b[3]; a[4] ^= b[4]; a[5] ^= b[5]; a[6] ^= b[6]; a[7] ^= b[7]; a[8] ^= b[8]; a[9] ^= b[9]; a[10] ^= b[10]; return a; }
template <> inline A12 &in_place_xor (A12 &a, const A12 &b)       { a[0] ^= b[0]; a[1] ^= b[1]; a[2] ^= b[2]; a[3] ^= b[3]; a[4] ^= b[4]; a[5] ^= b[5]; a[6] ^= b[6]; a[7] ^= b[7]; a[8] ^= b[8]; a[9] ^= b[9]; a[10] ^= b[10]; a[11] ^= b[11]; return a; }
template <> inline A13 &in_place_xor (A13 &a, const A13 &b)       { a[0] ^= b[0]; a[1] ^= b[1]; a[2] ^= b[2]; a[3] ^= b[3]; a[4] ^= b[4]; a[5] ^= b[5]; a[6] ^= b[6]; a[7] ^= b[7]; a[8] ^= b[8]; a[9] ^= b[9]; a[10] ^= b[10]; a[11] ^= b[11]; a[12] ^= b[12]; return a; }
template <> inline A14 &in_place_xor (A14 &a, const A14 &b)       { a[0] ^= b[0]; a[1] ^= b[1]; a[2] ^= b[2]; a[3] ^= b[3]; a[4] ^= b[4]; a[5] ^= b[5]; a[6] ^= b[6]; a[7] ^= b[7]; a[8] ^= b[8]; a[9] ^= b[9]; a[10] ^= b[10]; a[11] ^= b[11]; a[12] ^= b[12]; a[13] ^= b[13]; return a; }
template <> inline A15 &in_place_xor (A15 &a, const A15 &b)       { a[0] ^= b[0]; a[1] ^= b[1]; a[2] ^= b[2]; a[3] ^= b[3]; a[4] ^= b[4]; a[5] ^= b[5]; a[6] ^= b[6]; a[7] ^= b[7]; a[8] ^= b[8]; a[9] ^= b[9]; a[10] ^= b[10]; a[11] ^= b[11]; a[12] ^= b[12]; a[13] ^= b[13]; a[14] ^= b[14]; return a; }
template <> inline A16 &in_place_xor (A16 &a, const A16 &b)       { a[0] ^= b[0]; a[1] ^= b[1]; a[2] ^= b[2]; a[3] ^= b[3]; a[4] ^= b[4]; a[5] ^= b[5]; a[6] ^= b[6]; a[7] ^= b[7]; a[8] ^= b[8]; a[9] ^= b[9]; a[10] ^= b[10]; a[11] ^= b[11]; a[12] ^= b[12]; a[13] ^= b[13]; a[14] ^= b[14]; a[15] ^= b[15]; return a; }
#endif

template <> inline A1  &in_place_sub (A1  &a, _T b)               { a[0] -= b; return a; }
template <> inline A2  &in_place_sub (A2  &a, _T b)               { a[0] -= b; a[1] -= b; return a; }
template <> inline A3  &in_place_sub (A3  &a, _T b)               { a[0] -= b; a[1] -= b; a[2] -= b; return a; }
template <> inline A4  &in_place_sub (A4  &a, _T b)               { a[0] -= b; a[1] -= b; a[2] -= b; a[3] -= b; return a; }
template <> inline A5  &in_place_sub (A5  &a, _T b)               { a[0] -= b; a[1] -= b; a[2] -= b; a[3] -= b; a[4] -= b; return a; }
template <> inline A6  &in_place_sub (A6  &a, _T b)               { a[0] -= b; a[1] -= b; a[2] -= b; a[3] -= b; a[4] -= b; a[5] -= b; return a; }
template <> inline A7  &in_place_sub (A7  &a, _T b)               { a[0] -= b; a[1] -= b; a[2] -= b; a[3] -= b; a[4] -= b; a[5] -= b; a[6] -= b; return a; }
template <> inline A8  &in_place_sub (A8  &a, _T b)               { a[0] -= b; a[1] -= b; a[2] -= b; a[3] -= b; a[4] -= b; a[5] -= b; a[6] -= b; a[7] -= b; return a; }
template <> inline A9  &in_place_sub (A9  &a, _T b)               { a[0] -= b; a[1] -= b; a[2] -= b; a[3] -= b; a[4] -= b; a[5] -= b; a[6] -= b; a[7] -= b; a[8] -= b; return a; }
template <> inline A10 &in_place_sub (A10 &a, _T b)               { a[0] -= b; a[1] -= b; a[2] -= b; a[3] -= b; a[4] -= b; a[5] -= b; a[6] -= b; a[7] -= b; a[8] -= b; a[9] -= b; return a; }
template <> inline A11 &in_place_sub (A11 &a, _T b)               { a[0] -= b; a[1] -= b; a[2] -= b; a[3] -= b; a[4] -= b; a[5] -= b; a[6] -= b; a[7] -= b; a[8] -= b; a[9] -= b; a[10] -= b; return a; }
template <> inline A12 &in_place_sub (A12 &a, _T b)               { a[0] -= b; a[1] -= b; a[2] -= b; a[3] -= b; a[4] -= b; a[5] -= b; a[6] -= b; a[7] -= b; a[8] -= b; a[9] -= b; a[10] -= b; a[11] -= b; return a; }
template <> inline A13 &in_place_sub (A13 &a, _T b)               { a[0] -= b; a[1] -= b; a[2] -= b; a[3] -= b; a[4] -= b; a[5] -= b; a[6] -= b; a[7] -= b; a[8] -= b; a[9] -= b; a[10] -= b; a[11] -= b; a[12] -= b; return a; }
template <> inline A14 &in_place_sub (A14 &a, _T b)               { a[0] -= b; a[1] -= b; a[2] -= b; a[3] -= b; a[4] -= b; a[5] -= b; a[6] -= b; a[7] -= b; a[8] -= b; a[9] -= b; a[10] -= b; a[11] -= b; a[12] -= b; a[13] -= b; return a; }
template <> inline A15 &in_place_sub (A15 &a, _T b)               { a[0] -= b; a[1] -= b; a[2] -= b; a[3] -= b; a[4] -= b; a[5] -= b; a[6] -= b; a[7] -= b; a[8] -= b; a[9] -= b; a[10] -= b; a[11] -= b; a[12] -= b; a[13] -= b; a[14] -= b; return a; }
template <> inline A16 &in_place_sub (A16 &a, _T b)               { a[0] -= b; a[1] -= b; a[2] -= b; a[3] -= b; a[4] -= b; a[5] -= b; a[6] -= b; a[7] -= b; a[8] -= b; a[9] -= b; a[10] -= b; a[11] -= b; a[12] -= b; a[13] -= b; a[14] -= b; a[15] -= b; return a; }

template <> inline A1  &in_place_add (A1  &a, _T b)               { a[0] += b; return a; }
template <> inline A2  &in_place_add (A2  &a, _T b)               { a[0] += b; a[1] += b; return a; }
template <> inline A3  &in_place_add (A3  &a, _T b)               { a[0] += b; a[1] += b; a[2] += b; return a; }
template <> inline A4  &in_place_add (A4  &a, _T b)               { a[0] += b; a[1] += b; a[2] += b; a[3] += b; return a; }
template <> inline A5  &in_place_add (A5  &a, _T b)               { a[0] += b; a[1] += b; a[2] += b; a[3] += b; a[4] += b; return a; }
template <> inline A6  &in_place_add (A6  &a, _T b)               { a[0] += b; a[1] += b; a[2] += b; a[3] += b; a[4] += b; a[5] += b; return a; }
template <> inline A7  &in_place_add (A7  &a, _T b)               { a[0] += b; a[1] += b; a[2] += b; a[3] += b; a[4] += b; a[5] += b; a[6] += b; return a; }
template <> inline A8  &in_place_add (A8  &a, _T b)               { a[0] += b; a[1] += b; a[2] += b; a[3] += b; a[4] += b; a[5] += b; a[6] += b; a[7] += b; return a; }
template <> inline A9  &in_place_add (A9  &a, _T b)               { a[0] += b; a[1] += b; a[2] += b; a[3] += b; a[4] += b; a[5] += b; a[6] += b; a[7] += b; a[8] += b; return a; }
template <> inline A10 &in_place_add (A10 &a, _T b)               { a[0] += b; a[1] += b; a[2] += b; a[3] += b; a[4] += b; a[5] += b; a[6] += b; a[7] += b; a[8] += b; a[9] += b; return a; }
template <> inline A11 &in_place_add (A11 &a, _T b)               { a[0] += b; a[1] += b; a[2] += b; a[3] += b; a[4] += b; a[5] += b; a[6] += b; a[7] += b; a[8] += b; a[9] += b; a[10] += b; return a; }
template <> inline A12 &in_place_add (A12 &a, _T b)               { a[0] += b; a[1] += b; a[2] += b; a[3] += b; a[4] += b; a[5] += b; a[6] += b; a[7] += b; a[8] += b; a[9] += b; a[10] += b; a[11] += b; return a; }
template <> inline A13 &in_place_add (A13 &a, _T b)               { a[0] += b; a[1] += b; a[2] += b; a[3] += b; a[4] += b; a[5] += b; a[6] += b; a[7] += b; a[8] += b; a[9] += b; a[10] += b; a[11] += b; a[12] += b; return a; }
template <> inline A14 &in_place_add (A14 &a, _T b)               { a[0] += b; a[1] += b; a[2] += b; a[3] += b; a[4] += b; a[5] += b; a[6] += b; a[7] += b; a[8] += b; a[9] += b; a[10] += b; a[11] += b; a[12] += b; a[13] += b; return a; }
template <> inline A15 &in_place_add (A15 &a, _T b)               { a[0] += b; a[1] += b; a[2] += b; a[3] += b; a[4] += b; a[5] += b; a[6] += b; a[7] += b; a[8] += b; a[9] += b; a[10] += b; a[11] += b; a[12] += b; a[13] += b; a[14] += b; return a; }
template <> inline A16 &in_place_add (A16 &a, _T b)               { a[0] += b; a[1] += b; a[2] += b; a[3] += b; a[4] += b; a[5] += b; a[6] += b; a[7] += b; a[8] += b; a[9] += b; a[10] += b; a[11] += b; a[12] += b; a[13] += b; a[14] += b; a[15] += b; return a; }

template <> inline A1  &in_place_mul (A1  &a, _T b)               { a[0] *= b; return a; }
template <> inline A2  &in_place_mul (A2  &a, _T b)               { a[0] *= b; a[1] *= b; return a; }
template <> inline A3  &in_place_mul (A3  &a, _T b)               { a[0] *= b; a[1] *= b; a[2] *= b; return a; }
template <> inline A4  &in_place_mul (A4  &a, _T b)               { a[0] *= b; a[1] *= b; a[2] *= b; a[3] *= b; return a; }
template <> inline A5  &in_place_mul (A5  &a, _T b)               { a[0] *= b; a[1] *= b; a[2] *= b; a[3] *= b; a[4] *= b; return a; }
template <> inline A6  &in_place_mul (A6  &a, _T b)               { a[0] *= b; a[1] *= b; a[2] *= b; a[3] *= b; a[4] *= b; a[5] *= b; return a; }
template <> inline A7  &in_place_mul (A7  &a, _T b)               { a[0] *= b; a[1] *= b; a[2] *= b; a[3] *= b; a[4] *= b; a[5] *= b; a[6] *= b; return a; }
template <> inline A8  &in_place_mul (A8  &a, _T b)               { a[0] *= b; a[1] *= b; a[2] *= b; a[3] *= b; a[4] *= b; a[5] *= b; a[6] *= b; a[7] *= b; return a; }
template <> inline A9  &in_place_mul (A9  &a, _T b)               { a[0] *= b; a[1] *= b; a[2] *= b; a[3] *= b; a[4] *= b; a[5] *= b; a[6] *= b; a[7] *= b; a[8] *= b; return a; }
template <> inline A10 &in_place_mul (A10 &a, _T b)               { a[0] *= b; a[1] *= b; a[2] *= b; a[3] *= b; a[4] *= b; a[5] *= b; a[6] *= b; a[7] *= b; a[8] *= b; a[9] *= b; return a; }
template <> inline A11 &in_place_mul (A11 &a, _T b)               { a[0] *= b; a[1] *= b; a[2] *= b; a[3] *= b; a[4] *= b; a[5] *= b; a[6] *= b; a[7] *= b; a[8] *= b; a[9] *= b; a[10] *= b; return a; }
template <> inline A12 &in_place_mul (A12 &a, _T b)               { a[0] *= b; a[1] *= b; a[2] *= b; a[3] *= b; a[4] *= b; a[5] *= b; a[6] *= b; a[7] *= b; a[8] *= b; a[9] *= b; a[10] *= b; a[11] *= b; return a; }
template <> inline A13 &in_place_mul (A13 &a, _T b)               { a[0] *= b; a[1] *= b; a[2] *= b; a[3] *= b; a[4] *= b; a[5] *= b; a[6] *= b; a[7] *= b; a[8] *= b; a[9] *= b; a[10] *= b; a[11] *= b; a[12] *= b; return a; }
template <> inline A14 &in_place_mul (A14 &a, _T b)               { a[0] *= b; a[1] *= b; a[2] *= b; a[3] *= b; a[4] *= b; a[5] *= b; a[6] *= b; a[7] *= b; a[8] *= b; a[9] *= b; a[10] *= b; a[11] *= b; a[12] *= b; a[13] *= b; return a; }
template <> inline A15 &in_place_mul (A15 &a, _T b)               { a[0] *= b; a[1] *= b; a[2] *= b; a[3] *= b; a[4] *= b; a[5] *= b; a[6] *= b; a[7] *= b; a[8] *= b; a[9] *= b; a[10] *= b; a[11] *= b; a[12] *= b; a[13] *= b; a[14] *= b; return a; }
template <> inline A16 &in_place_mul (A16 &a, _T b)               { a[0] *= b; a[1] *= b; a[2] *= b; a[3] *= b; a[4] *= b; a[5] *= b; a[6] *= b; a[7] *= b; a[8] *= b; a[9] *= b; a[10] *= b; a[11] *= b; a[12] *= b; a[13] *= b; a[14] *= b; a[15] *= b; return a; }

template <> inline A1  &in_place_div (A1  &a, _T b)               { a[0] /= b; return a; }
template <> inline A2  &in_place_div (A2  &a, _T b)               { a[0] /= b; a[1] /= b; return a; }
template <> inline A3  &in_place_div (A3  &a, _T b)               { a[0] /= b; a[1] /= b; a[2] /= b; return a; }
template <> inline A4  &in_place_div (A4  &a, _T b)               { a[0] /= b; a[1] /= b; a[2] /= b; a[3] /= b; return a; }
template <> inline A5  &in_place_div (A5  &a, _T b)               { a[0] /= b; a[1] /= b; a[2] /= b; a[3] /= b; a[4] /= b; return a; }
template <> inline A6  &in_place_div (A6  &a, _T b)               { a[0] /= b; a[1] /= b; a[2] /= b; a[3] /= b; a[4] /= b; a[5] /= b; return a; }
template <> inline A7  &in_place_div (A7  &a, _T b)               { a[0] /= b; a[1] /= b; a[2] /= b; a[3] /= b; a[4] /= b; a[5] /= b; a[6] /= b; return a; }
template <> inline A8  &in_place_div (A8  &a, _T b)               { a[0] /= b; a[1] /= b; a[2] /= b; a[3] /= b; a[4] /= b; a[5] /= b; a[6] /= b; a[7] /= b; return a; }
template <> inline A9  &in_place_div (A9  &a, _T b)               { a[0] /= b; a[1] /= b; a[2] /= b; a[3] /= b; a[4] /= b; a[5] /= b; a[6] /= b; a[7] /= b; a[8] /= b; return a; }
template <> inline A10 &in_place_div (A10 &a, _T b)               { a[0] /= b; a[1] /= b; a[2] /= b; a[3] /= b; a[4] /= b; a[5] /= b; a[6] /= b; a[7] /= b; a[8] /= b; a[9] /= b; return a; }
template <> inline A11 &in_place_div (A11 &a, _T b)               { a[0] /= b; a[1] /= b; a[2] /= b; a[3] /= b; a[4] /= b; a[5] /= b; a[6] /= b; a[7] /= b; a[8] /= b; a[9] /= b; a[10] /= b; return a; }
template <> inline A12 &in_place_div (A12 &a, _T b)               { a[0] /= b; a[1] /= b; a[2] /= b; a[3] /= b; a[4] /= b; a[5] /= b; a[6] /= b; a[7] /= b; a[8] /= b; a[9] /= b; a[10] /= b; a[11] /= b; return a; }
template <> inline A13 &in_place_div (A13 &a, _T b)               { a[0] /= b; a[1] /= b; a[2] /= b; a[3] /= b; a[4] /= b; a[5] /= b; a[6] /= b; a[7] /= b; a[8] /= b; a[9] /= b; a[10] /= b; a[11] /= b; a[12] /= b; return a; }
template <> inline A14 &in_place_div (A14 &a, _T b)               { a[0] /= b; a[1] /= b; a[2] /= b; a[3] /= b; a[4] /= b; a[5] /= b; a[6] /= b; a[7] /= b; a[8] /= b; a[9] /= b; a[10] /= b; a[11] /= b; a[12] /= b; a[13] /= b; return a; }
template <> inline A15 &in_place_div (A15 &a, _T b)               { a[0] /= b; a[1] /= b; a[2] /= b; a[3] /= b; a[4] /= b; a[5] /= b; a[6] /= b; a[7] /= b; a[8] /= b; a[9] /= b; a[10] /= b; a[11] /= b; a[12] /= b; a[13] /= b; a[14] /= b; return a; }
template <> inline A16 &in_place_div (A16 &a, _T b)               { a[0] /= b; a[1] /= b; a[2] /= b; a[3] /= b; a[4] /= b; a[5] /= b; a[6] /= b; a[7] /= b; a[8] /= b; a[9] /= b; a[10] /= b; a[11] /= b; a[12] /= b; a[13] /= b; a[14] /= b; a[15] /= b; return a; }

#if _T_INT == 1
template <> inline A1  &in_place_mod (A1  &a, _T b)               { a[0] %= b; return a; }
template <> inline A2  &in_place_mod (A2  &a, _T b)               { a[0] %= b; a[1] %= b; return a; }
template <> inline A3  &in_place_mod (A3  &a, _T b)               { a[0] %= b; a[1] %= b; a[2] %= b; return a; }
template <> inline A4  &in_place_mod (A4  &a, _T b)               { a[0] %= b; a[1] %= b; a[2] %= b; a[3] %= b; return a; }
template <> inline A5  &in_place_mod (A5  &a, _T b)               { a[0] %= b; a[1] %= b; a[2] %= b; a[3] %= b; a[4] %= b; return a; }
template <> inline A6  &in_place_mod (A6  &a, _T b)               { a[0] %= b; a[1] %= b; a[2] %= b; a[3] %= b; a[4] %= b; a[5] %= b; return a; }
template <> inline A7  &in_place_mod (A7  &a, _T b)               { a[0] %= b; a[1] %= b; a[2] %= b; a[3] %= b; a[4] %= b; a[5] %= b; a[6] %= b; return a; }
template <> inline A8  &in_place_mod (A8  &a, _T b)               { a[0] %= b; a[1] %= b; a[2] %= b; a[3] %= b; a[4] %= b; a[5] %= b; a[6] %= b; a[7] %= b; return a; }
template <> inline A9  &in_place_mod (A9  &a, _T b)               { a[0] %= b; a[1] %= b; a[2] %= b; a[3] %= b; a[4] %= b; a[5] %= b; a[6] %= b; a[7] %= b; a[8] %= b; return a; }
template <> inline A10 &in_place_mod (A10 &a, _T b)               { a[0] %= b; a[1] %= b; a[2] %= b; a[3] %= b; a[4] %= b; a[5] %= b; a[6] %= b; a[7] %= b; a[8] %= b; a[9] %= b; return a; }
template <> inline A11 &in_place_mod (A11 &a, _T b)               { a[0] %= b; a[1] %= b; a[2] %= b; a[3] %= b; a[4] %= b; a[5] %= b; a[6] %= b; a[7] %= b; a[8] %= b; a[9] %= b; a[10] %= b; return a; }
template <> inline A12 &in_place_mod (A12 &a, _T b)               { a[0] %= b; a[1] %= b; a[2] %= b; a[3] %= b; a[4] %= b; a[5] %= b; a[6] %= b; a[7] %= b; a[8] %= b; a[9] %= b; a[10] %= b; a[11] %= b; return a; }
template <> inline A13 &in_place_mod (A13 &a, _T b)               { a[0] %= b; a[1] %= b; a[2] %= b; a[3] %= b; a[4] %= b; a[5] %= b; a[6] %= b; a[7] %= b; a[8] %= b; a[9] %= b; a[10] %= b; a[11] %= b; a[12] %= b; return a; }
template <> inline A14 &in_place_mod (A14 &a, _T b)               { a[0] %= b; a[1] %= b; a[2] %= b; a[3] %= b; a[4] %= b; a[5] %= b; a[6] %= b; a[7] %= b; a[8] %= b; a[9] %= b; a[10] %= b; a[11] %= b; a[12] %= b; a[13] %= b; return a; }
template <> inline A15 &in_place_mod (A15 &a, _T b)               { a[0] %= b; a[1] %= b; a[2] %= b; a[3] %= b; a[4] %= b; a[5] %= b; a[6] %= b; a[7] %= b; a[8] %= b; a[9] %= b; a[10] %= b; a[11] %= b; a[12] %= b; a[13] %= b; a[14] %= b; return a; }
template <> inline A16 &in_place_mod (A16 &a, _T b)               { a[0] %= b; a[1] %= b; a[2] %= b; a[3] %= b; a[4] %= b; a[5] %= b; a[6] %= b; a[7] %= b; a[8] %= b; a[9] %= b; a[10] %= b; a[11] %= b; a[12] %= b; a[13] %= b; a[14] %= b; a[15] %= b; return a; }

template <> inline A1  &in_place_and (A1  &a, _T b)               { a[0] &= b; return a; }
template <> inline A2  &in_place_and (A2  &a, _T b)               { a[0] &= b; a[1] &= b; return a; }
template <> inline A3  &in_place_and (A3  &a, _T b)               { a[0] &= b; a[1] &= b; a[2] &= b; return a; }
template <> inline A4  &in_place_and (A4  &a, _T b)               { a[0] &= b; a[1] &= b; a[2] &= b; a[3] &= b; return a; }
template <> inline A5  &in_place_and (A5  &a, _T b)               { a[0] &= b; a[1] &= b; a[2] &= b; a[3] &= b; a[4] &= b; return a; }
template <> inline A6  &in_place_and (A6  &a, _T b)               { a[0] &= b; a[1] &= b; a[2] &= b; a[3] &= b; a[4] &= b; a[5] &= b; return a; }
template <> inline A7  &in_place_and (A7  &a, _T b)               { a[0] &= b; a[1] &= b; a[2] &= b; a[3] &= b; a[4] &= b; a[5] &= b; a[6] &= b; return a; }
template <> inline A8  &in_place_and (A8  &a, _T b)               { a[0] &= b; a[1] &= b; a[2] &= b; a[3] &= b; a[4] &= b; a[5] &= b; a[6] &= b; a[7] &= b; return a; }
template <> inline A9  &in_place_and (A9  &a, _T b)               { a[0] &= b; a[1] &= b; a[2] &= b; a[3] &= b; a[4] &= b; a[5] &= b; a[6] &= b; a[7] &= b; a[8] &= b; return a; }
template <> inline A10 &in_place_and (A10 &a, _T b)               { a[0] &= b; a[1] &= b; a[2] &= b; a[3] &= b; a[4] &= b; a[5] &= b; a[6] &= b; a[7] &= b; a[8] &= b; a[9] &= b; return a; }
template <> inline A11 &in_place_and (A11 &a, _T b)               { a[0] &= b; a[1] &= b; a[2] &= b; a[3] &= b; a[4] &= b; a[5] &= b; a[6] &= b; a[7] &= b; a[8] &= b; a[9] &= b; a[10] &= b; return a; }
template <> inline A12 &in_place_and (A12 &a, _T b)               { a[0] &= b; a[1] &= b; a[2] &= b; a[3] &= b; a[4] &= b; a[5] &= b; a[6] &= b; a[7] &= b; a[8] &= b; a[9] &= b; a[10] &= b; a[11] &= b; return a; }
template <> inline A13 &in_place_and (A13 &a, _T b)               { a[0] &= b; a[1] &= b; a[2] &= b; a[3] &= b; a[4] &= b; a[5] &= b; a[6] &= b; a[7] &= b; a[8] &= b; a[9] &= b; a[10] &= b; a[11] &= b; a[12] &= b; return a; }
template <> inline A14 &in_place_and (A14 &a, _T b)               { a[0] &= b; a[1] &= b; a[2] &= b; a[3] &= b; a[4] &= b; a[5] &= b; a[6] &= b; a[7] &= b; a[8] &= b; a[9] &= b; a[10] &= b; a[11] &= b; a[12] &= b; a[13] &= b; return a; }
template <> inline A15 &in_place_and (A15 &a, _T b)               { a[0] &= b; a[1] &= b; a[2] &= b; a[3] &= b; a[4] &= b; a[5] &= b; a[6] &= b; a[7] &= b; a[8] &= b; a[9] &= b; a[10] &= b; a[11] &= b; a[12] &= b; a[13] &= b; a[14] &= b; return a; }
template <> inline A16 &in_place_and (A16 &a, _T b)               { a[0] &= b; a[1] &= b; a[2] &= b; a[3] &= b; a[4] &= b; a[5] &= b; a[6] &= b; a[7] &= b; a[8] &= b; a[9] &= b; a[10] &= b; a[11] &= b; a[12] &= b; a[13] &= b; a[14] &= b; a[15] &= b; return a; }

template <> inline A1  &in_place_or  (A1  &a, _T b)               { a[0] |= b; return a; }
template <> inline A2  &in_place_or  (A2  &a, _T b)               { a[0] |= b; a[1] |= b; return a; }
template <> inline A3  &in_place_or  (A3  &a, _T b)               { a[0] |= b; a[1] |= b; a[2] |= b; return a; }
template <> inline A4  &in_place_or  (A4  &a, _T b)               { a[0] |= b; a[1] |= b; a[2] |= b; a[3] |= b; return a; }
template <> inline A5  &in_place_or  (A5  &a, _T b)               { a[0] |= b; a[1] |= b; a[2] |= b; a[3] |= b; a[4] |= b; return a; }
template <> inline A6  &in_place_or  (A6  &a, _T b)               { a[0] |= b; a[1] |= b; a[2] |= b; a[3] |= b; a[4] |= b; a[5] |= b; return a; }
template <> inline A7  &in_place_or  (A7  &a, _T b)               { a[0] |= b; a[1] |= b; a[2] |= b; a[3] |= b; a[4] |= b; a[5] |= b; a[6] |= b; return a; }
template <> inline A8  &in_place_or  (A8  &a, _T b)               { a[0] |= b; a[1] |= b; a[2] |= b; a[3] |= b; a[4] |= b; a[5] |= b; a[6] |= b; a[7] |= b; return a; }
template <> inline A9  &in_place_or  (A9  &a, _T b)               { a[0] |= b; a[1] |= b; a[2] |= b; a[3] |= b; a[4] |= b; a[5] |= b; a[6] |= b; a[7] |= b; a[8] |= b; return a; }
template <> inline A10 &in_place_or  (A10 &a, _T b)               { a[0] |= b; a[1] |= b; a[2] |= b; a[3] |= b; a[4] |= b; a[5] |= b; a[6] |= b; a[7] |= b; a[8] |= b; a[9] |= b; return a; }
template <> inline A11 &in_place_or  (A11 &a, _T b)               { a[0] |= b; a[1] |= b; a[2] |= b; a[3] |= b; a[4] |= b; a[5] |= b; a[6] |= b; a[7] |= b; a[8] |= b; a[9] |= b; a[10] |= b; return a; }
template <> inline A12 &in_place_or  (A12 &a, _T b)               { a[0] |= b; a[1] |= b; a[2] |= b; a[3] |= b; a[4] |= b; a[5] |= b; a[6] |= b; a[7] |= b; a[8] |= b; a[9] |= b; a[10] |= b; a[11] |= b; return a; }
template <> inline A13 &in_place_or  (A13 &a, _T b)               { a[0] |= b; a[1] |= b; a[2] |= b; a[3] |= b; a[4] |= b; a[5] |= b; a[6] |= b; a[7] |= b; a[8] |= b; a[9] |= b; a[10] |= b; a[11] |= b; a[12] |= b; return a; }
template <> inline A14 &in_place_or  (A14 &a, _T b)               { a[0] |= b; a[1] |= b; a[2] |= b; a[3] |= b; a[4] |= b; a[5] |= b; a[6] |= b; a[7] |= b; a[8] |= b; a[9] |= b; a[10] |= b; a[11] |= b; a[12] |= b; a[13] |= b; return a; }
template <> inline A15 &in_place_or  (A15 &a, _T b)               { a[0] |= b; a[1] |= b; a[2] |= b; a[3] |= b; a[4] |= b; a[5] |= b; a[6] |= b; a[7] |= b; a[8] |= b; a[9] |= b; a[10] |= b; a[11] |= b; a[12] |= b; a[13] |= b; a[14] |= b; return a; }
template <> inline A16 &in_place_or  (A16 &a, _T b)               { a[0] |= b; a[1] |= b; a[2] |= b; a[3] |= b; a[4] |= b; a[5] |= b; a[6] |= b; a[7] |= b; a[8] |= b; a[9] |= b; a[10] |= b; a[11] |= b; a[12] |= b; a[13] |= b; a[14] |= b; a[15] |= b; return a; }

template <> inline A1  &in_place_xor (A1  &a, _T b)               { a[0] ^= b; return a; }
template <> inline A2  &in_place_xor (A2  &a, _T b)               { a[0] ^= b; a[1] ^= b; return a; }
template <> inline A3  &in_place_xor (A3  &a, _T b)               { a[0] ^= b; a[1] ^= b; a[2] ^= b; return a; }
template <> inline A4  &in_place_xor (A4  &a, _T b)               { a[0] ^= b; a[1] ^= b; a[2] ^= b; a[3] ^= b; return a; }
template <> inline A5  &in_place_xor (A5  &a, _T b)               { a[0] ^= b; a[1] ^= b; a[2] ^= b; a[3] ^= b; a[4] ^= b; return a; }
template <> inline A6  &in_place_xor (A6  &a, _T b)               { a[0] ^= b; a[1] ^= b; a[2] ^= b; a[3] ^= b; a[4] ^= b; a[5] ^= b; return a; }
template <> inline A7  &in_place_xor (A7  &a, _T b)               { a[0] ^= b; a[1] ^= b; a[2] ^= b; a[3] ^= b; a[4] ^= b; a[5] ^= b; a[6] ^= b; return a; }
template <> inline A8  &in_place_xor (A8  &a, _T b)               { a[0] ^= b; a[1] ^= b; a[2] ^= b; a[3] ^= b; a[4] ^= b; a[5] ^= b; a[6] ^= b; a[7] ^= b; return a; }
template <> inline A9  &in_place_xor (A9  &a, _T b)               { a[0] ^= b; a[1] ^= b; a[2] ^= b; a[3] ^= b; a[4] ^= b; a[5] ^= b; a[6] ^= b; a[7] ^= b; a[8] ^= b; return a; }
template <> inline A10 &in_place_xor (A10 &a, _T b)               { a[0] ^= b; a[1] ^= b; a[2] ^= b; a[3] ^= b; a[4] ^= b; a[5] ^= b; a[6] ^= b; a[7] ^= b; a[8] ^= b; a[9] ^= b; return a; }
template <> inline A11 &in_place_xor (A11 &a, _T b)               { a[0] ^= b; a[1] ^= b; a[2] ^= b; a[3] ^= b; a[4] ^= b; a[5] ^= b; a[6] ^= b; a[7] ^= b; a[8] ^= b; a[9] ^= b; a[10] ^= b; return a; }
template <> inline A12 &in_place_xor (A12 &a, _T b)               { a[0] ^= b; a[1] ^= b; a[2] ^= b; a[3] ^= b; a[4] ^= b; a[5] ^= b; a[6] ^= b; a[7] ^= b; a[8] ^= b; a[9] ^= b; a[10] ^= b; a[11] ^= b; return a; }
template <> inline A13 &in_place_xor (A13 &a, _T b)               { a[0] ^= b; a[1] ^= b; a[2] ^= b; a[3] ^= b; a[4] ^= b; a[5] ^= b; a[6] ^= b; a[7] ^= b; a[8] ^= b; a[9] ^= b; a[10] ^= b; a[11] ^= b; a[12] ^= b; return a; }
template <> inline A14 &in_place_xor (A14 &a, _T b)               { a[0] ^= b; a[1] ^= b; a[2] ^= b; a[3] ^= b; a[4] ^= b; a[5] ^= b; a[6] ^= b; a[7] ^= b; a[8] ^= b; a[9] ^= b; a[10] ^= b; a[11] ^= b; a[12] ^= b; a[13] ^= b; return a; }
template <> inline A15 &in_place_xor (A15 &a, _T b)               { a[0] ^= b; a[1] ^= b; a[2] ^= b; a[3] ^= b; a[4] ^= b; a[5] ^= b; a[6] ^= b; a[7] ^= b; a[8] ^= b; a[9] ^= b; a[10] ^= b; a[11] ^= b; a[12] ^= b; a[13] ^= b; a[14] ^= b; return a; }
template <> inline A16 &in_place_xor (A16 &a, _T b)               { a[0] ^= b; a[1] ^= b; a[2] ^= b; a[3] ^= b; a[4] ^= b; a[5] ^= b; a[6] ^= b; a[7] ^= b; a[8] ^= b; a[9] ^= b; a[10] ^= b; a[11] ^= b; a[12] ^= b; a[13] ^= b; a[14] ^= b; a[15] ^= b; return a; }

template <> inline A1  &in_place_shl (A1  &a, size_t b)           { a[0] <<= b; return a; }
template <> inline A2  &in_place_shl (A2  &a, size_t b)           { a[0] <<= b; a[1] <<= b; return a; }
template <> inline A3  &in_place_shl (A3  &a, size_t b)           { a[0] <<= b; a[1] <<= b; a[2] <<= b; return a; }
template <> inline A4  &in_place_shl (A4  &a, size_t b)           { a[0] <<= b; a[1] <<= b; a[2] <<= b; a[3] <<= b; return a; }
template <> inline A5  &in_place_shl (A5  &a, size_t b)           { a[0] <<= b; a[1] <<= b; a[2] <<= b; a[3] <<= b; a[4] <<= b; return a; }
template <> inline A6  &in_place_shl (A6  &a, size_t b)           { a[0] <<= b; a[1] <<= b; a[2] <<= b; a[3] <<= b; a[4] <<= b; a[5] <<= b; return a; }
template <> inline A7  &in_place_shl (A7  &a, size_t b)           { a[0] <<= b; a[1] <<= b; a[2] <<= b; a[3] <<= b; a[4] <<= b; a[5] <<= b; a[6] <<= b; return a; }
template <> inline A8  &in_place_shl (A8  &a, size_t b)           { a[0] <<= b; a[1] <<= b; a[2] <<= b; a[3] <<= b; a[4] <<= b; a[5] <<= b; a[6] <<= b; a[7] <<= b; return a; }
template <> inline A9  &in_place_shl (A9  &a, size_t b)           { a[0] <<= b; a[1] <<= b; a[2] <<= b; a[3] <<= b; a[4] <<= b; a[5] <<= b; a[6] <<= b; a[7] <<= b; a[8] <<= b; return a; }
template <> inline A10 &in_place_shl (A10 &a, size_t b)           { a[0] <<= b; a[1] <<= b; a[2] <<= b; a[3] <<= b; a[4] <<= b; a[5] <<= b; a[6] <<= b; a[7] <<= b; a[8] <<= b; a[9] <<= b; return a; }
template <> inline A11 &in_place_shl (A11 &a, size_t b)           { a[0] <<= b; a[1] <<= b; a[2] <<= b; a[3] <<= b; a[4] <<= b; a[5] <<= b; a[6] <<= b; a[7] <<= b; a[8] <<= b; a[9] <<= b; a[10] <<= b; return a; }
template <> inline A12 &in_place_shl (A12 &a, size_t b)           { a[0] <<= b; a[1] <<= b; a[2] <<= b; a[3] <<= b; a[4] <<= b; a[5] <<= b; a[6] <<= b; a[7] <<= b; a[8] <<= b; a[9] <<= b; a[10] <<= b; a[11] <<= b; return a; }
template <> inline A13 &in_place_shl (A13 &a, size_t b)           { a[0] <<= b; a[1] <<= b; a[2] <<= b; a[3] <<= b; a[4] <<= b; a[5] <<= b; a[6] <<= b; a[7] <<= b; a[8] <<= b; a[9] <<= b; a[10] <<= b; a[11] <<= b; a[12] <<= b; return a; }
template <> inline A14 &in_place_shl (A14 &a, size_t b)           { a[0] <<= b; a[1] <<= b; a[2] <<= b; a[3] <<= b; a[4] <<= b; a[5] <<= b; a[6] <<= b; a[7] <<= b; a[8] <<= b; a[9] <<= b; a[10] <<= b; a[11] <<= b; a[12] <<= b; a[13] <<= b; return a; }
template <> inline A15 &in_place_shl (A15 &a, size_t b)           { a[0] <<= b; a[1] <<= b; a[2] <<= b; a[3] <<= b; a[4] <<= b; a[5] <<= b; a[6] <<= b; a[7] <<= b; a[8] <<= b; a[9] <<= b; a[10] <<= b; a[11] <<= b; a[12] <<= b; a[13] <<= b; a[14] <<= b; return a; }
template <> inline A16 &in_place_shl (A16 &a, size_t b)           { a[0] <<= b; a[1] <<= b; a[2] <<= b; a[3] <<= b; a[4] <<= b; a[5] <<= b; a[6] <<= b; a[7] <<= b; a[8] <<= b; a[9] <<= b; a[10] <<= b; a[11] <<= b; a[12] <<= b; a[13] <<= b; a[14] <<= b; a[15] <<= b; return a; }

template <> inline A1  &in_place_shr (A1  &a, size_t b)           { a[0] >>= b; return a; }
template <> inline A2  &in_place_shr (A2  &a, size_t b)           { a[0] >>= b; a[1] >>= b; return a; }
template <> inline A3  &in_place_shr (A3  &a, size_t b)           { a[0] >>= b; a[1] >>= b; a[2] >>= b; return a; }
template <> inline A4  &in_place_shr (A4  &a, size_t b)           { a[0] >>= b; a[1] >>= b; a[2] >>= b; a[3] >>= b; return a; }
template <> inline A5  &in_place_shr (A5  &a, size_t b)           { a[0] >>= b; a[1] >>= b; a[2] >>= b; a[3] >>= b; a[4] >>= b; return a; }
template <> inline A6  &in_place_shr (A6  &a, size_t b)           { a[0] >>= b; a[1] >>= b; a[2] >>= b; a[3] >>= b; a[4] >>= b; a[5] >>= b; return a; }
template <> inline A7  &in_place_shr (A7  &a, size_t b)           { a[0] >>= b; a[1] >>= b; a[2] >>= b; a[3] >>= b; a[4] >>= b; a[5] >>= b; a[6] >>= b; return a; }
template <> inline A8  &in_place_shr (A8  &a, size_t b)           { a[0] >>= b; a[1] >>= b; a[2] >>= b; a[3] >>= b; a[4] >>= b; a[5] >>= b; a[6] >>= b; a[7] >>= b; return a; }
template <> inline A9  &in_place_shr (A9  &a, size_t b)           { a[0] >>= b; a[1] >>= b; a[2] >>= b; a[3] >>= b; a[4] >>= b; a[5] >>= b; a[6] >>= b; a[7] >>= b; a[8] >>= b; return a; }
template <> inline A10 &in_place_shr (A10 &a, size_t b)           { a[0] >>= b; a[1] >>= b; a[2] >>= b; a[3] >>= b; a[4] >>= b; a[5] >>= b; a[6] >>= b; a[7] >>= b; a[8] >>= b; a[9] >>= b; return a; }
template <> inline A11 &in_place_shr (A11 &a, size_t b)           { a[0] >>= b; a[1] >>= b; a[2] >>= b; a[3] >>= b; a[4] >>= b; a[5] >>= b; a[6] >>= b; a[7] >>= b; a[8] >>= b; a[9] >>= b; a[10] >>= b; return a; }
template <> inline A12 &in_place_shr (A12 &a, size_t b)           { a[0] >>= b; a[1] >>= b; a[2] >>= b; a[3] >>= b; a[4] >>= b; a[5] >>= b; a[6] >>= b; a[7] >>= b; a[8] >>= b; a[9] >>= b; a[10] >>= b; a[11] >>= b; return a; }
template <> inline A13 &in_place_shr (A13 &a, size_t b)           { a[0] >>= b; a[1] >>= b; a[2] >>= b; a[3] >>= b; a[4] >>= b; a[5] >>= b; a[6] >>= b; a[7] >>= b; a[8] >>= b; a[9] >>= b; a[10] >>= b; a[11] >>= b; a[12] >>= b; return a; }
template <> inline A14 &in_place_shr (A14 &a, size_t b)           { a[0] >>= b; a[1] >>= b; a[2] >>= b; a[3] >>= b; a[4] >>= b; a[5] >>= b; a[6] >>= b; a[7] >>= b; a[8] >>= b; a[9] >>= b; a[10] >>= b; a[11] >>= b; a[12] >>= b; a[13] >>= b; return a; }
template <> inline A15 &in_place_shr (A15 &a, size_t b)           { a[0] >>= b; a[1] >>= b; a[2] >>= b; a[3] >>= b; a[4] >>= b; a[5] >>= b; a[6] >>= b; a[7] >>= b; a[8] >>= b; a[9] >>= b; a[10] >>= b; a[11] >>= b; a[12] >>= b; a[13] >>= b; a[14] >>= b; return a; }
template <> inline A16 &in_place_shr (A16 &a, size_t b)           { a[0] >>= b; a[1] >>= b; a[2] >>= b; a[3] >>= b; a[4] >>= b; a[5] >>= b; a[6] >>= b; a[7] >>= b; a[8] >>= b; a[9] >>= b; a[10] >>= b; a[11] >>= b; a[12] >>= b; a[13] >>= b; a[14] >>= b; a[15] >>= b; return a; }
#endif

#undef A1
#undef A2
#undef A3
#undef A4
#undef A5
#undef A6
#undef A7
#undef A8
#undef A9
#undef A10
#undef A11
#undef A12
#undef A13
#undef A14
#undef A15
#undef A16
