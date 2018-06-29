/*
**********************************************************************
* $Header$
*
*  Copyright (c) 2000-2012 Vivotek Inc. All rights reserved.
*
*  +-----------------------------------------------------------------+
*  | THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY ONLY BE USED |
*  | AND COPIED IN ACCORDANCE WITH THE TERMS AND CONDITIONS OF SUCH  |
*  | A LICENSE AND WITH THE INCLUSION OF THE THIS COPY RIGHT NOTICE. |
*  | THIS SOFTWARE OR ANY OTHER COPIES OF THIS SOFTWARE MAY NOT BE   |
*  | PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY OTHER PERSON. THE   |
*  | OWNERSHIP AND TITLE OF THIS SOFTWARE IS NOT TRANSFERRED.        |
*  |                                                                 |
*  | THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT   |
*  | ANY PRIOR NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY |
*  | VIVOTEK INC.                                                    |
*  +-----------------------------------------------------------------+
*
* $History$
*
***********************************************************************
*/

/*!
 ***********************************************************************
 * Copyright 2000-2012 Vivotek, Inc. All rights reserved.
 *
 * \file
 * url_parser.c
 *
 * \brief
 * parser the url_parser - homework
 *
 * \date
 * 2012/08/06
 *
 * \author
 * walter lin
 *
 *
 **********************************************************************
*/

#include "url_parser.h"

void PrintAndFreeParamList(TParam *pIndex)
{
	if (pIndex != NULL) 
	{
		printf("param :%s\t", pIndex->szName);
		printf("value :%s\n", pIndex->szValue);
		PrintAndFreeParamList(pIndex->pNext);
		free(pIndex);
	}
}

int main(void)
{
    char szUrl[256];
    char* pctmp;
    char* pctmp2;
    TUrlData UrlData;
	TParam *pItem;
	TParam *pIndex;

	strcpy(UrlData.szProtocol,"");
	strcpy(UrlData.szDomain,"");
	UrlData.iport = 0;
	strcpy(UrlData.szPath,"");
	UrlData.pParamList = NULL;
	
    printf("enter Url to parser :\n");
    scanf("%s",szUrl);

	// example : http://192.168.0.56:8080/login.cgi?username=root&password=123
    // protocol , http
	// strtok fine the : and replace by \0
    pctmp = strtok(szUrl, ":");
    if (pctmp != NULL) 
    {
        strcpy(UrlData.szProtocol, pctmp);
    }
    	
    printf("----------%s \n", pctmp);
    // ip or domain name with port  
	// pctmp = 192.168.0.56:8080
	// pctmp2 = :8080
    pctmp = strtok(NULL, "/");
   //	printf("----------%s \n", pctmp);
    if (pctmp != NULL) 
    {
        pctmp2 = strchr(pctmp, ':');
        if ( pctmp2 != NULL) 
        {
			UrlData.iport = atoi(pctmp2 + 1);
			*pctmp2 = '\0';
            strcpy(UrlData.szDomain, pctmp);
        }
        else 
        {
            UrlData.iport = 80;
            strcpy(UrlData.szDomain, pctmp);
        }
    }

    // host file path
    pctmp = strtok(NULL, "?");
    if (pctmp != NULL) 
    {
        strcpy(UrlData.szPath, pctmp);
    }

    // param list 
	// pctmp = username=root&password=123
    pctmp = strtok(NULL, "");	
	pIndex = NULL;
    if (pctmp != NULL) 
    {
		pctmp2 = strtok(pctmp, "=");

        while (pctmp2 != NULL) 
        {
			if( !(pItem = (TParam *)malloc(sizeof(TParam))) )
			{
				printf("Out of memory!\n");
				return 0;
			}
			
			// pctmp2 = username
            strcpy(pItem->szName, pctmp2);
			
			// pctmp2 = root		
            pctmp2 = strtok(NULL, "&");
            strcpy(pItem->szValue, pctmp2);
			
            pItem->pNext = NULL;

            if (UrlData.pParamList == NULL) 
            {
                UrlData.pParamList = pItem;
            }
            else 
            {
                pIndex->pNext = pItem;
            }
            pIndex = pItem;
			
            pctmp2 = strtok(NULL, "=");
        }
    }
	
    printf("Protocol : %s\n", UrlData.szProtocol);
    printf("Domain   : %s\n", UrlData.szDomain);
    printf("Port     : %d\n", UrlData.iport);
    printf("Path     : %s\n", UrlData.szPath);

	PrintAndFreeParamList(UrlData.pParamList);

    return 0;
}
