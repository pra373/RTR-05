#include<stdio.h>
#include<conio.h>

int main(void)
{
	//variable declarations

	char option, ch = '\0';

	//code

	printf("\n\n");
	printf("Once the infinite Loop begins, Enter 'Q' or 'q' to Quit the infinite while loop : \n\n");
	printf("Print 'Y' or 'y' to Initiate user controlled Infinite Loop : ");
	printf("\n\n");

	option = getch();
	if (option == 'Y' || option == 'y')
	{
		while (1) //infinite loop
		{
			printf("In Loop... \n");
			ch = getch();
			if (ch == 'Q' || ch == 'q')
			{
				break;  //user controll 
			}

		}
		printf("\n\n");
		printf("Exiting User Controlled Infinite Loop....");
		printf("\n\n");
	}
	else
	{
		printf("You must press 'Y' or 'y' to initiate the user controlled Infinite Loop....Please Try Again...\n\n");

	}

	return(0);
}