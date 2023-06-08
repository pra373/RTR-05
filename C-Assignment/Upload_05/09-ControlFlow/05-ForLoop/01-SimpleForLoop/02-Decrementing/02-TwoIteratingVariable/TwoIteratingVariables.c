#include<stdio.h>

int main(void)
{
	// variable declaration
	int PLP_I, PLP_J;

		//code

		printf("\n\n");

	printf("Printing digits from 10 to 1 and 100 to 10 : \n\n");

	for (PLP_I = 10,PLP_J=100; PLP_I >= 1,PLP_J>=10; PLP_I--,PLP_J=PLP_J-10)
	{
		printf("\t %d \t %d \n", PLP_I,PLP_J);

	}

	printf("\n\n");

	return(0);

}