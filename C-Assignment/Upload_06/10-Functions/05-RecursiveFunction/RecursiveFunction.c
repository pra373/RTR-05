#include<stdio.h>

int main(int argc, char* argv[], char* envp[])
{
	//Variable declarations
	unsigned int PLP_NUM;

	//function prototype
	void recursive(unsigned int);

	//code

	printf("\n\n");
	printf("Enter any number : \n\n");
	scanf("%u", &PLP_NUM);
	printf("\n\n");
	printf("Output of recursive function : \n\n");

	recursive(PLP_NUM);
	return(0);
}

void recursive(unsigned int PLP_A)
{
	//code
	printf("PLP_A = %d\n", PLP_A);
	if (PLP_A > 0)
	{
		recursive(PLP_A - 1);
	}
}