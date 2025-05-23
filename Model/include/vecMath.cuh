#pragma once

#include <cuda_runtime.h>
#include <math_constants.h>

#include <math.h>

__device__ inline  float3 vecSub(const float3& a, const float3& b)
{
	return make_float3(a.x - b.x, a.y - b.y, a.z - b.z);
}

__device__ inline float vecDot(const float3& a, const float3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

__device__ inline float3 vecCross(const float3& a, const float3& b)
{
	return make_float3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

__device__ inline float vecLength(const float3& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

__device__ inline  float3 vecNormalize(const float3& v) {
	float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return (len > 0.0f) ? make_float3(v.x / len, v.y / len, v.z / len)
		: make_float3(0.0f, 0.0f, 0.0f);
}
