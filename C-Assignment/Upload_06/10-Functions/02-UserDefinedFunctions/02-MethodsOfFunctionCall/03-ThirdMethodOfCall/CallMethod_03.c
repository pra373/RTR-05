#include<stdio.h>


/* User defined functions : Second method of calling functions
   Calling only one functions directly in main(),Rest of the functions trace their call directly to main() */ 

int main(int argc, char* argv[], char* envp[])
{
	// function prototype
	void Function_Country(void);

	//code
	
	Function_Country();
	return(0);
}

void Function_Country(void)
{
	// function prototype
	void Function_OfAMC(void);

	//code

	Function_OfAMC();

	printf("\n\n");

	printf("I live in INDIA");

	printf("\n\n");
}

void Function_OfAMC(void)
{
	// function prototype
	void Function_Surname(void);

	//code
	Function_Surname();

	printf("\n\n");
	printf("Of ASTROMEDICOMP");
}

void Function_Surname(void)
{
	// function prototype
	void Function_MiddleName(void);

	//code

	Function_MiddleName();
	printf("\n\n");
	printf("Paropkari");
}

void Function_MiddleName(void)
{
	// function prototype
	void Function_FirstName(void);

	//code

	Function_FirstName();
	printf("\n\n");
	printf("Laxmikant");

}

void Function_FirstName(void)
{
	// function prototype
	void Function_Is(void);

	//code
	
	Function_Is();
	printf("\n\n");
	printf("Prathamesh");
}

void Function_Is(void)
{
	// function prototype
	void Function_Name(void);

	//code

	Function_Name();
	printf("\n\n");
	printf("Is");
}

void Function_Name(void)
{
	// function prototype
	void Function_My(void);

	//code

	Function_My();
	printf("\n\n");
	printf("Name");
}

void Function_My(void)
{
	//code

	printf("\n\n");
	printf("My");
}





