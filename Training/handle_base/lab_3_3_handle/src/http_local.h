#ifndef _HTTP_LOCAL_H_
#define _HTTP_LOCAL_H_

#define DIR             "www/"
#define HOMEPAGE        "index.html"
#define BUFF_SIZE       1024
#define HEADER_SIZE     128
#define FILENAME_LEN    64

typedef struct
{
	int iHttpPort;
	char acFilePath[FILENAME_LEN];
	int iFileType;
	int iFileSize;
	pthread_mutex_t *ptMutex;
} THttpInfo;

typedef enum
{
	FileType_Invalid,
	FileType_HTML,
	FileType_JPEG,
} EFileType;

#endif
