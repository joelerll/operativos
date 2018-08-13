#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tabla.h"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define MAXANSWERSIZE 800

int status = 0;
pid_t pmaster = 0;
const char *userFormato = "%s/%d_%d_%d_%d_%d_%d_ID%d.c";
const char *formato = "%s/%d.c";
const char *salida = "%s.out";

void procesarArchivo(int sockfd);

void error(char *msg) { perror(msg); exit(-1); }

int random_number(int min_num, int max_num) {
  int result = 0, low_num = 0, hi_num = 0;
  if (min_num < max_num) {
    low_num = min_num;
    hi_num = max_num + 1;
  } else {
    low_num = max_num + 1;
    hi_num = min_num;
  }
  srand(time(NULL));
  result = (rand() % (hi_num - low_num)) + low_num;
  return result;
}

void matarServidor(int senal) {
  printf(ANSI_COLOR_RED "Se ha detenido el servidor %d" ANSI_COLOR_RESET "\n",pmaster);
  wait(NULL);
  close(pmaster);
}

int main(int argc, char *argv[]) {
  int sockfd, newsockfd = 0;
  struct sockaddr_in serv_addr;
  int pid;

  // inicializacion del socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR al iniciar el socket");

  // set el puerto y los datos necesarios para inicializar el server
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(5000);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    error("ERROR en bind socket");

  listen(sockfd,100000);
  signal(SIGINT, SIG_IGN);
  signal(SIGTSTP, matarServidor); // matar el server con contro+z

  while (1) {
    pmaster = sockfd;
    if (sockfd > 0) {
     newsockfd = accept(sockfd,NULL,NULL);
    }
     if (newsockfd < 0) {
       error("ERROR on accept");
     }
     pid = fork();

     if (pid < 0) {
        error("ERROR on fork");
     }
     if (pid == 0)  {
         close(sockfd);
         procesarArchivo(newsockfd);
         exit(0);
     } else {
       wait(NULL);
       close(newsockfd);
     }
  }
  return 0;
}

void procesarArchivo(int newsockfd) {
  unsigned char recvBuff[8000000];
  // time_t t = time(NULL);
  // struct tm tm = *localtime(&t);
  // int year = tm.tm_year + 1900;
  // int mon = tm.tm_mon + 1;
  // int day = tm.tm_mday;
  // int hora = tm.tm_hour;
  // int minuto = tm.tm_min;
  // int sec = tm.tm_sec;

  // char *userId;
  // userId = malloc (MAXANSWERSIZE);
  // snprintf (userId, MAXANSWERSIZE, userFormato, ".data", year, mon, day, hora, minuto, sec, random_number(1,8000));
  int userId = random_number(1,8000000);
  char *nombreArchivo = malloc (MAXANSWERSIZE);
  // sprintf(nombreArchivo, "%d", userId);
  snprintf(nombreArchivo, MAXANSWERSIZE, formato, ".data", userId);
  FILE *fp = fopen(nombreArchivo,"w");
  if (fp == NULL) {
    printf("No error al guardar el archivo\n");
  } else {
    int bytesReceived = 0;
    bytesReceived = read(newsockfd, recvBuff, sizeof(recvBuff));
    while(bytesReceived > 0) {
      fwrite(recvBuff, 1,bytesReceived,fp);
      bzero(recvBuff,sizeof(recvBuff));
      bytesReceived = read(newsockfd, recvBuff, sizeof(recvBuff));
      if(bytesReceived == 0) {
        // write(newsockfd,"final",sizeof("final"));
        fclose(fp);
      }
    }
    write(newsockfd,"final",sizeof("final"));
    int link[2];
    pid_t pid;
    if (pipe(link) == -1) {
      perror("Error al crear pipe: ");
      exit(1);
    }

    pid = fork();
    if ( pid == -1) {
      perror("Error al crear proceso: ");
      exit(1);
    }

    if(pid == 0) { // compilar
      char rutaCompilado[50];
      memset(rutaCompilado, 0, sizeof(rutaCompilado));
      dup2(link[1], STDERR_FILENO);
      close(link[0]);
      close(link[1]);
      snprintf (rutaCompilado, sizeof(rutaCompilado), salida, nombreArchivo);
      execl("/usr/bin/gcc", "/usr/bin/gcc", nombreArchivo, "-o", rutaCompilado, (char *)0);
    } else {
      char error[4096];
      close(link[1]);
      int nbytes = read(link[0], error, sizeof(error));
      if (nbytes > 0) { // si ocurrio un error
        printf("ERROR: %s", error);
      } else {
        // si compilo correctamente lo ejecutara y obtendra el resuldato del stdout
        int link[2];
        char resultado[4096];
        if (pipe(link) == -1) {
          perror("Error al crear pipe: ");
          exit(1);
        }

        pid_t pid2 = fork();
        if (pid2 == -1) {
          perror("Error al crear proceso: ");
          exit(1);
        }
        if (pid2 == 0) {
          char rutaCompilado[50];
          snprintf (rutaCompilado, sizeof(rutaCompilado), salida, nombreArchivo);
          char ejecutar[55] = "./";
          dup2(link[1], STDOUT_FILENO);
          close(link[0]);
          close(link[1]);
          strcat(ejecutar, rutaCompilado);
          execl(ejecutar, ejecutar, (char *)0);
        } else {
          close(link[1]);
          read(link[0], resultado, sizeof(resultado));
          printf("%s\n", resultado);
          write(newsockfd,"hgello",sizeof("hgello"));
          close(newsockfd);
        }
        wait(NULL);
      }
      close(link[0]);
    }
  }
}
