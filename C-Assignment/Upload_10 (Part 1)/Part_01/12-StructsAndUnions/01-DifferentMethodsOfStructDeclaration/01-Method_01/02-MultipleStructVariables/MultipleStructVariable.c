#include<stdio.h>

//Defining struct

struct MyPoint
{
	int PLP_X;
	int PLP_Y;
}point_A, point_B, point_C, point_D, point_E; //declaration of 5 struct variables of type struct MyPoint globally

 

int main(void)
{
	//code
	//Assigning data values to data members of struct MyPoint variables

	point_A.PLP_X = 3;
	point_A.PLP_Y = 0;

	point_B.PLP_X = 1;
	point_B.PLP_Y = 2;

	point_C.PLP_X = 9;
	point_C.PLP_Y = 6;

	point_D.PLP_X = 8;
	point_D.PLP_Y = 2;

	point_E.PLP_X = 11;
	point_E.PLP_Y = 8;

	// Displaying values of all data members of all struct variables

	printf("\n\n");
	printf("Co-ordinates (x,y) of point 'A' are : (%d,%d)\n\n", point_A.PLP_X,point_A.PLP_Y);
	printf("Co-ordinates (x,y) of point 'B' are : (%d,%d)\n\n", point_B.PLP_X, point_B.PLP_Y);
	printf("Co-ordinates (x,y) of point 'C' are : (%d,%d)\n\n", point_C.PLP_X, point_C.PLP_Y);
	printf("Co-ordinates (x,y) of point 'D' are : (%d,%d)\n\n", point_D.PLP_X, point_D.PLP_Y);
	printf("Co-ordinates (x,y) of point 'E' are : (%d,%d)\n\n", point_E.PLP_X, point_E.PLP_Y);

	return(0);





}




