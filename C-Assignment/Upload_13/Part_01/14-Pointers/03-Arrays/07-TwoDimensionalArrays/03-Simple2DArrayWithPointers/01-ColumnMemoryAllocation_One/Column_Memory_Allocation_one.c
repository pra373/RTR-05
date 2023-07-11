#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
	//variable declaration
	int *PLP_IARRAY[NUM_ROWS];  // Pointer variable array 
	int i, j;

	//code

	printf("\n\n");

	for (i = 0; i < NUM_ROWS; i++)
	{
		PLP_IARRAY[i] = (int *)malloc(NUM_COLUMNS * sizeof(int));

		if (PLP_IARRAY[i] == NULL)
		{
			printf(" failed to allocate memory to row %d of 2D integer array !!! Exiting now ...\n\n", i);
			exit(0);
		}
		else
		{
			printf(" Memory allocation to row %d of 2D integer array successful !!!\n\n", i);
		}

	}

	//assigning values to 2D array

	printf("\n\n");

	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLUMNS; j++)
		{
			PLP_IARRAY[i][j] = (i + 1) * (j + 1);
		}
	}

	//displaying 2d array
	printf("\n\n");
	printf("Displaying 2D array : \n\n");

	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLUMNS; j++)
		{
			printf("PLP_IARRAY[%d][%d] = %d\n", i, j, PLP_IARRAY[i][j]);
		}

		printf("\n\n");
	}

	printf("\n\n");

	//FREEING MEMORY ASSIGNED TO 2D ARRAY (in reverse order)

	for (i = (NUM_ROWS - 1); i >= 0; i--)
	{
		free(PLP_IARRAY[i]);
		PLP_IARRAY[i] = NULL;

		printf("Memory allocated to row %d of 2D integer array has been successfully freed !!! \n\n", i);

	}

	return(0);


}