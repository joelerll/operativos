// https://github.com/Leo-G/DevopsWiki/wiki/How-Linux-CPU-Usage-Time-and-Percentage-is-calculated
// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
// https://www.linuxquestions.org/questions/programming-9/proc-stat-file-problem-for-cpu-usage-369302/
// https://linux.die.net/man/2/getrusage
// https://www.centos.org/docs/5/html/5.2/Deployment_Guide/s2-proc-stat.html
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"
#include "proc.h"
#define CPUs sysconf(_SC_NPROCESSORS_ONLN) // _SC_NPROCESSORS_CON

ProcFile *proc_crear (char *nombre, char* user, char *nice, char *systemProc, char *idle, char *iowait, char *irq, char *softirq, char *steal, char *guest, char *guest_nice) {
  ProcFile *nuevo = (ProcFile *)malloc(sizeof(ProcFile));
  nuevo->nombre = (char *)malloc(sizeof((char *) strlen(nombre)));
  strcpy(nuevo->nombre, nombre);
  nuevo->PrevIdle = atoi(idle) + atoi(iowait);
  nuevo->PrevNonIdle = atoi(user) + atoi(nice) + atoi(systemProc) + atoi(softirq) + atoi(steal);
  nuevo->PrevTotal = nuevo->PrevIdle + nuevo->PrevNonIdle;
  return nuevo;
}

void proc_crearSiguiente (ProcFile *proc, char* user, char *nice, char *systemProc, char *idle, char *iowait, char *irq, char *softirq, char *steal, char *guest, char *guest_nice) {
  int PrevIdle = proc->PrevIdle;
  int PrevTotal = proc->PrevTotal;

  int Idle = atoi(idle) + atoi(iowait);
  int NonIdle = atoi(user) + atoi(nice) + atoi(systemProc) + atoi(softirq) + atoi(steal);
  int Total = Idle + NonIdle;

  int Totald = Total - PrevTotal;
  int Idled = Idle - PrevIdle;
  if (Totald != 0) {
    double tmp = (double)(Totald - Idled);
    proc->porcentajeUso = (double)(tmp/Totald) * 100;
  } else {
    proc->porcentajeUso = 0.0;
  }
}

ProcFile **proc_listaProcesos (char *archivo, long int tiempo) {
  ProcFile **procesos = malloc(sizeof(ProcFile*) * (CPUs + 2) );
  char file[] = "/proc/stat";
  FILE *fp;
  if (archivo != NULL) {
    fp = fopen(archivo,"r");
  } else {
    fp = fopen(file,"r");
  }
  char * line = NULL;
  size_t len = 0;
  for (size_t i = 0; i < CPUs + 1; i++) {
    ssize_t read = getline(&line, &len, fp);
    if (read != -1) {
      char **s = u_split(line, ' ');
      procesos[i] = proc_crear(s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8], s[9], s[10]);
    }
  }
  fclose(fp);
  usleep(tiempo); // porque si se lee apenas se cierra el archivo, no da estadisticas correctas. Practicamente da lo mismo que la primera lectura
  if (archivo != NULL) {
    fp = fopen(archivo,"r");
  } else {
    fp = fopen(file,"r");
  }
  for (size_t i = 0; i < CPUs + 1; i++) {
    ssize_t read = getline(&line, &len, fp);
    if (read != -1) {
      char **s = u_split(line, ' ');
      proc_crearSiguiente(procesos[i], s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8], s[9], s[10]);
    }
  }
  fclose(fp);
  return procesos;
}

int proc_obtenerProcesosMenosUsado (char *archivo) {
  int procesador = 0;
  double procentajeTmp = 0.0;
  int contador = 1; // empieza en uno porque al leer el archiv /proc/stat, lo que primero esta es las estadisticas de todos los procesadores
  ProcFile **listaProcesadores = proc_listaProcesos(archivo, 500);
  ProcFile *cpu;
  do {
    cpu = listaProcesadores[contador];
    if (cpu != NULL && ((procentajeTmp > cpu->porcentajeUso) || (procentajeTmp == 0.0))) {
      procentajeTmp = cpu->porcentajeUso;
      procesador = contador;
    }
    contador++;
  } while (cpu != NULL);
  return procesador;
}

void proc_imprimir_porcentajesCPUs (char *archivo) {
	int contador = 1;
	ProcFile **listaProcesadores = proc_listaProcesos(archivo, 1e+6); // 500000 1e+6
	ProcFile *cpu;
	do {
		cpu = listaProcesadores[contador];
		if (cpu != NULL) {
			printf("%s %.2f\n", cpu->nombre, cpu->porcentajeUso);
		}
		contador++;
	} while (cpu != NULL);
	sleep(1);
	// limpiar el stdout
	for (size_t i = 0; i < CPUs; i++) {
		fputs("\033[A\033[2K",stdout);
		rewind(stdout);
	}
	free(listaProcesadores);
}

// * uso memoria del cpu cat /proc/2311/statm * uso memoria de un hilo, cat /proc/2311/task/10935/statm
// 715888 144996 39581 52 0 437733 0
// // /proc/<pid>/statm
// //
// // /proc/<pid>/statm fields: columns are (in pages):
// // total program size|
// // resident set size|
// // shared pages|
// // text (code) |
// // data/stack |
// // library |
// // dirty pages |
// //
// // Here an example: 693 406 586 158 0 535 0
// // en Kb
//
//
// * disco cat /proc/2311/task/10935/io
// rchar: 0
// wchar: 0
// syscr: 0
// syscw: 0
// read_bytes: 106496
// write_bytes: 0
// cancelled_write_bytes: 0
//
// // rchar: number of bytes the process read, using any read-like system call (from files, pipes, tty...).
// //
// // wchar: number of bytes the process wrote using any write-like system call.
// //
// // syscr: number of read-like system call invocations that the process performed.
// //
// // syscr: number of write-like system call invocations that the process performed.
// //
// // read_bytes: number of bytes the process directly read from disk. write_bytes: number of bytes the process originally dirtied in the page-cache (assuming they will go to disk later). cancelled_write_bytes: number of bytes the process "un-dirtied" - e.g. using an "ftruncate" call that truncated pages from the page-cache.
//
//
// * cpu cat /proc/2311/task/10935/stat
// 10935 (FS Broker 10918) S 2310 2309 2309 0 -1 4194624 104 191674 0 31 52 144 651 137 20 0 81 0 50564 2764587008 142913 18446744073709551615 4194304 4406196 140721785014208 139924852149232 139925611411789 0 0 4096 17663 1 0 0 -1 4 0 0 27 0 0 6503376 6505552 14000128 140721785016687 140721785016742 140721785016742 140721785020361 0
//
// * contar cantidad de procesos usados y terminados por minutos
