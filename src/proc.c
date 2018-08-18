// https://github.com/Leo-G/DevopsWiki/wiki/How-Linux-CPU-Usage-Time-and-Percentage-is-calculated
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "proc.h"

ProcFile *proc_crear (char *nombre, char* user, char *nice, char *systemProc, char *idle, char *iowait, char *irq, char *softirq, char *steal, char *guest, char *guest_nice) {
  ProcFile *nuevo = (ProcFile *)malloc(sizeof(ProcFile));
  nuevo->nombre = (char *)malloc(sizeof((char *) strlen(nombre)));
  strcpy(nuevo->nombre, nombre);
  nuevo->user = atoi(user);
  nuevo->nice = atoi(nice);
  nuevo->systemProc = atoi(systemProc);
  nuevo->idle = atoi(idle);
  nuevo->iowait = atoi(iowait);
  nuevo->irq = atoi(irq);
  nuevo->softirq = atoi(softirq);
  nuevo->steal = atoi(steal);
  nuevo->guest = atoi(guest);
  nuevo->guest_nice = atoi(guest_nice);
  int totalCpuTimeSinceBoot = nuevo->user+nuevo->nice+nuevo->systemProc+nuevo->idle+nuevo->iowait+nuevo->irq+nuevo->softirq+nuevo->steal;
  int totalCpuIdleTimeSinceBoot = nuevo->idle + nuevo->iowait;
  int totalCpuUsageTimeSinceBoot = totalCpuTimeSinceBoot - totalCpuIdleTimeSinceBoot;
  nuevo->porcentajeUso = (double)totalCpuUsageTimeSinceBoot/totalCpuTimeSinceBoot * 100;
  return nuevo;
}

ProcFile **proc_listaProcesos (char *archivo) {
  ProcFile **procesos = malloc(sizeof(ProcFile*) * 20);
  char file[] = "/proc/stat";
  FILE *fp;
  if (archivo != NULL) {
    fp = fopen(archivo,"r");
  } else {
    fp = fopen(file,"r");
  }
  char * line = NULL;
  size_t len = 0;
  int CPUs = sysconf(_SC_NPROCESSORS_ONLN);
  for (size_t i = 0; i < CPUs; i++) {
    ssize_t read = getline(&line, &len, fp);
    if (read != -1) {
      char **s = u_split(line, ' ');
      procesos[i] = proc_crear(s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8], s[9], s[10]);
    }
  }
  return procesos;
}

// int proc_obtenerProcesosMenosUsado (char *archivo) {
//
// }
//
// int **proc_porcentajesCPUs () {
//
// }
