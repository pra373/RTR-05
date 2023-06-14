#include<stdio.h>

int main(int argc, char* argv[], char* envp[])
{
	//Function Prototype

	int MyAddition(int, int);

	//variable declaration

	int PLP_RESULT;
	int PLP_NUM01, PLP_NUM02;

	//code

	PLP_NUM01 = 30;
	PLP_NUM02 = 22;

	printf("\n\n");
	printf("%d + %d = %d\n", PLP_NUM01, PLP_NUM02, MyAddition(PLP_NUM01, PLP_NUM02));
	printf("\n\n");

	return(0);


}

int MyAddition(int PLP_A, int PLP_B)
{
	//function declaration
	int add(int, int);

	//code
	return(add(PLP_A, PLP_B)); //return value of add() as return value of MyAddition().

}

int add(int a, int b)
{
	return(a + b);
}