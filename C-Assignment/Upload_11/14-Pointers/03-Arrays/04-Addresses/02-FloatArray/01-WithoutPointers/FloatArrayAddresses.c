#include<stdio.h>

int main(void)
{
	//variable declaration
	float PLP_FARRAY[10];
	int i;

	//code
	for (i = 0; i < 10; i++)
	{
		PLP_FARRAY[i] = (float)1.5f * (i + 1);
	}

	printf("\n\n");
	printf("Elements of the floating point array : \n\n");
	for (i = 0; i < 10; i++)
	{
		printf("PLP_FARRAY[%d] = %f\n", i, PLP_FARRAY[i]);
	}
	printf("\n\n");
	for (i = 0; i < 10; i++)
	{
		printf("PLP_FARRAY[%d] = %f \t \t Address = %p\n", i, PLP_FARRAY[i], &PLP_FARRAY[i]);
	}

	printf("\n\n");

	return(0);
}




