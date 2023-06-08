#include<stdio.h>

int main(void)
{
	//variable declaration

	int PLP_I;

	//code
	printf("\n\n");

	printf("Printing Even Numbers From 0 to 100 : \n\n");

	for (PLP_I = 1; PLP_I <= 100; PLP_I++)
	{
		if (PLP_I % 2 != 0)
		{
			continue;
		}

		else
		{
			printf("\t%d\n", PLP_I);
		}


	}

	printf("\n\n");

	return(0);
}