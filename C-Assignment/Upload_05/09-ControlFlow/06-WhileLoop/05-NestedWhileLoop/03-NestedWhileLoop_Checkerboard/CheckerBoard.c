#include<stdio.h>

int main(void)
{
	//variable declaration

	int PLP_I, PLP_J, PLP_K;

	//code
	printf("\n\n");

	PLP_I = 0;
	while (PLP_I < 64)
	{
		PLP_J = 0;
		while (PLP_J < 64)
		{
			PLP_K = ((PLP_I & 0x8) == 0) ^ ((PLP_J & 0x8) == 0);
			if (PLP_K == 0)
			{
				printf(" ");

			}

			if (PLP_K == 1)
			{
				printf("* ");
			}
			PLP_J++;

		}
		printf("\n\n");
		PLP_I++;
	}
	return(0);
}