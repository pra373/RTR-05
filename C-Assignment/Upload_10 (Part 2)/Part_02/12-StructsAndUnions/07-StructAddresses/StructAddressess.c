#include<stdio.h>

struct MyData
{
	int PLP_I;
	float PLP_F;
	double PLP_D;
	char PLP_C;
};

int main(void)
{
	//variable declarations
	struct MyData data;

	//code
	//Assigning data Values to data members of struct my data

	data.PLP_I = 30;
	data.PLP_F = 11.45;
	data.PLP_D = 1.2995;
	data.PLP_C = 'P';

	//Displaying values of the data members of struct MyData

	printf("\n\n");
	printf("Data members of struct MyData are : \n\n");
	printf("PLP_I = %d\n", data.PLP_I);
	printf("PLP_F = %f\n", data.PLP_F);
	printf("PLP_D = %lf\n", data.PLP_D);
	printf("PLP_C = %c\n", data.PLP_C);

	printf("\n\n");
	printf("ADDRESS OF DATA MEMBERS OF 'struct mydata' ARE : \n\n");
	printf("'PLP_I' Occupies address from %p\n", &data.PLP_I);
	printf("'PLP_F' Occupies address from %p\n", &data.PLP_F);
	printf("'PLP_D' Occupies address from %p\n", &data.PLP_D);
	printf("'PLP_C' Occupies address from %p\n", &data.PLP_C);

	printf("Starting address of 'struct MyData' variable 'data' = %p\n\n", &data);

	return(0);
}