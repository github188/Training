#ifndef _SNAPSHOT_LOCAL_H_
#define _SNAPSHOT_LOCAL_H_

#define SNAPSHOT_URL    "http://172.16.23.3/cgi-bin/viewer/video.jpg"
#define DIR             "www/"
#define PREFIX_NAME     "video"
#define SUFFIX_NAME     ".jpg"
#define TEMP_JPG        "temp.jpg"
#define CMD_LEN         256
#define URL_LEN         128
#define FILENAME_LEN    64

typedef struct
{
	char acSnapshotURL[URL_LEN];
	int iSnapshotInterval;
	int iSnapshotNumber;
	pthread_mutex_t *ptMutex;
} TSnapshotInfo;

#endif
