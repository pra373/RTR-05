#include<stdio.h>
#include<conio.h>


//defining struct

struct MyData
{
	int PLP_I;
	float PLP_F;
	double PLP_D;
	char PLP_C;
};

int main(void)
{
	//variable declaration

	struct MyData data; // declaration of single struct variable

	//code
	// User input for data members

	printf("\n\n");
	printf("Enter integer value for data member 'PLP_I' of struct MyData : \n ");
	scanf("%d", &data.PLP_I);

	printf("Enter Float value for data member 'PLP_F' of struct MyData : \n ");
	scanf("%f", &data.PLP_F);

	printf("Enter double value for data member 'PLP_D' of struct MyData : \n ");
	scanf("%lf", &data.PLP_D);

	printf("Enter character value for data member 'PLP_C' of struct MyData : \n ");
	data.PLP_C = getch();

	//Displaying values of the data members of struct MyData

	printf("\n\n");
	printf("Data members of struct MyData are : \n\n");
	printf("PLP_I = %d\n", data.PLP_I);
	printf("PLP_F = %f\n", data.PLP_F);
	printf("PLP_D = %lf\n", data.PLP_D);
	printf("PLP_C = %c\n", data.PLP_C);

	return(0);







}