#include<stdio.h>

int main(void)
{
	//variable declarations

	int PLP_NUM;

	//code
	printf("\n\n");

	printf("Enter value for 'PLP_ NUM' : ");
	scanf("%d", &PLP_NUM);

	if (PLP_NUM < 0)    //if-01
	{
		printf("PLP_NUM = %d is less than 0 (negative) !!!\n\n", PLP_NUM);

	}
	else    // else-01
	{
		if ((PLP_NUM > 0) && (PLP_NUM <= 100)) // if-02
		{
			printf("PLP_NUM = %d is between 0 and 100 !!!\n\n", PLP_NUM);

		}
		else   // else-02
		{
			if ((PLP_NUM > 100) && (PLP_NUM <= 200)) // if-03
			{
				printf("PLP_NUM = %d is between 100 and 200 !!!\n\n", PLP_NUM);

			}

			else   // else-03
			{
				if ((PLP_NUM > 200) && (PLP_NUM <= 300)) // if-04
				{
					printf("PLP_NUM = %d is between 200 and 300 !!!\n\n", PLP_NUM);


				}
				else //else-04
				{
					if ((PLP_NUM > 300) && (PLP_NUM <= 400)) // if-05
					{
						printf("PLP_NUM = %d is between 300 and 400 !!!\n\n", PLP_NUM);


					}
					else // else-05
					{
						if ((PLP_NUM > 400) && (PLP_NUM <= 500)) // if-06
						{
							printf("PLP_NUM = %d is between 400 and 500 !!!\n\n", PLP_NUM);
						}
						else //else-06
						{	
							printf("PLP_NUM = %d is greater than 500 !!\n\n", PLP_NUM);

						}


					}


				}

				

			}


		}


	}
	return(0);
	
		
	

	
}