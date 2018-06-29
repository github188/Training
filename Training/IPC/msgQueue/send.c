/*
** kirk.c -- writes to a message queue
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf {
	long mtype;
	char mtext[200];
};

struct pair{
	int a;
	int b;
};

struct yoyo{
	long mtype;
	struct pair mypair;
};

int main(void)
{
	struct my_msgbuf buf;
	struct yoyo yoyobuf;

	int msqid;
	key_t key = 56;
    if ((key = ftok("msgQQ", 'B')) == -1) {
		perror("ftok");
		exit(1);
	}

	if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1) {
		perror("msgget");
		exit(1);
	}

	printf("Enter lines of text, ^D to quit:\n");

	buf.mtype = 1; /* we don't really care in this case */
	yoyobuf.mtype = 5566;
	yoyobuf.mypair.a = 11;
	yoyobuf.mypair.b = 22;

	while(scanf("%s", buf.mtext)) {
		int len = strlen(buf.mtext);
		printf("len: %d\n", len);
		/* ditch newline at end, if it exists */
		//if (buf.mtext[len-1] == '\n') buf.mtext[len-1] = '\0';
		if (msgsnd(msqid, &yoyobuf, sizeof(struct yoyo), 0) == -1)
			perror("msgend");
		if (msgsnd(msqid, &buf, len+1, 0) == -1) /* +1 for '\0' */
			perror("msgsnd");
	}

	if (msgctl(msqid, IPC_RMID, NULL) == -1) {
		perror("msgctl");
		exit(1);
	}

	return 0;
}
