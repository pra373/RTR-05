#include<stdio.h>

int main(void)
{
	//variable declaration

	int PLP_IARRAY[] = { 12,24,36,48,60,72,84,96,108,120 };
	float PLP_FARRAY[] = { 9.8f,8.7f,7.6f,6.5f,5.4f };
	double PLP_DARRAY[] = { 1.222222,2.333333,3.444444 };
	char PLP_CARRAY[] = { 'A','S','T','R','O','M','E','D','I','C','O','M','P','\0' };

	//code

	//Name of a array is its base address

	printf("\n\n");
	printf("Integer array elements and the addresses they occupy are as follows : \n\n");
	printf("PLP_IARRAY[0] = %d \t Address : %p\n", *(PLP_IARRAY + 0), (PLP_IARRAY + 0));
	printf("PLP_IARRAY[1] = %d \t Address : %p\n", *(PLP_IARRAY + 1), (PLP_IARRAY + 1));
	printf("PLP_IARRAY[2] = %d \t Address : %p\n", *(PLP_IARRAY + 2), (PLP_IARRAY + 2));
	printf("PLP_IARRAY[3] = %d \t Address : %p\n", *(PLP_IARRAY + 3), (PLP_IARRAY + 3));
	printf("PLP_IARRAY[4] = %d \t Address : %p\n", *(PLP_IARRAY + 4), (PLP_IARRAY + 4));
	printf("PLP_IARRAY[5] = %d \t Address : %p\n", *(PLP_IARRAY + 5), (PLP_IARRAY + 5));
	printf("PLP_IARRAY[6] = %d \t Address : %p\n", *(PLP_IARRAY + 6), (PLP_IARRAY + 6));
	printf("PLP_IARRAY[7] = %d \t Address : %p\n", *(PLP_IARRAY + 7), (PLP_IARRAY + 7));
	printf("PLP_IARRAY[8] = %d \t Address : %p\n", *(PLP_IARRAY + 8), (PLP_IARRAY + 8));
	printf("PLP_IARRAY[9] = %d \t Address : %p\n", *(PLP_IARRAY + 9), (PLP_IARRAY + 9));

	printf("\n\n");
	printf("Float array elements and the addresses they occupy are as follows : \n\n");
	printf("PLP_FARRAY[0] = %f \t Address : %p\n", *(PLP_FARRAY + 0), (PLP_IARRAY + 0));
	printf("PLP_FARRAY[1] = %f \t Address : %p\n", *(PLP_FARRAY + 1), (PLP_IARRAY + 1));
	printf("PLP_FARRAY[2] = %f \t Address : %p\n", *(PLP_FARRAY + 2), (PLP_IARRAY + 2));
	printf("PLP_FARRAY[3] = %f \t Address : %p\n", *(PLP_FARRAY + 3), (PLP_IARRAY + 3));
	printf("PLP_FARRAY[4] = %f \t Address : %p\n", *(PLP_FARRAY + 4), (PLP_IARRAY + 4));

	printf("\n\n");
	printf("Double array elements and the addresses they occupy are as follows : \n\n");
	printf("PLP_DARRAY[0] = %lf \t Address : %p\n", *(PLP_DARRAY + 0), (PLP_IARRAY + 0));
	printf("PLP_DARRAY[1] = %lf \t Address : %p\n", *(PLP_DARRAY + 1), (PLP_IARRAY + 1));
	printf("PLP_DARRAY[2] = %lf \t Address : %p\n", *(PLP_DARRAY + 2), (PLP_IARRAY + 2));

	printf("\n\n");
	printf("Character array elements and the addresses they occupy are as follows : \n\n");
	printf("PLP_CARRAY[0] = %c \t Address : %p\n", *(PLP_CARRAY + 0), (PLP_CARRAY + 0));
	printf("PLP_CARRAY[1] = %c \t Address : %p\n", *(PLP_CARRAY + 1), (PLP_CARRAY + 1));
	printf("PLP_CARRAY[2] = %c \t Address : %p\n", *(PLP_CARRAY + 2), (PLP_CARRAY + 2));
	printf("PLP_CARRAY[3] = %c \t Address : %p\n", *(PLP_CARRAY + 3), (PLP_CARRAY + 3));
	printf("PLP_CARRAY[4] = %c \t Address : %p\n", *(PLP_CARRAY + 4), (PLP_CARRAY + 4));
	printf("PLP_CARRAY[5] = %c \t Address : %p\n", *(PLP_CARRAY + 5), (PLP_CARRAY + 5));
	printf("PLP_CARRAY[6] = %c \t Address : %p\n", *(PLP_CARRAY + 6), (PLP_CARRAY + 6));
	printf("PLP_CARRAY[7] = %c \t Address : %p\n", *(PLP_CARRAY + 7), (PLP_CARRAY + 7));
	printf("PLP_CARRAY[8] = %c \t Address : %p\n", *(PLP_CARRAY + 8), (PLP_CARRAY + 8));
	printf("PLP_CARRAY[9] = %c \t Address : %p\n", *(PLP_CARRAY + 9), (PLP_CARRAY + 9));
	printf("PLP_CARRAY[10] = %c \t Address : %p\n", *(PLP_CARRAY + 10), (PLP_CARRAY + 10));
	printf("PLP_CARRAY[11] = %c \t Address : %p\n", *(PLP_CARRAY + 11), (PLP_CARRAY + 11));
	printf("PLP_CARRAY[12] = %c \t Address : %p\n", *(PLP_CARRAY + 12), (PLP_CARRAY + 12));
	printf("PLP_CARRAY[13] = %c \t Address : %p\n", *(PLP_CARRAY + 13), (PLP_CARRAY + 13));

	return(0);
}