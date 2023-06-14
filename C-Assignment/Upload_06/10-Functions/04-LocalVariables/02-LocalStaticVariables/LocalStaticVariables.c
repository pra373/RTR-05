#include<stdio.h>

// Global Scope 

int main(int argc, char* argv[], char* envp[])
{
	// Local scope of main begins.

	//variable declarations
	// 'PLP_A' is a local variable to main().

	int PLP_A = 5;

	//function prototype

	void change_count(void);

	//code

	printf("\n\n");
	printf("PLP_A = %d\n\n", PLP_A);

	// local_count variable is declared and initializedto 0 in change_count().
	// local_count = local_count + 1

	change_count();

	//since local_count is a static local variable of change_count().
	// it will retain its value from previous call of change_count().
	//so local_count is initialized to the new value when function gets a call again

	change_count();

	//since local_count is a static local variable of change_count().
	// it will retain its value from previous call to change_count().
	//so local_count is initialized to the new value when function gets a call again

	change_count();

	return(0);

	// Local scope of main() ends.

}

// global scope

void change_count(void)
{
	// local scope of change_count() begins

	//variable declarations

	static int local_count = 0;

	//local count is a local variable of change_count() only

	//code

	local_count += 1;
	printf("local_count = %d\n", local_count);


	// local scope of change_count() ends
}

//  global scope 