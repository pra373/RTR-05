#include<stdio.h>

// ********** User Defined Function : First method of calling functions ********
// ********** Calling all functions in main () directly ********

int main(int argc, char* argv[], char* envp[])
{
	//Function prototypes or declarations
	void MyAddition(void);
	int MySubtraction(void);
	void MyMultiplication(int, int);
	int MyDivision(int, int);

	//Variable declarations

	int PLP_SUB_RESULT, PLP_AM, PLP_BM, PLP_AD, PLP_BD, PLP_DIV_RESULT;

	//Addition function call

	MyAddition();

	//Subtraction function call

	PLP_SUB_RESULT = MySubtraction();
	printf("\n\n");
	printf("Subtraction yields result = %d\n", PLP_SUB_RESULT);

	//Multiplication Function call

	printf("\n\n");
	printf("Enter Integer value for 'PLP_AM' for Multiplication : ");
	scanf("%d", &PLP_AM);

	printf("\n\n");
	printf("Enter Integer value for 'PLP_BM' for Multiplication : ");
	scanf("%d", &PLP_BM);

	MyMultiplication(PLP_AM, PLP_BM);

	//Division function call

	printf("\n\n");
	printf("Enter Integer value for 'PLP_AD' for Division : ");
	scanf("%d", &PLP_AD);

	printf("\n\n");
	printf("Enter Integer value for 'PLP_BD' for Division : ");
	scanf("%d", &PLP_BD);

	PLP_DIV_RESULT = MyDivision(PLP_AD, PLP_BD);
	printf("\n\n");

	printf("Division of %d and %d gives = %d (Quotient)\n", PLP_AD, PLP_BD, PLP_DIV_RESULT);
	printf("\n\n");

	return(0);


}

void MyAddition(void) //Function definition
{
	//variable declarations
	int PLP_A, PLP_B, PLP_SUM; //local variables

	printf("\n\n");
	printf("Enter Integer value for 'PLP_A' for addition : ");
	scanf("%d", &PLP_A);

	printf("\n\n");
	printf("Enter Integer value for 'PLP_B' for addition : ");
	scanf("%d", &PLP_B);

	PLP_SUM = PLP_A + PLP_B;
	printf("\n\n");
	printf("Sum of %d and %d = %d\n\n", PLP_A, PLP_B, PLP_SUM);



}

int MySubtraction(void)  //function definition
{
	//variable declarations
	int PLP_A, PLP_B, PLP_RESULT;

	//code

	printf("\n\n");
	printf("Enter Integer value for 'PLP_A' for Subtraction : ");
	scanf("%d", &PLP_A);

	printf("\n\n");
	printf("Enter Integer value for 'PLP_B' for Subtraction : ");
	scanf("%d", &PLP_B);

	PLP_RESULT = PLP_A - PLP_B;
	return(PLP_RESULT);


}

void MyMultiplication(int PLP_A, int PLP_B) // Function definition
{
	//variable declarations
	int PLP_RESULT;

	//code

	PLP_RESULT = PLP_A * PLP_B;
	printf("\n\n");
	printf("Multiplication of %d and %d = %d", PLP_A, PLP_B, PLP_RESULT);
	

}

int MyDivision(int PLP_A, int PLP_B)
{
	//variable declarations
	
	int PLP_RESULT;

	//code

	if (PLP_A > PLP_B)
	{
		PLP_RESULT = PLP_A / PLP_B;

	}

	else
	{
		PLP_RESULT = PLP_B / PLP_A;
	}

	return(PLP_RESULT);

}



