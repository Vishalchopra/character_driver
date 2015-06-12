#include<stdio.h>
#include<pthread.h>
#include<fcntl.h>

void *read_fun(void *);
pthread_t thrd[4];
pthread_attr_t tattr;

int thread_no;

int main(int argc, char *argv[])
{
	int lv, ret, fd;
	thread_no = 0;

	ret = pthread_attr_init(&tattr);
	pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
	if(ret == -1)
	{
		perror("pthread_attr_int");
		goto OUT;
	}
	fd = open(argv[1], O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		goto OUT;
	}
	//while(1)
	//{
		for(lv < 0; lv < 3; lv++)
		{
			pthread_create(&thrd[lv], &tattr, read_fun, (void *)&fd);
			sleep(1);
		}	
		sleep(10);
//	}
	return 0;
OUT:
	return -1;
}

void *read_fun(void *var)
{
	int fd;
	char ch[1024];
	fd = *(int*)var;
	printf("%d\n", thread_no);
	read(fd , ch, 10);
	printf("%s in thread_no = %d\n", ch, thread_no);
	thread_no++;
}
