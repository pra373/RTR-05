#include<stdio.h>

int main(void)
{
	//variable declaration
	int PLP_A;

	//code

	printf("\n\n");

	PLP_A = 5;

	if (PLP_A) // non zero positive value
	{
		printf("if-block 1 : 'PLP_A' exists and has value = %d !!!\n\n", PLP_A);
		
	}

	PLP_A = -5;

	if (PLP_A) // non zero negative value
	{
		printf("if-block 2 : 'PLP_A' exists and has value = %d !!!\n\n", PLP_A);

	}

	PLP_A = 0;

	if (PLP_A) //  zero 
	{
		printf("if-block 3 : 'PLP_A' exists and has value = %d !!!\n\n", PLP_A);

	}

	printf("All Three if statements are Done !!\n\n");

	return(0);


}