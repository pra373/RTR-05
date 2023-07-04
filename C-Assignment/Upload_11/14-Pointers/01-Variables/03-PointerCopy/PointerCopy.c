#include<stdio.h>

int main(void)
{
	// variable declaration
	int PLP_NUM;
	int *PLP_PTR = NULL;
	int *PLP_COPY_PTR = NULL;

	//code

	PLP_NUM = 5;
	PLP_PTR = &PLP_NUM;

	printf("\n\n");
	printf("*********** Brfore PLP_COPY_PTR = PLP_PTR *************  \n\n");
	printf(" PLP_NUM        = %d\n", PLP_NUM);
	printf(" &PLP_NUM       = %p\n", &PLP_NUM);
	printf(" *(&PLP_NUM)    = %d\n", *(&PLP_NUM));
	printf(" PLP_PTR        = %p\n", PLP_PTR);
	printf(" *PLP_PTR       = %d\n", *PLP_PTR);


	// PLP_PTR is a integer pointer variable 
	// PLP_PTR will contain address of an integer variable
	// if we assign PLP_PTR to PLP_COPY_PTR, PLP_COPY_PTR will also contain address of that integer variable

	// hence now both PLP_PTR and PLP_COPY_PTR will point to PLP_NUM

	PLP_COPY_PTR = PLP_PTR;

	printf("\n\n");
	printf("*********** After PLP_COPY_PTR = PLP_PTR *************  \n\n");
	printf(" PLP_NUM          = %d\n", PLP_NUM);
	printf(" &PLP_NUM         = %p\n", &PLP_NUM);
	printf(" *(&PLP_NUM)      = %d\n", *(&PLP_NUM));
	printf(" PLP_PTR          = %p\n", PLP_PTR);
	printf(" *PLP_PTR         = %d\n", *PLP_PTR);
	printf(" PLP_COPY_PTR     = %p\n", PLP_COPY_PTR);
	printf(" *PLP_COPY_PTR    = %d\n", *PLP_COPY_PTR);

	return(0);


}