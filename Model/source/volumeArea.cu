#include <cuda_runtime.h>

#ifdef __INTELLISENSE__
// Dummy definitions to suppress IntelliSense errors
typedef struct { unsigned int x, y, z; } dim3;
extern dim3 threadIdx;
extern dim3 blockIdx;
extern dim3 blockDim;
extern dim3 gridDim;
#endif

void myCudaKernelLauncher()
{

}

__global__ void myKernel(int* data) {
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    // do work per thread
}