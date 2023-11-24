#include<stdio.h>
void FactRev(int iNo)
{
	int i = 0;
	if (iNo < 0)
	{
		iNo = -iNo;
	}

	else if (iNo == 0)
	{
		printf("0\n");
	}

	else if (iNo == 1)
	{
		printf("1\n");
	}

	else
	{
		for (i = iNo; i >= 1; i--)
		{
			if (iNo % i == 0)
			{
				printf("%d\n", i);
			}
		}
	}
}
int main()
{
	int iValue = 0;
	printf("\nEnter Number:\t");
	scanf("%d", &iValue);
	printf("\nFactors of %d in reverse order:\n\n",iValue);
	FactRev(iValue);
	printf("\n");
	return 0;
}