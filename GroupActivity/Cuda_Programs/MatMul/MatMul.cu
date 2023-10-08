#include<stdio.h>
#include<stdlib.h>

//cuda headers
#include<cuda.h>
#include"helper_timer.h"

//macros
#define BLOCK_WIDTH 128

//global variables

int *HostA = NULL;
int *HostB = NULL;
int *HostC = NULL;
int *Gold = NULL;

int *DeviceA = NULL;
int *DeviceB = NULL;
int *DeviceC = NULL;

float timeOnCPU = 0.0f;
float timeOnGPU = 0.0f;

//cuda kernel function

__global__ void matMulGPU(int *A, int *B, int *C, int numARows, int numAColumns, int numBColumns, int numCColumns)
{
	//variable declaration
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int column = blockIdx.x * blockDim.x + threadIdx.x;

	//code

	if ((row < numARows) && (column < numBColumns))
	{
		int value = 0.0;
		for (int k = 0; k < numAColumns; k++)
		{
			int a = A[row * numAColumns + k];
			int b = B[k * numBColumns + column];
			value += a * b;
		}
		C[row * numCColumns + column] = value;
	}
}

int main(int argc, char *argv[])
{
	//function declarations
	void InitA(int *data, int, int);
	void InitB(int *data, int, int);
	void matMulCPU(int *, int *, int *, int, int, int, int);
	void cleanup(void);

	//variable declaration

	int numARows = BLOCK_WIDTH;
	int numAColumns = BLOCK_WIDTH;
	int numBRows = BLOCK_WIDTH;
	int numBColumns = BLOCK_WIDTH;

	int numCRows = numARows;
	int numCColumns = numBColumns;

	int numGoldRows = numARows;
	int numGoldColumns = numBColumns;

	int sizeA = numARows * numAColumns * sizeof(int);
	int sizeB = numBRows * numBColumns * sizeof(int);
	int sizeC = numCRows * numCColumns * sizeof(int);
	int sizeGold = numGoldRows * numGoldColumns * sizeof(int);

	cudaError_t result = cudaSuccess;

	//code
	//host memory allocation

	HostA = (int *)malloc(sizeA);
	if (HostA == NULL)
	{
		printf("Host memory allocation is failed for HostA matrix.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	HostB = (int *)malloc(sizeB);
	if (HostB == NULL)
	{
		printf("Host memory allocation is failed for HostB matrix.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	HostC = (int *)malloc(sizeC);
	if (HostC == NULL)
	{
		printf("Host memory allocation is failed for HostC matrix.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	Gold = (int *)malloc(sizeGold);
	if (Gold == NULL)
	{
		printf("Host memory allocation is failed for Gold matrix.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	//printing matrix dimension and sizes

	printf("The Dimensions of matrix 'HostA' are : %d x %d \n", numARows, numAColumns);
	printf("The Dimensions of matrix 'HostB' are : %d x %d \n", numBRows, numBColumns);
	printf("The Dimensions of matrix 'HostC' are : %d x %d \n", numCRows, numCColumns);
	printf("The Dimensions of matrix 'Gold' are : %d x %d \n", numGoldRows, numGoldColumns);

	printf("Size of matrix HostA = %d\n", sizeA);
	printf("Size of matrix HostB = %d\n", sizeB);
	printf("Size of matrix HostC = %d\n", sizeC);
	printf("Size of matrix Gold = %d\n", sizeGold);

	//fill source matrices

	InitA(HostA, numARows, numAColumns);
	InitB(HostB, numBRows, numBColumns);

	//device memory allocation

	result = cudaMalloc((void **)&DeviceA, sizeA);
	if(result!=cudaSuccess)
	{
		printf("Device memory allocation is failed for DeviceA matrix.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	result = cudaMalloc((void **)&DeviceB, sizeB);
	if (result != cudaSuccess)
	{
		printf("Device memory allocation is failed for DeviceB matrix.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	result = cudaMalloc((void **)&DeviceC, sizeC);
	if (result != cudaSuccess)
	{
		printf("Device memory allocation is failed for DeviceC matrix.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	//copy data from host matrices to device matrices

	result = cudaMemcpy(DeviceA, HostA, sizeA, cudaMemcpyHostToDevice);
	if (result != cudaSuccess)
	{
		printf("Host to Device data copy is failed for DeviceA matrix.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	result = cudaMemcpy(DeviceB, HostB, sizeB, cudaMemcpyHostToDevice);
	if (result != cudaSuccess)
	{
		printf("Host to Device data copy is failed for DeviceB matrix.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	//cuda kernel configuration

	dim3 dimGrid = dim3(ceil((int)numBColumns / (int)BLOCK_WIDTH), ceil((int)numARows / (int)BLOCK_WIDTH), 1);
	dim3 dimBlock = dim3(BLOCK_WIDTH, BLOCK_WIDTH, 1);

	//cuda kernel for matrix multiplication

	StopWatchInterface *timer = NULL;
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	matMulGPU << <dimGrid, dimBlock >> > (DeviceA, DeviceB, DeviceC, numARows, numAColumns, numBColumns, numCColumns);

	sdkStopTimer(&timer);
	timeOnGPU = sdkGetTimerValue(&timer);
	sdkDeleteTimer(&timer);
	timer = NULL;

	//copy data from device matrices to host matrices
	
	result = cudaMemcpy(HostC, DeviceC, sizeC, cudaMemcpyDeviceToHost);

	if (result != cudaSuccess)
	{
		printf("Device to Host data copy is failed for Hostc matrix.\n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	//matrix multiplication on host

	matMulCPU(HostA, HostB, Gold, numARows, numAColumns, numBColumns, numCColumns);

	//comparison

	int breakValue = -1;
	bool bAccuracy = true;

	for (int i = 0; i < numCRows * numCColumns; i++)
	{
		int val1 = Gold[i];
		int val2 = HostC[i];
		if (val1 != val2)
		{
			bAccuracy = false;
			breakValue = i;
			break;
		}
	}

	char str[128];
	if (bAccuracy == false)
	{
		sprintf(str, "Comparison of CPU and GPU matrix multiplication is not accurate at array index %d.\n", breakValue);
	}

	else
	{
		sprintf(str, "Comparison of CPU and GPU matrix multiplication is accurate.\n");

	}

	printf("Time taken for matrix multiplication on CPU =%.6f\n", timeOnCPU);
	printf("Time taken for matrix multiplication on GPU =%.6f\n", timeOnGPU);

	printf("%s\n", str);

	//cleanup()

	cleanup();

	return(0);
}

void InitA(int *data, int row, int col)
{
	int num = 1;
	//code
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			*(data + i * col + j) = num;
			num++;
		}
	}
}

void InitB(int *data, int row, int col)
{
	int num = BLOCK_WIDTH;
	//code
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			*(data + i * col + j) = num;
			num--;
		}
	}
}

void matMulCPU(int *A, int *B, int *C, int numARows, int numAColumns, int numBColumns, int numCColumns)
{
	//code

	StopWatchInterface *timer = NULL;
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	for (int i = 0; i < numARows; ++i)
	{
		for (int j = 0; j < numBColumns; ++j)
		{
			int value = 0.0f;
			for (int k = 0; k < numAColumns; ++k)
			{
				int a = A[i * numAColumns + k];
				int b = B[k * numBColumns + j];
				value += a * b;

			}

			C[i * numCColumns + j] = value;

		}
	}
	sdkStopTimer(&timer);
	timeOnCPU = sdkGetTimerValue(&timer);
	sdkDeleteTimer(&timer);
	timer = NULL;

}

void cleanup(void)
{
	if (DeviceC)
	{
		cudaFree(DeviceC);
		DeviceC = NULL;
	}

	if (DeviceB)
	{
		cudaFree(DeviceB);
		DeviceB = NULL;
	}

	if (DeviceA)
	{
		cudaFree(DeviceA);
		DeviceA = NULL;
	}

	if (Gold)
	{
		free(Gold);
		Gold = NULL;
	}

	if (HostC)
	{
		free(HostC);
		HostC = NULL;
	}

	if (HostB)
	{
		free(HostB);
		HostB = NULL;
	}

	if (HostA)
	{
		free(HostA);
		HostA = NULL;
	}


}

