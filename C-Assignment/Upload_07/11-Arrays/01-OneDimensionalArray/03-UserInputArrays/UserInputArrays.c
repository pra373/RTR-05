#include<stdio.h>
#include<conio.h>

// MACRO CONSTANT USED AS ARRAY SIZE IN SUBSCRIPT AND AS ARRAY LENGTH.
//HENCE , THIS PROGRAM'S ARRAYS SIZE CAN BE SIMPLY CHANGED BY CHANGING THESE FOLLOWING 3 
//GlOBAL MACRO CONSTANT VALUES, BEFORE COMPILING, LINKING AND EXECUTING THE PROGRAM !!!

#define INT_ARRAY_NUM_ELEMENTS 5
#define FLOAT_ARRAY_NUM_ELEMENTS 3
#define CHAR_ARRAY_NUM_ELEMENTS 15

int main(int argc, char* argv[], char* envp[])
{
	//Variable declaration
	int PLP_IARRAY[INT_ARRAY_NUM_ELEMENTS];
	float PLP_FARRAY[FLOAT_ARRAY_NUM_ELEMENTS];
	char PLP_CARRAY[CHAR_ARRAY_NUM_ELEMENTS];

	int PLP_I, PLP_NUM;

	//code

	// *************** ARRAY ELEMENTS INPUT **********

	printf("\n\n");
	printf("Enter elements for integer array : \n");
	for (PLP_I = 0; PLP_I < INT_ARRAY_NUM_ELEMENTS; PLP_I++)
	{
		scanf("%d", &PLP_IARRAY[PLP_I]);
	}

	printf("\n\n");

	printf("Enter elements for floating point array : \n");
	for (PLP_I = 0; PLP_I < FLOAT_ARRAY_NUM_ELEMENTS; PLP_I++)
	{
		scanf("%f", &PLP_FARRAY[PLP_I]);
	}

	printf("\n\n");

	printf("Enter elements for Character array : \n");
	for (PLP_I = 0; PLP_I < CHAR_ARRAY_NUM_ELEMENTS; PLP_I++)
	{
		PLP_CARRAY[PLP_I] = getch();
		printf("%c\n",PLP_CARRAY[PLP_I]);
	}

	//******************Array Elements Output ***********
	
	printf("\n\n");
	printf("Integer array entered by you : \n\n");
	for (PLP_I = 0; PLP_I < INT_ARRAY_NUM_ELEMENTS; PLP_I++)
	{
		printf("%d\n", PLP_IARRAY[PLP_I]);
	}

	printf("\n\n");
	printf("Floating point array entered by you : \n\n");
	for (PLP_I = 0; PLP_I < FLOAT_ARRAY_NUM_ELEMENTS; PLP_I++)
	{
		printf("%f\n", PLP_FARRAY[PLP_I]);
	}

	printf("\n\n");
	printf("Character array entered by you : \n\n");

	for (PLP_I = 0; PLP_I < CHAR_ARRAY_NUM_ELEMENTS; PLP_I++)
	{
		printf("%c\n", PLP_CARRAY[PLP_I]);
	}

	return(0);









}
