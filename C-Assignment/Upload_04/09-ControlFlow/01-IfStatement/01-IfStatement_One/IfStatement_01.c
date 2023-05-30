#include<stdio.h>

int main(void)
{
	//variable declarations
	int PLP_A, PLP_B, PLP_P;

	//code

	PLP_A = 9;
	PLP_B = 30;
	PLP_P = 30;

	printf("\n\n");
	if (PLP_A < PLP_B)
	{
		printf("PLP_A is less than PLP_B !!!\n\n");

	}

	if (PLP_B != PLP_P)
	{
		printf("PLP_B is not equal to PLP_P !!!\n\n");
	}

	printf("Both comparisons have been done !!!\n\n");

	return(0);
}