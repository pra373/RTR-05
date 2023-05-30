#include<stdio.h>

int main(void)
{
	//Variable declarations

	int PLP_AGE;

	//code

	printf("\n\n");
	printf("Enter Age : ");
	scanf("%d", &PLP_AGE);
	if (PLP_AGE >= 18)
	{
		printf("You are eligible for voting !!\n\n");
	}

	return(0);

}