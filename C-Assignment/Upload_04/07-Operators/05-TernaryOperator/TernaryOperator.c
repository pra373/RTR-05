#include<stdio.h>

int main(void)
{
	//variable declarations
	int PLP_A, PLP_B, PLP_P, PLP_Q, i_result_01, i_result_02;
	char ch_result_01, ch_result_02;

	//code

	printf("\n\n");
	PLP_A = 7;
	PLP_B = 5;
	ch_result_01 = (PLP_A > PLP_B) ? 'A' : 'B';
	i_result_01 = (PLP_A > PLP_B) ? PLP_A : PLP_B;

	printf("Ternary operator answer 1 ------ %c and %d.\n\n", ch_result_01, i_result_01);

	PLP_P = 30;
	PLP_Q = 30;

	ch_result_02 = (PLP_P != PLP_Q) ? 'P' : 'Q';
	i_result_02 = (PLP_P != PLP_Q) ? PLP_P : PLP_Q;
	printf("Ternary operator answer 2 ------ %c and %d.\n\n", ch_result_02, i_result_02);

	printf("\n\n");

	return(0);




}