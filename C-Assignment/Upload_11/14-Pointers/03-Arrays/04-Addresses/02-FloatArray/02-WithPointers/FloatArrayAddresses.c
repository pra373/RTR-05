#include<stdio.h>

int main(void)
{
	//variable declaration
	float PLP_FARRAY[10];
	int i;
	float *ptr_farray = NULL;

	//code

	for (i = 0; i < 10; i++)
	{
		PLP_FARRAY[i] = (float)1.5f * (i + 1);
	}

	// name of an array is its own base address
	// assigning base address of Float array 'PLP_FARRAY' to float pointer 'ptr_farray'

	ptr_farray = PLP_FARRAY;

	printf("\n\n");
	printf("Elements of the float array : \n\n");
	for (i = 0; i < 10; i++)
	{
		printf("PLP_FARRAY[%d] = %f\n", i, *(ptr_farray + i));

	}

	printf("\n\n");

	for (i = 0; i < 10; i++)
	{
		printf("PLP_FARRAY[%d] = %f \t \t Address = %p\n", i, *(ptr_farray + i), ptr_farray + i);
	}

	printf("\n\n");

	return(0);
}



