#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "snapshot.h"

SCODE Snapshot_Initial(HANDLE *phSnapshot, char **pcArgv, pthread_mutex_t *ptMutex)
{
	TSnapshotInfo *ptSnapshotInfo = (TSnapshotInfo*)malloc(sizeof(TSnapshotInfo));
	if (ptSnapshotInfo == NULL)
	{
		printf("malloc error\n");
		return S_FAIL;
	}
	memset(ptSnapshotInfo, 0, sizeof(ptSnapshotInfo));
	strcpy(ptSnapshotInfo -> szSanpshotURL, "http://");
	strcat(ptSnapshotInfo -> szSanpshotURL, pcArgv[1]);
	strcat(ptSnapshotInfo -> szSanpshotURL, JPG_CGI);

	ptSnapshotInfo -> iSnapshotNumber = atoi(pcArgv[3]);
	ptSnapshotInfo -> iSnapshotInterval = atoi(pcArgv[4]);
	ptSnapshotInfo -> ptMutex = ptMutex;

	*phSnapshot = ptSnapshotInfo;

	return S_OK;
}

SCODE Snapshot_Release(HANDLE *phSnapshot)
{
	if (!phSnapshot || !*phSnapshot)
	{
		printf("Fail when release\n");
		return S_FAIL;
	}
	TSnapshotInfo *pThis = (TSnapshotInfo*) *phSnapshot;
	free(pThis);
	pThis = NULL;
	*phSnapshot = NULL;

	return S_OK;
}

SCODE Snapshot_Run(HANDLE hSnapshot)
{
	TSnapshotInfo *ptSnapshotInfo;
	char szCgiCommand[MAX_LENGTH] = {0};
	char szOld[MAX_LENGTH] = {0};
	char szNew[MAX_LENGTH] = {0};
	int iIndex = 0;
	int i;

	if (hSnapshot == NULL)
	{
		printf("Run Error\n");
		return S_FAIL;
	}
	ptSnapshotInfo = (TSnapshotInfo*) hSnapshot;

	printf("Cgi: %s\n", ptSnapshotInfo -> szSanpshotURL);
	sprintf(szCgiCommand, "wget -q %s -O %s%s", ptSnapshotInfo -> szSanpshotURL, FOLDER, TEMP);

	while(1)
	{
		if (system(szCgiCommand) < 0)
		{
			printf("Cgi command error\n");
			return S_FAIL;
		}

		if (iIndex < ptSnapshotInfo -> iSnapshotNumber)
		{
			sprintf(szOld, "%s%s", FOLDER, TEMP);
			sprintf(szNew, "%s%s%d%s", FOLDER, JPG_PREFIX, iIndex+1, JPG_SUFFIX);
			pthread_mutex_lock(ptSnapshotInfo -> ptMutex);
			if (rename(szOld, szNew) != 0)
			{
				printf("Rename error\n");
				pthread_mutex_unlock(ptSnapshotInfo -> ptMutex);
				return S_FAIL;
			}
			pthread_mutex_unlock(ptSnapshotInfo -> ptMutex);
			printf("File %s is generated\n", szNew);
			++iIndex;
		}

		else
		{
			pthread_mutex_lock(ptSnapshotInfo -> ptMutex);
			for (i = 1; i < ptSnapshotInfo -> iSnapshotNumber; ++i)
			{
				sprintf(szOld,"%s%s%d%s", FOLDER, JPG_PREFIX, i+1, JPG_SUFFIX);
				sprintf(szNew, "%s%s%d%s", FOLDER, JPG_PREFIX, i, JPG_SUFFIX);
				printf("%s → %s\n", szOld, szNew);
				if (rename(szOld, szNew) != 0)
				{
					printf("Rename error\n");
					pthread_mutex_unlock(ptSnapshotInfo -> ptMutex);
					return S_FAIL;
				}
			}
			sprintf(szOld, "%s%s", FOLDER, TEMP);
			sprintf(szNew, "%s%s%d%s", FOLDER, JPG_PREFIX, i, JPG_SUFFIX);
			printf("Add the newest %s\n",szNew);
			if (rename(szOld, szNew) != 0)
			{
				printf("Rename error\n");
				pthread_mutex_unlock(ptSnapshotInfo -> ptMutex);
				return S_FAIL;
			}
			printf("Files are updated\n");
			pthread_mutex_unlock(ptSnapshotInfo -> ptMutex);
		}
		sleep(ptSnapshotInfo -> iSnapshotInterval);
	}

	return S_OK;

}
