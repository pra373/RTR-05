#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
	//function prototype
	int MyStrlen(char[]);

	// A string is an array of characters ... so char[] is a char array and hence, char[] is a string.
	// array of char[] is array of strings !!!!
	//hence, char[][] is an array of char arrays hence, ia an array af strings

	/*here the string array can allow a maximum number of 10 strings(10 rows)
	and each of this 10 strings can only have 15 characters maximum (15 columns)*/

	//variable declaration

	char PLP_STR_ARRAY[10][15] = { "Hello!","Welcome","To","Real","Time","Rendering","Batch","(2023-2024)","Of","ASTROMEDICOMP." };//Inline Initialization

	int char_size;
	int str_array_size;
	int str_array_num_elements;
	int str_array_num_rows;
	int str_array_num_columns;
	int actual_num_chars = 0;
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

	for (i = 0; i < str_array_num_rows; i++)
	{
		actual_num_chars = actual_num_chars + MyStrlen(PLP_STR_ARRAY[i]);
	}

	printf("Actual Number of Elements (characters) In Two Dimentiona (2D) character array (string array) is = %d\n\n", actual_num_chars);

	printf("\n\n");
	printf("Strings In the 2D Array : \n\n");

	printf("%s ", PLP_STR_ARRAY[0]);
	printf("%s ", PLP_STR_ARRAY[1]);
	printf("%s ", PLP_STR_ARRAY[2]);
	printf("%s ", PLP_STR_ARRAY[3]);
	printf("%s ", PLP_STR_ARRAY[4]);
	printf("%s ", PLP_STR_ARRAY[5]);
	printf("%s ", PLP_STR_ARRAY[6]);
	printf("%s ", PLP_STR_ARRAY[7]);
	printf("%s ", PLP_STR_ARRAY[8]);
	printf("%s ", PLP_STR_ARRAY[9]);

	printf("\n\n");

	return(0);





}
MyStrlen(char str[])
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