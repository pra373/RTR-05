#include<stdio.h>

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
	// structure variable declarations

	// 35 will be assigned to 'PLP_I' of 'data_01'
	// 3.9 will be assigned to 'PLP_F' of 'data_01'
	// 1.23765 will be assigned to 'PLP_D' of 'data_01'
	// A will be assigned to 'PLP_C' of 'data_01'

	struct MyData data_01 = { 35,3.9f,1.23765,'A' }; // Inline Initialization

	/* 
	
	'P' will be assigned to 'PLP_I' of data_02 ... but 'P' is a character and 'PLP_I' is an Integer
	 so 'P' is converted into decimal integer ASCII value(80) and 80 is assigned to 'PLP_I' of data_02

	 6.2 will be assigned to 'PLP_F' of 'data_02'

	 12.199523 will be assigned to 'PLP_D' of 'data_02'


	 68  will be assigned to 'PLP_C' of 'data_02' as PLP_C is a character variable corresponding character will be stored in PLP_C
	 

	 */
	struct MyData data_02 = { 'P',6.2f,12.199523,68 };  // Inline Initialization

	/*
		36 will be assigned to 'PLP_I' of 'data_03'

		'G' will be assigned to float variable 'PLP_F' but as 'PLP_F' is a float variable 'G' is converted into corresponding ASCII decimal
		value (71) and then 71 is converted into float(71.00000) 
		
		No character will be assigned to 'PLP_C' of 'data_03'

		0.000000 will be assigned to 'PLP_D' of 'data_03'





	*/
	struct MyData data_03 = { 36,'G' };  // Inline Initialization

	/*
		79 will be assigned to 'PLP_I' of 'data_04'

		No character will be assigned to 'PLP_C' of 'data_04'

		0.000000 will be assigned to 'PLP_D' of 'data_04'

		0.00000 will be assigned to 'PLP_F' of 'data_04'




	*/

	struct MyData data_04 = { 79 }; // Inline Initialization

	//Displaying values of the data members of struct MyData

	printf("\n\n");
	printf("Data members of struct MyData data_01 are : \n\n");
	printf("PLP_I = %d\n", data_01.PLP_I);
	printf("PLP_F = %f\n", data_01.PLP_F);
	printf("PLP_D = %lf\n", data_01.PLP_D);
	printf("PLP_C = %c\n", data_01.PLP_C);

	printf("\n\n");
	printf("Data members of struct MyData data_02 are : \n\n");
	printf("PLP_I = %d\n", data_02.PLP_I);
	printf("PLP_F = %f\n", data_02.PLP_F);
	printf("PLP_D = %lf\n", data_02.PLP_D);
	printf("PLP_C = %c\n", data_02.PLP_C);

	printf("\n\n");
	printf("Data members of struct MyData data_03 are : \n\n");
	printf("PLP_I = %d\n", data_03.PLP_I);
	printf("PLP_F = %f\n", data_03.PLP_F);
	printf("PLP_D = %lf\n", data_03.PLP_D);
	printf("PLP_C = %c\n", data_03.PLP_C);

	printf("\n\n");
	printf("Data members of struct MyData data_04 are : \n\n");
	printf("PLP_I = %d\n", data_04.PLP_I);
	printf("PLP_F = %f\n", data_04.PLP_F);
	printf("PLP_D = %lf\n", data_04.PLP_D);
	printf("PLP_C = %c\n", data_04.PLP_C);

	return(0);




}