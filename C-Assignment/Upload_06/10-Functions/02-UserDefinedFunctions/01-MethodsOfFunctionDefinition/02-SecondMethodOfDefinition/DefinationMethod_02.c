#include<stdio.h>

int main(int argc, char* argv[], char* envp[])
{
	int MyAddition(void); // Function declaration or function signature or function prototype

	// variable declaration

	int PLP_RESULT;

	PLP_RESULT = MyAddition(); //Function call

	printf("Sum = %d\n\n",PLP_RESULT);

	
	return(0);

}

int MyAddition(void)  //Function defination ('int' return value and no parameters)
{
	//Variable declaration : Local to MyAddition function.
	int PLP_A, PLP_B, PLP_SUM;

	//code
	printf("\n\n");
	printf("Enter Integer Value for 'PLP_A' : ");
	scanf("%d", &PLP_A);

	printf("\n\n");
	printf("Enter Integer Value for 'PLP_B' : ");
	scanf("%d", &PLP_B);

	PLP_SUM = PLP_A + PLP_B;
	return(PLP_SUM);

}  //Definition body ends
