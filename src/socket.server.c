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
#include <sys/wait.h>
#include <sys/mman.h>

#include "proc.h"
#include "utils.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define NUMERO_HILOS_MAXIMO system("/proc/sys/kernel/threads-max")
#define MAXANSWERSIZE 10

const char *archivoFormato = "%s/%d.c";
const char *compilar = "gcc .data/%d.c -o .data/%d.out"; //  2>&1
const char *salida = ".data/%d.out";
const char *ejecutar = "./.data/%d.out"; //  2>&1
const char *enviar = "{ \"estado\": \"%s\", \"mensaje\": \"%s\", \"datos\": \"%s\"}";
const char *pid = "cat /proc/%d/stat";
const char *pid2 = "ps  -u -p %d"; //--no-headers
const char *pid3 = "ps H -o 'pid tid comm pcpu' %d";
pthread_mutex_t lock;
pid_t hiloMonitor;
pid_t pmaster = 0;
pthread_t thread_id;
int size = 500 * sizeof(int);
void error(char *msg) { perror(msg); exit(-1); }
void procesar (int newsockfd);
void *monitor(void *arg);
#define LENGTH            sizeof(struct procesos)
#define ARRAY_ELEMENTS    500
typedef struct procesos {
  unsigned int array[ARRAY_ELEMENTS];
	unsigned int cantidad;
  pthread_mutex_t  mutex;
} procesos;

void matarServidor(int senal) {
  pid_t pmaster = getpid();
	kill(pmaster, SIGKILL);
  printf(ANSI_COLOR_RED "Se ha detenido el servidor %d" ANSI_COLOR_RESET "\n", pmaster);
	printf("Se ha detenido el hilo monitor: %d\n", hiloMonitor);
	pthread_cancel(hiloMonitor);
	pthread_exit(NULL);
	wait(NULL);
  close(pmaster);
}

int main() {
	struct procesos *mapping = mmap(0, LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	pthread_mutexattr_t mutexattr;
  int rc = pthread_mutexattr_init(&mutexattr);
  if (rc != 0)
    error("pthread_mutexattr_init");
  rc = pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);
  if (rc != 0)
    error("pthread_mutexattr_setpshared");
  pthread_mutex_init(&mapping->mutex, &mutexattr);
  if (rc != 0)
    error("pthread_mutex_init");
	int sockfd, newsockfd = 0;
  int pid;
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

  listen(sockfd,1000);
  signal(SIGINT, SIG_IGN);
  signal(SIGTSTP, matarServidor);

  printf("El servidor esta corriendo\n");

	// cliente monitor
	// int *param = (int *)malloc(2 * sizeof(int));
	// param[0] = pmaster;
  // param[1] = mapping;
	if(pthread_create(&thread_id, NULL, monitor, mapping) < 0) {
		error("No se pudo crear el monitor\n");
		return EXIT_FAILURE;
	}

	for(;;) {
		newsockfd = accept(sockfd,NULL,NULL);
		if ((pid = fork()) == -1) {
      close(newsockfd);
    	continue;
    } else if (pid > 0) {
			close(newsockfd);
      continue;
		} else if (pid == 0) {
			// asignacion al procesador
		  int procesador = proc_obtenerProcesosMenosUsado(NULL);
		  cpu_set_t my_set;
		  CPU_ZERO(&my_set);
		  CPU_SET(procesador, &my_set);
		  sched_setaffinity(0, sizeof(cpu_set_t), &my_set);
			pthread_mutex_lock(&mapping->mutex);
      pid_t pmaster = getpid();
      mapping->array[mapping->cantidad] = pmaster;
			mapping->cantidad++;
			pthread_mutex_unlock(&mapping->mutex);
		  // int cpu = sched_getcpu();
		  // printf("Asignado el procesador: %d\n", cpu); // CLEAN: mensaje para debug
			procesar(newsockfd);
			close(newsockfd);
      break;
		}
	}
  return EXIT_SUCCESS;
}

void *monitor (void *arg) {
	// procesos *procesosIds  = (procesos*)arg;
	// hiloMonitor = syscall(SYS_gettid);
	while (1) {
		// for (size_t i = 0; i < ARRAY_ELEMENTS; i++) {
		// 	if (procesosIds->array[i] == 0) {
		// 		break;
		// 	}
		// 	printf("%d\n", procesosIds->array[i]);
		// }
		proc_imprimir_porcentajesCPUs(NULL);
	}
	return NULL;
}

void procesar (int sock) {
	srand(time(NULL) ^ (getpid()<<16)); // uso de random
	/* === PRUEBAS ==== */
  // pid_t pmaster = param[1];
  // pid_t hiloId = syscall(SYS_gettid);
  // char datos[30];
  // snprintf(datos, sizeof(datos), pid2, pmaster);
  // system(datos);
  // printf("El id el hilo: %d\n",hiloId);
	/* === PRUEBAS ==== */

  // generar archivo
  int userId = u_random_number(1,8000000); // FIX: cambiar esta por uno que genere un string "mas" random
  char nombreArchivo[30];
  memset(nombreArchivo, 0, sizeof(nombreArchivo));
  snprintf(nombreArchivo, sizeof(nombreArchivo), archivoFormato, ".data", userId);
  int fp = open(nombreArchivo, O_WRONLY | O_APPEND | O_CREAT, 0777);

  if (fp < 0) { error("Error al crear archivo"); }

  unsigned char recvBuff[800000];
  int bytesReceived = 0;
  bytesReceived = read(sock, recvBuff, sizeof(recvBuff));
  write(fp, recvBuff, bytesReceived);
  close(fp);

	char rutaCompilado[60];
	char rutaEjecutar[35];
	char respuesta[900];
	char enviarDatos[90000];

	int link[2];
	pid_t pid;
	if (pipe(link) == -1) { error("Error al crear pipe: "); }
	if ((pid = fork()) == -1) { error("Error al crear proceso "); }

	if (pid == 0) {
		// redirigirlo
		int procesador = proc_obtenerProcesosMenosUsado(NULL);
		cpu_set_t my_set;
		CPU_ZERO(&my_set);
		CPU_SET(procesador, &my_set);
		sched_setaffinity(0, sizeof(cpu_set_t), &my_set);
		// redirigirlo
		dup2(link[1], STDERR_FILENO);
		close(link[0]);
		close(link[1]);
	  snprintf(rutaCompilado, sizeof(rutaCompilado), salida, userId);
		execl("/usr/bin/gcc", "/usr/bin/gcc", nombreArchivo, "-o", rutaCompilado, (char *)0);
	} else {
		close(link[1]);
		int nbytes = read(link[0], respuesta, sizeof(respuesta));
		if (nbytes > 0) { // si ocurrio un error
			snprintf(enviarDatos, sizeof(enviarDatos), enviar, "false", "hubo un error al compilar", respuesta);
			write(sock, error, sizeof(error));
		} else {
			// si compilo correctamente lo ejecutara y obtendra el resuldato del stdout
			int link[2];
			pid_t pid2;
			if (pipe(link) == -1) { error("Error al crear pipe: "); }
			if ((pid2 = fork()) == -1) { error("Error al crear proceso "); }
			if (pid2 == 0) {
				// redirigirlo
				int procesador = proc_obtenerProcesosMenosUsado(NULL);
			  cpu_set_t my_set;
			  CPU_ZERO(&my_set);
			  CPU_SET(procesador, &my_set);
			  sched_setaffinity(0, sizeof(cpu_set_t), &my_set);
				// redirigirlo
				memset(rutaEjecutar, 0, sizeof(rutaEjecutar));
				snprintf(rutaEjecutar, sizeof(rutaEjecutar), ejecutar, userId);
				dup2(link[1], STDOUT_FILENO);
				close(link[0]);
				close(link[1]);
				execl(rutaEjecutar, rutaEjecutar, (char *)0);
			} else {
				close(link[1]);
				read(link[0], respuesta, sizeof(respuesta));
				snprintf(enviarDatos, sizeof(enviarDatos), enviar, "true", "todo en orden", respuesta);
				write(sock, enviarDatos, sizeof(enviarDatos));
				wait(NULL);
				close(sock);
			}
		}
		close(link[0]);
	}
}
