#include<stdio.h>
int main(void)
{
	// variable declarations
	float PLP_F;
	float PLP_NUM = 1.7f;

	//code

	printf("\n\n");
	printf("Printing Numbers %f to %f : \n\n", PLP_NUM, (PLP_NUM * 10.0f));

	for (PLP_F = PLP_NUM; PLP_F <= (PLP_NUM * 10.0f); PLP_F = PLP_F + PLP_NUM)
	{
		printf("\t%f\n", PLP_F);
	}

	printf("\n\n");

	return(0);

}