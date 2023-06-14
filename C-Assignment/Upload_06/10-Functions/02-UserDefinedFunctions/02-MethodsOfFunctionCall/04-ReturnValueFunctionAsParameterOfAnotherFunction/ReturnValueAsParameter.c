#include<stdio.h>
#include<conio.h>

int main(int argc, char* argv[], char* envp[])
{
	//Function Prototype
	int MyAddition(int, int);

	//variable declaration

	int PLP_RESULT, PLP_NUM01, PLP_NUM02, PLP_NUM03, PLP_NUM04;
	
	//code

	PLP_NUM01 = 10;
	PLP_NUM02 = 20;
	PLP_NUM03 = 30;
	PLP_NUM04 = 40;

	//return value of Myaddition() is sent as a parameter to another function of MyAddition();
	PLP_RESULT = MyAddition(MyAddition(PLP_NUM01, PLP_NUM02), MyAddition(PLP_NUM03, PLP_NUM04));

	printf("\n\n");
	printf("%d + %d + %d + %d = %d", PLP_NUM01, PLP_NUM02, PLP_NUM03, PLP_NUM04, PLP_RESULT);
	printf("\n\n");
	getch();

	return(0);

}

int MyAddition(int PLP_A, int PLP_B)
{
	//Variable declaration
	int PLP_SUM;

	//code
	PLP_SUM = PLP_A + PLP_B;
	return(PLP_SUM);
}
