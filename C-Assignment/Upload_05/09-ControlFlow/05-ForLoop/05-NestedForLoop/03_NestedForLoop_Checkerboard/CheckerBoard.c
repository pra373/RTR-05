#include<stdio.h>

int main(void)
{
	// variable declarations
	int PLP_I, PLP_J, PLP_K;

	//code
	printf("\n\n");
	for (PLP_I = 0; PLP_I < 64; PLP_I++)
	{
		for (PLP_J = 0; PLP_J < 64; PLP_J++)
		{
			PLP_K = ((PLP_I & 0x8) == 0 ^ ((PLP_J & 0x8) == 0));

			if (PLP_K == 0)
			{
				printf("  ");
			}
			if (PLP_K == 1)
			{
				printf("* ");
			}
		}
		printf("\n\n");
	}

	return(0);
}