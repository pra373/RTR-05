#include<stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
	//variable declaration
	int PLP_IARRAY_2D[NUM_ROWS][NUM_COLUMNS];
	int PLP_IARRAY_1D[NUM_ROWS * NUM_COLUMNS]; // Total Elements = rows*columns

	int i, j, num;

	//code

	printf("Enter the elements to fill up 2D integer array : \n\n");

	for (i = 0; i < NUM_ROWS; i++)
	{
		printf("for row number %d : \n", i + 1);
		for (j = 0; j < NUM_COLUMNS; j++)
		{
			printf("Enter Element Number %d : \n",(j + 1));
			scanf("%d", &num);
			PLP_IARRAY_2D[i][j] = num;

		}
		printf("\n\n");
	}

	// Display of 2D array

	printf("\n\n");
	printf("Two Dimensional (2D) array of Integers : \n\n");

	for (i = 0; i < NUM_ROWS; i++)
	{
		printf("**************ROW %d ************** : \n\n", i + 1);
		for(j = 0; j < NUM_COLUMNS; j++)
		{
			printf("PLP_IARRAY_2D[%d][%d] = %d\n", i, j, PLP_IARRAY_2D[i][j]);
		}

		printf("\n\n");
	}

	// Converting 2D integer array to 1D integer array

	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLUMNS; j++)
		{
			PLP_IARRAY_1D[(i * NUM_COLUMNS) + j] = PLP_IARRAY_2D[i][j];
		}
	}

	//printing 1D array

	printf("\n\n");
	printf("One Dimensional (1D) array of Integers : \n\n");
	for (i = 0; i < (NUM_ROWS* NUM_COLUMNS); i++)
	{
		printf("PLP_IARRAY_1D[%d] = %d\n", i, PLP_IARRAY_1D[i]);
	}

	printf("\n\n");

	return(0);







}