#pragma once

#include <array>

#define SWIZZLE4(NAME, I0, I1, I2, I3) \
template <typename T, size_t N> constexpr std::array<T, 4> \
NAME(const std::array<T, N> &a) { return std::array<T, 4> { a[I0], a[I1], a[I2], a[I3] }; }

#define SWIZZLE3(NAME, I0, I1, I2) \
template <typename T, size_t N> constexpr std::array<T, 3> \
NAME(const std::array<T, N> &a) { return std::array<T, 3> { a[I0], a[I1], a[I2] }; }

#define SWIZZLE2(NAME, I0, I1) \
template <typename T, size_t N> constexpr std::array<T, 2> \
NAME(const std::array<T, N> &a) { return std::array<T, 2> { a[I0], a[I1] }; }

#define SWIZZLE1(NAME, I0) \
template <typename T, size_t N> constexpr T NAME(const std::array<T, N> &a) { return a[I0]; }

SWIZZLE4(wxzy, 3, 0, 2, 1)  SWIZZLE4(xyzw, 0, 1, 2, 3)  SWIZZLE4(yzwx, 1, 2, 3, 0)  SWIZZLE4(zyxw, 2, 1, 0, 3)
SWIZZLE4(zxyw, 2, 0, 1, 3)  SWIZZLE4(zwxy, 2, 3, 0, 1)  SWIZZLE4(zxwy, 2, 0, 3, 1)  SWIZZLE4(zwyx, 2, 3, 1, 0)
SWIZZLE4(wzyx, 3, 2, 1, 0)  SWIZZLE4(ywxz, 1, 3, 0, 2)  SWIZZLE4(xzwy, 0, 2, 3, 1)  SWIZZLE4(wyxz, 3, 1, 0, 2)
SWIZZLE4(yzxw, 1, 2, 0, 3)  SWIZZLE4(zywx, 2, 1, 3, 0)  SWIZZLE4(wxyz, 3, 0, 1, 2)  SWIZZLE4(yxzw, 1, 0, 2, 3)
SWIZZLE4(yxwz, 1, 0, 3, 2)  SWIZZLE4(ywzx, 1, 3, 2, 0)  SWIZZLE4(xzyw, 0, 2, 1, 3)  SWIZZLE4(xwzy, 0, 3, 2, 1)
SWIZZLE4(wyzx, 3, 1, 2, 0)  SWIZZLE4(xwyz, 0, 3, 1, 2)  SWIZZLE4(wzxy, 3, 2, 0, 1)  SWIZZLE4(xywz, 0, 1, 3, 2)

SWIZZLE3(zwx,  2, 3, 0)     SWIZZLE3(wzx,  3, 2, 0)     SWIZZLE3(yxw,  1, 0, 3)     SWIZZLE3(ywx,  1, 3, 0)
SWIZZLE3(zxw,  2, 0, 3)     SWIZZLE3(ywz,  1, 3, 2)     SWIZZLE3(zyw,  2, 1, 3)     SWIZZLE3(zwy,  2, 3, 1)
SWIZZLE3(wyx,  3, 1, 0)     SWIZZLE3(wxy,  3, 0, 1)     SWIZZLE3(zyx,  2, 1, 0)     SWIZZLE3(yzw,  1, 2, 3)
SWIZZLE3(wxz,  3, 0, 2)     SWIZZLE3(xzy,  0, 2, 1)     SWIZZLE3(xyz,  0, 1, 2)     SWIZZLE3(xyw,  0, 1, 3)
SWIZZLE3(xwz,  0, 3, 2)     SWIZZLE3(yzx,  1, 2, 0)     SWIZZLE3(wyz,  3, 1, 2)     SWIZZLE3(zxy,  2, 0, 1)
SWIZZLE3(wzy,  3, 2, 1)     SWIZZLE3(xzw,  0, 2, 3)     SWIZZLE3(xwy,  0, 3, 1)     SWIZZLE3(yxz,  1, 0, 2)

SWIZZLE2(zx,   2, 0)        SWIZZLE2(wz,   3, 2)        SWIZZLE2(yw,   1, 3)        SWIZZLE2(zw,   2, 3)
SWIZZLE2(yx,   1, 0)        SWIZZLE2(wy,   3, 1)        SWIZZLE2(xw,   0, 3)        SWIZZLE2(wx,   3, 0)
SWIZZLE2(zy,   2, 1)        SWIZZLE2(xy,   0, 1)        SWIZZLE2(yz,   1, 2)        SWIZZLE2(xz,   0, 2)

SWIZZLE1(y,    1)           SWIZZLE1(x,    0)           SWIZZLE1(z,    2)           SWIZZLE1(w,    3)

#undef SWIZZLE1
#undef SWIZZLE2
#undef SWIZZLE3
#undef SWIZZLE4
