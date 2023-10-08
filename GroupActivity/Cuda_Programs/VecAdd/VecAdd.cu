#include<stdio.h>
#include<stdlib.h>

//cuda headers
#include<cuda.h>
#include"helper_timer.h"

//global variables
const int iNumberOfArrayElements = 11444777;

float *HostInput1 = NULL;
float *HostInput2 = NULL;
float *HostOutput = NULL;

float *gold = NULL;

float *DeviceInput1 = NULL;
float *DeviceInput2 = NULL;
float *DeviceOutput = NULL;

float timeOnCPU = 0.0f;
float timeOnGPU = 0.0f;

//cuda kernel

__global__ void VecAddGPU(float *in1, float *in2, float *out, int len)
{
	//code
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < len)
	{
		out[i] = in1[i] + in2[i];
	}
}

//entry point function

int main(void)
{
	//function declaration
	void Cleanup(void);
	void fillFloatArrayWithRandomNumbers(float *, int);
	void VecAddCPU(const float *, const float *, float *, int);

	//variable declaration
	int size = iNumberOfArrayElements * sizeof(float);
	cudaError_t result = cudaSuccess;

	//code
	//host memory allocation

	HostInput1 = (float *)malloc(size);
	if (HostInput1 == NULL)
	{
		printf("Host memory allocation is failed for HostInput1 array.\n");
		Cleanup();
		exit(EXIT_FAILURE);
	}

	HostInput2 = (float *)malloc(size);
	if (HostInput2 == NULL)
	{
		printf("Host memory allocation is failed for HostInput2 array.\n");
		Cleanup();
		exit(EXIT_FAILURE);
	}

	HostOutput = (float *)malloc(size);

	if (HostOutput == NULL)
	{
		printf("Host memory allocation is failed for HostOutput array.\n");
		Cleanup();
		exit(EXIT_FAILURE);
	}

	gold = (float *)malloc(size);
	if (gold == NULL)
	{
		printf("Host memory allocation is failed for gold array.\n");
		Cleanup();
		exit(EXIT_FAILURE);
	}

	//filling values into host arrays
	fillFloatArrayWithRandomNumbers(HostInput1, iNumberOfArrayElements);
	fillFloatArrayWithRandomNumbers(HostInput2, iNumberOfArrayElements);

	//device memory allocation

	result = cudaMalloc((void **)&DeviceInput1, size);
	if (result != cudaSuccess)
	{
		printf("Device memory allocation is failed for DeviceInput1 array.\n");
		Cleanup();
		exit(EXIT_FAILURE);
	}

	result = cudaMalloc((void **)&DeviceInput2, size);
	if (result != cudaSuccess)
	{
		printf("Device memory allocation is failed for DeviceInput2 array.\n");
		Cleanup();
		exit(EXIT_FAILURE);
	}

	result = cudaMalloc((void **)&DeviceOutput, size);
	if (result != cudaSuccess)
	{
		printf("Device memory allocation is failed for DeviceOutput array.\n");
		Cleanup();
		exit(EXIT_FAILURE);
	}

	//copy data from host arrays into device arrays

	result = cudaMemcpy(DeviceInput1, HostInput1, size, cudaMemcpyHostToDevice);
	if (result != cudaSuccess)
	{
		printf("Host to device data copy is failed for DeviceInput1 array.\n");
		Cleanup();
		exit(EXIT_FAILURE);
	}

	result = cudaMemcpy(DeviceInput2, HostInput2, size, cudaMemcpyHostToDevice);
	if (result != cudaSuccess)
	{
		printf("Host to device data copy is failed for DeviceInput2 array.\n");
		Cleanup();
		exit(EXIT_FAILURE);
	}

	//cuda kernel configuration

	dim3 dimGrid = dim3((int)ceil((float)iNumberOfArrayElements / 256.0f), 1, 1);
	dim3 dimBlock = dim3(256, 1, 1);

	//CUDA Kernel for vector addition

	StopWatchInterface *timer = NULL;
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	VecAddGPU << <dimGrid, dimBlock >> > (DeviceInput1, DeviceInput2, DeviceOutput, iNumberOfArrayElements);
	sdkStopTimer(&timer);
	timeOnGPU = sdkGetTimerValue(&timer);
	sdkDeleteTimer(&timer);
	timer = NULL;

	//copy data from device array into host array

	result = cudaMemcpy(HostOutput, DeviceOutput, size, cudaMemcpyDeviceToHost);

	if (result != cudaSuccess)
	{
		printf("Device to host data copy is failed for HostOutput array.\n");
		Cleanup();
		exit(EXIT_FAILURE);
	}

	//vector addition on host

	VecAddCPU(HostInput1, HostInput2, gold, iNumberOfArrayElements);

	//comparison

	const float epsilon = 0.000001f;
	int breakValue = -1;
	bool bAccuracy = true;

	for (int i = 0; i < iNumberOfArrayElements; i++)
	{
		float val1 = gold[i];
		float val2 = HostOutput[i];
		if (fabs(val1 - val2) > epsilon)
		{
			bAccuracy = false;
			breakValue = i;
			break;
		}
	}

	char str[128];
	if (bAccuracy == false)
	{
		sprintf(str, "Comparison of CPU and GPU Vector Addition is not within accuracy of 0.000001 at array index %d\n", breakValue);

	}

	else
	{
		sprintf(str, "Comparison of CPU and GPU Vector Addition is within accuracy of 0.000001 at array index\n");

	}

	//output
	printf("\n\n");
	printf("Array1 begins from 0th index %.6f to %dth index %.6f\n\n", HostInput1[0], iNumberOfArrayElements - 1, HostInput1[iNumberOfArrayElements - 1]);
	printf("Array2 begins from 0th index %.6f to %dth index %.6f\n\n", HostInput2[0], iNumberOfArrayElements - 1, HostInput2[iNumberOfArrayElements - 1]);
	printf("Cuda Kernel Grid dimension = %d,%d,%d and Block dimension = %d,%d,%d\n\n", dimGrid.x, dimGrid.y, dimGrid.z, dimBlock.x, dimBlock.y, dimBlock.z);

	printf("Output array begins from 0th index %.6f to %dth index %.6f\n\n", HostOutput[0], iNumberOfArrayElements - 1, HostOutput[iNumberOfArrayElements - 1]);
	printf("Time taken for vector addition on CPU = %.6f\n\n", timeOnCPU);
	printf("Time taken for vector addition on GPU = %.6f\n\n", timeOnGPU);
	printf("%s\n", str);

	//cleanup
	Cleanup();

	return(0);

}

void fillFloatArrayWithRandomNumbers(float *arr, int len)
{
	//code

	const float fscale = 1.0f / (float)RAND_MAX;
	for (int i = 0; i < len; i++)
	{
		arr[i] = fscale * rand();
	}

}
void VecAddCPU(const float *arr1, const float *arr2, float *out, int len)
{
	//code
	StopWatchInterface *timer = NULL;
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	for (int i = 0; i < len; i++)
	{
		out[i] = arr1[i] + arr2[i];
	}

	sdkStopTimer(&timer);
	timeOnCPU = sdkGetTimerValue(&timer);
	sdkDeleteTimer(&timer);
	timer = NULL;


	
	
}

void Cleanup(void)
{
	//code
	if (DeviceOutput)
	{
		cudaFree(DeviceOutput);
		DeviceOutput = NULL;
	}

	if (DeviceInput2)
	{
		cudaFree(DeviceInput2);
		DeviceInput2 = NULL;
	}

	if (DeviceInput1)
	{
		cudaFree(DeviceInput1);
		DeviceInput1 = NULL;
	}

	if (gold)
	{
		cudaFree(gold);
		gold = NULL;
	}

	if (HostOutput)
	{
		cudaFree(HostOutput);
		HostOutput = NULL;
	}

	if (HostInput2)
	{
		cudaFree(HostInput2);
		HostInput2 = NULL;
	}

	if (HostInput1)
	{
		cudaFree(HostInput1);
		HostInput1 = NULL;
	}


}
