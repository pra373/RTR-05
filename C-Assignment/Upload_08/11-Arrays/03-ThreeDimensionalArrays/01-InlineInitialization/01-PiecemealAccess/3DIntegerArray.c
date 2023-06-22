#include<stdio.h>

int main(void)
{
	//variable declaration

	//Inline Initialization

	int PLP_IARRAY[5][3][2] = { { {9,18},{27,36},{45,54}},
								{ {8,16},{24,32},{40,48}},
								{ {7,14},{21,28},{35,42}},
								{ {6,12},{18,24},{30,36}},
								{ {5,10},{15,20},{25,30}} };

	int int_size, int_array_size, array_num_elements;
	int array_width, array_height, array_depth;

	//code

	printf("\n\n");

	int_size = sizeof(int);
	int_array_size = sizeof(PLP_IARRAY);

	printf("Size of three dimentional (3D) Integer array is = %d \n\n", int_array_size);

	array_width = int_array_size / sizeof(PLP_IARRAY[0]);

	printf("Number of rows (width) in three dimentional (3D) Integer array is = %d \n\n", array_width);

	array_height = sizeof(PLP_IARRAY[0]) / sizeof(PLP_IARRAY[0][0]);

	printf("Number of columns (Height) in three dimentional (3D) Integer array is = %d \n\n", array_height);

	array_depth = sizeof(PLP_IARRAY[0][0]) / int_size;

	printf("Depth in three Dimensional (3D) integer array is = %d \n\n", array_depth);

	array_num_elements = array_width * array_height * array_depth;

	printf("Number of elements in three dimentional (3D) Integer array is = %d \n\n", array_num_elements);

	printf("\n\n");
	printf("Elements in 3D Array : \n\n");

	//*** PIECE MEAL DISPLAY
	//*** ROW 1 ***

	printf("********** ROW 1 ***********\n\n");
	printf("********** COLUMN 1 ***********\n\n");

	printf("PLP_IARRAY[0][0][0] = %d\n",PLP_IARRAY[0][0][0]);
	printf("PLP_IARRAY[0][0][1] = %d\n",PLP_IARRAY[0][0][1]);
	printf("\n");

	printf("********** COLUMN 2 ***********\n\n");

	printf("PLP_IARRAY[0][1][0] = %d\n",PLP_IARRAY[0][1][0]);
	printf("PLP_IARRAY[0][1][1] = %d\n",PLP_IARRAY[0][1][1]);
	printf("\n");

	printf("********** COLUMN 3 ***********\n\n");
	printf("PLP_IARRAY[0][2][0] = %d\n",PLP_IARRAY[0][2][0]);
	printf("PLP_IARRAY[0][2][1] = %d\n",PLP_IARRAY[0][2][1]);
	printf("\n");

	//********ROW 2 ********

	printf("********** ROW 2 ***********\n\n");

	printf("********** COLUMN 1 ***********\n\n");

	printf("PLP_IARRAY[1][0][0] = %d\n",PLP_IARRAY[1][0][0]);
	printf("PLP_IARRAY[1][0][1] = %d\n",PLP_IARRAY[1][0][1]);
	printf("\n");

	printf("********** COLUMN 2 ***********\n\n");

	printf("PLP_IARRAY[1][1][0] = %d\n",PLP_IARRAY[1][1][0]);
	printf("PLP_IARRAY[1][1][1] = %d\n",PLP_IARRAY[1][1][1]);
	printf("\n");

	printf("********** COLUMN 3 ***********\n\n");
	printf("PLP_IARRAY[1][2][0] = %d\n",PLP_IARRAY[1][2][0]);
	printf("PLP_IARRAY[1][2][1] = %d\n",PLP_IARRAY[1][2][1]);
	printf("\n");

	//********ROW 3 ********

	printf("********** ROW 3 ***********\n\n");

	printf("********** COLUMN 1 ***********\n\n");
	printf("PLP_IARRAY[2][0][0] = %d\n",PLP_IARRAY[2][0][0]);
	printf("PLP_IARRAY[2][0][1] = %d\n",PLP_IARRAY[2][0][1]);
	printf("\n");

	printf("********** COLUMN 2 ***********\n\n");

	printf("PLP_IARRAY[2][1][0] = %d\n",PLP_IARRAY[2][1][0]);
	printf("PLP_IARRAY[2][1][1] = %d\n",PLP_IARRAY[2][1][1]);
	printf("\n");

	printf("********** COLUMN 3 ***********\n\n");
	printf("PLP_IARRAY[2][2][0] = %d\n",PLP_IARRAY[2][2][0]);
	printf("PLP_IARRAY[2][2][1] = %d\n",PLP_IARRAY[2][2][1]);
	printf("\n");

	//******** ROW 4 ********

	printf("********** ROW 4 ***********\n\n");
	printf("********** COLUMN 1 ***********\n\n");
	printf("PLP_IARRAY[3][0][0] = %d\n",PLP_IARRAY[3][0][0]);
	printf("PLP_IARRAY[3][0][1] = %d\n",PLP_IARRAY[3][0][1]);
	printf("\n");

	printf("********** COLUMN 2 ***********\n\n");

	printf("PLP_IARRAY[3][1][0] = %d\n",PLP_IARRAY[3][1][0]);
	printf("PLP_IARRAY[3][1][1] = %d\n",PLP_IARRAY[3][1][1]);
	printf("\n");

	printf("********** COLUMN 3 ***********\n\n");
	printf("PLP_IARRAY[3][2][0] = %d\n",PLP_IARRAY[3][2][0]);
	printf("PLP_IARRAY[3][2][1] = %d\n",PLP_IARRAY[3][2][1]);
	printf("\n");

	//******** ROW 5 ********

	printf("********** ROW 5 ***********\n\n");
	printf("********** COLUMN 1 ***********\n\n");
	printf("PLP_IARRAY[4][0][0] = %d\n",PLP_IARRAY[4][0][0]);
	printf("PLP_IARRAY[4][0][1] = %d\n",PLP_IARRAY[4][0][1]);
	printf("\n");

	printf("********** COLUMN 2 ***********\n\n");

	printf("PLP_IARRAY[4][1][0] = %d\n",PLP_IARRAY[4][1][0]);
	printf("PLP_IARRAY[4][1][1] = %d\n",PLP_IARRAY[4][1][1]);
	printf("\n");

	printf("********** COLUMN 3 ***********\n\n");
	printf("PLP_IARRAY[4][2][0] = %d\n",PLP_IARRAY[4][2][0]);
	printf("PLP_IARRAY[4][2][1] = %d\n",PLP_IARRAY[4][2][1]);
	printf("\n");

	return(0);

}