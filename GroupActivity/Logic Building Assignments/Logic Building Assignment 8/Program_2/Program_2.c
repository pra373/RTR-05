#include<stdio.h>
void RangeDisplayEven(int iStart, int iEnd)
{
	// Logic
	int i = 0;
	if (iStart >= iEnd)
	{
		printf("Invalid range\n");
	}

	else
	{
		for (i = iStart; i <= iEnd; i++)
		{
			if (i % 2 == 0)
			{
				printf("%d ", i);
			}
			
		}

		printf("\n");
	}
}
int main()
{
	int iValue1 = 0;
	int iValue2 = 0;
	printf("\nEnter Starting point:\t");
	scanf("%d", &iValue1);
	printf("\nEnter ending point:\t");
	scanf("%d", &iValue2);
	printf("\n\n");
	RangeDisplayEven(iValue1, iValue2);
	return 0;
}