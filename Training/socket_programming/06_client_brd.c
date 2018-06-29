#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "header.h"



int Udp_client(char *pcServerAddr, char *pcServerPort, char *pcMsg)
{
	int iClientSocket;
	unsigned int uiSendAddrLen, uiRecvAddrLen;
	struct sockaddr_in tSendAddr, tRecvAddr;
	char acBuffer[BUFFERSIZE] = {0};
	int iMsgLen, iReceived;
	int broadcast = 1;

	// Create the UDP socket
	if ((iClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		printf("[Client] Failed to create socket\n");
		return FAIL;
	}

	if (setsockopt(iClientSocket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
	{
		printf("[Server] Fail to set broadcast\n");
		return FAIL;
	}

	// Naming & address socket
	memset(&tSendAddr, 0, sizeof(tSendAddr));
	tSendAddr.sin_family = AF_INET;
	tSendAddr.sin_addr.s_addr = inet_addr(pcServerAddr);
	tSendAddr.sin_port = htons(atoi(pcServerPort));
	uiSendAddrLen = sizeof(tSendAddr);

	// Send the word to the server
	iMsgLen = strlen(pcMsg);
	if (sendto(iClientSocket, pcMsg, iMsgLen, 0, (struct sockaddr *) &tSendAddr, uiSendAddrLen) < 0)
	{
		printf("[Client] Could not send data\n");
		return FAIL;
	}

	printf("[Client] Send to %s:%d\n", inet_ntoa(tSendAddr.sin_addr), ntohs(tSendAddr.sin_port));

	// Receive the word back from the server
	uiRecvAddrLen = sizeof(tRecvAddr);
	if ((iReceived = recvfrom(iClientSocket, acBuffer, BUFFERSIZE, 0, (struct sockaddr *) &tRecvAddr, &uiRecvAddrLen)) < 0)
	{
		printf("[Client] Fail to receive from server\n");
		return FAIL;
	}

	printf("[Client] Recieve from %s:%d\n", inet_ntoa(tRecvAddr.sin_addr), ntohs(tRecvAddr.sin_port));


	acBuffer[iReceived] = '\0';	// Assure null-terminated string
	printf("[Client] Server reply: %s\n", acBuffer);

	close(iClientSocket);

	return SUCCESS;



}

int main(int argc, char *argv[])
{
	if (argc == 4)
	{
		// Client function
		Udp_client(argv[1], argv[2], argv[3]);
	}
	else
	{
		printf("Usage: %s <ip> <port> <message>\n", argv[0]);
		return FAIL;
	}

	return SUCCESS;
}

