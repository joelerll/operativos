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
#include "tabla.h"

#define TE 3
#define NOMBRE_LOG "log"
#define LENGTH            sizeof(struct procesos)
#define ARRAY_ELEMENTS    500

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define NUMERO_HILOS_MAXIMO system("/proc/sys/kernel/threads-max")
#define MAXANSWERSIZE 10

// Char strings
const char *archivoFormato = "%s/%d.c";
const char *compilar = "gcc .data/%d.c -o .data/%d.out"; //  2>&1
const char *salida = ".data/%d.out";
const char *ejecutar = "./.data/%d.out"; //  2>&1
const char *enviar = "{ \"estado\": \"%s\", \"mensaje\": \"%s\", \"datos\": \"%s\"}";
const char *pid = "cat /proc/%d/stat";
const char *pid2 = "ps  -u -p %d"; //--no-headers
const char *pid3 = "ps H -o 'pid tid comm pcpu' %d";

// Variables iniciales
pthread_mutex_t lock;
pid_t hiloMonitor;
pid_t pmaster = 0;
pthread_t thread_id;
int size = 500 * sizeof(int);

// lista de procesos
typedef struct procesos {
  unsigned int array[ARRAY_ELEMENTS];
	unsigned int cantidad;
  pthread_mutex_t  mutex;
} procesos;

typedef struct TablasNuevo {
  char *datos[300];
  int cantidad;
  pthread_mutex_t mutex;
} TablasNuevo;

typedef struct params {
  struct procesos *proceso;
	Tablas *tabla;
} params;

// Funciones locales
void error(char *msg) { perror(msg); exit(-1); }
void procesar (int sock, procesos *mapping, Tablas *tablas);
void alarm_handler(int sig);
void child_handler(int sig);
void *monitor(void *arg);
void matarServidor(int senal);
char *gen_fecha ();

int main() {
	u_limpiar_log(NOMBRE_LOG);
	struct procesos *mapping = mmap(0, LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	Tablas *tablas = mmap(0, sizeof(Tablas *), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	tabla_master_crear(tablas);
	// struct TablasNuevo *tablasN = mmap(0, sizeof(Tablas *), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	// memset(tablasN->datos, 0, 300);
	// tabla_master_crear(tablasN);

	// pthread_mutexattr_t mutexattr;
  // int rc = pthread_mutexattr_init(&mutexattr);
  // if (rc != 0)
  //   error("pthread_mutexattr_init");
  // rc = pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);
  // if (rc != 0)
  //   error("pthread_mutexattr_setpshared");
  // pthread_mutex_init(&mapping->mutex, &mutexattr);
  // if (rc != 0)
  //   error("pthread_mutex_init");
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
	struct params *param = malloc(sizeof(params *));
	param->proceso = mapping;
	param->tabla = tablas;
	if(pthread_create(&thread_id, NULL, monitor, param) < 0) {
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
			// registro del signal y del alarm
			signal(SIGALRM, alarm_handler);
			// signal(SIGCHLD, child_handler);
			alarm(TE);

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
			procesar(newsockfd, mapping, tablas);
			close(newsockfd);
      break;
		}
	}
  return EXIT_SUCCESS;
}

void alarm_handler(int sig) {
	char *file = "El proceso %d ha superado el tiempo TE %d";
	char mensaje[60];
	memset(mensaje, 0, sizeof(mensaje));
	pid_t pmaster = getpid();
	snprintf(mensaje, sizeof(mensaje), file, pmaster, TE);
	printf(ANSI_COLOR_RED "proceso %d ha superado el tiempo Te %d" ANSI_COLOR_RESET "\n", pmaster, TE);
	// proteger escritura
	u_escribir_log(mensaje, NOMBRE_LOG);
	// proteger escritura
}

void *monitor (void *arg) {
	params *param  = (params*)arg;
	procesos *procesosIds  = param->proceso;
	hiloMonitor = syscall(SYS_gettid);
	printf("aaaa%s\n");
	while (1) {
		// printf("%d\n", tablas->cantidad);
		// pthread_mutex_lock(&tablas->mutex);
		// 	// tabla_imprimir(tablas);
		// pthread_mutex_unlock(&tablas->mutex);

		pthread_mutex_lock(&procesosIds->mutex);
			proc_guardar_estaticticas_procesos (procesosIds->array, procesosIds->cantidad, NOMBRE_LOG);
		pthread_mutex_unlock(&procesosIds->mutex);
		proc_imprimir_porcentajesCPUs(NULL);
		usleep(250000);
	}
	return NULL;
}

void procesar (int sock, procesos *mapping, Tablas *tablas) {
	time_t t = time(NULL);
	srand(t ^ (getpid()<<16)); // uso de random

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

	// anadir a tabla
	// char *nombreArchivoTmp = malloc(sizeof(char) *strlen(nombreArchivo));
	// strcpy(nombreArchivoTmp, nombreArchivo);
	// char datos[9000];
	// pthread_mutex_lock(&tablas->mutex);
		// userId,
		// char *guardar = "%d, %s, %s, %s, %s, %d";
		// tablaDatos *tabla = tabla_crear(userId, "", nombreArchivo, "", "", tablas->cantidad);
		// snprintf(enviarDatos, sizeof(datos), guardar, userId, tabla->fechaEnvio, nombreArchivo, "", "", tablas->cantidad);
		// u_escribir_log (enviarDatos, "procesos");
		// 	tablaDatos *tabla = mmap(0, sizeof(tablaDatos), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
		// 	tabla = tabla_crear(userId, "", nombreArchivo, "", "", tablas->cantidad);
		// 	tabla_anadirTabla(tablas, tabla);
		// 	// tabla_imprimir(tablas);
	// pthread_mutex_unlock(&tablas->mutex);
	// anadir a tabla


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
		// pthread_mutex_lock(&mapping->mutex);
		// pid_t pmaster = getpid();
		// mapping->array[mapping->cantidad] = pmaster;
		// mapping->cantidad++;
		// pthread_mutex_unlock(&mapping->mutex);
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
				// pthread_mutex_lock(&mapping->mutex);
				// 	pid_t pmaster = getpid();
				// 	mapping->array[mapping->cantidad] = pmaster;
				// 	mapping->cantidad++;
				// pthread_mutex_unlock(&mapping->mutex);
				// redirigirlo
				int procesador = proc_obtenerProcesosMenosUsado(NULL);
			  cpu_set_t my_set;
			  CPU_ZERO(&my_set);
			  CPU_SET(procesador, &my_set);
			  sched_setaffinity(0, sizeof(cpu_set_t), &my_set);
				// redirigirlo
				memset(rutaEjecutar, 0, sizeof(rutaEjecutar));
				snprintf(rutaEjecutar, sizeof(rutaEjecutar), ejecutar, userId);
				// anadir a tabla
				// pthread_mutex_lock(&tablas->mutex);
				// 	tablaDatos *a = tabla_buscarPorUserId(userId, tablas);
				// 	if (a != NULL) {
				// 		tabla_actualizar_ruta(a, rutaEjecutar);
				// 	}
				// pthread_mutex_unlock(&tablas->mutex);
				// anadir a tabla
				dup2(link[1], STDOUT_FILENO);
				close(link[0]);
				close(link[1]);
				execl(rutaEjecutar, rutaEjecutar, (char *)0);
			} else {
				close(link[1]);
				read(link[0], respuesta, sizeof(respuesta));
				snprintf(enviarDatos, sizeof(enviarDatos), enviar, "true", "todo en orden", respuesta);
				// anadir a tabla
				// pthread_mutex_lock(&tablas->mutex);
					// tablaDatos *a = tabla_buscarPorUserId(userId, tablas);
					// if (a != NULL) {
					// 	 tabla_actualizar_respuesta(a, respuesta);
					// }
					// tabla_imprimir(tablas);
				// pthread_mutex_unlock(&tablas->mutex);
				// anadir a tabla
				write(sock, enviarDatos, sizeof(enviarDatos));
				wait(NULL);
				close(sock);
			}
		}
		close(link[0]);
	}
}

void matarServidor (int senal) {
  pid_t pmaster = getpid();
  printf(ANSI_COLOR_RED "Se ha detenido el servidor %d" ANSI_COLOR_RESET "\n", pmaster);
	printf("Se ha detenido el hilo monitor: %d\n", hiloMonitor);
	pthread_cancel(hiloMonitor);
	pthread_exit(NULL);
	wait(NULL);
	close(pmaster);
	kill(pmaster, SIGKILL);
}

void child_handler (int sig) {
	pid_t pmaster = getpid();
	printf("Child: %d\n", pmaster);
}

char *gen_fecha () {
	char *fecha = malloc(sizeof(50));
	const char *userFormato = "%d/%d/%dH%d:%d:%d";
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int year = tm.tm_year + 1900;
	int mon = tm.tm_mon + 1;
	int day = tm.tm_mday;
	int hora = tm.tm_hour;
	int minuto = tm.tm_min;
	int sec = tm.tm_sec;
	snprintf (fecha, 50, userFormato, year, mon, day, hora, minuto, sec);
	return fecha;
}
