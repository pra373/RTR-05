#include<stdio.h>

int main(void)
{
	//code

	printf("\n\n");

	printf("Size of 'int'             = %lu Bytes\n",sizeof(int));
	printf("Size of 'unsigned int'    = %lu Bytes\n",sizeof(unsigned int));
	printf("Size of 'long'            = %lu Bytes\n",sizeof(long));
	printf("Size of 'long long'       = %lu Bytes\n", sizeof(long long));
	printf("Size of 'float'           = %lu Bytes\n", sizeof(float));
	printf("Size of 'double'          = %lu Bytes\n", sizeof(double));
	printf("Size of 'long double'     = %lu Bytes\n", sizeof(long double));

	printf("\n\n");

	return(0);


}