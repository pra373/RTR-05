#include<stdio.h>

int main(void)
{
	// variable declarations
	int PLP_I = 10;

	//code
	printf("\n\n");
	printf("Printing Digits 10 to 1 : \n\n");
	do
	{
		printf("\t%d\n", PLP_I);
		PLP_I--;
	} while (PLP_I >= 1);

	printf("\n\n");

	return(0);


}