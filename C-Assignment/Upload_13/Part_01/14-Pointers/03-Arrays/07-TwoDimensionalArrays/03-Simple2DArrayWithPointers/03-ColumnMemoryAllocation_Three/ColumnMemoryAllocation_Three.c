#include<stdio.h>
#include<stdlib.h>


#define NUM_ROWS 5
#define NUM_COLUMNS 5

int main(void)
{
	int *PLP_IARRAY[NUM_ROWS]; //array of pointers which will point to a integer
	int i, j;

	//code
	printf("\n\n");
	printf("************** Memory allocation to 2D Integer array *************\n\n");
	for (i = 0; i < NUM_ROWS; i++)
	{
		//ROW 0 will have (NUM_COLUMNS - 0) = (5 - 0) = 5 COLUMNS
		//ROW 1 will have (NUM_COLUMNS - 1) = (5 - 1) = 4 COLUMNS
		//ROW 2 will have (NUM_COLUMNS - 2) = (5 - 2) = 3 COLUMNS

		// and so on

		// Because of this , There is no Contiguous memory allocation .. Hence,Although we may use the data as a 2D array, IT is not Really a 2D array in memory ...

		PLP_IARRAY[i] = (int *)malloc((NUM_COLUMNS - i) * sizeof(int));
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

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < (NUM_COLUMNS - i); j++)
		{
			PLP_IARRAY[i][j] = (i * 1) + (j * 1);
		}
	}

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < (NUM_COLUMNS - i); j++)
		{
			printf("PLP_IARRAY[%d][%d] = %d \t at address : %p\n", i, j, PLP_IARRAY[i][j], &PLP_IARRAY[i][j]);
		}

		printf("\n");
	}

	for (i = (NUM_ROWS - 1); i >= 0; i--)
	{
		if (PLP_IARRAY[i])
		{
			free(PLP_IARRAY[i]);
			PLP_IARRAY[i] = NULL;
			printf("Memory Allocated to row %d has been successfully freed !!! \n\n",i);
		}
	}

	return(0);
}