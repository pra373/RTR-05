#include<stdio.h>

int main(void)
{
	//variable declaration
	int PLP_NUM, num, i;

	//code
	printf("\n\n");

	printf("Enter an integer value from which iteration must begin : \n");
	scanf("%d", &PLP_NUM);
	printf("How many digits you want to print from %d onwards ? : \n", PLP_NUM);
	scanf("%d", &num);

	printf("Printing Digits %d to %d : \n\n", PLP_NUM, (PLP_NUM + num));
	i = PLP_NUM;
	while (i <= (PLP_NUM + num))
	{
		printf("\t%d\n", i);
		i++;
	}

	printf("\n\n");

	return(0);


}