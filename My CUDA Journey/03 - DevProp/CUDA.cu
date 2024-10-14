#include<stdio.h>
#include<stdlib.h>
#include<cuda.h>


int main(void)
{
	cudaDeviceProp properties;

	int deviceCount;

	cudaGetDeviceCount(&deviceCount);

	printf("\n\nTotal number of CUDA Devices: %d\n\n", deviceCount);

	cudaGetDeviceProperties(&properties, 0);

	printf("\n\n ------ General Information for device 0 ------\n\n");

    // Print device properties
    printf("\nDevice 0: %s\n", properties.name);
	printf("Clock Rate: %d\n", properties.clockRate);
	printf("Compute capability: %d.%d\n", properties.major, properties.minor);
	printf("Multiprocessor count: %d\n",properties.multiProcessorCount);
	printf("Max threads per block: %d\n", properties.maxThreadsPerBlock);
	printf("Max Number of blocks in X Direction %d\n", properties.maxGridSize[0]);
	printf("Max Number of blocks in Y Direction %d\n", properties.maxGridSize[1]);
	printf("Max Number of blocks in Z Direction %d\n", properties.maxGridSize[2]);
	
	printf("\n\n --- Memory Information for device 0 ---\n\n");
	printf("Total global mem: %zu\n", properties.totalGlobalMem);
	printf("Total constant Mem: %zu\n", properties.totalConstMem);
	printf("Max mem pitch: %zu\n", properties.memPitch);
	printf("Texture Alignment: %zu\n", properties.textureAlignment);

   

    return 0;

	
}