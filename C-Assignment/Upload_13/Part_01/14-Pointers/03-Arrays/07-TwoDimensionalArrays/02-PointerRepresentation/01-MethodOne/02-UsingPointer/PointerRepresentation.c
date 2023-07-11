#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
	//variable declaration
	int PLP_IARRAY[NUM_ROWS][NUM_COLUMNS];
	int i, j;

	int *ptr_PLP_IARRAY_ROW = NULL;

	//code

	//code
	// Every row of a 2D array is an integer array itself 
	// so each row will have its own base address

	for (i = 0; i < NUM_ROWS; i++)
	{
		ptr_PLP_IARRAY_ROW = PLP_IARRAY[i];  // base address of i th row
		for (j = 0; j < NUM_COLUMNS; j++)
		{
			*(ptr_PLP_IARRAY_ROW + j) = (i + 1) * (j + 1); // 'iarray[i] can be treated as 1D array using pointers
		}
	}

	printf("\n\n");
	printf("2D integer array elements along with addresses : \n\n");

	for (i = 0; i < NUM_ROWS; i++)
	{
		ptr_PLP_IARRAY_ROW = PLP_IARRAY[i];

		for (j = 0; j < NUM_COLUMNS; j++)
		{
			printf("*(ptr_PLP_IARRAY_ROW + %d) = %d \t \t at address (ptr_PLP_IARRAY_ROW + j) : %p\n", j, *(ptr_PLP_IARRAY_ROW + j), (ptr_PLP_IARRAY_ROW + j));
		}

		printf("\n\n");
	}


}