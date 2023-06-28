#include<stdio.h>

int main(void)
{
	struct MyData
	{
		int PLP_I;
		float PLP_F;
		double PLP_D;
		
	}data;  // declaring a single struct variable of type struct Mydata locally

	//variable declarations
	int i_size;
	int f_size;
	int d_size;
	int struct_MyData_size;

	//code

	//assigning data values to the data members of struct 'MyData'
	
	data.PLP_I = 30;
	data.PLP_F = 11.45;
	data.PLP_D = 1.2995;
	
	//Displaying values of the data members of struct MyData
	
	printf("\n\n");
	printf("Data members of struct MyData are : \n\n");
	printf("PLP_I = %d\n", data.PLP_I);
	printf("PLP_F = %f\n", data.PLP_F);
	printf("PLP_D = %lf\n", data.PLP_D);
	
	//calculating sizes (In bytes) of data members of struct 'MyData'
	
	i_size = sizeof(data.PLP_I);
	f_size = sizeof(data.PLP_F);
	d_size = sizeof(data.PLP_D);
	

	//displaying sizes of data members

	printf("\n\n");
	printf("Sizes (in bytes) of data members of struct MyData are :\n\n");
	printf("size of PLP_I = %d\n", i_size);
	printf("size of PLP_F = %d\n", f_size);
	printf("size of PLP_D = %d\n", d_size);

	//calculating size of entire structure

	struct_MyData_size = sizeof(struct MyData);

	//Display size in byte for entire structure
	
	printf("\n\n");
	printf("Size of struct MyData : %d bytes\n\n", struct_MyData_size);
	return(0);
	
	
}
