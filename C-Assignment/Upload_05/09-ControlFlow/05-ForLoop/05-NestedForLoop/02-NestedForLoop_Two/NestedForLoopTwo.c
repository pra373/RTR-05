#include<stdio.h>
int main(void)
{
	//variable declaration
	int PLP_I, PLP_J, PLP_K;

	//code
	printf("\n\n");
	for (PLP_I = 1; PLP_I <= 10; PLP_I++)
	{
		printf("PLP_I = %d\n", PLP_I);
		printf("--------------\n\n");
		for (PLP_J = 1; PLP_J <= 5; PLP_J++)
		{
			printf("\tPLP_J = %d\n", PLP_J);
			printf("\t---------------\n\n");
			for (PLP_K = 1; PLP_K <= 3; PLP_K++)
			{
				printf("\t\tPLP_K = %d\n", PLP_K);
			}
			printf("\n\n");
		}
		printf("\n\n");
	}
	return(0);

}