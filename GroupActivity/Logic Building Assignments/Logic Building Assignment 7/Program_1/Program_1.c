#include<stdio.h>

double CircleArea(float fRadius)
{
	// Logic
	double area = 3.14 * fRadius * fRadius;
	return area;

}

int main()
{
	float fValue = 0.0;
	double dRet = 0.0;
	printf("\nEnter radius : \t");
	scanf("%f", &fValue);
	printf("\n");
	dRet = CircleArea(fValue);
	printf("Area of circle is %.4lf\n", dRet);
	return 0;
}