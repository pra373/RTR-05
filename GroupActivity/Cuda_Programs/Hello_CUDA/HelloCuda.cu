#include<stdio.h>
#include<stdlib.h>

//cuda headers
#include<cuda.h>

//global variables
const int iNmuberofArrayElements = 5;

float *HostInput1 = NULL;
float *HostInput2 = NULL;
float *HostOutput = NULL;

float *DeviceInput1 = NULL;
float *DeviceInput2 = NULL;
float *DeviceOutput = NULL;

//cuda Kernel

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

	//variable declaration
	int size = iNmuberofArrayElements * sizeof(float);
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

	//filling values into host arrays
	HostInput1[0] = 101.0f;
	HostInput1[1] = 102.0f;
	HostInput1[2] = 103.0f;
	HostInput1[3] = 104.0f;
	HostInput1[4] = 105.0f;

	HostInput2[0] = 201.0f;
	HostInput2[1] = 202.0f;
	HostInput2[2] = 203.0f;
	HostInput2[3] = 204.0f;
	HostInput2[4] = 205.0f;

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

	dim3 dimGrid = dim3(iNmuberofArrayElements, 1, 1);
	dim3 dimBlock = dim3(1, 1, 1);

	//cuda kernel for vector addition

	VecAddGPU << <dimGrid, dimBlock >> > (DeviceInput1, DeviceInput2, DeviceOutput, iNmuberofArrayElements);

	//copy data from device array into host array

	result = cudaMemcpy(HostOutput, DeviceOutput, size, cudaMemcpyDeviceToHost);

	if (result != cudaSuccess)
	{
		printf("Device to host data copy is failed for HostOutput array.\n");
		Cleanup();
		exit(EXIT_FAILURE);
	}
	// vector addition on host 
	for (int i = 0; i < iNmuberofArrayElements; i++)
	{
		printf("%f + %f = %f\n", HostInput1[i], HostInput2[i], HostOutput[i]);
	}

	//cleanup
	Cleanup();

	return(0);









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

	
}


