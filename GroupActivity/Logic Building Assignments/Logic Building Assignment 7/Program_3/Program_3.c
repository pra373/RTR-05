#include<stdio.h>
int KMtoMeter(int iNo)
{
	// Logic
    iNo = iNo * 1000;
	return iNo;

}
int main()
{
	int iValue = 0, iRet = 0;
	printf("\nEnter distance:\t");
	scanf("%d", &iValue);
	iRet = KMtoMeter(iValue);
	printf("\nDistance in meter:%d\n", iRet);
    return (0);
}