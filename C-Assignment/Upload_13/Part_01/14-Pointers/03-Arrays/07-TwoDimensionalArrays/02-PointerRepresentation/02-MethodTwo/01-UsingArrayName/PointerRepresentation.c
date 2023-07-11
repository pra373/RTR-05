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

	// Name of an array itself is its base address 
	// hence 'PLP_IARRAY' is base address of 2D Integer array PLP_IARRAY[]

	
	// PLP_IARRAY[0] => is the 0th row ... Hence,it is the base address of row 0
	// PLP_IARRAY[1] => is the 1th row ... Hence,it is the base address of row 1
	// PLP_IARRAY[2] => is the 2nd row ... Hence,it is the base address of row 2

	// (PLP_IARRAY[0]+0) => address of 0th element from base address of 0th row PLP_IARRAY[0][0]
	// (PLP_IARRAY[0]+1) => address of 1st element from base address of 0th row PLP_IARRAY[0][1]
	// (PLP_IARRAY[0]+2) => address of 2nd element from base address of 0th row PLP_IARRAY[0][2]

	// (PLP_IARRAY[1]+0) => address of 0th element from base address of 1st row PLP_IARRAY[1][0]
	// (PLP_IARRAY[1]+1) => address of 1st element from base address of 1st row PLP_IARRAY[1][1]
	// (PLP_IARRAY[1]+2) => address of 2nd element from base address of 2nd row PLP_IARRAY[1][2]

	//PLP_IARRAY[0], PLP_IARRAY[1] ... are 1D integer arrays and hencecan be treated as 1D integer arrays using pointers ....

	// (*(iarray + 0) + 0) = (PLP_IARRAY[0]+0) = Address of 0th element from base address of 0th row = (PLP_IARRAY[0] +0)=(PLP_IARRAY[0][0])

	// (*(iarray + 0) + 1) = (PLP_IARRAY[0]+1) = Address of 1th element from base address of 0th row = (PLP_IARRAY[0] +1)=(PLP_IARRAY[0][0])

	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLUMNS; j++)
		{
			*(*(PLP_IARRAY + i) + j) = (i + 1) * (j + 1);
		}
	}

	printf("\n\n");
	printf("2D integer array elements along with addresses : \n\n");
	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLUMNS; j++)
		{
			printf("*(*PLP_IARRAY + %d) + %d \t\t at address (*(PLP_IARRAY + %d) + %d) : %p\n", i, j, *(*(PLP_IARRAY + i) + j), i, j, (*(PLP_IARRAY + i) + j));
		}

		printf("\n\n");
	}

	return(0);

}