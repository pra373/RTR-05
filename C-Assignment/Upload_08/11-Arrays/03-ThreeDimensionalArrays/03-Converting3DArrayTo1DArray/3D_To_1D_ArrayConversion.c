#include<stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3
#define DEPTH 2

int main(void)
{
	//variable declaration

	//Inline Initialization

	int PLP_IARRAY_3D[NUM_ROWS][NUM_COLUMNS][DEPTH]= { { {9,18},{27,36},{45,54}},
								                    { {8,16},{24,32},{40,48}},
								                    { {7,14},{21,28},{35,42}},
								                    { {6,12},{18,24},{30,36}},
								                    { {5,10},{15,20},{25,30}} };

	int i, j, k;
	int PLP_IARRAY_1D[NUM_ROWS * NUM_COLUMNS * DEPTH]; //Number of Elements in 1D array

	//code

	// Display in 3D Array

	printf("\n\n");

	for (i = 0; i < NUM_ROWS; i++)
	{
		printf("*********** ROW %d ************\n",( i + 1));
		for (j = 0; j < NUM_COLUMNS; j++)
		{
			printf("*********** COLUMN %d ************\n", (j + 1));
			for (k = 0; k < DEPTH; k++)
			{
				printf("PLP_IARRAY_3D[%d][%d][%d] = %d\n", i, j, k, PLP_IARRAY_3D[i][j][k]);
			}
			printf("\n");
		}
		printf("\n\n");

	}

	//Converting 3D to 1D

	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLUMNS; j++)
		{
			for (k = 0; k < DEPTH; k++)
			{
				PLP_IARRAY_1D[(i * NUM_COLUMNS * DEPTH) + (j * DEPTH) + k] = PLP_IARRAY_3D[i][j][k];
			}
		}
	}

	// DIsplay 1D array

	printf("\n\n\n\n");
	printf("Elements in 1D array : \n\n");

	for (i = 0; i < (NUM_ROWS * NUM_COLUMNS * DEPTH); i++)
	{
		printf("PLP_IARRAY_1D[%d] = %d\n", i,PLP_IARRAY_1D[i]);
	}

	return(0);





}