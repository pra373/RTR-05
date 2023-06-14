#include<stdio.h>


/* User defined functions : Second method of calling functions 
   Calling only two functions directly in main(),Rest of the functions trace their call directly to main() function */

int main(int argc, char* argv[], char* envp[])
{
	//function prototypes
	void Display_Information(void);
	void Function_Country(void);

	//code

	Display_Information(); //function call
	Function_Country();    //function call

	return(0);

}

void Display_Information(void) //function definition
{
	//Function prototypes

	void Function_My(void);
	void Function_Name(void);
	void Function_Is(void);
	void Function_FirstName(void);
	void Function_MiddleName(void);
	void Function_LastName(void);
	void Function_OfAMC(void);

	//code

	// Function calls for all the declared functions in current function definition

	Function_My();
	Function_Name();
	Function_Is();
	Function_FirstName();
	Function_MiddleName();
	Function_LastName();
	Function_OfAMC();
}

void Function_My(void)
{
	printf("\n\n");
	printf("My");
}

void Function_Name(void)
{
	printf("\n\n");
	printf("Name");
}

void Function_Is(void)
{
	printf("\n\n");
	printf("Is");
}

void Function_FirstName(void)
{
	printf("\n\n");
	printf("Prathamesh");
}

void Function_MiddleName(void)
{
	printf("\n\n");
	printf("Laxmikant");
}

void Function_LastName(void)
{
	printf("\n\n");
	printf("Paropkari");
}

void Function_OfAMC(void)
{
	printf("\n\n");
	printf("Of ASTROMEDICOMP");
}

void Function_Country(void)
{
	printf("\n\n");
	printf("I live in INDIA");
	printf("\n\n");

}











