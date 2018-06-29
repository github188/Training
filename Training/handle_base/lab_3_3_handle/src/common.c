#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h"

SCODE IsValidPort(char *pcHttpPort)
{
	int iHttpPort;
	
	if (!pcHttpPort)
	{
		printf("NULL\n");
		return S_FAIL;
	}
	
	iHttpPort = atoi(pcHttpPort);
	
	if (iHttpPort < 1024 || iHttpPort > 65535)
	{
		printf("Port range: 1024 ~ 65535\n");
		return S_FAIL;
	}
	
	return S_OK;
}

SCODE IsInteger(char *pcString)
{
	char *pcTemp;
	
	if (!pcString)
	{
		printf("NULL\n");
		return S_FAIL;
	}
	
	pcTemp = pcString;
	
	while(*pcString != '\0')
	{
		if (!isdigit(*pcString))
		{
			printf("%s is not an integer.\n", pcTemp);
			return S_FAIL;
		}
		pcString++;
	}
	
	return S_OK;
}
