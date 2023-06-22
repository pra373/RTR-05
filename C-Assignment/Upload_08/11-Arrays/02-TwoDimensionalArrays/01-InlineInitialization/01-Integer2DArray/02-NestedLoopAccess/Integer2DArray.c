#include<stdio.h>

int main(void)
{
	//variable declaration
	int PLP_IARRAY[5][3] = { {1,2,3},{2,4,6},{3,6,9},{4,8,12},{5,10,15} }; //Inline initialization

	int int_size, int_array_size, array_num_elements;
	int array_num_rows, array_num_columns;
	int i, j;

	//code

	printf("\n\n");

	int_size = sizeof(int);
	int_array_size = sizeof(PLP_IARRAY);

	printf("Size of two dimentional (2D) Integer array is = %d \n\n", int_array_size);

	array_num_rows = int_array_size / sizeof(PLP_IARRAY[0]);

	printf("Number of rows in two dimentional (2D) Integer array is = %d \n\n", array_num_rows);

	array_num_columns = sizeof(PLP_IARRAY[0]) / int_size;

	printf("Number of columns in two dimentional (2D) Integer array is = %d \n\n", array_num_columns);

	array_num_elements = array_num_rows * array_num_columns;

	printf("Number of elements in two dimentional (2D) Integer array is = %d \n\n", array_num_elements);

	printf("\n\n");
	printf("Elements in 2D Array : \n\n");

	// *** array indices begin from 0, Hence 1st row is 0th row and 1st column is 0th column ********

	for (i = 0; i < array_num_rows; i++)
	{
		printf("******** ROW %d *******\n\n", i + 1);
		for (j = 0; j < array_num_columns; j++)
		{
			printf("PLP_IARRAY[%d][%d] = %d\n", i, j, PLP_IARRAY[i][j]);
		}

		printf("\n\n");

	}

	return(0);
}