#include<stdio.h>

int main(int argc, char* argv[], char* envp[])
{
	int MyAddition(int, int); // Function declaration or function signature or function prototype

	// variable declaration

	int PLP_A, PLP_B,PLP_RESULT;

	//code

	printf("\n\n");
	printf("Enter Integer Value for 'PLP_A' : ");
	scanf("%d", &PLP_A);

	printf("\n\n");
	printf("Enter Integer Value for 'PLP_B' : ");
	scanf("%d", &PLP_B);



	PLP_RESULT = MyAddition(PLP_A, PLP_B);  //Function call
	printf("\n\n");
	printf("Sum of %d and %d = %d\n\n", PLP_A, PLP_B, PLP_RESULT);

	return(0);

}

int MyAddition(PLP_A, PLP_B)  //Function defination ('int' return value and two parameters)
{
	//Variable declaration : Local to MyAddition function.
	int PLP_SUM;

	//code
	PLP_SUM = PLP_A + PLP_B;
	return(PLP_SUM);
	

}  //Definition body ends
