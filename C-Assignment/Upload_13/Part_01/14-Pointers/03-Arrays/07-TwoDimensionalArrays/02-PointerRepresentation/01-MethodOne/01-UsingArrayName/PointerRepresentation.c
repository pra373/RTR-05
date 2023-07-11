#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
	//variable declaration
	int PLP_IARRAY[NUM_ROWS][NUM_COLUMNS];
	int i, j;

	//code
	// Every row of a 2D array is an integer array itself 
	// so each row will have its own base address

	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLUMNS; j++)
		{
			*(PLP_IARRAY[i] + j) = (i + 1) * (j + 1); // 'PLP_IARRAY[i] can be Treated as 1D array using pointers ....
		}
	}

	printf("\n\n");
	printf("2D Integer arrayelements along with Addresses : \n\n");
	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLUMNS; j++)
		{
			printf("*(PLP_IARRAY[%d] + [%d]) = %d \t \t at address (PLP_IARRAY[i] + j) : %p \n", i, j, *(PLP_IARRAY[i] + j), (PLP_IARRAY[i] + j));
		}
		printf("\n\n");
	}

	return(0);


}