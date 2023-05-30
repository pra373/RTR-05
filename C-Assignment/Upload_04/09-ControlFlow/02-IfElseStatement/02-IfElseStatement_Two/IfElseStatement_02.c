#include<stdio.h>

int main(void)
{
	//Variable declarations

	int PLP_AGE;

	//code
	printf("\n\n");
	printf("Enter Age : ");
	scanf("%d", &PLP_AGE);
	printf("\n\n");

	if (PLP_AGE >= 18)
	{
		printf("Entering if-block...\n\n");
		printf("You are eligible for voting !!\n\n");


	}

	else
	{
		printf("Entering else-block...\n\n");
		printf("You are not eligible for voting !!\n\n");


	}

	printf("Bye !!!\n\n");
	return(0);

}