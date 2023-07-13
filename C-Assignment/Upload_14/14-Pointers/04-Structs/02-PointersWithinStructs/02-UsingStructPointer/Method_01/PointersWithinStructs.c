#include<stdio.h>
#include<stdlib.h>

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
	// variable declarations

	struct MyData *struct_pointer = NULL;

	//code
	printf("\n\n");

	struct_pointer = (struct MyData *)malloc(sizeof(struct MyData));

	if (struct_pointer == NULL)
	{
		printf("Failed to allocate memory to struct MyData !!! Exiting NOW ... \n\n");
		exit(0);
	}

	else
	{
		printf("Successfully Allocated memory to 'Struct Mydata' !!!!\n\n");
	}

	(*struct_pointer).i = 9;
	(*struct_pointer).PLP_PTR_INT = &(*struct_pointer).i;

	(*struct_pointer).f = 3.1425;
	(*struct_pointer).PLP_PTR_FLOAT = &(*struct_pointer).f;

	(*struct_pointer).d = 32.112233;
	(*struct_pointer).PLP_PTR_DOUBLE = &(*struct_pointer).d;

	printf("\n\n");
	printf("i = %d\n", (*struct_pointer).i);
	printf("Address of 'i' = %p\n", &(*struct_pointer).i);

	printf("\n\n");
	printf("f = %f\n", (*struct_pointer).f);
	printf("Address of 'f' = %p\n", &(*struct_pointer).f);

	printf("\n\n");
	printf("d = %lf\n", (*struct_pointer).f);
	printf("Address of 'd' = %p\n", &(*struct_pointer).f);

	if (struct_pointer)
	{
		free(struct_pointer);
		struct_pointer = NULL;
		printf("Memory allocated to struct MyData has been freed successfully !!!\n\n");

	}

	return(0);










	


}