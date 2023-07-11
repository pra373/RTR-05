#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	int **PLP_PTR_IARRAY = NULL; // a pointer to a pointer variable which is pointing to a integer
	                             // this pointer variable will hold the base address of 2D array
	int i, j;
	int num_rows, num_columns;

	//code

	// *** Accept number of rows from user

	printf("\n\n");
	printf("Enter number of rows : ");
	scanf("%d", &num_rows);

	// *** Accept number of columns from user

	printf("\n\n");
	printf("Enter number of Columns : ");
	scanf("%d", &num_columns);

	// Allocating memory to array of pointers which will contain base addresses of rows od 2D array
	printf("\n\n");
	printf(" **********************Memory allocation to 2D integer array *********************\n\n");

	PLP_PTR_IARRAY = (int **)malloc(num_rows * sizeof(int *));
	if (PLP_PTR_IARRAY == NULL)
	{
		printf(" failed to allocate memory to row %d of 2D integer array !!! Exiting now ...\n\n", num_rows);
		exit(0);
	}

	else
	{
		printf(" Memory allocation to row %d of 2D integer array successful !!!\n\n", num_rows);

	}

	// Allocating memory to each row which is a 1D array 

	for (i = 0; i < num_rows; i++)
	{
		PLP_PTR_IARRAY[i] = (int *)malloc(num_columns * sizeof(int));

		if (PLP_PTR_IARRAY == NULL)
		{
			printf(" failed to allocate memory to columns of row %d of 2D integer array !!! Exiting now ...\n\n", i);
			exit(0);

		}

		else
		{
			printf(" Memory allocation to columns of row %d of 2D integer array successful !!!\n\n", i);

		}
	}

	// *** Filling Up values ***

	for (i = 0; i < num_rows; i++)
	{
		for (j = 0; j < num_columns; j++)
		{
			PLP_PTR_IARRAY[i][j] = (i * 1) + (j * 1);
		}
	}

	//displaying values

	for (i = 0; i < num_rows; i++)
	{
		printf("Base address of row %d : PLP_PTR_IARRAY[%d] %p \t at address : %p\n", i, i, PLP_PTR_IARRAY[i], &PLP_PTR_IARRAY[i]);
	}

	printf("\n\n");

	for (i = 0; i < num_rows; i++)
	{
		for (j = 0; j < num_columns; j++)
		{
			printf("PLP_PTR_IARRAY[%d][%d] = %d \t at address : %p\n", i, j, PLP_PTR_IARRAY[i][j], &PLP_PTR_IARRAY[i][j]);
		}

		printf("\n");
	}

	// freeing memory allocated to each row

	for (i = (num_rows - 1); i >= 0; i--)
	{
		if (PLP_PTR_IARRAY[i])
		{
			free(PLP_PTR_IARRAY[i]);
			PLP_PTR_IARRAY[i] = NULL;
			printf("Memory Allocated to row %d has been successfully freed !!! \n\n", i);
		}
	}

	//freeing memory allocated to 1D array consisting of base addresses of rows ***

	if (PLP_PTR_IARRAY)
	{
		free(PLP_PTR_IARRAY);
		PLP_PTR_IARRAY[i] = NULL;
		printf("Memory Allocated to PLP_PTR_IARRAY has been successfully freed !!! \n\n");

	}

	return(0);




	
}