#include<stdio.h>

int main(void)
{
	//variable declaration
	int PLP_I, PLP_J, PLP_K;

	//code
	printf("\n\n");
	PLP_I = 1;
	while (PLP_I <= 10)
	{
		printf("PLP_I = %d\n", PLP_I);
		printf("-----------\n\n");

		PLP_J = 1;
		while (PLP_J <= 5)
		{
			printf("\tPLP_J = %d\n", PLP_J);
			printf("\t-----------\n\n");
			PLP_K = 1;
			while (PLP_K <= 3)
			{
				printf("\t\tPLP_K = %d\n",PLP_K);
				PLP_K++;
			}
			printf("\n\n");
			PLP_J++;
				

		
		}
		printf("\n\n");
		PLP_I++;


	}

}