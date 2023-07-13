#include<stdio.h>
#include<stdarg.h>

#define NUM_TO_BE_FOUND 3
#define NUM_ELEMENTS 10

int main(void)
{
	//function prototype
	void FindNumber(int, int, ...);

	//code

	printf("\n\n");

	FindNumber(NUM_TO_BE_FOUND, NUM_ELEMENTS, 10, 42, 23, 34, 17, 987, 991, 1005, 1987, 2000);

	return(0);

}

void FindNumber(int num_to_be_found, int num, ...)
{
	//variable declaration
	int PLP_COUNT = 0;
	int PLP_N;
	va_list number_list;

	//code

	va_start(number_list, num);

	while (num)
	{
		PLP_N = va_arg(number_list, int);
		if (PLP_N == num_to_be_found)
		{
			PLP_COUNT++;
		}
		num--;
	}

	if (PLP_COUNT == 0)
	{
		printf("Number %d is not present in the list !! \n\n", num_to_be_found);

	}

	else
	{
		printf("Number %d is present in the list !! \n\n", num_to_be_found);

	}

	va_end(number_list);
}

