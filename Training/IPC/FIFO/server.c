#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define MYPIPE "mypipe"

int main(void)
{
        int fd1;
        char buffer[100];  
        fd1=open(MYPIPE,O_RDONLY);  //open 
        read(fd1,buffer,100);       //read
        printf("%s\n",buffer);      
        close(fd1);                 //close
}