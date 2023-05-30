#include<stdio.h>

int main(void)
{
	// variable declarations

	int PLP_NUM;

	//code

	printf("\n\n");
	printf("Enter value for 'num' : ");
	scanf("%d", &PLP_NUM);

	if (PLP_NUM < 0)
	{
		printf("Num = %d Is less than zero (Negative).\n\n", PLP_NUM);

	}

	if ((PLP_NUM > 0) && (PLP_NUM <= 100))
	{
		printf("Num = %d Is Between 0 and 100.\n\n", PLP_NUM);

	}

	if ((PLP_NUM > 100) && (PLP_NUM <= 200))
	{
		printf("Num = %d Is Between 100 and 200.\n\n", PLP_NUM);

	}

	if ((PLP_NUM > 200) && (PLP_NUM <= 300))
	{
		printf("Num = %d Is Between 200 and 300.\n\n", PLP_NUM);

	}

	if ((PLP_NUM > 300) && (PLP_NUM <= 400))
	{
		printf("Num = %d Is Between 300 and 400.\n\n", PLP_NUM);

	}

	if ((PLP_NUM > 400) && (PLP_NUM <= 500))
	{
		printf("Num = %d Is Between 400 and 500.\n\n", PLP_NUM);

	}

	if (PLP_NUM > 500)
	{
		printf("Num = %d Is greater than 500.\n\n", PLP_NUM);

	}

	return(0);
	
		
	
}