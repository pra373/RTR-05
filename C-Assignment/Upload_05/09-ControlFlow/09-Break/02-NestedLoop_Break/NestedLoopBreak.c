#include<stdio.h>
#include<conio.h>

int main(void)
{
	int PLP_I, PLP_J;

	//code

	for (PLP_I = 1; PLP_I <= 20; PLP_I++)
	{
		for (PLP_J = 1; PLP_J <= 20; PLP_J++)
		{
			if (PLP_J > PLP_I)
			{
				break;
			}
			else
			{
				printf("* ");
			}

		}
		printf("\n");
	}
	printf("\n\n");
	return(0);

}