#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void error(char *msg) {
  perror(msg);
  exit(0);
}

int main(int argc, char *argv[]) {
  int sockfd;
  struct sockaddr_in serv_addr;
  unsigned char buffInfo[8000000]={0};
  // unsigned char recvBuff[30];
  bzero(buffInfo,sizeof(buffInfo));

  if (argc < 2) {
    fprintf(stderr,"Ingresar la ruta del archivo\n");
    exit(-1);
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
      error("ERROR opening socket");

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(5000);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
      error("ERROR connecting");
  }
  FILE *fp = fopen(argv[1],"r");
  if(fp == NULL) {
    printf("El archivo no existe\n");
  } else {
    int nread;
    while((nread = fread(buffInfo,1,sizeof(buffInfo),fp)) > 0) {
      write(sockfd,buffInfo,nread);
      bzero(buffInfo,sizeof(buffInfo));
      if (feof(fp)){
        write(sockfd,"",0);
        fclose(fp);
      }
    }
    unsigned char recvBuff[80000];
    read(sockfd, recvBuff, sizeof(recvBuff));
    printf("La respuesta es: %s\n", recvBuff);
    close(sockfd);
  }
  return 0;
}
