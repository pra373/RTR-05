#include<stdio.h>
#include<conio.h>

//ASCII Values for 'A' to 'Z' => 65 to 90

#define CHAR_ALPHABET_UPPER_CASE_BEGINNING 65
#define CHAR_ALPHABET_UPPER_CASE_ENDING 90

//ASCII Values for 'a' to z' => 97 to 122

#define CHAR_ALPHABET_LOWER_CASE_BEGINNING 97
#define CHAR_ALPHABET_LOWER_CASE_ENDING 122

#define CHAR_DIGIT_BEGINNING 48
#define CHAR_DIGIT_ENDING 57



int main(void)
{
	//variable declaration
	char PLP_CH;
	int PLP_CH_VALUE;

	//code
	printf("\n\n");
	printf("Enter Character : ");
	PLP_CH = getch();
	printf("\n\n");

	switch (PLP_CH)
	{
		//Fall through condition
	case 'A':
	case 'a':

	case 'E':
	case 'e':

	case 'I':
	case 'i':

	case 'O':
	case 'o':

	case 'U':
	case 'u':
		printf("Character \'%c\' Entered by you,Is a Vowel Character from ENglish Alphaabet !!!\n\n", PLP_CH);
		break;
	default:
		PLP_CH_VALUE = (int)PLP_CH;

		//If the character has ASCII Value Between 65 and 90 or between 97 and 122 then its a consonent letter...

		if ((PLP_CH_VALUE >= CHAR_ALPHABET_UPPER_CASE_BEGINNING && PLP_CH_VALUE <= CHAR_ALPHABET_UPPER_CASE_ENDING) || (PLP_CH_VALUE >= CHAR_ALPHABET_LOWER_CASE_BEGINNING && PLP_CH_VALUE <= CHAR_ALPHABET_LOWER_CASE_ENDING))
		{
			printf("Character \'%c\' Entered by you, Is a Consonent Character from ENglish Alphaabet !!!\n\n", PLP_CH);

		}

		else if (PLP_CH_VALUE >= CHAR_DIGIT_BEGINNING && PLP_CH_VALUE <= CHAR_DIGIT_ENDING)
		{
			printf("Character \'%c\' Entered by you, Is a DIGIT Character !!!\n\n", PLP_CH);

		}

		else
		{
			printf("Character \'%c\' Entered by you, Is a SPECIAL Character !!!\n\n", PLP_CH);
		}
		break;



	}

	printf("Switch case block complete !!!\n");
	return(0);





}