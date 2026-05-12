#pragma once

#include <array>
#include <chrono>
#include <iostream>
#include <immintrin.h>

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

template <typename T, size_t N, size_t M = N>
__attribute__((noinline)) auto matrix_mul_rolled(const Matrix<T, N, M> &a, const Matrix<T, M, N> &b)
{
    Matrix<T, std::min(N, M), std::min(M, N)> c;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            for (size_t k = 0; k < M; ++k) {
                c[i][j] = a[i][k] * b[k][j];
            }
        }
    }
    return c;
}

// probably don't even need to unroll these, but can't get constexpr if I don't
template <typename T>
__attribute__((noinline)) auto matrix_mul(const Matrix<T, 3, 4> &a, const Matrix<T, 4, 3> &b)
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

template <typename T>
__attribute__((noinline)) auto matrix_mul(const Matrix<T, 4, 4> &a, const Matrix<T, 4, 4> &b)
{
    return Matrix<T, 4, 4> {
        Row<T, 4> {
            a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0] + a[0][3] * b[3][0],
            a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1] + a[0][3] * b[3][1],
            a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2] + a[0][3] * b[3][2],
            a[0][0] * b[0][3] + a[0][1] * b[1][3] + a[0][2] * b[2][3] + a[0][3] * b[3][3],
        },
        Row<T, 4> {
            a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0] + a[1][3] * b[3][0],
            a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1] + a[1][3] * b[3][1],
            a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2] + a[1][3] * b[3][2],
            a[1][0] * b[0][3] + a[1][1] * b[1][3] + a[1][2] * b[2][3] + a[1][3] * b[3][3],
        },
        Row<T, 4> {
            a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0] + a[2][3] * b[3][0],
            a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1] + a[2][3] * b[3][1],
            a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2] + a[2][3] * b[3][2],
            a[2][0] * b[0][3] + a[2][1] * b[1][3] + a[2][2] * b[2][3] + a[2][3] * b[3][3],
        },
        Row<T, 4> {
            a[3][0] * b[0][0] + a[3][1] * b[1][0] + a[3][2] * b[2][0] + a[3][3] * b[3][0],
            a[3][0] * b[0][1] + a[3][1] * b[1][1] + a[3][2] * b[2][1] + a[3][3] * b[3][1],
            a[3][0] * b[0][2] + a[3][1] * b[1][2] + a[3][2] * b[2][2] + a[3][3] * b[3][2],
            a[3][0] * b[0][3] + a[3][1] * b[1][3] + a[3][2] * b[2][3] + a[3][3] * b[3][3],
        }
    };
}

__attribute__((noinline)) Matrix<float, 3, 3> matrix_mul_simd(const Matrix<float, 3, 4> &a, const Matrix<float, 4, 3> &b)
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

__attribute__((noinline)) Matrix<float, 4, 4> matrix_mul_simd(const Matrix<float, 4, 4> &a, const Matrix<float, 4, 4> &b)
{
    Matrix<float, 4, 4> result;

    for (int i = 0; i < 4; ++i) {
        // Load the row of matrix a
        __m128 a_row = _mm_loadu_ps(a[i].data());

        for (int j = 0; j < 4; ++j) {
            // Load the column of matrix b
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
    
    Matrix<float, 3, 4> a = {{
        row3_t {1.0, 2.0, 3.0},
        row3_t {4.0, 5.0, 6.0},
        row3_t {7.0, 8.0, 9.0},
        row3_t {10.0, 11.0, 12.0}
    }};

    Matrix<float, 4, 3> b = {{
        row4_t {1.0, 2.0, 3.0, 4.0},
        row4_t {5.0, 6.0, 7.0, 8.0},
        row4_t {9.0, 10.0, 11.0, 12.0}
    }};
    
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    /*
    Matrix<float, 3, 3> d;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 10000000; i++) {
        d = matrix_mul(a, b);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Matrix multiplication took " << duration.count() << " ms\n";
    print(std::cout, d);
    
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 10000000; i++) {
        d = matrix_mul_rolled(a, b);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Matrix multiplication took " << duration.count() << " ms\n";
    print(std::cout, d);
    
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 10000000; i++) {
        d = matrix_mul_simd(a, b);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Matrix multiplication took " << duration.count() << " ms\n";
    print(std::cout, d);
    */
    
    Matrix<float, 4, 4> e = {{
        row4_t {  1.0,  2.0,  3.0, 2.1 },
        row4_t {  4.0,  5.0,  6.0, 5.2 },
        row4_t {  7.0,  8.0,  9.0, 1.1 },
        row4_t { 10.0, 11.0, 12.0, 8.3 }
    }};
    
    Matrix<float, 4, 4> g = {{
        row4_t {  1.0,  2.0,  3.0,  4.0 },
        row4_t {  5.0,  6.0,  7.0,  8.0 },
        row4_t {  9.0, 10.0, 11.0, 12.0 },
        row4_t { 13.0, 14.0, 15.0, 16.0 }
    }};
    
    Matrix<float, 4, 4> f;
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 10000000; i++) {
        f = matrix_mul(e, g);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Matrix multiplication took " << duration.count() << " ms\n";
    print(std::cout, f);
    
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 10000000; i++) {
        f = matrix_mul_rolled(e, g);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Matrix multiplication took " << duration.count() << " ms\n";
    print(std::cout, f);

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 10000000; i++) {
        f = matrix_mul_simd(e, g);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Matrix multiplication took " << duration.count() << " ms\n";
    print(std::cout, f);

    return 0;
}
