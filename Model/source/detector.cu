
#include <cuda_runtime.h>
#include <math_constants.h>
#include <vecMath.cuh>


#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include <scanObject.h>
#include <detector.h>


#ifdef __INTELLISENSE__
// Dummy definitions to suppress IntelliSense errors
typedef struct { unsigned int x, y, z; } dim3;
extern dim3 threadIdx;
extern dim3 blockIdx;
extern dim3 blockDim;
extern dim3 gridDim;
#endif


__global__ void getPixelPyramidMethodKernel(const float3* detectorPixels, size_t numberPixels, float3 sourceCenter, const float3* d_areaPoints, size_t numberOfPoints, unsigned short* d_pixelValue)
{
	int idx = threadIdx.x + blockIdx.x * blockDim.x;
	if (idx >= numberPixels)
		return;

	float3 xV = vecSub(detectorPixels[idx], sourceCenter);
	float h = vecLength(xV);
	
	if (h < 1e-6f) 
		return; // avoid zero length

	xV = vecNormalize(xV);

	// Build orthonormal frame
	float3 arbitrary = (fabs(vecDot(xV, make_float3(0, 0, 1))) < 0.99f) ? make_float3(0, 0, 1) : make_float3(1, 0, 0);
	float3 yV = vecNormalize(vecCross(xV, arbitrary));
	float3 zV = vecCross(xV, yV); // right-handed frame

	//&%&%&%&%&%&%&%&%&xVoxelPitch!!!!!!!!!!!!!!!!!!!!!!!!!! fix this
	float half_side = static_cast<float>(0.1 / 2.0);

	ushort currentValue = 0;

	for (int t = 0; t < numberOfPoints; ++t)
	{
		float3 apexToPoint = vecSub(d_areaPoints[t], sourceCenter);

		float px = vecDot(apexToPoint, xV);  // along pyramid axis (height)
		float py = vecDot(apexToPoint, yV);  // lateral
		float pz = vecDot(apexToPoint, zV);  // lateral

		if (px >= 0 && px <= h &&
			fabs(py) <= half_side &&
			fabs(pz) <= half_side)
		{
			currentValue += 1;
		}
	}

	d_pixelValue[idx] = currentValue;
}


void getPixelsPyramidMethodKernelLouncher(const Detector * const detector, const glm::vec3& sourceCenter, const std::vector<glm::vec3>& areaPoints, cv::Mat& detectorImage)
{
	size_t numberPixels = nDetectorResZ * nDetectorResY;

	std::vector<float3> h_detectorPixels(numberPixels);

	for (int jj = 0; jj < nDetectorResZ; jj++)
	{
		for (int ii = 0; ii < nDetectorResY; ii++)
		{
			//std::cout << jj << " " << ii << '\n';
			glm::vec3 pixel = detector->getPixel(jj, ii);

			h_detectorPixels[jj * nDetectorResY + ii] = make_float3(pixel.x, pixel.y, pixel.z);
		}
	}

	float3* d_detectorPoints;
	cudaMalloc(&d_detectorPoints, numberPixels * sizeof(float3));
	cudaMemcpy(d_detectorPoints, h_detectorPixels.data(), h_detectorPixels.size() * sizeof(float3), cudaMemcpyHostToDevice);


	float3 h_sCenter = make_float3(sourceCenter.x, sourceCenter.y, sourceCenter.z);

	std::vector<float3> h_areaPoints(areaPoints.size());

	for (int ii = 0; ii < areaPoints.size(); ii++)
	{
		h_areaPoints[ii] = make_float3(areaPoints[ii].x, areaPoints[ii].y, areaPoints[ii].z);
	}

	float3* d_areaPoints;
	cudaMalloc(&d_areaPoints, h_areaPoints.size() * sizeof(float3));
	cudaMemcpy(d_areaPoints, h_areaPoints.data(), h_areaPoints.size() * sizeof(float3), cudaMemcpyHostToDevice);

	unsigned short* d_pixelValues;
	cudaMalloc(&d_pixelValues, numberPixels * sizeof(unsigned short));
	cudaMemset(d_pixelValues, 0, numberPixels * sizeof(unsigned short));

	unsigned int threadsPerBlock = 256;
	unsigned int blocks = static_cast<unsigned int>((numberPixels + threadsPerBlock - 1) / threadsPerBlock);
	getPixelPyramidMethodKernel<<<blocks, threadsPerBlock>>> (d_detectorPoints, numberPixels, h_sCenter, d_areaPoints, h_areaPoints.size(), d_pixelValues);

	cudaError_t err = cudaDeviceSynchronize();

	if (err != cudaSuccess)
	{
		std::cerr << "CUDA error after getPixelPyramidMethodKernel lounch: " << cudaGetErrorString(err) << std::endl;
	}

	err = cudaGetLastError();
	if (err != cudaSuccess) {
		std::cerr << "CUDA kernel getPixelPyramidMethodKernel error: " << cudaGetErrorString(err) << std::endl;
		return;
	}

	ushort* h_pixelValues = new ushort[numberPixels];

	cudaMemcpy(h_pixelValues, d_pixelValues, numberPixels * sizeof(unsigned short), cudaMemcpyDeviceToHost);

	ushort maxValue = 1;

	unsigned short* iter = h_pixelValues;

	for (int ii = 0; ii < numberPixels; ii++, iter++)
	{
		if (*iter > maxValue)
			maxValue = *iter;
	}

	for (int jj = 0; jj < nDetectorResZ; jj++)
	{
		for (int ii = 0; ii < nDetectorResY; ii++)
		{
			detectorImage.at<ushort>(jj, ii) = static_cast<ushort>((1.0f - (float(h_pixelValues[jj * nDetectorResY + ii]) / maxValue)) * 65535);
		}
	}

	delete[] h_pixelValues;
	cudaFree(d_areaPoints);
	cudaFree(d_pixelValues);
	cudaFree(d_detectorPoints);
}


