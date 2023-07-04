#include<stdio.h>

int main(void)
{
	//variable declaration
	int PLP_IARRAY[10];
	int i, *ptr_iarray;

	//code

	for (i = 0; i < 10; i++)
	{
		PLP_IARRAY[i] = 3 * (i + 1);
	}

	// name of an array is its own base address
	// assigning base address of Integer array 'PLP_IARRAY' to integer pointer 'ptr_iarray'

	ptr_iarray = PLP_IARRAY;

	printf("\n\n");
	printf("Elements of the integer array : \n\n");
	for (i = 0; i < 10; i++)
	{
		printf("PLP_IARRAY[%d] = %d\n", i, *(ptr_iarray+i));

	}

	printf("\n\n");
	
	for (i = 0; i < 10; i++)
	{
		printf("PLP_IARRAY[%d] = %d \t \t Address = %p\n", i, *(ptr_iarray+i), ptr_iarray + i);
	}

	printf("\n\n");

	return(0);
}



