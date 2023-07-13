#include<stdio.h>
#include<stdlib.h>

//defining struct

struct MyData
{
	int PLP_I;
	float PLP_F;
	double PLP_D;
};

int main(void)
{
	//function Prototype
	void ChangeValues(struct MyData *);

	//variable declaration
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

	// Assigning data values to the data members of struct MyData

	struct_pointer->PLP_I = 30;
	struct_pointer->PLP_F = 11.45f;
	struct_pointer->PLP_D = 125.67345;

	//Displaying values of the data members of 'struct Mydata'

	printf("\n\n");
	printf("Data members of struct MyData are : \n\n");
	printf("PLP_I = %d\n", struct_pointer->PLP_I);
	printf("PLP_F = %f\n", struct_pointer->PLP_F);
	printf("PLP_D = %lf\n", struct_pointer->PLP_D);

	ChangeValues(struct_pointer);

	//Displaying values of the data members of 'struct Mydata'

	printf("\n\n");
	printf("Data members of struct MyData are : \n\n");
	printf("PLP_I = %d\n", struct_pointer->PLP_I);
	printf("PLP_F = %f\n", struct_pointer->PLP_F);
	printf("PLP_D = %lf\n", struct_pointer->PLP_D);
	
	if (struct_pointer)
	{
		free(struct_pointer);
		struct_pointer = NULL;
		printf("Memory allocated to 'struct MyData' has been successfully freed !!!\n\n");
	}

	return(0);





}

void ChangeValues(struct MyData *pParam_Data)
{
	//code

	pParam_Data->PLP_I = 9;
	pParam_Data->PLP_F = 8.2f;
	pParam_Data->PLP_D = 6.1998;





}