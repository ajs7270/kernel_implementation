/* gcc -pthread -o result thread.c */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_OF_THREAD 2

void *tid_print(void *data)
{
	char *thread_name = (char*)data;
	pid_t pid = getpid();
	pthread_t tid = pthread_self();

	printf("[%s] pid : %u tid : %x \n",thread_name,(unsigned int)pid,(unsigned int)tid);
}

int main()
{
	int thr_id;
	int status;
	pthread_t p_thread[2];

	int i;
	for(i = 0;i < NUM_OF_THREAD; i++){
		thr_id = pthread_create(&p_thread[i],NULL,tid_print,(void*)"child_thread");

		if (thr_id < 0) {
			perror("thread create error : ");
			exit(0);
		}
	}

	tid_print((void*)"parent_thread");

	for (i = 0;i < NUM_OF_THREAD; i++){
		pthread_join(p_thread[i], (void **)&status);
	}

	return 0;
}

