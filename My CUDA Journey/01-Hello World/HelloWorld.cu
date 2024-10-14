#include<stdio.h>
#include<cuda.h>

__global__ void kernel(void)
{

}

int main(void)
{
	kernel << <1, 1 >> > ();
	printf("Hello World Host Line !!!");
	return(0);
}