#include<stdio.h>

int main(void)
{
	//variable declaration
	double PLP_DARRAY[10];
	int i;
	double *ptr_darray = NULL;

	//code

	for (i = 0; i < 10; i++)
	{
		PLP_DARRAY[i] = (float)1.5f * (i + 1);
	}

	// name of an array is its own base address
	// assigning base address of double array 'PLP_DARRAY' to double pointer 'ptr_darray'

	ptr_darray = PLP_DARRAY;

	printf("\n\n");
	printf("Elements of the 'double' array : \n\n");
	for (i = 0; i < 10; i++)
	{
		printf("PLP_DARRAY[%d] = %lf\n", i, *(ptr_darray + i));

	}

	printf("\n\n");

	for (i = 0; i < 10; i++)
	{
		printf("PLP_FARRAY[%d] = %lf \t \t Address = %p\n", i, *(ptr_darray + i), ptr_darray + i);
	}

	printf("\n\n");

	return(0);
}



