#include<stdio.h>

int main(void)
{
	//variable declaration
	int PLP_AGE;

	//code
	printf("\n\n");
	printf("Enter AGE : ");
	scanf("%d", &PLP_AGE);
	printf("\n");
	if (PLP_AGE >= 18)
	{
		printf("You are Eligible for voting !!!\n\n");
	}
	printf("You are not Eligible for voting !!!\n\n");
	return(0);


}