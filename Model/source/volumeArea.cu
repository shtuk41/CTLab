
#include <cuda_runtime.h>
#include <math_constants.h>
#include <vecMath.cuh>

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include <scanObject.h>

#ifdef __INTELLISENSE__
// Dummy definitions to suppress IntelliSense errors
typedef struct { unsigned int x, y, z; } dim3;
extern dim3 threadIdx;
extern dim3 blockIdx;
extern dim3 blockDim;
extern dim3 gridDim;
#endif


__global__ void findInnerPointsKernel(const float3* d_mesh, int numberOfTriangles, const float3*d_areaPoints, bool*d_includeFlags, size_t numberOfPoints) {
	int idx = threadIdx.x + blockIdx.x * blockDim.x;
	if (idx >= numberOfPoints)
		return;

	float3 p = d_areaPoints[idx];

	float totalSolidAngle = 0.0f;

	for (int t = 0; t < numberOfTriangles; ++t) 
	{
		float3 a = vecSub(d_mesh[t * 3 + 0], p);
		float3 b = vecSub(d_mesh[t * 3 + 1], p);
		float3 c = vecSub(d_mesh[t * 3 + 2], p);

		float la = vecLength(a);
		float lb = vecLength(b);
		float lc = vecLength(c);

		float numerator = vecDot(a, vecCross(b, c));

		float denominator = la * lb * lc
			+ vecDot(a, b) * lc
			+ vecDot(b, c) * la
			+ vecDot(c, a) * lb;

		float omega = 2.0f * atan2f(numerator, denominator);
		totalSolidAngle += omega;

	}

	const float epsilon = 1e-2f;
	const float four_pi = 4.0f * CUDART_PI_F;

	d_includeFlags[idx] = fabsf(totalSolidAngle - four_pi) < epsilon;
}

void getPointsInsideObjectCudaKernelLouncher(const ScanObject& obj, ZArray* scanBox, std::vector<glm::vec3> & internalPoints)
{
	unsigned int numberOfTriangles = obj.GetNumberOfTriangles();
	glm::vec4* ptr = obj.GetTrianglesWithOffset();
	std::vector<float3> meshPtr(numberOfTriangles * 3);

	for (unsigned int ii = 0; ii < numberOfTriangles; ii++) 
	{
		meshPtr[ii * 3 + 0] = make_float3(ptr[ii * 3 + 0].x, ptr[ii * 3 + 0].y, ptr[ii * 3 + 0].z);
		meshPtr[ii * 3 + 1] = make_float3(ptr[ii * 3 + 1].x, ptr[ii * 3 + 1].y, ptr[ii * 3 + 1].z);
		meshPtr[ii * 3 + 2] = make_float3(ptr[ii * 3 + 2].x, ptr[ii * 3 + 2].y, ptr[ii * 3 + 2].z);
	}

	std::vector<float3> areaPoints(nVoxelsZ * nVoxelsY * nVoxelsX);

	for (int h = 0; h < nVoxelsZ; h++)
	{
		for (int w = 0; w < nVoxelsY; w++)
		{
			for (int d = 0; d < nVoxelsX; d++)
			{
				glm::vec3 p = (*scanBox)[h][w][d].first;

				int index = ((h * nVoxelsY + w) * nVoxelsX + d);

				areaPoints[index] = make_float3(p.x, p.y, p.z);
			}
		}
	}

	size_t totalVoxes = nVoxelsZ * nVoxelsY * nVoxelsX;

	float3* d_mesh;
	cudaMalloc(&d_mesh, numberOfTriangles * 3 * sizeof(float3));
	cudaMemcpy(d_mesh, meshPtr.data(), numberOfTriangles * 3 * sizeof(float3), cudaMemcpyHostToDevice);

	float3* d_areaPoints;
	cudaMalloc(&d_areaPoints, totalVoxes * sizeof(float3));
	cudaMemcpy(d_areaPoints, areaPoints.data(), totalVoxes * sizeof(float3), cudaMemcpyHostToDevice);

	bool* d_includeFlag;
	cudaMalloc(&d_includeFlag, totalVoxes * sizeof(bool));
	cudaMemset(d_includeFlag, 0, totalVoxes);

	int threadsPerBlock = 256;
	int blocks = static_cast<int>((totalVoxes + threadsPerBlock - 1) / threadsPerBlock);
	findInnerPointsKernel<<<blocks, threadsPerBlock>>>(d_mesh, numberOfTriangles, d_areaPoints, d_includeFlag, totalVoxes);

	cudaError_t err = cudaDeviceSynchronize();

	if (err != cudaSuccess) 
	{
		std::cerr << "CUDA error after findInnerPointsKernel launch: " << cudaGetErrorString(err) << std::endl;
	}

	err = cudaGetLastError();
	if (err != cudaSuccess) {
		std::cerr << "CUDA kernel findInnerPointsKernel error: " << cudaGetErrorString(err) << std::endl;
		return;
	}

	bool* h_includeFlag = new bool[totalVoxes];
	cudaMemcpy(h_includeFlag, d_includeFlag, totalVoxes * sizeof(bool), cudaMemcpyDeviceToHost);

	for (int ii = 0; ii < totalVoxes; ii++)
	{
		if (h_includeFlag[ii])
		{
			int d = ii % nVoxelsX;
			int w = (ii / nVoxelsX) % nVoxelsY;
			int h = ii / (nVoxelsX * nVoxelsY);

			internalPoints.push_back((*scanBox)[h][w][d].first);
		}
	}

	delete[] h_includeFlag;

	cudaFree(d_mesh);
	cudaFree(d_areaPoints);
	cudaFree(d_includeFlag);

}

