#include<stdio.h>

int main(void)
{
	int PLP_IARRAY[3][5]; //3 rows and 5 columns
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

	// ********* PIECE_MEAL ASSIGNMENT **************
	// ********* ROW 1 *********

	PLP_IARRAY[0][0] = 21;
	PLP_IARRAY[0][1] = 42;
	PLP_IARRAY[0][2] = 63;
	PLP_IARRAY[0][3] = 84;
	PLP_IARRAY[0][4] = 105;

	// ********* ROW 2 *********

	PLP_IARRAY[1][0] = 22;
	PLP_IARRAY[1][1] = 42;
	PLP_IARRAY[1][2] = 66;
	PLP_IARRAY[1][3] = 88;
	PLP_IARRAY[1][4] = 110;

	// ********* ROW 3 *********

	PLP_IARRAY[2][0] = 23;
	PLP_IARRAY[2][1] = 46;
	PLP_IARRAY[2][2] = 69;
	PLP_IARRAY[2][3] = 92;
	PLP_IARRAY[2][4] = 115;

	// ********* Display ********

	for (i = 0; i < array_num_rows; i++)
	{
		printf("************* ROW %d *************\n", (i + 1));
		for (j = 0; j < array_num_columns; j++)
		{
			printf("PLP_IARRAY[%d][%d] = %d\n", i, j, PLP_IARRAY[i][j]);
		}

		printf("\n\n");
	}

	return(0);














}