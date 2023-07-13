// This is the same code as (method one), just creating two files for the sake of submission
//dont check this code again.




#include<stdio.h>
#include<stdarg.h>

int main(void)
{
	//function prototype
	int calculatesum(int, ...);

	//variable declaration
	int PLP_ANSWER;

	//code

	printf("\n\n");

	PLP_ANSWER = calculatesum(5, 10, 20, 30, 40, 50);
	printf("Answer : %d\n\n", PLP_ANSWER);

	PLP_ANSWER = calculatesum(10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	printf("Answer : %d\n\n", PLP_ANSWER);

	PLP_ANSWER = calculatesum(0);
	printf("Answer : %d\n\n", PLP_ANSWER);

	return(0);




}

int calculatesum(int num, ...) // variadic function
{
	// variable declaration
		int PLP_SUM_TOTAL = 0, PLP_N;

	va_list number_list = NULL;

	//code
	va_start(number_list, num);

	while (num)
	{
		PLP_N = va_arg(number_list, int);
		PLP_SUM_TOTAL = PLP_SUM_TOTAL + PLP_N;
		num--;
	}

	va_end(number_list);
	return(PLP_SUM_TOTAL);

}


