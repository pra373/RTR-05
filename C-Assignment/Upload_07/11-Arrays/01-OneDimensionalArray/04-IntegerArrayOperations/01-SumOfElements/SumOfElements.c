#include<stdio.h>

#define PLP_NUM_ELEMENTS 10

int main(void)
{
	//variable declaration

	int PLP_IARRAY[PLP_NUM_ELEMENTS];
	int PLP_I, PLP_SUM = 0;

	//code

	printf("\n\n");
	printf("Enter Integer Elements for Array (Upto 10 Elements) for sumation : \n\n");

	for (PLP_I = 0; PLP_I < PLP_NUM_ELEMENTS; PLP_I++)
	{
		scanf("%d", &PLP_IARRAY[PLP_I]);
	}

	for (PLP_I = 0; PLP_I < PLP_NUM_ELEMENTS; PLP_I++)
	{
		PLP_SUM = PLP_SUM + PLP_IARRAY[PLP_I];
	}

	printf("\n\n");
	printf("Sum of all Elements of Array = %d \n\n", PLP_SUM);

	return(0);

}