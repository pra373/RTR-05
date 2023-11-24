#include<stdio.h>
double FhtoCs(float fTemp)
{
	// Logic
	float celsius = 0.0f;
	celsius = (fTemp - 32.0f) * (5.0f / 9.0f);
	return celsius;
}

int main()
{
	float fValue = 0.0;
	double dRet = 0.0;
	printf("\nEnter temperature in Fahrenheit :\t");
	scanf("%f", &fValue);
	printf("\n");
	dRet = FhtoCs(fValue);
	printf("Temperature in celsius : %.4lf\n", dRet);
	return 0;
}