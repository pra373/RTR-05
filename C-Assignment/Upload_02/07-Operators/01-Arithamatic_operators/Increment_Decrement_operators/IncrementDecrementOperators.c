#include<stdio.h>

int main(void)
{
	//variable declarations

	int PLP_A = 5;
	int PLP_B = 10;

	//code

	printf("\n\n");
	printf("PLP_A = %d\n", PLP_A);  //answer should be 5
	printf("PLP_A = %d\n", PLP_A++); //answer should be 5
	printf("PLP_A = %d\n", PLP_A);   //answer should be 6
	printf("PLP_A = %d\n\n", ++PLP_A); //answer should be 7

	printf("\n\n");
	printf("PLP_B = %d\n", PLP_B);    //answer should be 10
	printf("PLP_B = %d\n", PLP_B--);  //answer should be 10
	printf("PLP_B = %d\n", PLP_B);    //answer should be 9
	printf("PLP_B = %d\n\n", --PLP_B); //answer should be 8

	getch(0);

	return(0);





}
