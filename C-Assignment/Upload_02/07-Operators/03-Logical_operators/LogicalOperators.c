#include<stdio.h>
#include<conio.h>

int main()
{
	//variable declaration.

	int PLP_A;
	int PLP_B;
	int PLP_C;
	int PLP_RESULT;

	//code

	printf("\n\n");
	printf("Enter First Integer : ");
	scanf("%d", &PLP_A);

	printf("\n\n");
	printf("Enter Second Integer : ");
	scanf("%d", &PLP_B);

	printf("\n\n");
	printf("Enter Third Integer : ");
	scanf("%d", &PLP_C);

	printf("\n\n");
	printf("If answer = 0, It is 'FALSE'.\n");
	printf("If answer = 1, It is 'TRUE'.\n\n");

	PLP_RESULT = (PLP_A <= PLP_B) && (PLP_B != PLP_C);
	printf("Logical AND (&&) : Answer is TRUE (1) If and only if both conditions are true. The answer is false (0), If any one or Both conditions are false \n\n");
	printf("PLP_A = %d Is less yhan or equal to PLP_B = %d AND PLP_B = %d is not equal to PLP_C = %d    \t Answer = %d\n\n", PLP_A, PLP_B, PLP_B, PLP_C, PLP_RESULT);

	PLP_RESULT = (PLP_B >= PLP_A) || (PLP_A == PLP_C);
	printf("Logical OR (||) : Answer is FALSE (0) If and only if both conditions are FALSE. The answer is TRUE (1), If any one or Both conditions are TRUE \n\n");
	printf("Either PLP_B = %d Is graeter than or equal to PLP_A = %d OR PLP_A = %d Is equal to PLP_C = %d \t Answer = %d\n\n",PLP_B,PLP_A,PLP_A,PLP_C,PLP_RESULT);

	PLP_RESULT = !PLP_A;
	printf("PLP_A = %d and using Logical NOT (!) operator on PLP_A given result = %d\n\n", PLP_A, PLP_RESULT);

	PLP_RESULT = !PLP_B;
	printf("PLP_B = %d and using Logical NOT (!) operator on PLP_B given result = %d\n\n", PLP_B, PLP_RESULT);

	PLP_RESULT = !PLP_C;
	printf("PLP_C = %d and using Logical NOT (!) operator on PLP_C given result = %d\n\n", PLP_C, PLP_RESULT);

	PLP_RESULT = (!(PLP_A <= PLP_B) && !(PLP_B != PLP_C));
	printf("using logical NOT(!) on (PLP_A  <= PLP_B And Also On (PLP_B != PLP_C) and then ANDing them afterwards gives result = %d\n", PLP_RESULT);

	printf("\n\n");

	PLP_RESULT = !((PLP_B >= PLP_A) || (PLP_A == PLP_C));
	printf("Using logical NOT (!) on Entire Logical Expression (PLP_B >= PLP_A) || (PLP_A == PLP_C) Gives Result = %d\n", PLP_RESULT);
	printf("\n\n");

	getch();
	return(0);

	



}