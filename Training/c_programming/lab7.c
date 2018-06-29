#include<stdio.h>
#define URL_LENGTH 1000
#define STRING_SIZE 100
void main()
{
	char aURL[URL_LENGTH] = {0}; //Good job! Never forget to initialize vairables
	char aIP[STRING_SIZE] = {0};
	char aPort[STRING_SIZE] = {0};
	char aURI[STRING_SIZE] = {0};
	char aParameter1[STRING_SIZE] = {0};
	char aParameter2[STRING_SIZE] = {0};
	printf("Enter input URL:\n");
	scanf("%s", aURL);
	sscanf(aURL, "%*[^//] %*[//] %[^:] :%[^/] /%[^?] ?%[^&] &%s", aIP, aPort, aURI, aParameter1, aParameter2);
	//kelly: 這樣的寫法感覺只能符合題目要求, 假設我稍為修改輸入方式
	//http://192.168.0.56/login.cgi?username=root
	//http://172.16.7.107/setup/system/parafile.html
	//這樣你的結果就不對了唷~
	printf("Input URL: %s\n", aURL);
	printf("IP: %s\n", aIP);
	printf("Port: %s\n", aPort);
	printf("URI: %s\n", aURI);
	printf("Parameter1: %s\n", aParameter1);
	printf("Parameter2: %s\n", aParameter2);

}
