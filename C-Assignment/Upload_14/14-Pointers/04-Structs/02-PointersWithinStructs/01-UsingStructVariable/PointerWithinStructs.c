#include<stdio.h>

//defining struct

struct MyData
{
	int *PLP_PTR_INT;
	int i;

	float *PLP_PTR_FLOAT;
	float f;

	double *PLP_PTR_DOUBLE;
	double d;
};

int main(void)
{
	//variable declaration

	struct MyData data;

	//code

	data.i = 9;
	data.PLP_PTR_INT = &data.i;

	data.f = 99.99f;
	data.PLP_PTR_FLOAT = &data.f;

	data.d = 157.7629854;
	data.PLP_PTR_DOUBLE = &data.d;

	printf("\n\n");
	printf("i = %d\n", data.i);
	printf("Address of 'i' = %p\n", data.PLP_PTR_INT);

	printf("\n\n");
	printf("f = %f\n", data.f);
	printf("Address of 'f' = %p\n", data.PLP_PTR_FLOAT);

	printf("\n\n");
	printf("d = %lf\n", data.d);
	printf("Address of 'd' = %p\n", data.PLP_PTR_DOUBLE);

	return(0);






}