#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <limits.h>
#define PRIME 1
#define NOT_PRIME 0
int i, j;
void Sieve(int *aRecord, int iNum)
{
	for(i = 2; i <= iNum; ++i)
	{
		aRecord[i] = PRIME;
	}
	for(i = 2; i <= sqrt(iNum); ++i)
	{
		if(aRecord[i])
		{
			for(j = i; j*i <= iNum; ++j)
			{
				aRecord[j*i] = NOT_PRIME;
			}
		}
	}
	for(i = 2; i <= iNum; ++i)
	{
		if(aRecord[i])
		{
			printf("%d ", i);
		}
	}
	printf("\n");
}



int main()
{
	int iNum = 0, iSize;
	char cMaxInput[10] = {2147483648};
	char cInput[10] = {0};
	printf("Insert the number\n");
	if (!scanf("%s", cInput))
	{
		printf("ERROR\n");
		exit(EXIT_FAILURE);
	}
	if (strcmp(cMaxInput, cInput) > 0)
	{

	}
	/*
	if (!scanf("%d", &iNum))
	{
		printf("ERROR\n");
		exit(EXIT_FAILURE);
	}*/
	printf("%d\n", iNum);
	if (iNum <= 0)
	{
		printf("Negative Input\n");
		exit(EXIT_FAILURE);
	}
	iSize = iNum + 1;
	if (/*(iSize > INT_MAX) ||*/ ((iSize * sizeof(int)) > INT_MAX))
	{   printf("%ld\n", iSize*sizeof(int));
		printf("Overflow\n");
		exit(EXIT_FAILURE);
	}
	int *aRecord = malloc(iSize * sizeof(int));
	Sieve(aRecord, iNum);
	free(aRecord);
	return 0;
}
