#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "header.h"

int Tcp_Client(char *pcServerIP, char *pcServerPort, char *pcMsg)
{

	int iClientSocket;
	char acBuffer[BUFFERSIZE] = {0};
	struct sockaddr_in tSendAddr;
	int iReceived, iMsgLenth;

	//建立socket

	if ((iClientSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("[Client] Failed to create socket\n");
		return FAIL;
	}

    bzero((char *)&tSendAddr, sizeof(tSendAddr));
	tSendAddr.sin_family = AF_INET;
	tSendAddr.sin_addr.s_addr = inet_addr(pcServerIP);
	tSendAddr.sin_port = htons(atoi(pcServerPort));

	if (connect(iClientSocket, (struct sockaddr*)& tSendAddr, sizeof(tSendAddr)) < 0)
	{
		printf("[Client] Fail to connect\n");
		return FAIL;
	}

	printf("%d\n", iClientSocket);

	iMsgLenth = strlen(pcMsg);

	if (send(iClientSocket, pcMsg, iMsgLenth, 0) < 0)
	{
		printf("[Client] Fail to send data\n");
		return FAIL;
	}

	iReceived = recv(iClientSocket, acBuffer, BUFFERSIZE, 0);
		if (iReceived  < 0)
		{
			printf("[Client] Failed to receive data from server\n");
			return FAIL;
		}
		else if (iReceived > 0)
		{

			acBuffer[iReceived] = '\0';
			printf("[Client] Receive reply: %s from server\n", acBuffer);
		}
		close(iClientSocket);
		return SUCCESS;
}

int main(int argc, char *argv[])
{
	if (argc == 4)
	{
		Tcp_Client(argv[1], argv[2], argv[3]);
	}
	else
	{
		printf("Usage %s <ip> <port> <message>\n", argv[0]);
		return FAIL;
	}
	return SUCCESS;
}
