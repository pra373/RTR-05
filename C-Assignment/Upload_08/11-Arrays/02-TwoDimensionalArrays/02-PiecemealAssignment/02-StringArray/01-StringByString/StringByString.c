#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	// Function prototype
	void MyStrCpy(char[], char[]);

	// A string is an array of characters ... so char[] is a char array and hence, char[] is a string.
	// array of char[] is array of strings !!!!
	//hence, char[][] is an array of char arrays hence, ia an array af strings

	/*here the string array can allow a maximum number of 10 strings(10 rows)
	and each of this 10 strings can only have 15 characters maximum (15 columns)*/

	//variable declaration

	char PLP_STR_ARRAY[5][10];
	int char_size;
	int str_array_size;
	int str_array_num_elements;
	int str_array_num_rows;
	int str_array_num_columns;
	int i;

	//code

	printf("\n\n");

	char_size = sizeof(char);

	str_array_size = sizeof(PLP_STR_ARRAY);

	printf("Size of two dimentional (2D) Character array (string array) is = %d \n\n", str_array_size);

	str_array_num_rows = str_array_size / sizeof(PLP_STR_ARRAY[0]);

	printf("Number of rows in two dimentional (2D) Character array (string array) is = %d \n\n", str_array_num_rows);

	str_array_num_columns = sizeof(PLP_STR_ARRAY[0]) / char_size;

	printf("Number of columns in two dimentional (2D) Character array (string array) is = %d \n\n", str_array_num_columns);

	str_array_num_elements = str_array_num_rows * str_array_num_columns;

	printf("Number of elements in two dimentional (2D) Character array (string array) is = %d \n\n", str_array_num_elements);

	// **** PIECE-MEAL ASSIGNMENT ***

	MyStrCpy(PLP_STR_ARRAY[0], "My");
	MyStrCpy(PLP_STR_ARRAY[1], "Name");
	MyStrCpy(PLP_STR_ARRAY[2], "Is");
	MyStrCpy(PLP_STR_ARRAY[3], "Prathamesh");
	MyStrCpy(PLP_STR_ARRAY[4], "Paropkari");
	printf("\n\n");

	printf("Strings In the 2D Array are : \n\n");
	for (i = 0; i < str_array_num_rows; i++)
	{
		printf("%s ", PLP_STR_ARRAY[i]);
	}

	printf("\n\n");

		return(0);



}

void MyStrCpy(char str_destination[], char str_source[])
{
	// Function prototype
	int MyStrLen(char[]);

	//Variable declaration

	int PLP_STRING_LENGTH = 0;
	int PLP_J;

	//code
	PLP_STRING_LENGTH = MyStrLen(str_source);
	for (PLP_J = 0; PLP_J < PLP_STRING_LENGTH; PLP_J++)
	{
		str_destination[PLP_J] = str_source[PLP_J];

	}
	str_destination[PLP_J] = '\0';



}

int MyStrLen(char str[])
{
	int PLP_J, PLP_STRING_LENGTH = 0;
	// Determining exact length of the string, by detecting the first occurence of Null-Terminating character(\0)

	for (PLP_J = 0; PLP_J < MAX_STRING_LENGTH; PLP_J++)
	{
		if (str[PLP_J] == '\0')
		{
			break;
		}
		else
		{
			PLP_STRING_LENGTH++;
		}
	}

	return(PLP_STRING_LENGTH);

}
