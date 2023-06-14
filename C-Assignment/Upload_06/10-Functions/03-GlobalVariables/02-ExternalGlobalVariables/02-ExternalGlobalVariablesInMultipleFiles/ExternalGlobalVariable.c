#include<stdio.h>

// Global scope

int Global_Variable = 0;

int main(int argc, char* argv[], char* envp[])
{
	//Function declarations

	void change_count(void);
	void change_count_one(void); // function defined in file_01.c
	void change_count_two(void); // function defined in file_02.c

	//code
	printf("\n\n");

	change_count();
	change_count_one();
	change_count_two();

	return(0);

}

void change_count(void)
{
	//code

	Global_Variable = Global_Variable + 1;
	printf("Value of GLobal Variable in change_count() = %d\n\n", Global_Variable);


}