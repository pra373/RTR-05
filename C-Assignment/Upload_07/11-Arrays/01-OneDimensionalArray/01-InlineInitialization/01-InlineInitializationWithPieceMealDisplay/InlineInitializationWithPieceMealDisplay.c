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

	//code

	// **********Integer Array ******************

	printf("\n\n");
	printf("In-line initialization and piece-meal display of elements of integer array 'PLP_ARRAY[]' : \n\n");
	printf("PLP_ARRAY[0] (First Element)   = %d\n", PLP_ARRAY[0]);
	printf("PLP_ARRAY[1] (Second Element)  = %d\n", PLP_ARRAY[1]);
	printf("PLP_ARRAY[2] (Third Element)   = %d\n", PLP_ARRAY[2]);
	printf("PLP_ARRAY[3] (Fourth Element)  = %d\n", PLP_ARRAY[3]);
	printf("PLP_ARRAY[4] (Fifth Element)   = %d\n", PLP_ARRAY[4]);
	printf("PLP_ARRAY[5] (Sixth Element)   = %d\n", PLP_ARRAY[5]);
	printf("PLP_ARRAY[6] (seventh Element) = %d\n", PLP_ARRAY[6]);
	printf("PLP_ARRAY[7] (Eighth Element)  = %d\n", PLP_ARRAY[7]);
	printf("PLP_ARRAY[8] (Ninth Element)   = %d\n", PLP_ARRAY[8]);
	printf("PLP_ARRAY[9] (Tenth Element)   = %d\n\n", PLP_ARRAY[9]);

	int_size = sizeof(int);
	PLP_ARRAY_SIZE = sizeof(PLP_ARRAY);

	PLP_ARRAY_NUM_ELEMENTS = PLP_ARRAY_SIZE / int_size;

	printf("Size of Data type 'int'                              = %d bytes\n",int_size);
	printf("Number of Elements in Integer array 'PLP_ARRAY[]'    = %d Elements\n ", PLP_ARRAY_NUM_ELEMENTS);
	printf("Size of array 'PLP_ARRAY[]' (%d Elements * %d Bytes) = %d Bytes \n\n", PLP_ARRAY_NUM_ELEMENTS, int_size, PLP_ARRAY_SIZE);

	// ************* Float Array **************

	printf("\n\n");
	printf("In-line initialization and piece-meal display of elements of Float array 'PLP_FARRAY[]' : \n\n");
	printf("PLP_FARRAY[0] (First Element)   = %f\n", PLP_FARRAY[0]);
	printf("PLP_FARRAY[1] (Second Element)  = %f\n", PLP_FARRAY[1]);
	printf("PLP_FARRAY[2] (Third Element)   = %f\n", PLP_FARRAY[2]);
	printf("PLP_FARRAY[3] (Fourth Element)  = %f\n", PLP_FARRAY[3]);
	printf("PLP_FARRAY[4] (Fifth Element)   = %f\n", PLP_FARRAY[4]);
	printf("PLP_FARRAY[5] (Sixth Element)   = %f\n", PLP_FARRAY[5]);
	printf("PLP_FARRAY[6] (seventh Element) = %f\n", PLP_FARRAY[6]);
	printf("PLP_FARRAY[7] (Eighth Element)  = %f\n", PLP_FARRAY[7]);
	printf("PLP_FARRAY[8] (Ninth Element)   = %f\n", PLP_FARRAY[8]);
	printf("PLP_FARRAY[9] (Tenth Element)   = %f\n\n", PLP_FARRAY[9]);

	float_size = sizeof(float);
	PLP_FARRAY_SIZE = sizeof(PLP_FARRAY);

	PLP_FARRAY_NUM_ELEMENTS = PLP_FARRAY_SIZE / float_size;

	printf("Size of Data type 'float'                                    = %d bytes\n", float_size);
	printf("Number of Elements in Floating point array 'PLP_FARRAY[]'    = %d Elements\n ", PLP_FARRAY_NUM_ELEMENTS);
	printf("Size of array 'PLP_FARRAY[]' (%d Elements * %d Bytes)         = %d Bytes \n\n", PLP_FARRAY_NUM_ELEMENTS, float_size, PLP_FARRAY_SIZE);

// ********** character array ***************

	printf("\n\n");
	printf("In-line initialization and piece-meal display of elements of character array 'PLP_CARRAY[]' : \n\n");
	printf("PLP_CARRAY[0] (First Element)         = %c\n", PLP_CARRAY[0]);
	printf("PLP_CARRAY[1] (Second Element)        = %c\n", PLP_CARRAY[1]);
	printf("PLP_CARRAY[2] (Third Element)         = %c\n", PLP_CARRAY[2]);
	printf("PLP_CARRAY[3] (Fourth Element)        = %c\n", PLP_CARRAY[3]);
	printf("PLP_CARRAY[4] (Fifth Element)         = %c\n", PLP_CARRAY[4]);
	printf("PLP_CARRAY[5] (Sixth Element)         = %c\n", PLP_CARRAY[5]);
	printf("PLP_CARRAY[6] (seventh Element)       = %c\n", PLP_CARRAY[6]);
	printf("PLP_CARRAY[7] (Eighth Element)        = %c\n", PLP_CARRAY[7]);
	printf("PLP_CARRAY[8] (Ninth Element)         = %c\n", PLP_CARRAY[8]);
	printf("PLP_CARRAY[9] (Tenth Element)         = %c\n", PLP_CARRAY[9]);
	printf("PLP_CARRAY[10] (eleventh Element)     = %c\n", PLP_CARRAY[10]);
	printf("PLP_CARRAY[11] (Twelth Element)       = %c\n", PLP_CARRAY[11]);
	printf("PLP_CARRAY[12] (thirteenth Element)   = %c\n\n", PLP_CARRAY[12]);


	char_size = sizeof(char);
	PLP_CARRAY_SIZE = sizeof(PLP_CARRAY);

	PLP_CARRAY_NUM_ELEMENTS = PLP_CARRAY_SIZE / char_size;

	printf("Size of Data type 'character'                          =  %d bytes\n", char_size);
	printf("Number of Elements in character array 'PLP_CARRAY[]'   = %d Elements\n ", PLP_CARRAY_NUM_ELEMENTS);
	printf("Size of array 'PLP_CARRAY[]' (%d Elements * %d Bytes)  = %d Bytes \n\n", PLP_CARRAY_NUM_ELEMENTS, char_size, PLP_CARRAY_SIZE);




	







;

}