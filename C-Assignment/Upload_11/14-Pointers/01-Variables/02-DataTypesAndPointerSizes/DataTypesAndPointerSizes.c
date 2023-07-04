#include<stdio.h>

struct Employee
{
	char PLP_NAME[100];
	int PLP_AGE;
	float PLP_SALARY;
	char PLP_SEX;
	char PLP_MARITAL_STATUS;
};

int main(void)
{
	//code

	printf("\n\n");
	printf("Sizes of data types and pointers to thos respective data types are : \n\n");
	printf("Size of (int)             = %zd \t\t\t Size of pointer to int (int *) = %zu\n\n", sizeof(int), sizeof(int *));
	printf("Size of (float)           = %zd \t\t\t Size of pointer to float (float *) = %zu\n\n", sizeof(float), sizeof(float *));
	printf("Size of (char)            = %zd \t\t\t Size of pointer to char (char *) = %zu\n\n", sizeof(char), sizeof(char *));
	printf("Size of (double)          = %zd \t\t\t Size of pointer to float (double *) = %zu\n\n", sizeof(double), sizeof(double *));
	printf("Size of (struct Employee) = %zd \t\t Size of pointer to struct Employee (struct Employee *) = %zu\n\n", sizeof(struct Employee), sizeof(struct Employee *));
	return(0);



}