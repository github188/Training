#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
	FILE *file;
	float sum = 0.0;
	float mean = 0.0;
	int count = 0;
	char buf[500] = {0};
	char *flag;
	//file = fopen("wow.txt", "r");
	//file = fopen("socket.txt", "r");
	//file = fopen("onesystem.txt", "r");
	if (argc < 2)
	{
		printf("Argv[1]: File name\n");
		return 0;
	}
	file = fopen(argv[1], "r");
	while(fgets(buf, sizeof(buf), file))
	{
		//printf("%s\n", buf);
		flag = strchr(buf, 'T');
		flag += 14;
		//printf("%s\n", flag);
		sum += strtod(flag,NULL)/1000000;
		++count;
		memset(buf, 0, sizeof(buf));
	}
	printf("Total time is: %f\n", sum);
	mean = sum/count;
	printf("Average time is: %f\n", mean);
	fclose(file);
}
