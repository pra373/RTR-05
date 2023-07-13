#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	//Function declaration
	void MyAlloc(int **, unsigned int);

	//variable declaration
	int *PLP_POINTER_TO_ARRAY = NULL;
	unsigned int num_elements;
	int i;

	//code

	printf("\n\n");
	printf("How many elements you want in integer array ?\n\n");
	scanf("%d", &num_elements);

	printf("\n\n");

	MyAlloc(&PLP_POINTER_TO_ARRAY, num_elements);

	printf("Enter %u elements to fill up your array :\n\n",num_elements);
	for (i = 0; i < num_elements; i++)
	{
		scanf("%d", &PLP_POINTER_TO_ARRAY[i]);
	}

	printf("\n\n");
	printf("Elements entered by you in the integer array : \n\n");
	for (i = 0; i < num_elements; i++)
	{
		printf("%d\n", PLP_POINTER_TO_ARRAY[i]);
	}

	printf("\n\n");

	if (PLP_POINTER_TO_ARRAY)
	{
		free(PLP_POINTER_TO_ARRAY);
		PLP_POINTER_TO_ARRAY = NULL;
		printf("Memory assigned to integer array has been freed successfully !!!\n\n");

	}

	return(0);




}

void MyAlloc(int **ptr, unsigned int NumberofElements)
{
	//code

	*ptr = (int *)malloc(NumberofElements * sizeof(int));
	if (*ptr == NULL)
	{
		printf("Could not allocate memory !!! Exiting now ...\n\n");
		exit(0);
	}

	printf("MyAlloc() has successfully allocated %zu bytes for Integer array !!!\n\n",(NumberofElements*sizeof(int)));
}