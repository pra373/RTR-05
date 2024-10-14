#include<stdio.h>
#include<stdlib.h>
#include<cuda.h>

__global__ void add(int num1, int num2, int *c)
{
	*c = num1 + num2;
}

int main(void)
{
	int result;
	int* dev_c;
	int a = 5;
	int b = 5;

	cudaError_t error;

	// allocate memory on the device

	error = cudaMalloc((void**)&dev_c, sizeof(int));

	if (error != cudaSuccess)
	{
		printf("\nCuda Malloc Failed !!\n");
		exit(0);
	}

	// call the kernel

	add << <1, 1 >> > (a, b, dev_c);   // do addition on GPU and store the result on allocated memory block on device

	// copy the reslut from devices memory to host memory to bring the result in host code

	error = cudaMemcpy(&result, dev_c, sizeof(int), cudaMemcpyDeviceToHost);

	if (error != cudaSuccess)
	{
		printf("\ncudaMemcpy() Failed !!!\n");
		exit(0);
	}

	printf("\n\n%d + %d = %d\n\n", a, b, result);

	cudaFree(dev_c);



	return(0);
}