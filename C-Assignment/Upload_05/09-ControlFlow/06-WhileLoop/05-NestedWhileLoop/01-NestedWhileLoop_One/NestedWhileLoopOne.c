#include<stdio.h>
int main(void)
{
	//variable declaration

	int PLP_I, PLP_J;

	//code
	printf("\n\n");

	PLP_I = 1;
	while (PLP_I <= 10)
	{
		printf("PLP_I = %d\n", PLP_I);
		printf("------------------\n\n");
		PLP_J = 1;
		while (PLP_J <= 5)
		{
			printf("\tPLP_J = %d\n", PLP_J);
			PLP_J++;
		}
		PLP_I++;
		printf("\n\n");
		
	}
	return(0);
}