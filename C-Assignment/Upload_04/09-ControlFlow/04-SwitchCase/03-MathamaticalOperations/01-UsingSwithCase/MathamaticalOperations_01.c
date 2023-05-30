#include<stdio.h>
#include<conio.h>

int main(void)
{
	int PLP_A, PLP_B, PLP_RESULT;
	char PLP_OPTION, PLP_OPTION_DIVISION;

	//code
	printf("\n\n");

	printf("Enter value for PLP_A : ");
	scanf("%d", &PLP_A);

	printf("Enter value for PLP_B : ");
	scanf("%d", &PLP_B);

	printf("Enter option in character : \n\n");
	printf("'A' or 'a' for Addition : \n");
	printf("'S' or 's' for subtraction : \n");
	printf("'M' or 'm' for Multiplication : \n");
	printf("'D' or 'd' for Division : \n\n");

	printf("Enter Option : ");
	PLP_OPTION = getch();
	printf("\n\n");

	switch (PLP_OPTION)
	{
		//Fall through condition for 'A' and 'a'
	case 'A':
	case 'a':
		PLP_RESULT = PLP_A + PLP_B;
		printf("Addition of PLP_A = %d And PLP_B = %d Gives result %d !!!\n\n ", PLP_A, PLP_B, PLP_RESULT);
		break;
		//Fall through condition for 'S' and 's'
	case 'S':
	case 's':
		PLP_RESULT = PLP_A - PLP_B;
		printf("Subtraction of PLP_A = %d And PLP_B = %d Gives result %d !!!\n\n ", PLP_A, PLP_B, PLP_RESULT);
		break;

		//Fall through condition for 'M' and 'm'
	case 'M':
	case 'm':
		PLP_RESULT = PLP_A * PLP_B;
		printf("Multiplication of PLP_A = %d And PLP_B = %d Gives result %d !!!\n\n ", PLP_A, PLP_B, PLP_RESULT);
		break;

		//Fall through condition for 'D' and 'd'
	case 'D':
	case 'd':
		printf("Enter option in character : \n\n");
		printf("'Q' or 'q' or '/' for Quotient upon Division : \n");
		printf("'R' or 'r' or '%' for Remainder upon Division : \n\n");
		printf("Enter Option : ");
		PLP_OPTION_DIVISION = getch();
		printf("\n\n");

		switch (PLP_OPTION_DIVISION)
		{
			//Fall through condition for 'D' and 'd'
		case 'Q':
		case 'q':
		case '/':
			if (PLP_A >= PLP_B)
			{
				PLP_RESULT = PLP_A / PLP_B;
				printf("Division of PLP_A = %d by PLP_B = %d gives Quotient = %d !!!\n", PLP_A, PLP_B, PLP_RESULT);


		    }
			else
			{
				PLP_RESULT = PLP_B / PLP_A;
				printf("Division of PLP_B = %d by PLP_A = %d gives Quotient = %d !!!\n", PLP_A, PLP_B, PLP_RESULT);
			}
			break;

			//Fall through condition for 'D' and 'd'
		case 'R':
		case 'r':
		case '%':
			if (PLP_A >= PLP_B)
			{
				PLP_RESULT = PLP_A % PLP_B;
				printf("Division of PLP_A = %d by PLP_B = %d gives Remainder = %d !!!\n", PLP_A, PLP_B, PLP_RESULT);


			}
			else
			{
				PLP_RESULT = PLP_B % PLP_A;
				printf("Division of PLP_B = %d by PLP_A = %d gives Remainder = %d !!!\n", PLP_A, PLP_B, PLP_RESULT);
			}
			break;
			default:
				printf("Invalid character %c entered for division", PLP_OPTION_DIVISION);
				break;





		}
		break;

	default:
		printf("Invalid Character %c Entered Please try again...\n\n", PLP_OPTION);
		break;





	
	


		
	}
	printf("Switch case block complete !!!");

	return(0);

}