#include<stdio.h>

int main(void)
{
	

	//variable declarations

	int PLP_ARRAY[] = { 9,30,6,12,98,95,20,23,2,45 };
	int int_size;
	int PLP_ARRAY_SIZE;
	int PLP_ARRAY_NUM_ELEMENTS;

	float PLP_FARRAY[] = { 1.2f,2.3f,3.4f,4.5f,5.6f,6.7f,7.8f,8.9f };
	int float_size;
	int PLP_FARRAY_SIZE;
	int PLP_FARRAY_NUM_ELEMENTS;

	char PLP_CARRAY[] = { 'A','S','T','R','O','M','E','D','I','C','O','M','P' };
	int char_size;
	int PLP_CARRAY_SIZE;
	int PLP_CARRAY_NUM_ELEMENTS;

	int i;

	//code

	// ********** Integer array **********
	printf("\n\n");
	printf("In-line Initialization and loop (for) Display of elements of array 'PLP_ARRAY[]' : \n\n ");

	int_size = sizeof(int);
	PLP_ARRAY_SIZE = sizeof(PLP_ARRAY);
	PLP_ARRAY_NUM_ELEMENTS = PLP_ARRAY_SIZE / int_size;

	for (i = 0; i < PLP_ARRAY_NUM_ELEMENTS; i++)
	{
		printf("PLP_ARRAY[%d] (Element %d) = %d\n", i, (i + 1), PLP_ARRAY[i]);
	}

	printf("\n\n");
	printf("Size of Data type 'int'                                    = %d bytes\n", int_size);
	printf("Number of Elements in Integer array 'PLP_ARRAY[]'          = %d Elements\n ", PLP_ARRAY_NUM_ELEMENTS);
	printf("Size of array 'PLP_ARRAY[]' (%d Elements * %d Bytes)       = %d Bytes \n\n", PLP_ARRAY_NUM_ELEMENTS, int_size, PLP_ARRAY_SIZE);

	// *********Float array[] ********

	printf("\n\n");
	printf("In-line Initialization and loop (while) Display of elements of array 'PLP_FARRAY[]' : \n\n ");
	float_size = sizeof(float);
	PLP_FARRAY_SIZE = sizeof(PLP_FARRAY);

	PLP_FARRAY_NUM_ELEMENTS = PLP_FARRAY_SIZE / float_size;
	int num = 0;

	while (num < PLP_FARRAY_NUM_ELEMENTS)
	{
		printf("PLP_FARRAY[%d] (Element %d) = %f\n", num, (num + 1), PLP_FARRAY[num]);
		num++;
	}
	printf("\n\n");
	printf("Size of Data type 'float'                                      = %d bytes\n", float_size);
	printf("Number of Elements in Floating point array 'PLP_FARRAY[]'      = %d Elements\n ", PLP_FARRAY_NUM_ELEMENTS);
	printf("Size of array 'PLP_FARRAY[]' (%d Elements * %d Bytes)          = %d Bytes \n\n", PLP_FARRAY_NUM_ELEMENTS, float_size, PLP_FARRAY_SIZE);


// ***************Character array[] *************

	printf("\n\n");
	printf("In-line Initialization and loop (do-while) Display of elements of array 'PLP_CARRAY[]' : \n\n ");

	char_size = sizeof(char);
	PLP_CARRAY_SIZE = sizeof(PLP_CARRAY);

	PLP_CARRAY_NUM_ELEMENTS = PLP_CARRAY_SIZE / char_size;
	int j = 0;
	do
	{
		printf("PLP_CARRAY[%d] (Element %d) = %c\n", j, (j + 1), PLP_CARRAY[j]);
		j++;
	} while (j < PLP_CARRAY_NUM_ELEMENTS);

	printf("\n\n");
	printf("Size of Data type 'character'                              = %d bytes\n", char_size);
	printf("Number of Elements in character array 'PLP_CARRAY[]'       = %d Elements\n ", PLP_CARRAY_NUM_ELEMENTS);
	printf("Size of array 'PLP_CARRAY[]' (%d Elements * %d Bytes)      = %d Bytes \n\n", PLP_CARRAY_NUM_ELEMENTS, char_size, PLP_CARRAY_SIZE);

	return(0);

}