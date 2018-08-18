#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <sched.h>

#include <stdio.h>
#include <setjmp.h>

#define TRY do{ jmp_buf ex_buf__; if( !setjmp(ex_buf__) ){
#define CATCH } else {
#define ETRY } }while(0)
#define THROW longjmp(ex_buf__, 1)

// pthread_self
// pthread_equal
// pthread_create
// pthread_join
//  pthread_exit(NULL);

// void* doSomeThing(void *arg) {
//   cpu_set_t my_set;
//   CPU_ZERO(&my_set);
//   CPU_SET(3, &my_set);
//   sched_setaffinity(0, sizeof(cpu_set_t), &my_set);
//   int *param = (int *)arg;
//   int first_val = param[0];
//   printf("%d\n", first_val);
//   int cpu = sched_getcpu();
//   printf("CPU: %d\n", cpu);
//   sleep(5);
//   return NULL;
// }

int main(void) {
  // char cwd[1024];
  // chdir("/path/to/change/directory/to");
  // fclose(stdout);
  // fclose(stdout);
  // stdout = fmemopen(buf, sizeof(buf), "w");
  // int stdin_copy = dup(0);
  // int stdout_copy = dup(1);
  // fclose(stdout);
  // fclose(stderr);
  // FILE *stream;
  // stream = freopen("temp","w",stdout);
  // int save_in, save_out;
  // save_in = dup(STDERR_FILENO);
  // save_out = dup(STDOUT_FILENO);
  // int status = system("gcc -Wall test/dump/helloError.c -o .data/prueba");
  // if( WIFEXITED( status ) ) {
  //   printf( "[system-test] print-argv exit status: %d\n", WEXITSTATUS( status ) );
  // } else {
  //   fprintf( stderr, "[system-test] print-argv wasn't executed\n" );
  // }
  // dup2(save_in, STDERR_FILENO);
  // dup2(save_out, STDOUT_FILENO);
  // dup2(stdin_copy, 0);
  // dup2(stdout_copy, 1);
  // close(stdin_copy);
  // close(stdout_copy);
  // printf("Estado: %d\n", status);
  // getcwd(cwd, sizeof(cwd));
  // printf("Current working dir: %s\n", cwd);
  // gcc -Wall test/dump/hello.c -o bin/prueba
  // test/dump/hello.c
  // FILE *ls = popen("ls -mesf 2>./.data/errFile", "r");

  FILE *ls = popen("gcc -Wall test/dump/helloError.c -o .data/prueba", "r"); //  2>./.data/errors/errFileA
  // FILE *ls = freopen("ls","r",stdout);
  if (ls == NULL) {
    printf("Ocurrio un error\n");
  }
  char buf[256];
  while (fgets(buf, sizeof(buf), ls) != 0) {
    printf("%s\n", buf);
  }
  pclose(ls);

  // TRY {
  //    printf("In Try Statement\n");
  //    // THROW;
  //    printf("I do not appear\n");
  // } CATCH {
  //    printf("Got Exception!\n");
  // }
  // ETRY;
  //
  // return 0;
    // pthread_t hilo1, hilo2;
    // int *param = (int *)malloc(2 * sizeof(int));
    // param[0] = 123;
    // param[1] = 456;
    // char *archivo = "hello";
    // int status = pthread_create(&hilo1, NULL, doSomeThing, param);
    // int status2 = pthread_create(&hilo2, NULL, doSomeThing, param);
    // // printf("%d\n", status);
    // // int i = 0;
    // // int err;
    // //
    // // while(i < 2)
    // // {
    // //     err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
    // //     if (err != 0)
    // //         printf("\ncan't create thread :[%s]", strerror(err));
    // //     else
    // //         printf("\n Thread created successfully\n");
    // //
    // //     i++;
    // // }
    // pthread_join(hilo1, NULL);
    // pthread_join(hilo2, NULL);
    // return EXIT_SUCCESS; // EXIT_FAILURE EXIT_SUCCESS
}
