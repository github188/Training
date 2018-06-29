#include<stdio.h>
float Power(float fBase, int iPowerNum)
{
        if (iPowerNum == 0)
        {
                return 1;
        }
        else
        {
                return fBase * Power(fBase, iPowerNum - 1);
        }
}
void main()
{
        int iPowerNum;
        float fBase;
        printf("Insert the number of base and power\n");
        scanf("%f %d", &fBase, &iPowerNum);
        printf("Answer: %f\n",Power(fBase, iPowerNum));
}
