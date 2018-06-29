#ifndef _HTTP_H_
#define _HTTP_H_

#define MAX_LENGTH 1000
#define TEMP "temp.jpg"
#define FOLDER "snapshot/"
#define BUFFERSIZE 1000
#define JPG_HEADER_OK "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nContent-Length: "


typedef struct
{
	char szport[6];
	char szPath[MAX_LENGTH];
	unsigned int uiFileSize;
	pthread_mutex_t *ptMutex;

} THttpInfo;

SCODE Http_Initial(HANDLE *phHttp, char **pcArgv, pthread_mutex_t *ptMutex);
SCODE Http_Release(HANDLE *phHttp);
SCODE Http_Run(HANDLE phHttp);

#endif
