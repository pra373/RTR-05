//  Global scope

/* Global_Variable is a global variable declared in source code file ExternalGlobalVariable.c

To access it in this file it must first be re-declared as an external variable In the global scope of this file
along with the 'extern' kewword and its poper data type

Then it can be used as any odinary variable throught this file as well */

#include<stdio.h>

extern int Global_Variable;

void change_count_two(void)
{
	//code

	Global_Variable = Global_Variable + 1;
	printf("change_count_two() : value of Global_Variable in file_02 = %d\n\n", Global_Variable);

}