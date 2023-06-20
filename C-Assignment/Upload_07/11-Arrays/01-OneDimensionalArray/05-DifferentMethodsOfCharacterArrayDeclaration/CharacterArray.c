#include<stdio.h>

int main(void)
{
	//variable declarations
	char PLP_CARRAY_01[] = { 'A','S','T','R','O','M','E','D','I','C','O','M','P','\0' };
	char PLP_CARRAY_02[9] = { 'W','E','L','C','O','M','E','S','\0' };
	char PLP_CARRAY_03[] = { 'Y','O','U','\0' };
	char PLP_CARRAY_04[] = "To";
	char PLP_CARRAY_05[] = "Real Time Rendering Batch of 2023-2024";

	char PLP_CARRAY_WithoutNullTerminator[] = { 'H','e','l','l','o' };

	//code

	printf("\n\n");
	printf("Size of PLP_CARRAY_01 : %zi \n\n", sizeof(PLP_CARRAY_01));
	printf("Size of PLP_CARRAY_02 : %zi \n\n", sizeof(PLP_CARRAY_02));
	printf("Size of PLP_CARRAY_03 : %zi \n\n", sizeof(PLP_CARRAY_03));
	printf("Size of PLP_CARRAY_04 : %zi \n\n", sizeof(PLP_CARRAY_04));
	printf("Size of PLP_CARRAY_05 : %zi \n\n", sizeof(PLP_CARRAY_05));
	printf("Size of PLP_CARRAY_06 : %zi \n\n", sizeof(PLP_CARRAY_WithoutNullTerminator));

	printf("\n\n");

	printf("The Strings are : \n\n");
	printf("PLP_CARRAY_01 : %s\n\n", PLP_CARRAY_01);
	printf("PLP_CARRAY_02 : %s\n\n", PLP_CARRAY_02);
	printf("PLP_CARRAY_03 : %s\n\n", PLP_CARRAY_03);
	printf("PLP_CARRAY_04 : %s\n\n", PLP_CARRAY_04);
	printf("PLP_CARRAY_05 : %s\n\n", PLP_CARRAY_05);
	
	printf("\n\n");
	printf("Size of PLP_CARRAY_WithoutNullTerminator : %zi\n\n", sizeof(PLP_CARRAY_WithoutNullTerminator));
	printf("PLP_CARRAY_WithoutNullTerminator : %s\n\n", PLP_CARRAY_WithoutNullTerminator);

	return(0);
}