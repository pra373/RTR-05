#include<stdio.h>

void DisplayFactor(int iNo)
{
	int i = 0;
	if (iNo <= 0)
	{
		iNo = -iNo;
	}
	for (i = 1; i <= iNo; i++)
	{
		if (iNo %i == 0)
		{
			if (i% 2 == 0)
			{
				printf("%d -%d\t", i, i);
			}
		}
	}
}
int main()
{
	int iValue = 0;
	printf("\n Enter number :\t");
	scanf("%d", &iValue);
	printf("\n\n");
	DisplayFactor(iValue);
	printf("\n");
	return (0);
}
