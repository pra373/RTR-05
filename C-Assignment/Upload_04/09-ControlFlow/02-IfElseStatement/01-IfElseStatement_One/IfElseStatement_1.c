#include<stdio.h>

int main(void)
{
	//variable declaration

	int PLP_A, PLP_B, PLP_P;

	//code

	PLP_A = 9;
	PLP_B = 30;
	PLP_P = 30;

	// *** First if-else pair ***

	if (PLP_A < PLP_B)
	{
		printf("\n");
		printf("Entering first if-block...\n\n");
		printf("PLP_A is less than PLP_B !!\n\n");

	}

	else
	{
		printf("Entering first else-block...\n\n");
		printf("PLP_A is not less than PLP_B !!\n\n");

	}

	printf("First if-else pair done !!\n\n");

	//*** Second if-else pair ***
	printf("\n\n");

	if (PLP_B != PLP_P)
	{
		printf("Entering second if-block...\n\n");
		printf("PLP_B is not equal to PLP_P !!\n\n");

	}

	else
	{
		printf("Entering second else-block...\n\n");
		printf("PLP_B is equal to PLP_P !!\n\n");

	}

	printf("second if-else pair done !!\n\n");

	return(0);


		


}
