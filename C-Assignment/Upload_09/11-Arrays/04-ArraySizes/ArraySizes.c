#include<stdio.h>

int main(void)
{
	//variable declarations

	int PLP_ARRAY_1D[5];
	int PLP_ARRAY_2D[5][3];
	int PLP_ARRAY_3D[100][100][5];

	int num_row_2D;
	int num_row_3D;

	int num_columns_2D;
	int num_columns_3D;
	int depth_3D;

	//code

	printf("\n\n");
	printf("Size of 1D integer array PLP_ARRAY_1D = %zu\n", sizeof(PLP_ARRAY_1D));
	printf("Number of elements in 1D integer array PLP_ARRAY_1D = %zu\n", (sizeof(PLP_ARRAY_1D) / sizeof(int)));

	printf("\n\n");
	printf("Size of 2D integer array PLP_ARRAY_2D = %zu\n", sizeof(PLP_ARRAY_2D));
	printf("Number of rows in 2D integer array PLP_ARRAY_2D = %zu\n ", (sizeof(PLP_ARRAY_2D) / sizeof(PLP_ARRAY_2D[0])));
	num_row_2D = (sizeof(PLP_ARRAY_2D) / sizeof(PLP_ARRAY_2D[0]));

	printf("Numbers of elements (columns) in each row in 2D integer array PLP_ARRAY_2D = %zu\n ", (sizeof(PLP_ARRAY_2D[0]) / sizeof(PLP_ARRAY_2D[0][0])));
	num_columns_2D = (sizeof(PLP_ARRAY_2D[0]) / sizeof(PLP_ARRAY_2D[0][0]));

	printf("Number of elements in total in 2D array PLP_ARRAY_2D = %d\n", num_row_2D * num_columns_2D);

	printf("\n\n");

	printf("\n\n");

	printf("Size of 3D integer array PLP_ARRAY_3D = %zu\n", sizeof(PLP_ARRAY_3D));

	printf("Number of rows in 3D integer array PLP_ARRAY_3D = %zu\n ", (sizeof(PLP_ARRAY_3D) / sizeof(PLP_ARRAY_3D[0])));

	num_row_3D = (sizeof(PLP_ARRAY_3D) / sizeof(PLP_ARRAY_3D[0]));

	printf("Numbers of elements (columns) in each row in 3D integer array PLP_ARRAY_3D = %zu\n ", (sizeof(PLP_ARRAY_3D[0]) / sizeof(PLP_ARRAY_3D[0][0])));

	num_columns_3D = (sizeof(PLP_ARRAY_3D[0]) / sizeof(PLP_ARRAY_3D[0][0]));

	printf("Number of elemnets (depth) in one column in one ow in 3D integer array PLP_ARRAY_3D = %zu \n", (sizeof(PLP_ARRAY_3D[0][0]) / sizeof(PLP_ARRAY_3D[0][0][0])));

	depth_3D = (sizeof(PLP_ARRAY_3D[0][0]) / sizeof(PLP_ARRAY_3D[0][0][0]));

	printf("Number of elements in total in 3D array PLP_ARRAY_3D = %d\n", num_row_3D * num_columns_3D*depth_3D);

	printf("\n\n");
	return(0);









}