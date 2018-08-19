#define _GNU_SOURCE
#define _BSD_SOURCE         /* See feature_test_macros(7) */
#include <stdlib.h>
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <setjmp.h>
#include <fcntl.h>

pthread_t tid[2];

// pthread_t tid[2];
// int counter;

// void contador () {
// 	printf("%s\n", a);
// }

void* trythis(void *arg)
{
	int *param = (int *)arg;
	int sock = param[0];
    unsigned long i = 0;
		int contador = 1;
		// printf("Hilo creado:\n");
		if (sock == 0) {
			sleep(2);
		}
		printf("Hilo %d, Contador%d\n", sock, contador);
		contador++;
		// sleep(2);
		// int counter = 1;
    // counter += 1;
    // printf("\n Job %d has started\n", counter);

    // for(i=0; i<(0xFFFFFFFF);i++);
    // printf("\n Job %d has finished\n", counter);

    return NULL;
}

int main(void)
{
    int i = 0;
    int error;

    while(i < 2)
    {
				int *param = (int *)malloc(2 * sizeof(int));
				param[0] = i;
				printf("%d\n", i);
        error = pthread_create(&(tid[i]), NULL, &trythis, param);
        if (error != 0)
            printf("\nThread can't be created : [%s]", strerror(error));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    return 0;
}
