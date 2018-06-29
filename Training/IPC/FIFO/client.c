

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

int main()
{
        int fd1;
		char buffer[100];
		scanf("%s", buffer);
		int len = strlen(buffer);
		buffer[len] = '\0';
        mknod("mypipe",0666, 0);           //build pipe
        fd1=open("mypipe", O_WRONLY);      //open
        write(fd1,buffer, len+1); //write
        close(fd1); //close
}



