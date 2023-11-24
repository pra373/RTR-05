#include<stdio.h>
double RectArea(float fWidth, float fHeight)
{
	// Logic
	double area = fWidth * fHeight;
	
	return (area);

}
int main()
{

	float fValue1 = 0.0, fValue2 = 0.0;
	double dRet = 0.0;
	printf("\nEnter width :\t");
	scanf("%f", &fValue1);
	printf("\n");
	printf("\nEnter height :\t");
	scanf("%f", &fValue2);
	printf("\n");
	dRet = RectArea(fValue1, fValue2);
	printf("Area of rectangle = %.3lf\n",dRet);
	return 0;

}