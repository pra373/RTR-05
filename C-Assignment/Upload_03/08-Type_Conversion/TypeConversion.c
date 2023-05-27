#include<stdio.h>

int main(void)
{
	//variable declaration
	int PLP_I, PLP_J, PLP_A, PLP_RESULT, PLP_EXPLICIT;
	char ch_01, ch_02;
	float f, result_float, f_explicit;

	//code

	printf("\n\n");

	//Interconversion and Implicit Type-casting Between 'char' AND 'int' data types

	PLP_I = 70;
	ch_01 = PLP_I;
	printf("I = %d\n", PLP_I);
	printf("Character 1 (after ch_01 = i) = %c\n\n", ch_01);

	ch_02 = 'Q';
	PLP_J = ch_02;
	printf("Character 2 = %c\n", ch_02);
	printf("J ( after j = ch_02 ) = %d\n\n", PLP_J);

	//Implicit conversion INT to Float

	PLP_A = 5;
	f = 7.8f;
	result_float = PLP_A + f;
	printf("Integer a = %d and Floating point number %f added gives floating point sum = %f\n\n", PLP_A, f, result_float);

	PLP_RESULT = PLP_A + f;
	printf("Integer a = %d and Floating point number %f added gives Integer sum = %d\n\n", PLP_A, f, PLP_RESULT);

	//Explicit type casting using cast operator

	f_explicit = 30.121995f;
	PLP_EXPLICIT = (int)f_explicit;
	printf("Floating point number which will be typecasted explicitly = %f\n", f_explicit);
	printf("result integer after explicit type casting of %f = %d\n\n\n", f_explicit,PLP_EXPLICIT);

	return(0);





	

}