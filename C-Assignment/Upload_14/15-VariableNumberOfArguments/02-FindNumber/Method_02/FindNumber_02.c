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

	FindNumber(NUM_TO_BE_FOUND, NUM_ELEMENTS,3,5,10,12,17,34,67,45,76,54);

	return(0);
}

void FindNumber(int num_to_be_found, int num, ...)
{
	//function prototype
	int va_FindNumber(int, int, va_list);

	//variable declaration

	int PLP_COUNT = 0;
	int PLP_N;
	va_list number_list;

	//code

	va_start(number_list, num);

	PLP_COUNT = va_FindNumber(num_to_be_found, num, number_list);

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

int va_FindNumber(int num_to_be_found, int num, va_list list)
{
	//variable declaration
	int count_of_num = 0;
	int PLP_N;

	//code

	while (num)
	{
		PLP_N = va_arg(list, int);
		if (PLP_N == num_to_be_found)
		{
			count_of_num++;
		}
		num--;
	}

	return(count_of_num);
}







