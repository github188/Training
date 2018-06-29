#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "common.h"
#include "snapshot.h"
#include "http.h"

int main(int argc, char *argv[])
{
	pthread_t tSnapshotThread;
	pthread_t tHttpThread;
	pthread_mutex_t tMutex;
	HANDLE hSnapshotInfo;
	HANDLE hHttpInfo;
	
	// Check the command line
	if (argc != 4) 
	{
		printf("The program can get snpahsots from camera and serve HTTP request to show the snapshots.\n");
		printf("Usage: %s <server port> <snapshot interval> <snapshot number>\n", argv[0]);
		return S_FAIL;
	}
	
	// Check options
	if (IsValidPort(argv[1]) != S_OK || IsInteger(argv[2]) != S_OK || IsInteger(argv[3]) != S_OK)
	{
		return S_FAIL;
	}
	
	pthread_mutex_init(&tMutex, NULL);
	
	// Init Snapshot & Http
	Snapshot_Init(&hSnapshotInfo, &tMutex, atoi(argv[2]), atoi(argv[3]));
	Http_Init(&hHttpInfo, &tMutex, atoi(argv[1]));
	
	// Run Snapshot & Http
	pthread_create(&tSnapshotThread, NULL, Snapshot_Run, hSnapshotInfo);
	pthread_create(&tHttpThread, NULL, Http_Run, hHttpInfo);
	
	// Wait for Snapshot & Http finish
	pthread_join(tSnapshotThread, NULL);
	pthread_join(tHttpThread, NULL);
	
	pthread_mutex_destroy(&tMutex);
	
	return S_OK;
}

