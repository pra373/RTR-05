#include<stdio.h>

int main(int argc, char* argv[], char* envp[])
{
	void MyAddition(void); // Function declaration or function signature or function prototype

	MyAddition(); //Function call
	return(0);

}

void MyAddition(void)  //Function defination (No return value no parameters)
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
	printf("\n\n");
	printf("SUm of %d and %d = %d\n\n", PLP_A, PLP_B, PLP_SUM);


}  //Definition body ends
