#include<stdio.h>

int main()
{
	// variable declaration

	int PLP_I=1, PLP_J=10;

	//code

	printf("\n\n");

	printf("Printing digits from 1 to 10 and 10 to 100: \n\n");

	while (PLP_I <= 10, PLP_J <= 100)
	{
		printf("\t %d \t %d \n", PLP_I, PLP_J);
		PLP_I++;
		PLP_J += 10;
	}

	printf("\n\n");

	return(0);


}