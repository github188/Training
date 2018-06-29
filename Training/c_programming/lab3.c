#include<stdio.h>
float Power(float fBase, int iPowerNum)
{
	float fResult = 1;
	int i;
	for(i = 1; i <= iPowerNum; ++i)
	{
		fResult *= fBase;
	}
	return fResult;
}
void main()
{
	int iPowerNum;
	float fBase;
	printf("Insert the number of base and power\n");

	scanf("%f %d", &fBase, &iPowerNum);	
	printf("Answer: %f\n", Power(fBase, iPowerNum));
}
