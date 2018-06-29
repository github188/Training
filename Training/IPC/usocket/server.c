#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

struct pair{
	int a;
	int b;
};
//char *socket_path = "./socket";
char *socket_path = "baba";

int main(int argc, char *argv[]) {
  struct sockaddr_un addr;
  char buf[100];
  int fd,cl,rc;

  if (argc > 1) socket_path=argv[1];

  if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(-1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);

  unlink(socket_path);

  if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("bind error");
    exit(-1);
  }

  if (listen(fd, 5) == -1) {
    perror("listen error");
    exit(-1);
  }

  struct pair *mypair;

  while (1) {
    if ( (cl = accept(fd, NULL, NULL)) == -1) {
      perror("accept error");
      continue;
    }

    while ( (rc=read(cl,mypair,sizeof(struct pair))) > 0) {
      	printf("%d %d\n", mypair->a, mypair->b);
		//printf("read %u bytes: %.*s\n", rc, rc, mypair);
		char *kerker = "kerker";
		write(rc, kerker, sizeof(kerker));

    }
    if (rc == -1) {
      perror("read");
      exit(-1);
    }
    else if (rc == 0) {
      printf("EOF\n");
      close(cl);
    }

  }


  return 0;
}
