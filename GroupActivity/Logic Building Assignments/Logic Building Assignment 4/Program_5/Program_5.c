#include<stdio.h>


int FactDiff(int iNo)
{
	int i;
	int sum_Factors = 0;
	int sum_NonFactors = 0;

	if (iNo == 0)
	{
		return(0);
	}

	else if (iNo == 1)
	{
		return(1);
	}

	


	else
	{
		printf("\n\nAs there are infinite number of non factors of every number, only considering till the number chosen by you.\n\n");

		for (i = 1; i <= iNo; i++)
		{
			if (iNo % i == 0)
			{
				sum_Factors = sum_Factors + i;
			}

			else
			{
				sum_NonFactors = sum_NonFactors + i;
			}
		}
		printf("Factors sum = %d\n", sum_Factors);
		printf("Non Factors sum = %d\n\n", sum_NonFactors);
		return(sum_Factors - sum_NonFactors);
	}
}


int main()
{
	int iValue = 0;
	int iRet = 0;
	printf("\nEnter number:\t");
	scanf("%d", &iValue);
	iRet = FactDiff(iValue);
	printf("(Factor's sum) - (Non factor's sum) = %d\n", iRet);
	return 0;
}