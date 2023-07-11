#include<stdio.h>

struct Employee
{
	char PLP_NAME[100];
	int PLP_AGE;
	float PLP_SALARY;
	char PLP_SEX;
	char marital_status;
};

int main(void)
{
	//code
	printf("\n\n");
	printf("sizes of datatypes and pointers to those respective data types are : \n\n");
	printf("size of (int)                               : %zd \n",sizeof(int));
	printf("size of pointer to int (int*)               : %zd \n",sizeof(int *));
	printf("size of pointer to pointer to int (int**)   : %zd \n", sizeof(int **));

	printf("\n\n");
	printf("size of (float)                               : %zd \n", sizeof(float));
	printf("size of pointer to int (float*)               : %zd \n", sizeof(float *));
	printf("size of pointer to pointer to int (float**)   : %zd \n", sizeof(float **));

	printf("\n\n");
	printf("size of (double)                               : %zd \n", sizeof(double));
	printf("size of pointer to int (double*)               : %zd \n", sizeof(double *));
	printf("size of pointer to pointer to int (double**)   : %zd \n", sizeof(double **));

	printf("\n\n");
	printf("size of (char)                               : %zd \n", sizeof(char));
	printf("size of pointer to int (char*)               : %zd \n", sizeof(char *));
	printf("size of pointer to pointer to int (char**)   : %zd \n", sizeof(char **));

	printf("\n\n");
	printf("size of (struct Employee)                                : %zd \n", sizeof(struct Employee));
	printf("size of pointer to int (struct Employee *)               : %zd \n", sizeof(struct Employee *));
	printf("size of pointer to pointer to int (struct Employee **)   : %zd \n", sizeof(struct Employee **));

	return(0);


	

}
