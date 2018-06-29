#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "common.h"
#include "http_local.h"

char *httpHeader[] = 
{
	"",
	"HTTP/1.1 200 OK\r\nContent-type: text/html\r\nContent-Length: ", 
	"HTTP/1.1 200 OK\r\nContent-type: image/jpeg\r\nContent-Length: "
};

SCODE Http_ParseRecvCmd(SOCKET sck, THttpInfo *ptHttpInfo)
{
	char acRecvBuffer[BUFF_SIZE] = {0};
	int iRecvSize = 0;
	char *pcStart = NULL;
	char *pcEnd = NULL;
	
	if ((iRecvSize = recv(sck, acRecvBuffer, BUFF_SIZE, 0)) < 0) 
	{
		printf("[%s] Failed to receive additional bytes from client\n", __FUNCTION__);
		return S_FAIL;
	}
	
	// Parse the request command
	pcStart = strstr(acRecvBuffer, "GET");
	pcEnd = strstr(acRecvBuffer, " HTTP");
	pcStart += 5;
	memset(ptHttpInfo->acFilePath, 0, sizeof(ptHttpInfo->acFilePath));
	strncpy(ptHttpInfo->acFilePath, DIR, sizeof(ptHttpInfo->acFilePath) - 1);
	strncat(ptHttpInfo->acFilePath, pcStart, pcEnd - pcStart);
	
	// No specific name means default home page
	if (strcmp(ptHttpInfo->acFilePath, DIR) == 0)
	{
		strncat(ptHttpInfo->acFilePath, HOMEPAGE, strlen(HOMEPAGE));
	}
	
	printf("[%s] Recv: acFilePath = %s\n", __FUNCTION__, ptHttpInfo->acFilePath);
	return S_OK;
}

SCODE Http_CheckFileType(THttpInfo *ptHttpInfo)
{
	// Check if the file exists
	if (access(ptHttpInfo->acFilePath, R_OK) != 0)
	{
		printf("[%s] Failed to access the file\n", __FUNCTION__);
		return S_FAIL;
	}

	// Check file type
	if (strstr(ptHttpInfo->acFilePath, ".html") != NULL)
	{
		ptHttpInfo->iFileType = FileType_HTML;
	}
	else if (strstr(ptHttpInfo->acFilePath, ".jpg") != NULL || strstr(ptHttpInfo->acFilePath, ".jpeg") != NULL)
	{
		ptHttpInfo->iFileType = FileType_JPEG;
	}
	else
	{
		ptHttpInfo->iFileType = FileType_Invalid;
		printf("[%s] Invalid file type\n", __FUNCTION__);
		return S_FAIL;
	}
	
	printf("[%s] File type = %d\n", __FUNCTION__, ptHttpInfo->iFileType);
	return S_OK;
}

int Http_CheckFileSize(FILE *pFile)
{
	int iSize = 0;

	fseek(pFile, 0, SEEK_END);
	iSize = ftell(pFile);
	rewind(pFile);
	
	return iSize;
}

void Http_ShowWebpage(SOCKET sck, THttpInfo *ptHttpInfo)
{
	FILE *pFile = NULL;
	char acHeader[HEADER_SIZE];
	char *pcBuffer = NULL;
	int iHeaderSize = 0;
	int iBufferSize = 0;
	
	// mutex lock
	pthread_mutex_lock(ptHttpInfo->ptMutex);
	
	// Open Content and check the size
	pFile = fopen(ptHttpInfo->acFilePath, "rb");
	ptHttpInfo->iFileSize = Http_CheckFileSize(pFile);
	printf("[%s] iFileSize = %d\n", __FUNCTION__, ptHttpInfo->iFileSize);
	
	// HTTP Header
	snprintf(acHeader, sizeof(acHeader), "%s%d\r\n\r\n", httpHeader[ptHttpInfo->iFileType], ptHttpInfo->iFileSize);
	iHeaderSize = strlen(acHeader);
	printf("[%s] iHeaderSize = %d\n", __FUNCTION__, iHeaderSize);

	// Fill Header & Content into the buffer
	iBufferSize = iHeaderSize + ptHttpInfo->iFileSize;
	printf("[%s] iBufferSize = %d\n", __FUNCTION__, iBufferSize);
	
	pcBuffer = (char *) malloc(iBufferSize);
	memset(pcBuffer, 0, iBufferSize);
	memcpy(pcBuffer, acHeader, iHeaderSize);	// Fill header
	fread(pcBuffer + iHeaderSize, sizeof(char), ptHttpInfo->iFileSize, pFile);	// Fill content
	fclose(pFile);

	// mutex unlock
	pthread_mutex_unlock(ptHttpInfo->ptMutex);
	
	//printf("++++++++++++++++++++++++++++++++++++++++++++++\n");
	//printf("%s\n", pcBuffer);
	//printf("++++++++++++++++++++++++++++++++++++++++++++++\n");
	
	// Send to Client
	if (send(sck, pcBuffer, iBufferSize, 0) != iBufferSize) 
	{
		printf("[%s] Failed to send data to client\n", __FUNCTION__);
	}
	
	sleep(1);
	
	free(pcBuffer);
	close(sck);

	printf("[%s] Send data to client!!!\n", __FUNCTION__);
	return;
}

SCODE Http_Init(HANDLE *phObject, pthread_mutex_t *ptMutex, int iHttpPort)
{
	THttpInfo *ptHttpInfo = malloc(sizeof(THttpInfo));
	
	memset(ptHttpInfo, 0, sizeof(ptHttpInfo));
	ptHttpInfo->iHttpPort = iHttpPort;
	ptHttpInfo->ptMutex = ptMutex;
	
	*phObject = ptHttpInfo;
	return S_OK;
}

HANDLE Http_Run(HANDLE hObject)
{
	SOCKET sckServer, sckClient;
	unsigned int uiServerAddrLen, uiClientAddrLen;
	struct sockaddr_in tServerAddr, tClientAddr;
	THttpInfo *ptHttpInfo = (THttpInfo *) hObject;
	
	// Create a TCP socket
	if ((sckServer = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("[%s] Failed to create socket\n", __FUNCTION__);
		pthread_exit(NULL);
	}
	
	// Naming & address socket
	tServerAddr.sin_family = AF_INET;
	tServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	tServerAddr.sin_port = htons(ptHttpInfo->iHttpPort);
	uiServerAddrLen = sizeof(tServerAddr);

	// Bind the server socket
	if ((bind(sckServer, (struct sockaddr *) &tServerAddr, uiServerAddrLen)) < 0)
	{
		printf("[%s] Failed to bind the server socket\n", __FUNCTION__);
		pthread_exit(NULL);
	}
	
	// Listen on the server socket
	if ((listen(sckServer, 5)) < 0)
	{
		printf("[%s] Failed to listen on server socket\n", __FUNCTION__);
		pthread_exit(NULL);
	}
	
	int i = 0;
	while (1)
	{
		printf("[%s] Server is waiting...%d...\n", __FUNCTION__, i++);
		
		// Accept connection
		uiClientAddrLen = sizeof(tClientAddr);
		if ((sckClient = accept(sckServer, (struct sockaddr *) &tClientAddr, &uiClientAddrLen)) < 0)
		{
			printf("[%s] Failed to accept client connection\n", __FUNCTION__);
			pthread_exit(NULL);
		}
		
		// Parse received command from client
		if (Http_ParseRecvCmd(sckClient, ptHttpInfo) == S_FAIL)
		{
			close(sckClient);
			continue;
		}
		
		// Check if the file exists & file type
		if (Http_CheckFileType(ptHttpInfo) == S_FAIL)
		{
			close(sckClient);
			continue;
		}
		
		// Send HTTP header & HTML page to client
		Http_ShowWebpage(sckClient, ptHttpInfo);
	}
	
	close(sckServer);
	
	pthread_exit(NULL);
}
