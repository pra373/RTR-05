#include<stdio.h>
#include<string.h>

#define INT_ARRAY_SIZE 10
#define FLOAT_ARRAY_SIZE 5
#define CHAR_ARRAY_SIZE 26

#define NUM_STRINGS 10
#define MAX_CHARACTERS_PER_STRING 20

#define ALPHABET_BEGINING 65 // 'A'

struct MyDataOne
{
	int PLP_IARRAY[INT_ARRAY_SIZE];
	float PLP_FARRAY[FLOAT_ARRAY_SIZE];
};

struct MyDataTwo
{
	char PLP_CARRAY[CHAR_ARRAY_SIZE];
	char PLP_STRARRAY[NUM_STRINGS][MAX_CHARACTERS_PER_STRING];
};

int main(void)
{
	//variable declaration
	struct MyDataOne data_01;
	struct MyDataTwo data_02;
	int i;

	//code

	// ***** PIECE-MEAL ASSIGNMENT (HARD COADED) *********

	data_01.PLP_FARRAY[0] = 0.1f;
	data_01.PLP_FARRAY[1] = 1.2f;
	data_01.PLP_FARRAY[2] = 2.3f;
	data_01.PLP_FARRAY[3] = 3.4f;
	data_01.PLP_FARRAY[4] = 4.5f;

	// Loop assignment user input

	printf("\n\n");
	printf("Enter %d integers : \n\n", INT_ARRAY_SIZE);
	for (i = 0; i < INT_ARRAY_SIZE; i++)
	{
		scanf("%d", &data_01.PLP_IARRAY[i]);
	}

	// loop assignment hard coaded
	
	for (i = 0; i < CHAR_ARRAY_SIZE; i++)
	{
		data_02.PLP_CARRAY[i] = (char)(i + ALPHABET_BEGINING);

	}

	// PIECE-MEAL ASSIGNMENT HARD COADED

	strcpy(data_02.PLP_STRARRAY[0], "Welcome !!! ");
	strcpy(data_02.PLP_STRARRAY[1], "This ");
	strcpy(data_02.PLP_STRARRAY[2], "Is ");
	strcpy(data_02.PLP_STRARRAY[3], "ASTROMEDICOMP'S ");
	strcpy(data_02.PLP_STRARRAY[4], "Real ");
	strcpy(data_02.PLP_STRARRAY[5], "Time ");
	strcpy(data_02.PLP_STRARRAY[6], "Rendering ");
	strcpy(data_02.PLP_STRARRAY[7], "Batch ");
	strcpy(data_02.PLP_STRARRAY[8], "of ");
	strcpy(data_02.PLP_STRARRAY[9], "2023-2024 !!! ");

	// displaying data members of data_01

	printf("\n\n");
	printf("Members of 'struct data_01' alongwith their assigned values are : \n\n");
	printf("Integer array (data_01.PLP_IARRAY) : \n\n");
	for (i = 0; i < INT_ARRAY_SIZE; i++)
	{
		printf("data_01.PLP_IARRAY[%d] = %d\n", i, data_01.PLP_IARRAY[i]);

	}

	printf("\n\n");
	printf("Floating point array (data_01.PLP_FARRAY) : \n\n");
	for (i = 0; i < FLOAT_ARRAY_SIZE; i++)
	{
		printf("data_01.PLP_FARRAY[%d] = %f\n", i, data_01.PLP_FARRAY[i]);


		
	}

	// displaying data members of data_02

	printf("\n\n");
	printf("Members of 'struct data_02' alongwith their assigned values are : \n\n");
	printf("\n\n");

	printf("Character array (data_02.PLP_CARRAY) : \n\n");
	for (i = 0; i < CHAR_ARRAY_SIZE; i++)
	{
		printf("data_02.PLP_CARRAY[%d] = %c\n", i, data_02.PLP_CARRAY[i]);
	}
	printf("\n\n");
	printf("String array (data_02.PLP_STRARRAY) : \n\n");
	for (i = 0; i < NUM_STRINGS; i++)
	{
		printf("%s", data_02.PLP_STRARRAY[i]);
	}

	printf("\n\n");

	return(0);


}
