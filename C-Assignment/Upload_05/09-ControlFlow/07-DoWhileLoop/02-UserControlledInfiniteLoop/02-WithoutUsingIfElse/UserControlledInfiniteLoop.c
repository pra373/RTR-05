#include<stdio.h>
#include<conio.h>
int main(void)
{
	//variable declaration
	char option, ch = '\0';

	//code
	printf("\n\n");
	printf("Once the Infinite Loop begins, Enter 'Q' or 'q' to Quit the Infinite DO WHILE Loop : \n\n");

	do
	{
		do
		{
			printf("\n");
			printf("In Loop...\n");
			ch = getch();

		} while (ch != 'Q' && ch != 'q');
		printf("\n\n");
		printf("Exiting User Controlled Infinite Loop...");
		printf("\n\n");

		printf("Do you want to begin user controlled Infinite loop Again....(Y/y - yes, Any other Key - No) :\n");
		option = getch();



	} while (option == 'Y' || option == 'y');

	return(0);

}