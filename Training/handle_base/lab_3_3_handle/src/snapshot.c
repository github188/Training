#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "snapshot_local.h"

SCODE Snapshot_Init(HANDLE *phObject, pthread_mutex_t *ptMutex, int iSnapshotInterval, int iSnapshotNumber)
{
	TSnapshotInfo *ptSnapshotInfo = malloc(sizeof(TSnapshotInfo));
	
	memset(ptSnapshotInfo, 0, sizeof(ptSnapshotInfo));
	strncpy(ptSnapshotInfo->acSnapshotURL, SNAPSHOT_URL, sizeof(ptSnapshotInfo->acSnapshotURL) - 1);
	ptSnapshotInfo->iSnapshotInterval = iSnapshotInterval;
	ptSnapshotInfo->iSnapshotNumber = iSnapshotNumber;
	ptSnapshotInfo->ptMutex = ptMutex;
	
	*phObject = ptSnapshotInfo;
	return S_OK;
}

HANDLE Snapshot_Run(HANDLE hObject)
{
	char szCgiCmd[CMD_LEN];
	char szOldName[FILENAME_LEN];
	char szNewName[FILENAME_LEN];
	TSnapshotInfo *ptSnapshotInfo = (TSnapshotInfo *) hObject;
	
	int i = 0, j = 0;
	
	while(1)
	{
		snprintf(szCgiCmd, sizeof(szCgiCmd), "wget -q %s -O %s%s", ptSnapshotInfo->acSnapshotURL, DIR, TEMP_JPG);
		printf("[%s] szCgiCmd = %s\n", __FUNCTION__, szCgiCmd);
		system(szCgiCmd);
		
		if (i < (ptSnapshotInfo->iSnapshotNumber))
		{
			snprintf(szOldName, sizeof(szOldName), "%s%s", DIR, TEMP_JPG);
			snprintf(szNewName, sizeof(szNewName), "%s%s%d%s", DIR, PREFIX_NAME, i+1, SUFFIX_NAME);
			
			// mutex lock
			pthread_mutex_lock(ptSnapshotInfo->ptMutex);
			rename(szOldName, szNewName);
			printf("[%s] %s --> %s\n", __FUNCTION__, szOldName, szNewName);
			// mutex unlock
			pthread_mutex_unlock(ptSnapshotInfo->ptMutex);
			i++;
		}
		// Replace the older one
		else
		{
			// mutex lock
			pthread_mutex_lock(ptSnapshotInfo->ptMutex);
		
			for (j = (ptSnapshotInfo->iSnapshotNumber); j > 1; j--)
			{
				snprintf(szOldName, sizeof(szOldName), "%s%s%d%s", DIR, PREFIX_NAME, j-1, SUFFIX_NAME);
				snprintf(szNewName, sizeof(szNewName), "%s%s%d%s", DIR, PREFIX_NAME, j, SUFFIX_NAME);
				rename(szOldName, szNewName);
				printf("[%s] %s --> %s\n", __FUNCTION__, szOldName, szNewName);
			}
			snprintf(szOldName, sizeof(szOldName), "%s%s", DIR, TEMP_JPG);
			snprintf(szNewName, sizeof(szNewName), "%s%s%d%s", DIR, PREFIX_NAME, 1, SUFFIX_NAME);
			rename(szOldName, szNewName);
			printf("[%s] %s --> %s\n", __FUNCTION__, szOldName, szNewName);
			
			// mutex unlock
			pthread_mutex_unlock(ptSnapshotInfo->ptMutex);
		}
		
		sleep(ptSnapshotInfo->iSnapshotInterval);
	}
	
	pthread_exit(NULL);
}
