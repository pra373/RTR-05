#include<stdio.h>
int main(void)
{
	//variable declaration
	int PLP_I = 1, PLP_J = 10;

	//code
	printf("\n\n");
	printf("Printing Digits from 1 to 10 and 10 to 100 : \n\n");
	do
	{
		printf("\t %d \t %d\n", PLP_I, PLP_J);
		PLP_I++;
		PLP_J += 10;
	} while (PLP_I <= 10, PLP_J <= 100);
	printf("\n\n");
	return(0);
}