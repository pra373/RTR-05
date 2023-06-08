#include<stdio.h>

int main(void)
{
	//variable declarations

	int PLP_I, PLP_J;

	//code
	printf("\n\n");

	PLP_I = 1;
	do
	{
		printf("PLP_I = %d\n", PLP_I);
		printf("-----------\n\n");

		PLP_J = 1;

		do
		{
			printf("\tPLP_J = %d\n", PLP_J);
			PLP_J++;

		} while (PLP_J <= 5);

		PLP_I++;
		printf("\n\n");

	} while (PLP_I <= 10);
	return(0);
}