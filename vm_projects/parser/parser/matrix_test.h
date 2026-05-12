#pragma once

#include <array>
#include <iostream>

template <typename T, size_t N>
using Row = std::array<T, N>;

template <typename T, size_t N, size_t M = N>
using Matrix = std::array<Row<T, N>, M>;

template <typename T, size_t N, size_t M = N>
std::ostream &print(std::ostream &os, const Matrix<T, N, M> &m)
{
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            os << m[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}

#include <immintrin.h>

template <typename T>
constexpr Matrix<T, 3, 3> matrix_mul(const Matrix<T, 3, 4> &a, const Matrix<T, 4, 3> &b)
{
    return Matrix<T, 3, 3> {
        Row<T, 3> {
            a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0] + a[0][3] * b[3][0],
            a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1] + a[0][3] * b[3][1],
            a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2] + a[0][3] * b[3][2],
        },
        Row<T, 3> {
            a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0] + a[1][3] * b[3][0],
            a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1] + a[1][3] * b[3][1],
            a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2] + a[1][3] * b[3][2],
        },
        Row<T, 3> {
            a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0] + a[2][3] * b[3][0],
            a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1] + a[2][3] * b[3][1],
            a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2] + a[2][3] * b[3][2],
        }
    };
}

inline Matrix<float, 3, 3> matrix_mul_simd(const Matrix<float, 3, 4> &a, const Matrix<float, 4, 3> &b)
{
    Matrix<float, 3, 3> result;

    for (int i = 0; i < 3; ++i) {
        // Load a row of matrix a
        __m128 a_row = _mm_loadu_ps(a[i].data());

        for (int j = 0; j < 3; ++j) {
            // Load a column of matrix b
            __m128 b_col = _mm_set_ps(b[3][j], b[2][j], b[1][j], b[0][j]);

            // Perform the dot product of a_row and b_col
            __m128 mul = _mm_mul_ps(a_row, b_col);

            // Sum the elements of the result
            __m128 shuf1 = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 1, 0, 3));
            __m128 sums1 = _mm_add_ps(mul, shuf1);
            __m128 shuf2 = _mm_shuffle_ps(sums1, sums1, _MM_SHUFFLE(1, 0, 3, 2));
            __m128 sums2 = _mm_add_ps(sums1, shuf2);

            // Store the result
            _mm_store_ss(&result[i][j], sums2);
        }
    }
    return result;
}

int matrix_test()
{
    typedef Row<float, 3> row3_t;
    typedef Row<float, 4> row4_t;

    Matrix<float, 3, 4> a = {row3_t {1, 1, 1}, row3_t {1, 1, 1}, row3_t {1, 1, 1}, row3_t {1, 1, 1}};
    Matrix<float, 4, 3> b = {row4_t {1, 1, 1, 1}, row4_t {1, 1, 1, 1}, row4_t {1, 1, 1, 1}};

    auto c = matrix_mul(a, b);
    print(std::cout, c);
    
    c = matrix_mul_simd(a, b);
    print(std::cout, c);

    return 0;
}
