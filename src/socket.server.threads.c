#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include <sched.h>
#include "proc.h"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void error(char *msg) { perror(msg); exit(-1); }
void *hiloProcesar(void *arg);
void matarServidor(int senal) {
  pid_t pmaster = getpid();
  printf(ANSI_COLOR_RED "Se ha detenido el servidor %d" ANSI_COLOR_RESET "\n", pmaster);
  // KILL ALL CHILDRENS
  // pthread_cleanup_push(NULL, NULL);
  // pthread_cancel(NULL);
  pthread_exit(NULL);
  close(pmaster);
}

int main() {
  int sockfd, client_socket;
  struct sockaddr_in serv_addr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    error("ERROR al iniciar el socket");
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(5000);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
    error("ERROR en bind socket");
  }

  listen(sockfd,100000);
  signal(SIGINT, SIG_IGN);
  signal(SIGTSTP, matarServidor);
  pthread_t thread_id;

  while(1) {
    client_socket = accept(sockfd, NULL, NULL);
    if (client_socket < 0) {
      close(client_socket);
      perror("Erro al conectarse\n");
    }
    int *param = (int *)malloc(1 * sizeof(int));
    param[0] = client_socket;
    if(pthread_create(&thread_id, NULL, hiloProcesar, param) < 0) {
     perror("No se pudo crear el socket\n");
     return EXIT_FAILURE;
    }
  }
  return 0;
}

void *hiloProcesar(void *arg) {
  int procesador = proc_obtenerProcesosMenosUsado(NULL);
  cpu_set_t my_set;
  CPU_ZERO(&my_set);
  CPU_SET(procesador, &my_set);
  sched_setaffinity(0, sizeof(cpu_set_t), &my_set);
  int cpu = sched_getcpu();
  printf("Asignado el procesador: %d\n", cpu);
  int *param = (int *)arg;
  int sock = param[0];
  int n = 900;
  for(int i=1; i<=n; ++i) {
    for(int i=1; i<=n; ++i) {
      for(int i=1; i<=n; ++i) {
      }
    }
  }
  write(sock,"a",1);
  close(sock);
  return NULL;
}
