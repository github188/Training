#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "header.h"
#define SERVER_NAME "checkip.dyndns.org"
#define SERVER_PORT 80


int main(int argc, char *argv[])
{
	int iSocket;
	struct hostent *THost;
	struct sockaddr_in tServAddr;
	char acBuffer[BUFFERSIZE] = {0};
	char acInput[BUFFERSIZE] = "GET / HTTP/1.1\r\n\r\n";
	char acOutput[BUFFERSIZE] = {0};
	int iReceived;

	if ((iSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("[Client] Failed to create socket\n");
		return FAIL;
	}

	if ((THost = gethostbyname(SERVER_NAME)) == NULL)
	{
		printf("No such host\n");
		return FAIL;
	}

	bzero((char *) &tServAddr, sizeof(tServAddr));
	tServAddr.sin_family = AF_INET;
	//tServAddr.sin_addr.s_addr = inet_addr(SERVER_NAME);
	tServAddr.sin_port = htons(SERVER_PORT);
	tServAddr.sin_addr = *((struct in_addr *)THost->h_addr);

	if (connect(iSocket, (struct sockaddr*)& tServAddr, sizeof(tServAddr)) < 0)
	{
		printf("[Client] Fail to connect\n");
		return FAIL;
	}

	if (send(iSocket, acInput, strlen(acInput), 0) < 0)
	{
		printf("[Client] Fail to send data\n");
		return FAIL;
	}


	iReceived = recv(iSocket, acBuffer, BUFFERSIZE, 0);
	if (iReceived  < 0)
	{
		printf("[Client] Failed to receive data from server\n");
		return FAIL;
	}
	else if (iReceived > 0)
	{
		sscanf(acBuffer, "%*[^<] <%*[^:] :%[^<]", acOutput);
		//printf("[Client] Receive reply: %s from server\n", acBuffer);
		printf("Current IP Address: %s\n", acOutput);
	}

	close(iSocket);

	return SUCCESS;
}
