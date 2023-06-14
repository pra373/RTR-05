#include<stdio.h>

// ************* Global Scope ************

int main(int argc, char* argv[], char* envp[])
{
	//funtion prototype

	void Change_Count(void);

	// variable declaration
	extern int Global_Variable;

	//code

	printf("\n\n");
	printf("Value of Global_Variable before Change_Count() = %d\n", Global_Variable);
	Change_Count();
	printf("Value of Global_Variable After Change_Count() = %d\n", Global_Variable);
	printf("\n\n");

	return(0);

}


int Global_Variable = 0;

void Change_Count(void)
{
	//code
	Global_Variable = 5;
	printf("Value of Global_Variable in Change_Count() = %d\n", Global_Variable);
}

