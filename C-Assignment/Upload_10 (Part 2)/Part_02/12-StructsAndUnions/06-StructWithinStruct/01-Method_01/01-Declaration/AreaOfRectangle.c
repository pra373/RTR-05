#include<stdio.h>

struct rectangle
{
	struct MyPoint
	{
		int PLP_Y;
		int PLP_X;
	}point_01, point_02;
}rect;

int main(void)
{
	//variable declaration

	int length, breadth, area;

	//code
	printf("\n\n");
	printf("Enter leftmost X-Coordinate of rectangle  :  ");
	scanf("%d", &rect.point_01.PLP_X);

	printf("\n\n");
	printf("Enter Bottommost Y-Coordinate of rectangle  :  ");
	scanf("%d", &rect.point_01.PLP_Y);

	printf("\n\n");
	printf("Enter rightmost X-Coordinate of rectangle  :  ");
	scanf("%d", &rect.point_02.PLP_X);

	printf("\n\n");
	printf("Enter topmost Y-Coordinate of rectangle  :  ");
	scanf("%d", &rect.point_02.PLP_Y);

	length = rect.point_02.PLP_Y - rect.point_01.PLP_Y;
	if (length < 0)
	{
		length = length * -1;
	}

	breadth = rect.point_02.PLP_X - rect.point_01.PLP_X;
	if (breadth < 0)
	{
		breadth = breadth * -1;

	}

	area = length * breadth;

	printf("\n\n");
	printf("Length of rectangle  = %d\n\n", length);
	printf("Breadth of rectangle = %d\n\n", breadth);
	printf("Area of Rectangle = %d\n\n",area);

	return(0);





}
