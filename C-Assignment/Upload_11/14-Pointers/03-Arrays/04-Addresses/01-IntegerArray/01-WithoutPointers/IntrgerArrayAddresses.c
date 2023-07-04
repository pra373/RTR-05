#include<stdio.h>

int main(void)
{
	//variable declaration
	int PLP_IARRAY[10];
	int i;

	//code
	for (i = 0; i < 10; i++)
	{
		PLP_IARRAY[i] = 3 * (i + 1);
	}

	printf("\n\n");
	printf("Elements of the integer array : \n\n");
	for (i = 0; i < 10; i++)
	{
		printf("PLP_IARRAY[%d] = %d\n", i, PLP_IARRAY[i]);
	}
	printf("\n\n");
	for (i = 0; i < 10; i++)
	{
		printf("PLP_IARRAY[%d] = %d \t \t Address = %p\n", i, PLP_IARRAY[i], &PLP_IARRAY[i]);
	}

	printf("\n\n");

	return(0);
}




