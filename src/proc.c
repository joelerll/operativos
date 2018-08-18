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

ProcFile **proc_listaProcesos (char *archivo) {
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

  usleep(50); // porque si se lee apenas se cierra el archivo, no da estadisticas correctas. Practicamente da lo mismo que la primera lectura
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
  ProcFile **listaProcesadores = proc_listaProcesos(archivo);
  ProcFile *cpu;
  do {
    cpu = listaProcesadores[contador];
    if (cpu != NULL) {
      if ((procentajeTmp > cpu->porcentajeUso) || (procentajeTmp == 0.0)) {
        procentajeTmp = cpu->porcentajeUso;
        procesador = contador;
      }
    }
    contador++;
  } while (cpu != NULL);
  return procesador;
}
//
// int **proc_porcentajesCPUs () {
//
// }
