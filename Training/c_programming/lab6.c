#include<stdio.h>
#define ROW 3
#define COLUMN 5
float Avg(float aMatrix[ROW][COLUMN], int iRowNum)
{
	int i;
	float fSum = 0.0;
	for(i = 0; i < COLUMN; ++i)
	{
		fSum += aMatrix[iRowNum][i];
	}
	return fSum / COLUMN;
}
float TotalAvg(float aMatrix[ROW][COLUMN])
{	
	int i, j;	
	float fSum = 0.0;
	for(i = 0; i < ROW; ++i)
	{
		for(j = 0; j < COLUMN; ++j)
		{
			fSum += aMatrix[i][j];
		}
	}
	return fSum / (ROW * COLUMN);
}
float MAX(float aMatrix[ROW][COLUMN])
{
	int i, j;
	float fMax = aMatrix[0][0];
	for(i = 1; i < ROW; ++i)
	{
		for(j = 1; j < COLUMN; ++j)
		{
			if (fMax <= aMatrix[i][j])
			{
				fMax = aMatrix[i][j];
			}
		}
	}
	return fMax;
}

void main()
{
	int i, j;
	float aMatrix[ROW][COLUMN];
	for(i = 0; i < ROW; ++i)
	{
		printf("The %d round: Enter COLUMN numbers:\n", i);
		for(j = 0; j < COLUMN; ++j)
		{
		
			scanf("%f", &aMatrix[i][j]);
		}
	}
	for(i = 0; i < ROW; ++i)
	{
		printf("Row average is: %f\n", Avg(aMatrix, i));
	}
	printf("Total average is: %f\n", TotalAvg(aMatrix));
	printf("Maximum is: %f\n", MAX(aMatrix));
}

