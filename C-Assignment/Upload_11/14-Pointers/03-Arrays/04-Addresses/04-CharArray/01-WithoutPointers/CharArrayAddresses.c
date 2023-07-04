#include<stdio.h>

int main(void)
{
	//variable declaration
	char PLP_CARRAY[10];
	int i;

	//code
	for (i = 0; i < 10; i++)
	{
		PLP_CARRAY[i] = (char)(i + 65);
	}

	printf("\n\n");
	printf("Elements of the charecter array : \n\n");
	for (i = 0; i < 10; i++)
	{
		printf("PLP_CARRAY[%d] = %c\n", i, PLP_CARRAY[i]);
	}
	printf("\n\n");
	for (i = 0; i < 10; i++)
	{
		printf("PLP_CARRAY[%d] = %c \t \t Address = %p\n", i, PLP_CARRAY[i], &PLP_CARRAY[i]);
	}

	printf("\n\n");

	return(0);
}




