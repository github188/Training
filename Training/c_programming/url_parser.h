
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct TParam 
{
    char szName[64];
    char szValue[64];
    struct TParam *pNext;
}TParam;

typedef struct TUrlData 
{
    char szProtocol[8];
    char szDomain[128];
    int iport;
    char szPath[256];
    TParam *pParamList;
}TUrlData;

void PrintAndFreeParamList(TParam *pIndex);
