#pragma once

#include <limits>
#include <cmath>
#include <cstddef>

namespace simd_avx
{

float dot(const float *p, const float * const p_end);
float dot(const float *p, const float * const p_end, float limit);
float dot(const float *p, const float *q, const float * const p_end);
float dot(const float *p, const float *q, const float * const p_end, float limit);
float dist_sq(const float *p, const float *q, const float * const p_end);
float dist_sq(const float *p, const float *q, const float * const p_end, float limit_sq);
float dist_sq(float mul_p, const float *p, const float *q, const float * const p_end);
float dist_sq(float mul_p, const float *p, const float *q, const float * const p_end, float limit_sq);
float dist_sq(float mul_p, float mul_q, const float *p, const float *q, const float * const p_end);
float dist_sq(float mul_p, float mul_q, const float *p, const float *q, const float * const p_end, float limit_sq);
float normalize(const float *p, float *out, const float * const p_end);

}
