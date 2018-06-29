#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common.h"
#include "http.h"
#include <signal.h>


SCODE iServerSocket;

void CloseSocket(int iServerSocket)
{
	close(iServerSocket);
	printf("Close\n");
	exit(EXIT_SUCCESS);
}

SCODE Http_Initial(HANDLE *phHttp, char **pcArgv, pthread_mutex_t *ptMutex)
{

	THttpInfo *ptHttpInfo = (THttpInfo*)malloc(sizeof(THttpInfo));
	if (ptHttpInfo == NULL)
	{
		printf("malloc error\n");
		return S_FAIL;
	}

	memset(ptHttpInfo, 0, sizeof(ptHttpInfo));
	strcpy(ptHttpInfo -> szport, pcArgv[2]);
	ptHttpInfo -> ptMutex = ptMutex;

	*phHttp = ptHttpInfo;

	return S_OK;

}

SCODE Http_Release(HANDLE *phHttp)
{

	if (!phHttp || !*phHttp)
	{
		printf("Fail when release\n");
		return S_FAIL;
	}
	THttpInfo *pThis = (THttpInfo*) *phHttp;
	free(pThis);
	pThis = NULL;
	*phHttp = NULL;

	return S_OK;
}


SCODE Http_Send(THttpInfo *ptHttpInfo, SCODE iClientSocket)
{
	FILE *fp;
	char szHeader[MAX_LENGTH];
	unsigned int uiTotalSize, uiTemp;
	char *pcTotalBuffer;

	pthread_mutex_lock(ptHttpInfo -> ptMutex);

	if ((fp = fopen(ptHttpInfo -> szPath, "rb")) == NULL)
	{
		printf("Open file error\n");
		pthread_mutex_unlock(ptHttpInfo -> ptMutex);
		close(iClientSocket);
		return S_FAIL;
	}

	fseek(fp, 0, SEEK_END);
	ptHttpInfo -> uiFileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	sprintf(szHeader, "%s%d\r\n\r\n", JPG_HEADER_OK, ptHttpInfo -> uiFileSize);

	uiTotalSize = strlen(szHeader) + ptHttpInfo -> uiFileSize;
	pcTotalBuffer = (char *) malloc(sizeof(char) * uiTotalSize);
	memset(pcTotalBuffer, 0, uiTotalSize);
	memcpy(pcTotalBuffer, szHeader, strlen(szHeader));
	uiTemp = fread(pcTotalBuffer + strlen(szHeader), sizeof(char), ptHttpInfo -> uiFileSize, fp);
	if (uiTemp != ptHttpInfo -> uiFileSize)
	{
		printf("Error\n");
		close(iClientSocket);
		pthread_mutex_unlock(ptHttpInfo -> ptMutex);
		return S_FAIL;
	}
	fclose(fp);

	pthread_mutex_unlock(ptHttpInfo -> ptMutex);

	if (send(iClientSocket, pcTotalBuffer, uiTotalSize, 0) < 0)
	{
		printf("[Server] Fail to send to client\n");
		close(iClientSocket);
		return S_FAIL;
	}
	sleep(1);

	free(pcTotalBuffer);
	close(iClientSocket);
	printf("==Send success!!!!==\n");
	return S_OK;
}

SCODE Http_Run(HANDLE hHttp)
{

	THttpInfo *ptHttpInfo;
	SCODE iClientSocket;
	struct sockaddr_in tServerAddr, tClientAddr;
	unsigned int uiClientAddrLen;
	int iReceived;
	char acBuffer[BUFFERSIZE];
	char *pcStart, *pcEnd;


	struct sigaction TSigHandler;
	TSigHandler.sa_handler = CloseSocket;
	sigemptyset(&TSigHandler.sa_mask);
	TSigHandler.sa_flags = 0;
	sigaction(SIGINT, &TSigHandler, NULL);


	if (hHttp == NULL)
	{
		printf("Run Error\n");
		return S_FAIL;
	}
	ptHttpInfo = (THttpInfo*) hHttp;

	if ((iServerSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("[Server] creat socket error\n");
		return S_FAIL;
	}

	bzero(&tServerAddr, sizeof(tServerAddr));
	tServerAddr.sin_family = AF_INET;
	tServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	tServerAddr.sin_port = htons(atoi(ptHttpInfo -> szport));

	if (bind(iServerSocket, (struct sockaddr*) &tServerAddr, sizeof(tServerAddr)) < 0)
	{
		printf("[Server] Fail to bind\n");
		close(iServerSocket);
		return S_FAIL;
	}

	if (listen(iServerSocket, 5) < 0)
	{
		printf("[Server] Fail to listen\n");
		close(iServerSocket);
		return S_FAIL;
	}

	while(1)
	{
		uiClientAddrLen = sizeof(tClientAddr);
		if ((iClientSocket = accept(iServerSocket, (struct sockaddr*)&tClientAddr, &uiClientAddrLen)) < 0)
		{
			 printf("[Server] Fail to accept client connection\n");
			 close(iClientSocket);
			 close(iServerSocket);
			 return S_FAIL;
		}

		if (iReceived = recv(iClientSocket, acBuffer, BUFFERSIZE, 0) < 0)
		{
			printf("[Server] Fail to receive data\n");
			close(iClientSocket);
			close(iServerSocket);
			return S_FAIL;
		}

		acBuffer[iReceived] = '\0';
		//=====================================================================

		pcStart = strstr(acBuffer, "GET");
		pcEnd = strstr(acBuffer, " HTTP");
		pcStart += 5;

		memset(ptHttpInfo -> szPath, 0, sizeof(ptHttpInfo -> szPath));
		strcpy(ptHttpInfo -> szPath, FOLDER);
		strncat(ptHttpInfo -> szPath, pcStart, pcEnd - pcStart);

		printf("==%s==\n", ptHttpInfo -> szPath);

		//======================================================================

		if (Http_Send(ptHttpInfo, iClientSocket) < 0)
		{
			close(iServerSocket);
			return S_FAIL;
		}

		close(iClientSocket);

	}

	close(iServerSocket);

	return S_OK;
}
