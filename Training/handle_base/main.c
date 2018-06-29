#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "snapshot.h"
#include "http.h"


SCODE main(int argc, char **argv)
{
	HANDLE hSnapshot;
	HANDLE hHttp;

	if (argc != 5)
	{
		printf("Usage: %s [Camera IP] [Port] [Snapshot number] [Snapshot interval]\n", argv[0]);
		return S_FAIL;
	}
	pthread_t tSnapshotThread, tHttpThread;
	pthread_mutex_t tMutex = PTHREAD_MUTEX_INITIALIZER;

	Snapshot_Initial(&hSnapshot, argv, &tMutex);
	Http_Initial(&hHttp, argv, &tMutex);

	pthread_create(&tSnapshotThread, NULL, (void*)Snapshot_Run, hSnapshot);
	pthread_create(&tHttpThread, NULL, (void*)Http_Run, hHttp);

	pthread_join(tSnapshotThread, NULL);
	pthread_join(tHttpThread, NULL);

	Snapshot_Release(&hSnapshot);
	Http_Release(&hHttp);

	return S_OK;
}
