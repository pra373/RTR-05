#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_STRING_LENGTH 1024

struct CharacterCount 
{
	char PLP_CH;
	int PLP_CH_COUNT;
} character_and_count[] = { {'A',0}, //character_and_count[0].PLP_CH = 'A' & character_and_count[0].PLP_CH_COUNT = 0 
							{'B',0},//character_and_count[1].PLP_CH = 'B' & character_and_count[1].PLP_CH_COUNT = 0 
							{'C',0},
							{'D',0},
							{'E',0},
							{'F',0},
							{'G',0},
							{'H',0},
							{'I',0},
							{'J',0},
							{'K',0},
							{'L',0},
							{'M',0},
							{'N',0},
							{'O',0},
							{'P',0},
							{'Q',0},
							{'R',0},
							{'S',0},
							{'T',0},
							{'U',0},
							{'V',0},
							{'W',0},
							{'X',0},
							{'Y',0},
							{'Z',0} };

							
#define SIZE_OF_ENTIRE_ARRAY_OF_STRUCTS sizeof(character_and_count)
#define SIZE_OF_ONE_STRUCT_FROM_THE_ARRAY_OF_STRUCTS sizeof(character_and_count[0])
#define NUM_ELEMENTS_IN_ARRAY (SIZE_OF_ENTIRE_ARRAY_OF_STRUCTS / SIZE_OF_ONE_STRUCT_FROM_THE_ARRAY_OF_STRUCTS)							
	
int main(void)
{
	//variable declarations 
	char str[MAX_STRING_LENGTH];
	int i, j, actual_string_length = 0;

	//code
	printf("\n\n");
	printf("Enter A String: \n\n");
	gets_s(str, MAX_STRING_LENGTH);

	actual_string_length = strlen(str);
	printf("\n\n");
	printf("The String You Have Entered Is \n\n");
	printf("%s\n\n", str);

	for (i = 0; i < actual_string_length; i++)
	{
		for (j = 0; j < NUM_ELEMENTS_IN_ARRAY; j++) //Run every character of the input string through the entire alphabet (A TO Z)
		{
			str[i] = toupper(str[i]); //If input character is in lower case, turn it to upper case for comparison

			if (str[i] == character_and_count[j].PLP_CH) //If character is present...
				character_and_count[j].PLP_CH_COUNT++;   //Increment its count by 1...
		}

		
	}

	printf("\n\n");
	printf("The Number Of Occurences of ALL Characters From The Alphabet Are As Follows: \n\n");

	for (i = 0; i < NUM_ELEMENTS_IN_ARRAY; i++)
	{
		printf("Character %c = %d\n", character_and_count[i].PLP_CH, character_and_count[i].PLP_CH_COUNT);
	}

	printf("\n\n");
	return(0);
		




}

