#include<stdio.h>

// Defining struct

struct MyPoint
{
	int PLP_X;
	int PLP_Y;
};

int main(void)
{
	//variable declaration

	struct MyPoint point_A, point_B, point_C, point_D, point_E; //declaration of 5 struct variables of type struct MyPoint

	//code
	// user input for data members of 'struct MyPoint point_A'
	printf("\n\n");
	printf("Enter X-Coordinate for point 'A' : ");
	scanf("%d", &point_A.PLP_X);
	printf("Enter Y-Coordinate for point 'A' : ");
	scanf("%d", &point_A.PLP_Y);

	// user input for data members of 'struct MyPoint point_B'

	printf("\n\n");
	printf("Enter X-Coordinate for point 'B' : ");
	scanf("%d", &point_B.PLP_X);
	printf("Enter Y-Coordinate for point 'B' : ");
	scanf("%d", &point_B.PLP_Y);

	// user input for data members of 'struct MyPoint point_C'

	printf("\n\n");
	printf("Enter X-Coordinate for point 'C' : ");
	scanf("%d", &point_C.PLP_X);
	printf("Enter Y-Coordinate for point 'C' : ");
	scanf("%d", &point_C.PLP_Y);

	// user input for data members of 'struct MyPoint point_D'

	printf("\n\n");
	printf("Enter X-Coordinate for point 'D' : ");
	scanf("%d", &point_D.PLP_X);
	printf("Enter Y-Coordinate for point 'D' : ");
	scanf("%d", &point_D.PLP_Y);

	// user input for data members of 'struct MyPoint point_E'

	printf("\n\n");
	printf("Enter X-Coordinate for point 'E' : ");
	scanf("%d", &point_E.PLP_X);
	printf("Enter Y-Coordinate for point 'E' : ");
	scanf("%d", &point_E.PLP_Y);

	// Displaying values of all data members of all struct variables

	printf("\n\n");
	printf("Co-ordinates (x,y) of point 'A' are : (%d,%d)\n\n", point_A.PLP_X, point_A.PLP_Y);
	printf("Co-ordinates (x,y) of point 'B' are : (%d,%d)\n\n", point_B.PLP_X, point_B.PLP_Y);
	printf("Co-ordinates (x,y) of point 'C' are : (%d,%d)\n\n", point_C.PLP_X, point_C.PLP_Y);
	printf("Co-ordinates (x,y) of point 'D' are : (%d,%d)\n\n", point_D.PLP_X, point_D.PLP_Y);
	printf("Co-ordinates (x,y) of point 'E' are : (%d,%d)\n\n", point_E.PLP_X, point_E.PLP_Y);

	return(0);










}