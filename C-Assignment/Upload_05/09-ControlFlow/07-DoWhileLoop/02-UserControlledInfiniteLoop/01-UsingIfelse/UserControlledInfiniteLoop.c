#include<stdio.h>
#include<conio.h>

int main(void)
{
	//variable declaration
	char option, ch = '\0';

	//code

	printf("\n\n");
	printf("Once the Infinite Loop begins, Enter 'Q' or 'q' to Quit the Infinite DO WHILE Loop : \n\n");
	printf("Enter 'Y' or 'y' to Initiate User Controlled Infinite Loop : ");
	printf("\n\n");

	option = getch();

	if (option == 'Y' || option == 'y')
	{
		do
		{
			printf("In Loop.....\n\n");
			ch = getch();
			if (ch == 'Q' || ch == 'q')
			{
				break;
			}

		} while (1);

		printf("Exiting user Controlled Infinite Loop....");
		printf("\n\n");

	}

	else
	{
		printf("You must press 'Y' or 'y' to Initiate user Controlled Infinite Loop....Please Try Again....\n\n");

	}

	return(0);


		

}
