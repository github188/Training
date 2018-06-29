#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include "header.h"
#include <signal.h>

int iServerSocket;

void CloseSocket(iServerSocket)
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
	int iReceived, iRead;
	int iFd;
	unsigned int uiClientAddrLen;
	fd_set tReadFds, tTempFds;

	//建立socket
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

	//Bind to address
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

	FD_ZERO(&tReadFds);
	FD_SET(iServerSocket, &tReadFds);


	while(1)
	{
		tTempFds = tReadFds;
		if (select(FD_SETSIZE, &tTempFds, NULL, NULL, NULL) < 1)
		{
			printf("Fail to select or timeout\n");
			return FAIL;
		}

		for (iFd = 0; iFd < FD_SETSIZE; ++iFd)
		{
			if (FD_ISSET(iFd, &tTempFds))
			{
				if (iFd == iServerSocket)
				{
					uiClientAddrLen = sizeof(iClientSocket);
					if ((iClientSocket = accept(iServerSocket, (struct sockaddr*)&tClientAddr, &uiClientAddrLen)) < 0)
					{
						printf("[Server] Failed to accept client connection\n");
						return FAIL;
					}
					FD_SET(iClientSocket, &tReadFds);
					printf("Add client fd %d\n", iClientSocket);

				}
				else
				{
					ioctl(iFd, FIONREAD, &iRead);
					if (iRead == 0)
					{
						close(iFd);
						FD_CLR(iFd, &tReadFds);
						printf("Remove client fd %d\n", iFd);
					}
					else
					{

						iReceived = recv(iClientSocket, acBuffer, BUFFERSIZE, 0);
						if (iReceived  < 0)
						{
							printf("[Server] Failed to receive data from client\n");
							return FAIL;
						}
						else if (iReceived > 0)
						{
							acBuffer[iReceived] = '\0';
							printf("[Server] Receive data: %s\n", acBuffer);
						 	if (send(iClientSocket, acReply, BUFFERSIZE, 0) < 0)
							{
								printf("[Server] Fail to reply\n");
								return FAIL;
							}

						}
	    				memset(acBuffer, 0, BUFFERSIZE);
					}
				}
			}
		}


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
