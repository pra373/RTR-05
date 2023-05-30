#include<stdio.h>

int main(void)
{
	//variable declaration

	int PLP_NUM;

	//code
	printf("\n\n");

	printf("Enter value for 'PLP_NUM' : ");
	scanf("%d", &PLP_NUM);

	if (PLP_NUM < 0)
	{
		printf("PLP_NUM = %d Is less than 0 (negative) !!!\n\n", PLP_NUM);
		

	}
	else if (PLP_NUM == 0)
	{
		printf("PLP_NUM = %d Is equal to 0 !!!\n\n", PLP_NUM);
	}

	else if ((PLP_NUM > 0) && (PLP_NUM <= 100))
	{
		printf("PLP_NUM = %d Is between 0 and 100 !!!\n\n", PLP_NUM);

	}

	else if ((PLP_NUM > 100) && (PLP_NUM <= 200))
	{
		printf("PLP_NUM = %d Is between 100 and 200 !!!\n\n", PLP_NUM);
	}

	else if ((PLP_NUM > 200) && (PLP_NUM <= 300))
	{
		printf("PLP_NUM = %d Is between 200 and 300 !!!\n\n", PLP_NUM);
	}

	else if ((PLP_NUM > 300) && (PLP_NUM <= 400))
	{
		printf("PLP_NUM = %d Is between 300 and 400 !!!\n\n", PLP_NUM);
	}

	else if ((PLP_NUM > 400) && (PLP_NUM <= 500))
	{
		printf("PLP_NUM = %d Is between 400 and 500 !!!\n\n", PLP_NUM);
	}

	else if (PLP_NUM>500)
	{
		printf("PLP_NUM = %d Is greater than 500 !!!\n\n", PLP_NUM);


	}

	else // Terminating else for this if else ladder.
	{
		printf("Invalid Value Entered !!!\n\n");

	}

	

	return(0);




}