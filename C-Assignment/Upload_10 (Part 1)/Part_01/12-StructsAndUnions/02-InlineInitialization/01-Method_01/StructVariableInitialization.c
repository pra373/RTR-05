#include<stdio.h>

//defining struct

struct MyData
{
	int PLP_I;
	float PLP_F;
	double PLP_D;
	char PLP_C;
}data = { 30,4.5f,11.451995,'A' };  //Inline initialization of struct variable dada of type struct MyData

int main(void)
{
	//Displaying values of the data members of struct MyData

	printf("\n\n");
	printf("Data members of struct MyData are : \n\n");
	printf("PLP_I = %d\n", data.PLP_I);
	printf("PLP_F = %f\n", data.PLP_F);
	printf("PLP_D = %lf\n", data.PLP_D);
	printf("PLP_C = %c\n", data.PLP_C);

	return(0);
}