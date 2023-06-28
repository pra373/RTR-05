#include<stdio.h>
#include<string.h>

// defining struct

struct MyPoint
{
	int PLP_X;
	int PLP_Y;

};

//defining struct

struct MyPointProperties
{
	int quadrant;
	char axis_location[10];
};

int main(void)
{
	struct MyPoint point;  // declaring a single variable of type struct Mypoint locally
	struct MyPointProperties point_properties;  // declaring a single variable of type struct MyPointProperties locally

	//code
	//user input for data members of struct MyPoint Variable 'point_A'

	printf("\n\n");
	printf("Enter X-Coordinate for A point : ");
	scanf("%d", &point.PLP_X);
	printf("Enter Y-Coordinate for A point : ");
	scanf("%d", &point.PLP_Y);

	printf("\n\n");
	printf("Point co-ordinates (x,y) are (%d, %d) !!!\n\n", point.PLP_X, point.PLP_Y);
	if (point.PLP_X == 0 && point.PLP_Y == 0)
	{
		printf("The point is the origin (%d, %d) !!!\n\n", point.PLP_X, point.PLP_Y);

	}
	else //at least one coordinate in non zero
	{
		if (point.PLP_X == 0) // if X is zero, obviously Y must be a non zero value
		{
			if (point.PLP_Y < 0) //If Y is negative
				strcpy(point_properties.axis_location, "Negative Y");

			if (point.PLP_Y > 0) //If Y is positive
				strcpy(point_properties.axis_location, "Positive Y");

			point_properties.quadrant = 0; //point lying on coordinate axis is not a part of any quadrant

			printf("The point lies on the %s axis !!!\n\n", point_properties.axis_location);



		}
		else if (point.PLP_Y == 0) // if Y is zero, obviously X must be a non zero value
		{
			if (point.PLP_X < 0) //If X is negative
				strcpy(point_properties.axis_location, "Negative X");

			if (point.PLP_X > 0) //If X is positive
				strcpy(point_properties.axis_location, "Positive X");

			point_properties.quadrant = 0; //point lying on coordinate axis is not a part of any quadrant

			printf("The point lies on the %s axis !!!\n\n", point_properties.axis_location);
		}
		else //Both X and Y are non zero
		{
			//point_properties.axis_location = NULL ; // A point lying in any of the quadrants cannot be lying on any of the coordinate axes..

			if (point.PLP_X > 0 && point.PLP_Y > 0)  // X is positive and y is positive
			{
				point_properties.quadrant = 1;
			}
			else if (point.PLP_X < 0 && point.PLP_Y > 0) // X is negative and y is positive
			{
				point_properties.quadrant = 2;
			}

			else if (point.PLP_X < 0 && point.PLP_Y < 0)
			{
				point_properties.quadrant = 3;
			}

			else
			{
				point_properties.quadrant = 4;
			}

			printf("The point Lies in quadrant number %d !!!\n\n", point_properties.quadrant);
		}


	}

	return(0);

}


