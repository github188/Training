#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef SCODE
typedef unsigned int SCODE;
#endif

#ifndef HANDLE
typedef void * HANDLE;
#endif

#ifndef SOCKET
typedef int SOCKET;
#endif

#ifndef S_OK
#define S_OK 0
#endif

#ifndef S_FAIL
#define S_FAIL (SCODE)(-1)
#endif

SCODE IsValidPort(char *pcHttpPort);
SCODE IsInteger(char *pcString);

#endif
