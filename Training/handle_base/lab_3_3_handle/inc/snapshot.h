#ifndef _SNAPSHOT_H_
#define _SNAPSHOT_H_

SCODE Snapshot_Init(HANDLE *phObject, pthread_mutex_t *ptMutex, int iSnapshotInterval, int iSnapshotNumber);
HANDLE Snapshot_Run(HANDLE hObject);

#endif
