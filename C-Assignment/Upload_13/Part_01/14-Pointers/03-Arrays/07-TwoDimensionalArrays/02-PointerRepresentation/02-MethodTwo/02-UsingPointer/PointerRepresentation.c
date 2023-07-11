#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
	//variable declaration
	int i, j;
	int **PLP_PTR_IARRAY = NULL;

	//code

	//  *** every row of 2D array is a 1D array itself comprising of NUM_COLUMS integer elements
	// hence address of 0th element of each row will be the base address of that respective 1D array 

	printf("\n\n");

	// *** Memory allocation ***

	PLP_PTR_IARRAY = (int **)malloc(NUM_ROWS * sizeof(int *)); // assigning memoy dynamically to a pointer variable array 
	                                                           // each ppointer in this array will store base address of a row 
	if (PLP_PTR_IARRAY == NULL)
	{
		printf("Memory allocation to the array of pointers having %d elements is failed !!! EXITING NOW ...\n\n", NUM_ROWS);
		exit(0);
	}

	else
	{
		printf("Memory allocation to the array of pointers having %d elements has succeeeded !!!\n\n", NUM_ROWS);

	}

	// *** Alocating memory for each row ****

	for (i = 0; i < NUM_ROWS; i++)
	{
		PLP_PTR_IARRAY[i] = (int *)malloc(NUM_COLUMNS * sizeof(int));  // base address of ith row
		if (PLP_PTR_IARRAY[i] == NULL)
		{
			printf("Memory allocation to the columns of row %d failed !!! EXITING NOW ... \n\n", i);
			exit(0);
		}

		else
		{
			printf("Memory allocation to the columns of row %d succedded \n\n", i);

		}
	}

	// Assigning values 

	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLUMNS; j++)
		{
			*(*(PLP_PTR_IARRAY + i) + j) = (i + 1) * (j + 1);  // PLP_PTR_IARRAY[i][j] = (i+1) * (j+1)
		}
	}

	// displaying values
	printf("\n\n");
	printf("2D integer array elements along with addresses : \n\n");

	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLUMNS; j++)
		{
			printf("PLP_PTR_IARRAY_ROW[%d][%d] = %d \t\t at address &PLP_PTR_IARRAY_ROW[%d][%d] : %p\n", i, j, PLP_PTR_IARRAY[i][j], i, j, &PLP_PTR_IARRAY[i][j]);
		}

		printf("\n\n");
	}

	// ******** freeing allocated memory ******
	// ******** freeing memory of each row *******

	for (i = (NUM_ROWS - 1); i >= 0; i--)
	{
		if (*(PLP_PTR_IARRAY + i))
		{
			free(*(PLP_PTR_IARRAY + i)); // free (PLP_PTR_IARRAY[i])
			*(PLP_PTR_IARRAY + i) = NULL;
			printf("Memory allocated to row %d has been Successfully freed !!! \n\n", i);
				
		}
	}

	// Freeing the memory of PLP_PTR_IARRAY which is the array of 5 integer pointers
	
	if (PLP_PTR_IARRAY)
	{
		free(PLP_PTR_IARRAY);
		PLP_PTR_IARRAY = NULL;
		printf("Memory allocated to PLP_PTR_IARRAY has been successfully freed !!! \n\n");
	}

	return(0);


}