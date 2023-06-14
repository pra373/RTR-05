#include<stdio.h>

// *********** Global Scope ***********

int Global_Variable = 0;

int main(int argc, char* argv[], char* envp[])
{
	// Function Signature

	void Change_Count_One(void);
	void Change_Count_Two(void);
	void Change_Count_Three(void);

	//code

	printf("\n\n");

	printf("main() : value of global_Variable = %d\n", Global_Variable);

	Change_Count_One();
	Change_Count_Two();
	Change_Count_Three();

	printf("\n\n");
	return(0);


}

//************ Global Scope **************

void Change_Count_One(void)
{
	//code

	Global_Variable = 100;

	printf("Change_Count_One() : Value of Global_Variable = %d\n", Global_Variable);

}
//************ Global Scope **************

void Change_Count_Two(void)
{
	//code

	Global_Variable = Global_Variable + 1;
	printf("Change_Count_Two() : Value of Global_Variable = %d\n", Global_Variable);

}
//************ Global Scope **************

void Change_Count_Three(void)
{
	//code

	Global_Variable = Global_Variable + 10;
	printf("Change_Count_Three() : Value of Global_Variable = %d\n", Global_Variable);

}





