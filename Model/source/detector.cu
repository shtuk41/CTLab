
#include <cuda_runtime.h>
#include <math_constants.h>

#include <opencv2/opencv.hpp>

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



void getPixelsPyramidMethodKernelLouncher(const glm::vec3& center, const std::vector<glm::vec3>& areaPoints, cv::Mat& detectorImage)
{

}


