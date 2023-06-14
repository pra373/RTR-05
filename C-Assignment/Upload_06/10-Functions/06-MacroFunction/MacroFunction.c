#include<stdio.h>

#define MAX_NUMBER(a,b) ((a > b) ? a : b)

int main(int argc, char* argv[], char* envp[])
{
	//Variable declarations
	int PLP_NUM_01;
	int PLP_NUM_02;
	int PLP_RESULT;

	float PLP_FNUM_01;
	float PLP_FNUM_02;
	float FRESULT = 0.0f;

	//code
	//  ********* COMPARING INTEGER VALUES ******

	printf("\n\n");
	printf("Enter an Integer Number : \n\n");
	scanf("%d", &PLP_NUM_01);

	printf("\n\n");
	printf("Enter another Integer Number : \n\n");
	scanf("%d", &PLP_NUM_02);

	PLP_RESULT = MAX_NUMBER(PLP_NUM_01, PLP_NUM_02);
	printf("\n\n");
	printf("Result of Macro Function MAX_NUMBER() = %d\n", PLP_RESULT);

	printf("\n\n");

	//  ********* COMPARING FLOATING POINT VALUES ******

	printf("\n\n");
	printf("Enter an Floating Point Number : \n\n");
	scanf("%f", &PLP_FNUM_01);

	printf("\n\n");
	printf("Enter another Floating Point  Number : \n\n");
	scanf("%f", &PLP_FNUM_02);

	FRESULT = MAX_NUMBER(PLP_FNUM_01, PLP_FNUM_02);
	printf("\n\n");
	printf("Result of Macro Function MAX_NUMBER() = %f\n", FRESULT);

	printf("\n\n");

	return(0);



}