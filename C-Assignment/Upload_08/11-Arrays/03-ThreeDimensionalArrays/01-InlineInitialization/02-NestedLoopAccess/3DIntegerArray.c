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
	int i, k, j;

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

	for (i = 0; i < array_width; i++)
	{
		printf("*********** ROW %d ************\n", i + 1);
		for (j = 0; j < array_height; j++)
		{
			printf("*********** COLUMN %d ************\n", j + 1);
			for (k = 0; k < array_depth; k++)
			{
				printf("PLP_IARRAY[%d][%d][%d] = %d\n", i, j, k, PLP_IARRAY[i][j][k]);
			}
			printf("\n");
		}
		printf("\n\n");

	}

	return(0);

}
	
	
	




