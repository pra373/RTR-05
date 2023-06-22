#include<stdio.h>

int main(void)
{
	//variable declaration
	int PLP_IARRAY[5][3] = { {1,2,3},{2,4,6},{3,6,9},{4,8,12},{5,10,15} }; //Inline initialization

	int int_size, int_array_size, array_num_elements;
	int array_num_rows, array_num_columns;

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

	// **** Row 1 *****

	printf("************** Row 1 ***************\n");
	printf("PLP_IARRAY[0][0] = %d\n", PLP_IARRAY[0][0]); // Column 1 (0th element) 
	printf("PLP_IARRAY[0][1] = %d\n", PLP_IARRAY[0][1]); // Column 1 (1st element) 
	printf("PLP_IARRAY[0][2] = %d\n", PLP_IARRAY[0][2]); // Column 1 (2nd element) 
	printf("\n\n");

	// **** Row 2 *****
	printf("************** Row 2 ***************\n");
	printf("PLP_IARRAY[1][0] = %d\n", PLP_IARRAY[1][0]); // Column 1 (0th element) 
	printf("PLP_IARRAY[1][1] = %d\n", PLP_IARRAY[1][1]); // Column 1 (1st element) 
	printf("PLP_IARRAY[1][2] = %d\n", PLP_IARRAY[1][2]); // Column 1 (2nd element) 
	printf("\n\n");

	// **** Row 3 *****
	printf("************** Row 3 ***************\n");
	printf("PLP_IARRAY[2][0] = %d\n", PLP_IARRAY[2][0]); // Column 1 (0th element) 
	printf("PLP_IARRAY[2][1] = %d\n", PLP_IARRAY[2][1]); // Column 1 (1st element) 
	printf("PLP_IARRAY[2][2] = %d\n", PLP_IARRAY[2][2]); // Column 1 (2nd element) 
	printf("\n\n");

	// **** Row 4 *****
	printf("************** Row 4 ***************\n");
	printf("PLP_IARRAY[3][0] = %d\n", PLP_IARRAY[3][0]); // Column 1 (0th element) 
	printf("PLP_IARRAY[3][1] = %d\n", PLP_IARRAY[3][1]); // Column 1 (1st element) 
	printf("PLP_IARRAY[3][2] = %d\n", PLP_IARRAY[3][2]); // Column 1 (2nd element) 
	printf("\n\n");

	// **** Row 5 *****
	printf("************** Row 5 ***************\n");
	printf("PLP_IARRAY[4][0] = %d\n", PLP_IARRAY[4][0]); // Column 1 (0th element) 
	printf("PLP_IARRAY[4][1] = %d\n", PLP_IARRAY[4][1]); // Column 1 (1st element) 
	printf("PLP_IARRAY[3][0] = %d\n", PLP_IARRAY[4][2]); // Column 1 (2nd element) 
	printf("\n\n");

	return(0);



}