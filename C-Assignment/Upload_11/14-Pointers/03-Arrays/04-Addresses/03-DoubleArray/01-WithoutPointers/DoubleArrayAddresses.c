#include<stdio.h>

int main(void)
{
	//variable declaration
	double PLP_DARRAY[10];
	int i;

	//code
	for (i = 0; i < 10; i++)
	{
		PLP_DARRAY[i] = (float)1.333333f * (i + 1);
	}

	printf("\n\n");
	printf("Elements of the 'double' array : \n\n");
	for (i = 0; i < 10; i++)
	{
		printf("PLP_DARRAY[%d] = %lf\n", i, PLP_DARRAY[i]);
	}
	printf("\n\n");
	for (i = 0; i < 10; i++)
	{
		printf("PLP_DARRAY[%d] = %lf \t \t Address = %p\n", i, PLP_DARRAY[i], &PLP_DARRAY[i]);
	}

	printf("\n\n");

	return(0);
}




