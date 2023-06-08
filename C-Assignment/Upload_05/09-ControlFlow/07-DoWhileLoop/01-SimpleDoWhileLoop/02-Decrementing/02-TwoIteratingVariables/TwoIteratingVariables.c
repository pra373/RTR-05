#include<stdio.h>
int main(void)
{
	//variable declaration
	int PLP_I = 10, PLP_J = 100;

	//code
	printf("\n\n");
	printf("Printing Digits from 10 to 1 and 100 to 10 : \n\n");
	do
	{
		printf("\t %d \t %d\n", PLP_I, PLP_J);
		PLP_I--;
		PLP_J -= 10;
	} while (PLP_I >= 1, PLP_J >= 10);
	printf("\n\n");
	return(0);
}