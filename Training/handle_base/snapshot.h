#ifndef _SNAPSHOT_H_
#define _SNAPSHOT_H_

#define MAX_LENGTH 1000
#define TEMP "temp.jpg"
#define FOLDER "snapshot/"
#define JPG_CGI "/cgi-bin/viewer/video.jpg"
#define JPG_PREFIX "video"
#define JPG_SUFFIX ".jpg"


typedef struct snapshotd
{
	char szSanpshotURL[MAX_LENGTH];
	int iSnapshotNumber;
	pthread_mutex_t *ptMutex;

} TSnapshotInfo;

SCODE Snapshot_Initial(HANDLE *phSnapshot, char **pcArgv, pthread_mutex_t *ptMutex);
SCODE Snapshot_Release(HANDLE *phSnapshot);
SCODE Snapshot_Run(HANDLE phSnapshot);

#endif
