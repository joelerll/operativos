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
#include <syscall.h>
#include <fcntl.h>

#include "proc.h"
#include "utils.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define NUMERO_HILOS_MAXIMO system("/proc/sys/kernel/threads-max")
#define MAXANSWERSIZE 10

const char *archivoFormato = "%s/%d.c";
const char *compilar = "gcc .data/%d.c -o .data/%d.out 2>&1";
const char *ejecutar = "./.data/%d.out 2>&1";
const char *enviar = "{ \"estado\": \"%s\", \"mensaje\": \"%s\", \"datos\": \"%s\"}";
const char *pid = "cat /proc/%d/stat";
const char *pid2 = "ps  -u -p %d"; //--no-headers
const char *pid3 = "ps H -o 'pid tid comm pcpu' %d";
pthread_mutex_t lock;
pid_t hiloMonitor;
void error(char *msg) { perror(msg); exit(-1); }
void *hiloProcesar(void *arg);
void *monitor(void *arg);
void matarServidor(int senal) {
  pid_t pmaster = getpid();
  printf(ANSI_COLOR_RED "Se ha detenido el servidor %d" ANSI_COLOR_RESET "\n", pmaster);
	printf("Se ha detenido el hilo monitor: %d\n", hiloMonitor);
	pthread_cancel(hiloMonitor);
  pthread_exit(NULL);
  close(pmaster);
}

int main() {
  srand(time(NULL));// porque se usa random
  int sockfd, client_socket;
  int option = 1;
  struct sockaddr_in serv_addr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
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
  pid_t pmaster = getpid();

  printf("El servidor esta corriendo\n");

	// cliente monitor
	int *param = (int *)malloc(2 * sizeof(int));
	param[0] = pmaster;
	if(pthread_create(&thread_id, NULL, monitor, param) < 0) {
		error("No se pudo crear el monitor\n");
		return EXIT_FAILURE;
	}

  while(1) {
    client_socket = accept(sockfd, NULL, NULL);
    if (client_socket < 0) {
      close(client_socket);
      error("Error al conectarse\n");
    }
    int *param = (int *)malloc(2 * sizeof(int));
    param[0] = client_socket;
    param[1] = pmaster;
    if(pthread_create(&thread_id, NULL, hiloProcesar, param) < 0) {
     error("No se pudo crear el socket\n");
     return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

void *monitor (void *arg) {
	hiloMonitor = syscall(SYS_gettid);
	while (1) {
		proc_imprimir_porcentajesCPUs(NULL);
	}
	return NULL;
}

void *hiloProcesar (void *arg) {
  int *param = (int *)arg;
  int sock = param[0];

	/* === PRUEBAS ==== */
  // pid_t pmaster = param[1];
  // pid_t hiloId = syscall(SYS_gettid);
  // char datos[30];
  // snprintf(datos, sizeof(datos), pid2, pmaster);
  // system(datos);
  // printf("El id el hilo: %d\n",hiloId);
	/* === PRUEBAS ==== */

  // asignacion al procesador
  int procesador = proc_obtenerProcesosMenosUsado(NULL);
  cpu_set_t my_set;
  CPU_ZERO(&my_set);
  CPU_SET(procesador, &my_set);
  sched_setaffinity(0, sizeof(cpu_set_t), &my_set);
  int cpu = sched_getcpu();
  printf("Asignado el procesador: %d\n", cpu); // CLEAN: mensaje para debug

  // generar archivo
  int userId = u_random_number(1,8000000); // FIX: cambiar esta por uno que genere un string "mas" random
  char nombreArchivo[30];
  memset(nombreArchivo, 0, sizeof(nombreArchivo));
  snprintf(nombreArchivo, sizeof(nombreArchivo), archivoFormato, ".data", userId);
  int fp = open(nombreArchivo, O_WRONLY | O_APPEND | O_CREAT, 0777);

  if (fp < 0) {
    printf("Error al crear el archivo\n");
    perror("Error");
  }
  unsigned char recvBuff[800000];
  int bytesReceived = 0;
  bytesReceived = read(sock, recvBuff, sizeof(recvBuff));
  write(fp, recvBuff, bytesReceived);
  close(fp);

  // compilar
  char rutaCompilado[60];
  char rutaEjecutar[35];
  char respuesta[900];
  char enviarDatos[90000];
  char buf[10];
  snprintf(rutaCompilado, sizeof(rutaCompilado), compilar, userId, userId);
  FILE *fp_compilar = popen(rutaCompilado, "r");
  int exiteError = 0;
  while (fgets(buf, sizeof(buf), fp_compilar) != 0) { // esperar a que termine de compilar
    strcat(respuesta, buf);
    exiteError = 1;
  }
  fclose(fp_compilar);

  // ejecutar
  if (exiteError) {
    snprintf(enviarDatos, sizeof(enviarDatos), enviar, "false", "hubo un error al compilar", respuesta);
    write(sock,enviarDatos,strlen(enviarDatos));
  } else {
    memset(rutaEjecutar, 0, sizeof(rutaEjecutar));
    snprintf(rutaEjecutar, sizeof(rutaEjecutar), ejecutar, userId);
    FILE *fp_ejecutar = popen(rutaEjecutar, "r");
    while (fgets(buf, sizeof(buf), fp_ejecutar) != 0) {
      strcat(respuesta, buf);
    }
    fclose(fp_ejecutar);
    snprintf(enviarDatos, sizeof(enviarDatos), enviar, "true", "todo en orden", respuesta);
    write(sock,enviarDatos,strlen(enviarDatos));
  }
  close(sock);
  return NULL;
}
