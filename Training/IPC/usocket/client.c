#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct pair{
	int a;
	int b;
};
//char *socket_path = "./socket";
char *socket_path = "baba";

int main(int argc, char *argv[]) {
  struct sockaddr_un addr;
  char buf[100];
  int fd,rc;

  if (argc > 1) socket_path=argv[1];

  if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(-1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);

  if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("connect error");
    exit(-1);
  }

  struct pair *mypair;
  mypair->a = 11;
  mypair->b = 22;
  int size = sizeof(struct pair);
  if( write(fd, mypair, size) > 0)
  {
	  printf("send OK\n");
  }
	/*
  while( (rc=read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
    if (write(fd, buf, rc) != rc) {
      if (rc > 0) fprintf(stderr,"partial write");
      else {
        perror("write error");
        exit(-1);
      }
    }
	char recvkerker[100] = {0};
	read(fd, recvkerker, sizeof(recvkerker));
	printf("%s\n", recvkerker);

  }*/

  return 0;
}
