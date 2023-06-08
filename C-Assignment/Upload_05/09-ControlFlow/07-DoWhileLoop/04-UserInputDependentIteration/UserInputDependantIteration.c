#include<stdio.h>

int main(void)
{
	//variable declarations

	int PLP_NUM, NUM, PLP_I;
	//code;

	printf("\n\n");
	printf("Enter an integer from which iteration must begin : ");
	scanf("%d", &PLP_NUM);

	printf("How many digits do you want to print from %d Onwards ? ", PLP_NUM);
	scanf("%d", &NUM);

	printf("Printing Digits from %d to %d : \n\n", PLP_NUM, (PLP_NUM + NUM));

	PLP_I = PLP_NUM;

	do
	{
		printf("\t%d\n", PLP_I);
		PLP_I++;
	} while (PLP_I <= (PLP_NUM + NUM));

	printf("\n\n");

	return(0);


}