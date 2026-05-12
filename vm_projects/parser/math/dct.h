#pragma once

#include <iostream>
#include <vector>
#include <cmath>

// Define pi for trigonometric calculations
const double PI = 3.14159265358979323846;

// Function to perform Discrete Cosine Transform
std::vector<double> dct(const std::vector<double>& data)
{
    size_t N = data.size();
    std::vector<double> result(N, 0.0);

    for (int k = 0; k < N; ++k) {
        double sum = 0.0;
        for (int n = 0; n < N; ++n) {
            sum += data[n] * cos(PI * k * (2 * n + 1) / (2 * N));
        }
        result[k] = sum * (k == 0 ? sqrt(1.0 / N) : sqrt(2.0 / N));
    }

    return result;
}

// Function to perform Inverse Discrete Cosine Transform
std::vector<double> idct(const std::vector<double>& data)
{
    size_t N = data.size();
    std::vector<double> result(N, 0.0);

    for (int n = 0; n < N; ++n) {
        double sum = 0.0;
        for (int k = 0; k < N; ++k) {
            sum += (k == 0 ? sqrt(1.0 / N) : sqrt(2.0 / N)) * data[k] * cos(PI * k * (2 * n + 1) / (2 * N));
        }
        result[n] = sum;
    }

    return result;
}

int dct_test_1()
{
    // Example data array
    std::vector<double> data = { 345.180555, 7.361945, 18.455183, 4.158345, 91.295366, 278.402253, 0.420412, 1278.445147 };

    // Perform DCT
    std::vector<double> dct_result = dct(data);
    std::cout << std::endl << "DCT Result: ";
    for (double val : dct_result) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    /*
    for (size_t i = dct_result.size() - 2; i < dct_result.size(); ++i) {
        dct_result[i] = 0.;
    }
    */
    // Perform IDCT
    std::vector<double> idct_result = idct(dct_result);
    std::cout << std::endl << "IDCT Result: ";
    for (double val : idct_result) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}

int dct_test() {
    // Example data array
    std::vector<double> data = {18.180555, 18.361945, 40.455183, 18.158345, 18.295366, 18.402253, 18.420412, 18.445147};
    //std::vector<double> data = { 345.180555, 7.361945, 18.455183, 4.158345, 91.295366, 278.402253, 0.420412, 1278.445147 };

    // Perform DCT
    std::vector<double> dct_result = dct(data);
    std::cout << "DCT Result: ";
    for (double val : dct_result) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Drop higher-order coefficients (set to zero for lossy compression)
    int num_terms_to_retain = 4; // Adjust this number for more or less compression
    for (int i = num_terms_to_retain; i < dct_result.size(); ++i) {
        dct_result[i] = 0.0;
    }

    // Perform IDCT
    std::vector<double> idct_result = idct(dct_result);
    std::cout << "IDCT Result: ";
    for (double val : idct_result) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
