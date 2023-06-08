#include<stdio.h>

int main(void)
{
	//variable declaration
	int PLP_I, PLP_J;

	//code
	printf("\n\n");

	printf("Outer Loop Prints odd Numbers Between 1 and 10. \n\n");
	printf("Inner Loop Prints Even Numbers Between 1 and 10 for every odd number printed by outer loop.\n\n");

	for (PLP_I = 1; PLP_I <= 10; PLP_I++)
	{
		if (PLP_I % 2 != 0)
		{
			printf("PLP_I = %d\n", PLP_I);
			printf("-----------------\n");
			for (PLP_J = 1; PLP_J <= 10; PLP_J++)
			{
				if (PLP_J % 2 == 0)
				{
					printf("\tPLP_J = %d\n", PLP_J);
				}

				else
				{
					continue;
				}
			}
			printf("\n\n");
		}
		else
		{
			continue;
		}
			

		
	}

	printf("\n\n");
	return(0);

}
