#include<stdio.h>
#include<cuda.h>
#include<stdlib.h>
#include<math.h>

#define N 4096

__global__ void vecAdd(int* a, int* b, int* c)
{
	int bid = 0;

	bid = (blockIdx.x * blockDim.x) + threadIdx.x;

	if (bid < N)
	{
		c[bid] = a[bid] + b[bid];
	}
}

int main(void)
{
	int a[N], b[N], c[N];  // allocate three arrays on host side first 2 to pass data to GPU and thied on to receive the result from the GPU

	int i = 0;
	unsigned int count = 0;

	
	int threadsPerBlock = 512;

	int blocks = (int)ceil(((float)(N) / (float)threadsPerBlock));

	int* dev_a, *dev_b, *dev_c;  // declaring 3 pointers to store device memory address after allocating memory on the device

	cudaError_t error;  //to catch error from Cuda APIs

	for (i = 0; i < N; i++)   // fill arrays on CPU side ( host side )
	{
		a[i] = -i;
		b[i] = i + i;
		count += 1;
	}

	error = cudaMalloc((void**)&dev_a, N * sizeof(int));

	if (error != cudaSuccess)
	{
		printf("Memory Allocation for input array A on the device is failed !!!");
		exit(0);
	}

	error = cudaMalloc((void**)&dev_b, N * sizeof(int));

	if (error != cudaSuccess)
	{
		printf("Memory Allocation for input array B on the device is failed !!!");
		exit(0);
	}

	error = cudaMalloc((void**)&dev_c, N * sizeof(int));

	if (error != cudaSuccess)
	{
		printf("Memory Allocation for input array C on the device is failed !!!");
		exit(0);
	}

	// now as the memory is allocated successfully on the GPU we wll copy data from host side to device side

	error = cudaMemcpy(dev_a, a, N * sizeof(int), cudaMemcpyHostToDevice);

	if (error != cudaSuccess)
	{
		printf("Copying memory from Host to device for array A is failed");
		exit(0);
	}

	error = cudaMemcpy(dev_b, b, N * sizeof(int), cudaMemcpyHostToDevice);

	if (error != cudaSuccess)
	{
		printf("Copying memory from Host to device for array b is failed");
		exit(0);
	}

	// now as the memory is copied from host to device successfully
	//we can run the kernel on this two arrays and store the result on array pointed by dev_c on the device

	vecAdd << <blocks, threadsPerBlock >> > (dev_a, dev_b, dev_c);

	// now as thr result is stored on array pointed by dev_c on the gpu we will copy the array on the host side

	error = cudaMemcpy(c, dev_c, N * sizeof(int), cudaMemcpyDeviceToHost);

	if (error != cudaSuccess)
	{
		printf("Copying memory from device to host for result array (c) is failed");
		exit(0);
	}

	//display the result

	printf("\n\n");

	for (i = 0; i < N; i++)
	{
		printf("%d + %d = %d\n", a[i], b[i], c[i]);
	}

	printf("count = %d", count);

	cudaFree(dev_a);
	cudaFree(dev_b);
	cudaFree(dev_c);

	dev_a = NULL;
	dev_b = NULL;
	dev_c = NULL;

	return(0);
}
