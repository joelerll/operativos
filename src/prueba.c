#define _GNU_SOURCE
#define _BSD_SOURCE         /* See feature_test_macros(7) */
     #include <stdlib.h>
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <setjmp.h>
#include <fcntl.h>
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

  // 1. Recibir el archivo en string y guardarlo
  // 2. Compliar el archivo y ver si dar error
  // 2.5 Si da error, retornar el error y decir que hubo error
  // 3. Si no hubo error, ejecutar el archivo
  // 4. Enviar lo que se ejecuto el archivo y decir que se ejecuto
  int fp = open("archivo.txt",  O_WRONLY | O_APPEND | O_CREAT,  0777);
  if (fp < 0) {
    printf("Error al crear el archivo\n");
    perror("Error");
  }
  write(fp, "Hola, como esta", strlen("Hola, como esta"));
  // FILE *ls = popen("gcc test/dump/hello.c -o .data/prueba 2>&1", "r"); //  2>./.data/errors/errFileA
  // // si esto de devuelve no vacio (quiere decir que hay error)
  // // FILE *ls = popen("ls","r");
  // // if (ls == NULL) {
  // //   printf("Ocurrio un error\n");
  // // }
  // // if (ls) {
  // //   printf("Erorr\n");
  // // }
  // char buf[256];
  // int exiteError = 0;
  // while (fgets(buf, sizeof(buf), ls) != 0) {
  //   // printf("%s\n", buf);
  //   exiteError = 1;
  // }
  // if (exiteError) {
  //   printf("Existe error\n");
  // } else {
  //   FILE *ls = popen("./.data/prueba 2>&1", "r");
  //   // como captar el exit status al ejecutar el archivo?
  //   while (fgets(buf, sizeof(buf), ls) != 0) {
  //     printf("%s\n", buf);
  //   }
  // }
  // pclose(ls);
  // int CPUs = sysconf(_SC_NPROCESSORS_ONLN);
  // printf("%d\n", CPUs);
  // struct rusage usage;
  // for (int i = 0; i < 700; i++) {
  //   /* Double loop for more interesting results. */
  //   for (int j = 0; j < 700; j++) {
  //     for (int k = 0; k < 700; k++) {
  //       if (k == 500) {
  //           getrusage (RUSAGE_SELF, &usage);
  //       }
  //     }
  //   }
  // }
  // printf ("CPU time: %ld.%06ld sec user, %ld.%06ld sec system\n",
  //    usage.ru_utime.tv_sec, usage.ru_utime.tv_usec,
  //    usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
  // struct rusage usage;
  // struct timeval start, end;
  // int i, j, k = 0;
  //
  // getrusage(RUSAGE_SELF, &usage);
  // start = usage.ru_stime;
  // for (i = 0; i < 10000; i++) {
  //   /* Double loop for more interesting results. */
  //   for (j = 0; j < 10000; j++) {
  //     k += 20;
  //   }
  // }
  // usleep(1000);
  // getrusage(RUSAGE_SELF, &usage);
  // end = usage.ru_stime;
  //
  // printf("Started at: %ld.%lds\n", start.tv_sec, start.tv_usec);
  // printf("Ended at: %ld.%lds\n", end.tv_sec, end.tv_usec);
       // int getloadavg(double loadavg[], int nelem);
    // long double a[4], b[4], loadavg;
    // FILE *fp;
    // char dump[50];
    //
    // for(;;)
    // {
    //     fp = fopen("/proc/stat","r");
    //     fscanf(fp,"%*s %Lf %Lf %Lf %Lf",&a[0],&a[1],&a[2],&a[3]);
    //     fclose(fp);
    //     sleep(1);
    //
    //     fp = fopen("/proc/stat","r");
    //     fscanf(fp,"%*s %Lf %Lf %Lf %Lf",&b[0],&b[1],&b[2],&b[3]);
    //     fclose(fp);
    //
    //     loadavg = ((b[0]+b[1]+b[2]) - (a[0]+a[1]+a[2])) / ((b[0]+b[1]+b[2]+b[3]) - (a[0]+a[1]+a[2]+a[3]));
    //     printf("The current CPU utilization is : %Lf\n",loadavg);
    // }
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
