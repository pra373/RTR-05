#include<stdio.h>

int main(void)
{
	//variable declaration

	int PLP_IARRAY[] = { 10,20,30,40,50,60,70,80,90,100 };
	int *ptr_iarray = NULL;

	//code

	// Using array name as a array i.e : value of xth element of PLP_IARRAY:PLP_IARRAY[x] AND address of xth Element of PLP_IARRAY : &PLP_IARRAY[x]

	printf("\n\n");

	printf(" ***** Using array name as array i.e : value of xth element of PLP_IARRAY : PLP_IARRAY[x] AND address of xth element of PLP_IARRAY : &PLP_IARRAY[x] ******* \n\n");
	printf("Integer Array Elements and their addresses : \n\n");
	printf("PLP_IARRAY[0] = %d \t \t At address &PLP_IARRAY[0] = %p\n", PLP_IARRAY[0], &PLP_IARRAY[0]);
	printf("PLP_IARRAY[1] = %d \t \t At address &PLP_IARRAY[1] = %p\n", PLP_IARRAY[1], &PLP_IARRAY[1]);
	printf("PLP_IARRAY[2] = %d \t \t At address &PLP_IARRAY[2] = %p\n", PLP_IARRAY[2], &PLP_IARRAY[2]);
	printf("PLP_IARRAY[3] = %d \t \t At address &PLP_IARRAY[3] = %p\n", PLP_IARRAY[3], &PLP_IARRAY[3]);
	printf("PLP_IARRAY[4] = %d \t \t At address &PLP_IARRAY[4] = %p\n", PLP_IARRAY[4], &PLP_IARRAY[4]);
	printf("PLP_IARRAY[5] = %d \t \t At address &PLP_IARRAY[5] = %p\n", PLP_IARRAY[5], &PLP_IARRAY[5]);
	printf("PLP_IARRAY[6] = %d \t \t At address &PLP_IARRAY[6] = %p\n", PLP_IARRAY[6], &PLP_IARRAY[6]);
	printf("PLP_IARRAY[7] = %d \t \t At address &PLP_IARRAY[7] = %p\n", PLP_IARRAY[7], &PLP_IARRAY[7]);
	printf("PLP_IARRAY[8] = %d \t \t At address &PLP_IARRAY[8] = %p\n", PLP_IARRAY[8], &PLP_IARRAY[8]);
	printf("PLP_IARRAY[9] = %d \t \t At address &PLP_IARRAY[9] = %p\n", PLP_IARRAY[9], &PLP_IARRAY[9]);

	// assigning base address of Integer array 'PLP_IARRAY' to integer pointer 'ptr_iarray'
	// name of an array is its own base address


	ptr_iarray = PLP_IARRAY;

	// Using pointer as pointer i.e : value of xth element of PLP_IARRAY : *(ptr_iarray+x) AND address of xth Element of PLP_IARRAY : (ptr_iarray+x)

	printf("\n\n");
	printf("Using pointer as pointer i.e : value of xth element of PLP_IARRAY : *(ptr_iarray + x) AND address of xth Element of PLP_IARRAY : (ptr_iarray + x)\n\n");
	printf("Integer Array Elements and their addresses : \n\n");

	printf("*(ptr_iarray + 0) = %d \t \t at address (ptr_iarray + 0) = %p\n", *(ptr_iarray + 0), (ptr_iarray + 0));
	printf("*(ptr_iarray + 1) = %d \t \t at address (ptr_iarray + 1) = %p\n", *(ptr_iarray + 1), (ptr_iarray + 1));
	printf("*(ptr_iarray + 2) = %d \t \t at address (ptr_iarray + 2) = %p\n", *(ptr_iarray + 2), (ptr_iarray + 2));
	printf("*(ptr_iarray + 3) = %d \t \t at address (ptr_iarray + 3) = %p\n", *(ptr_iarray + 3), (ptr_iarray + 3));
	printf("*(ptr_iarray + 4) = %d \t \t at address (ptr_iarray + 4) = %p\n", *(ptr_iarray + 4), (ptr_iarray + 4));
	printf("*(ptr_iarray + 5) = %d \t \t at address (ptr_iarray + 5) = %p\n", *(ptr_iarray + 5), (ptr_iarray + 5));
	printf("*(ptr_iarray + 6) = %d \t \t at address (ptr_iarray + 6) = %p\n", *(ptr_iarray + 6), (ptr_iarray + 6));
	printf("*(ptr_iarray + 7) = %d \t \t at address (ptr_iarray + 7) = %p\n", *(ptr_iarray + 7), (ptr_iarray + 7));
	printf("*(ptr_iarray + 8) = %d \t \t at address (ptr_iarray + 8) = %p\n", *(ptr_iarray + 8), (ptr_iarray + 8));
	printf("*(ptr_iarray + 9) = %d \t \t at address (ptr_iarray + 9) = %p\n", *(ptr_iarray + 9), (ptr_iarray + 9));

	return(0);

}