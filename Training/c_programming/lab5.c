#include<stdio.h>
#include<malloc.h>
#define ARRAY_SIZE 5
void main()
{
        int aSource[ARRAY_SIZE] = {1, 2, 3, 4, 5566};
        int aFirstcopy[ARRAY_SIZE] = {0};
        int *pSecondcopy = malloc(ARRAY_SIZE * sizeof(int));
        int i;
        for(i = 0; i < ARRAY_SIZE; ++i)
        {
                aFirstcopy[i] = aSource[i];
                *(pSecondcopy + i) = aSource[i];
        }
        for(i = 0; i < ARRAY_SIZE; ++i)
        {
                printf("%d ", aFirstcopy[i]);
                printf("%d\n", *(pSecondcopy + i));
        }
        free(pSecondcopy);
}
