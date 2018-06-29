#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "header.h"
#include <signal.h>

int iServerSocket;

void CloseSocket(iServerSocket)   //Signal control (Ctrl + C)
{
	close(iServerSocket);
	printf("Close\n");
	exit(EXIT_SUCCESS);
}

int Tcp_Server(char *pcServerPort)
{
	int iClientSocket;
	char acBuffer[BUFFERSIZE] = {0};
	char acReply[BUFFERSIZE] = "OK!";
	struct sockaddr_in tServerAddr, tClientAddr;
	int iReceived;
	unsigned int uiClientAddrLen;

	// Creat socket

	if ((iServerSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("[Server] Failed to create socket\n");
		return FAIL;
	}

	//Set up address

	bzero((char *)&tServerAddr, sizeof(tServerAddr));
	tServerAddr.sin_family = AF_INET;
	tServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	tServerAddr.sin_port = htons(atoi(pcServerPort));

	// Bind to address

	if (bind(iServerSocket, (struct sockaddr *)&tServerAddr, sizeof(tServerAddr)) < 0)
	{
		printf("[Server] Failed to bind\n");
		return FAIL;
	}

	//Listen

	if (listen(iServerSocket, 5) < 0)
	{
		printf("[Server] Fail to listen\n");
		return FAIL;
	}

	while(1)
	{
		uiClientAddrLen = sizeof(tClientAddr);
		if ((iClientSocket = accept(iServerSocket, (struct sockaddr*)&tClientAddr, &uiClientAddrLen)) < 0)
		{
			printf("[Server] Failed to accept client connection\n");
			return FAIL;
		}
		printf("%d\n", iClientSocket);

		iReceived = recv(iClientSocket, acBuffer, BUFFERSIZE, 0);

		if (iReceived  < 0)
		{
			printf("[Server] Failed to receive data from client\n");
			return FAIL;
		}
		else if (iReceived > 0)
		{
			printf("[Server] Client connected: %s:%d\n", inet_ntoa(tClientAddr.sin_addr), ntohs(tClientAddr.sin_port));
			acBuffer[iReceived] = '\0';
			printf("[Server] Receive data: %s\n", acBuffer);
			if (send(iClientSocket, acReply, BUFFERSIZE, 0) < 0)
			{
				printf("[Server] Fail to reply\n");
				return FAIL;
			}

		}

		close(iClientSocket);
	    memset(acBuffer, 0, BUFFERSIZE);

	}

	close(iServerSocket);
	return SUCCESS;
}

int main(int argc, char *argv[])
{
	if (argc == 2)
	{
        struct sigaction TSigHandler;
		TSigHandler.sa_handler = CloseSocket;
		sigemptyset(&TSigHandler.sa_mask);
		TSigHandler.sa_flags = 0;
		sigaction(SIGINT, &TSigHandler, NULL);   //signal control (ctrl + c)
		Tcp_Server(argv[1]);
	}
	else
	{
		printf("Usage %s <port>\n", argv[0]);
		return FAIL;
	}
	return SUCCESS;
}
