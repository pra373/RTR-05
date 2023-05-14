#include<stdio.h>
int main(void)
{
	//code

	printf("\n\n");
	printf("Going on to the next line...using \\n Escape Sequence\n\n");
	printf("Demonstrating \t Horizontal \t Tab \t using \t \\t Escape sequence!!!\n\n");
	printf("\"This is a double quoted output\" done using \\\" \\\" Escape sequence \n\n");
	printf("\'This is a single quoted output\' done using \\\' \\\' Escape sequence\n\n");
	printf("BACKSPACE turned to BACKSPACE\b using Escape sequence\\b\n\n");
	printf("\r Demostrating carriage return using \\r Escape sequence\n");
	printf("Demonstrating \r carriage return using \\r Escape sequence\n");
	printf("Demonstrating carriage \r return using \\r Escape sequence\n\n");
	printf("Demonstrating \x41 using \\xhh Escape sequence\n\n");
	printf("Demonstrating \102 using \\ooo Escape sequence\n\n");
	return(0);

}