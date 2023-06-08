#include<stdio.h>
int main(void)
{
	//variable declaration
	int PLP_I, PLP_J;

	//code
	printf("\n\n");
	for (PLP_I = 1; PLP_I <= 10; PLP_I++)
	{
		printf("PLP_I = %d\n", PLP_I);
		printf("--------------\n\n");
		for (PLP_J = 1; PLP_J <= 5; PLP_J++)
		{
			printf("\tPLP_J = %d\n", PLP_J);
		}
		printf("\n\n");
	}
	return(0);

}