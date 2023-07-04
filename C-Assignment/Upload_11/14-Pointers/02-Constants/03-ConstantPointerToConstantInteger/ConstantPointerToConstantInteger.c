#include<stdio.h>

int main(void)
{
	//variable declaration

	int PLP_NUM = 5;
	const int *const ptr = &PLP_NUM; // ptr is a constant pointer to a integer constant

	//code
	printf("\n\n");
	printf("Current Value of 'PLP_NUM' = %d\n", PLP_NUM);
	printf("Current 'ptr' (Address of 'num') = %p\n", ptr);

	//The following line does NOT give error 
	//as we are modifying the value of the variable individually

	PLP_NUM++;
	printf("After PLP_NUM++,Value of 'PLP_NUM' = %d\n", PLP_NUM);

	// The following line gives error and is hence commented out.
   // We cannot alter the 'ptr' value as 'ptr' is "a constant pointer to constant integer.
   
   // With respect to the pointer, the value it points to is constant AND the P pointer itself is also constant. 
   
	//ptr++;

	// The following line also give error and is hence commented out.
    // We cannot alter the value stored in 'ptr as 'ptr is "A constant pointer to constant integer.
    // With respect to the pointer, the value it points to is constant AND the pointer itself is also constant.
    
	//(*ptr++)

	return(0);

}