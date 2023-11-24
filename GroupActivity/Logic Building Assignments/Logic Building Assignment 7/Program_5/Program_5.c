#include<stdio.h>

double SquareMeter(int iValue)
{
	// Logic
	double squareMeter = 0.0929f * iValue;
	return squareMeter;

}

int main()
{
	int iValue = 0;
	double dRet = 0.0;
	printf("\nEnter area in square feet:\t");
	scanf("%d", &iValue);
	printf("\n");
	dRet = SquareMeter(iValue);
	printf("Area in square meter : %.6lf\t", dRet);
	printf("\n");
	return 0;
}