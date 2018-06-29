#ifndef _HTTP_H_
#define _HTTP_H_

SCODE Http_Init(HANDLE *phObject, pthread_mutex_t *ptMutex, int iHttpPort);
HANDLE Http_Run(HANDLE hObject);

#endif
