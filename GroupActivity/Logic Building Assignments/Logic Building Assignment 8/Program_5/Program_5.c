#include<stdio.h>
void RangeDisplayRev(int iStart, int iEnd)
{
	// Logic

	int i = 0;
	if (iStart >= iEnd)
	{
		printf("Invalid range\n");
	}

	else
	{
		for (i = iEnd; i >= iStart; i--)
		{
			printf("%d ", i);
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
	RangeDisplayRev(iValue1, iValue2);
	return 0;
}