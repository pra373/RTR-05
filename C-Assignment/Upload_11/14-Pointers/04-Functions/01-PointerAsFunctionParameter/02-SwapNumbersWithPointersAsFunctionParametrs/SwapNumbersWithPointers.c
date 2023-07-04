#include<stdio.h>

int main(void)
{
	// function declaration

	void SwapNumbers(int *, int *);

	//variable declaration
	int PLP_A;
	int PLP_B;

	printf("\n\n");
	printf("Enter Value For 'PLP_A' : ");
	scanf("%d", &PLP_A);

	printf("\n\n");
	printf("Enter Value For 'PLP_B' : ");
	scanf("%d", &PLP_B);

	printf("\n\n");
	printf("********* Before Swapping *********\n\n");
	printf("Value of 'PLP_A' : %d\n\n", PLP_A);
	printf("Value of 'PLP_B' : %d\n\n", PLP_B);

	SwapNumbers(&PLP_A, &PLP_B);  // ******* ARGUMENTS PASSED 'BY REFERENCE / ADDRESS'
	
	printf("\n\n");
	printf("*********** After Swapping **********\n\n");
	printf("Value of 'PLP_A' = %d \n\n",PLP_A);
	printf("Value of 'PLP_B' : %d\n\n", PLP_B);

	return(0);

}


void SwapNumbers(int *x, int *y)
{
	//variable declaration
	int temp;

	//code

	printf("\n\n");
	printf("********* Before Swapping *********\n\n");
	printf("Value present at the address which is stored in x = %d \n\n", *x);
	printf("Value present at the address which is stored in y = %d \n\n", *y);

	temp = *x;
	*x = *y;
	*y = temp;

	printf("\n\n");
	printf("********* After Swapping *********\n\n");
	printf("Value present at the address which is stored in x = %d \n\n", *x);
	printf("Value present at the address which is stored in y = %d \n\n", *y);


}

