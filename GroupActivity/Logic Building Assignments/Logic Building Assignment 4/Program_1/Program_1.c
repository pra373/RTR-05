#include<stdio.h>

int MultFact(int iNo)
{
	int i = 0;
	int multiplication = 1;
	if (iNo == 0)
	{
		return(0);
	}

	else if (iNo == 1)
	{
		return(1);
	}

	else if (iNo == 2)
	{
		return(2);
	}

	else if (iNo < 0)
	{
		iNo = -iNo;
			
	}

	for (i = 1; i <= iNo; i++)
	{
		if (iNo % i == 0)
		{
			multiplication = multiplication * i;
		}
	}

	return(multiplication);

}


int main()
{
	int iValue = 0;
	int iRet = 0;
	printf("\nEnter number:\t");
	scanf("%d", &iValue);
	iRet = MultFact(iValue);
	printf("multiplication of factors of %d is %d\n", iValue, iRet);
	return 0;
}