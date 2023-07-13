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
	//variable declaration

	int i_size;
	int f_size;
	int d_size;
	int struct_MyData_size;
	int pointer_to_struct_MyData_size;

	typedef struct MyData *MyDataPtr;

	MyDataPtr PLP_DATA;

	//code

	PLP_DATA = (struct MyData *)malloc(sizeof(struct MyData));

	if (PLP_DATA == NULL)
	{
		printf("Failed to allocate memory to struct MyData !!! Exiting NOW ... \n\n");
		exit(0);
	}

	else
	{
		printf("Successfully Allocated memory to 'Struct Mydata' !!!!\n\n");
	}

	// Assigning data values to the data members of struct MyData

	PLP_DATA->PLP_I = 30;
	PLP_DATA->PLP_F = 11.45f;
	PLP_DATA->PLP_D = 125.67345;

	//Displaying values of the data members of 'struct Mydata'

	printf("\n\n");
	printf("Data members of struct MyData are : \n\n");
	printf("PLP_I = %d\n", PLP_DATA->PLP_I);
	printf("PLP_F = %f\n", PLP_DATA->PLP_F);
	printf("PLP_D = %lf\n", PLP_DATA->PLP_D);

	//calculating sizes (in Bytes) of the data members of 'struct MyData'

	i_size = sizeof(PLP_DATA->PLP_I);
	f_size = sizeof(PLP_DATA->PLP_F);
	d_size = sizeof(PLP_DATA->PLP_D);

	// Displaying Sizes in bytes of data members of 'struct MyData'
	printf("\n\n");
	printf("Sizes (in Bytes) of data members of 'Struct Mydata' are : \n\n");
	printf("size of 'PLP_I' = %d Bytes\n", i_size);
	printf("size of 'PLP_F' = %d Bytes\n", f_size);
	printf("size of 'PLP_D' = %d Bytes\n", d_size);

	//Calculating size of the entire structure MyData

	struct_MyData_size = sizeof(struct MyData);
	pointer_to_struct_MyData_size = sizeof(PLP_DATA);

	//Displaying size of the entire structure MyData

	printf("\n\n");
	printf("Size of struct Mydata : %d bytes\n\n", struct_MyData_size);
	printf("size of pointer to struct MyData : %d bytes\n\n", pointer_to_struct_MyData_size);


	if (PLP_DATA)
	{
		free(PLP_DATA);
		PLP_DATA = NULL;
		printf("Memory allocated to struct MyData has been freed successfully !!!\n\n");

	}

	return(0);

}

