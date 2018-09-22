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
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "utils.h"
#include "proc.h"
#define CPUs sysconf(_SC_NPROCESSORS_ONLN) // _SC_NPROCESSORS_CON
#define HERTZ sysconf(_SC_CLK_TCK) // hertz del pc
ProcFile *proc_crear (char *nombre, char* user, char *nice, char *systemProc, char *idle, char *iowait, char *irq, char *softirq, char *steal, char *guest, char *guest_nice) {
  ProcFile *nuevo = (ProcFile *)malloc(sizeof(ProcFile));
  nuevo->nombre = (char *)malloc(sizeof((char *) strlen(nombre)));
  strcpy(nuevo->nombre, nombre);
  nuevo->PrevIdle = atoi(idle) + atoi(iowait);
  nuevo->PrevNonIdle = atoi(user) + atoi(nice) + atoi(systemProc) + atoi(softirq) + atoi(steal);
  nuevo->PrevTotal = nuevo->PrevIdle + nuevo->PrevNonIdle;
  return nuevo;
}

Memory *proc_memory_crear(char *totalProgramSize, char* redisentSetSize, char *sharedPages, char *text, char *dataStack, char *library, char *dirtyPages) {
	Memory *nuevo = (Memory *)malloc(sizeof(Memory));
	nuevo->totalProgramSize = atoi(totalProgramSize);
	nuevo->redisentSetSize = atoi(redisentSetSize);
	nuevo->sharedPages= atoi(sharedPages);
	nuevo->text = atoi(text);
	nuevo->dataStack = atoi(dataStack);
	nuevo->library = atoi(library);
	nuevo->dirtyPages = atoi(dirtyPages);
	return nuevo;
}

IO *proc_io_crear(char *rchar, char *wchar, char* syscr, char *syscw, char *read_bytes, char *write_bytes, char *cancelled_write_bytes) {
	IO *nuevo = (IO *)malloc(sizeof(IO));
	nuevo->rchar = atol(rchar);
	nuevo->wchar = atol(wchar);
	nuevo->syscr = atoi(syscr);
	nuevo->syscw= atoi(syscw);
	nuevo->read_bytes = atoi(read_bytes);
	nuevo->write_bytes = atoi(write_bytes);
	nuevo->cancelled_write_bytes = atoi(cancelled_write_bytes);
	return nuevo;
}

Uptime *proc_uptime_crear(char *uptime, char *idle) {
	Uptime *nuevo = (Uptime *)malloc(sizeof(Uptime));
	nuevo->uptime = atof(uptime);
	nuevo->idle = atof(idle);
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
  int contador = 1; // empieza en uno porque al leer el archivo /proc/stat lo que primero esta son las estadisticas de todos los procesadores
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
			printf("%s %.2f%%\n", cpu->nombre, cpu->porcentajeUso);
		}
		contador++;
	} while (cpu != NULL);
	printf("\n");
	// sleep(1);
	// limpiar el stdout
	// for (size_t i = 0; i < CPUs; i++) {
	// 	fputs("\033[A\033[2K",stdout);
	// 	rewind(stdout);
	// }
	free(listaProcesadores);
}

Uptime *proc_uptime (char *archivo) {
	char nombreArchivo[20] = "/proc/uptime";
	if (archivo) {
		memset(nombreArchivo, 0, sizeof(nombreArchivo));
		strcpy(nombreArchivo, archivo);
	}
	FILE *fp = fopen(nombreArchivo,"r");
	if (fp > 0) {
		char * line = NULL;
	  size_t len = 0;
		getline(&line, &len, fp);
		char **s = u_split(line, ' ');
		Uptime *uptime = proc_uptime_crear(s[0], s[1]);
		fclose(fp);
		return uptime;
	}
	fclose(fp);
	return NULL; // el archivo no existe
}

Memory *proc_read_mem (pid_t pid, char *archivo) {
	char *file = "/proc/%d/statm";
	char nombreArchivo[20];
	if (archivo) {
		strcpy(nombreArchivo, archivo);
	} else {
		memset(nombreArchivo, 0, sizeof(nombreArchivo));
		snprintf(nombreArchivo, sizeof(nombreArchivo), file, pid);
	}

	Memory *mem;
	int fp = open(nombreArchivo, O_RDONLY, 0);
	if (fp <= 0) {
		close(fp);
		return NULL;
	} else {
		int sz;
		char *c = (char *) calloc(200, sizeof(char));
		sz = read(fp, c, 50);
		if (sz != -1) {
			char **s = u_split(c, ' ');
			mem = proc_memory_crear(s[0], s[1], s[2], s[3], s[4], s[5], s[6]);
		}
	}
	close(fp);
	return mem;
}

IO *proc_read_io (pid_t pid, char *archivo) {
	char *file = "/proc/%d/io";
	char **datos = malloc(sizeof(char *) * 8);
	char nombreArchivo[30];
	if (archivo) {
		strcpy(nombreArchivo, archivo);
	} else {
		memset(nombreArchivo, 0, sizeof(nombreArchivo));
		snprintf(nombreArchivo, sizeof(nombreArchivo), file, pid);
	}
	IO *io;
	int fp = open(nombreArchivo, O_RDONLY, 0);
	if (fp <= 0) {
		close(fp);
		return NULL;
	} else {
		char *c = (char *) calloc(200, sizeof(char));
		read(fp, c, 100);
		int cont = 0;
		int contDatos = 0;
		int contLetra = 0;
		char tmp[30];
		do {
			char letra = c[cont];
			if (letra == '\n') {
				tmp[contLetra+1] = '\0';
				char *letraFinal = malloc(sizeof(char) * 30);
				strcpy(letraFinal, tmp);
				datos[contDatos] = letraFinal;
				memset(tmp, 0, 30);
				contLetra = 0;
			 	contDatos++;
			} else {
				tmp[contLetra] = letra;
				contLetra++;
			}
			cont++;
		} while(contDatos < 7);
	}
	io = proc_io_crear(datos[0], datos[1], datos[2], datos[3], datos[4], datos[5], datos[6]);
	close(fp);
	return io;
}

void proc_cpu_imprimir (Procinfo *pinfo) {
	printf("%lu\n", pinfo->rlim);
	printf("1 %d\n", pinfo->pid);
	printf("2 %s\n", pinfo->exName);
	printf("3 %c\n", pinfo->state);
	printf("4 %d\n", pinfo->ppid);
	printf("5 %d\n", pinfo->pgrp);
	printf("6 %d\n", pinfo->session);
	printf("7 %d\n", pinfo->tty);
	printf("8 %d\n", pinfo->tpgid);
	printf("9 %d\n", pinfo->flags);
	printf("10 %d\n", pinfo->minflt);
	printf("11 %d\n", pinfo->cminflt);
	printf("12 %d\n", pinfo->majflt);
	printf("13 %d\n", pinfo->cmajflt);
	printf("14 %d\n", pinfo->utime);
	printf("15 %d\n", pinfo->stime);
	printf("16 %d\n", pinfo->cutime);
	printf("17 %d\n", pinfo->cstime);
	printf("18 %d\n", pinfo->counter);
	printf("19 %d\n", pinfo->priority);
	printf("20 %d\n", pinfo->timeout);
	printf("21 %d\n", pinfo->itrealvalue);
	printf("22 %d\n", pinfo->starttime);
	printf("23 %d\n", pinfo->vsize);
	printf("24 %d\n", pinfo->rss);
	printf("25 %lu\n", pinfo->rlim);
	printf("26 %lu\n", pinfo->startcode);
	printf("27 %lu\n", pinfo->endcode);
	printf("28 %lu\n", pinfo->startstack);
	printf("29 %lu\n", pinfo->kstkesp);
	printf("30 %lu\n", pinfo->kstkeip);
	printf("31 %d\n", pinfo->signal);
	printf("32 %d\n", pinfo->blocked);
	printf("33 %d\n", pinfo->sigignore);
	printf("34 %d\n", pinfo->sigcatch);
	printf("35 %d\n", pinfo->wchan);
}

Procinfo *proc_cpu (pid_t pid, char *archivo) {
	Procinfo *pinfo = (Procinfo *)malloc(sizeof(Procinfo));
	char *file = "/proc/%d/stat";
	char nombreArchivo[20];
	memset(nombreArchivo, 0, sizeof(nombreArchivo));
	if (archivo) {
		strcpy(nombreArchivo, archivo);
	} else {
		snprintf(nombreArchivo, sizeof(nombreArchivo), file, pid);
	}
	int fp = open(nombreArchivo, O_RDONLY, 0);
	if (fp <= 0) {
		// printf("El archivo no existe\n");
		close(fp);
		return NULL;
	} else {
		char *line = (char *) calloc(200, sizeof(char));
		read(fp, line, 200);
		char **s = u_split(line, ' ');
		pinfo->pid = atoi(s[0]);
		sscanf(line, "%d %s %c %d %d %d %d %d %u %u %u %u %u %d %d %d %d %d %d %u %u %d %u %u %lu %lu %lu %lu %lu %lu %d %d %d %d %u",
			&(pinfo->pid),
			pinfo->exName,
			&(pinfo->state),
		  &(pinfo->ppid),
		  &(pinfo->pgrp),
		  &(pinfo->session),
		  &(pinfo->tty),
		  &(pinfo->tpgid),
		  &(pinfo->flags),
		  &(pinfo->minflt),
		  &(pinfo->cminflt),
		  &(pinfo->majflt),
		  &(pinfo->cmajflt),
		  &(pinfo->utime),
		  &(pinfo->stime),
		  &(pinfo->cutime),
		  &(pinfo->cstime),
		  &(pinfo->counter),
		  &(pinfo->priority),
		  &(pinfo->timeout),
		  &(pinfo->itrealvalue),
		  &(pinfo->starttime),
		  &(pinfo->vsize),
		  &(pinfo->rss),
		  &(pinfo->rlim),
		  &(pinfo->startcode),
		  &(pinfo->endcode),
		  &(pinfo->startstack),
		  &(pinfo->kstkesp),
		  &(pinfo->kstkeip),
		  &(pinfo->signal),
		  &(pinfo->blocked),
		  &(pinfo->sigignore),
		  &(pinfo->sigcatch),
		  &(pinfo->wchan));
	}
	close(fp);
	return pinfo;
}

double proc_cpu_procentaje (Procinfo *pinfo) {
	long unsigned int total_time = pinfo->utime + pinfo->stime;
	total_time = total_time + pinfo->cutime + pinfo->cstime;
	Uptime *uptime = proc_uptime(NULL);
	double tmp = pinfo->starttime / HERTZ;
	double seconds = uptime->uptime - tmp;
	double cpu_usage = 100 * ((total_time / HERTZ) / seconds);
	return cpu_usage;
}

/*
	aqui tambien guardar los del monitor
*/
void proc_guardar_estaticticas_procesos (unsigned int pids[], int cantidad, char *nombreArchivo) {
	char *file = "proceso pid:%d, cpu:%0.4f%%, memoria: %lu discoEscrito: %ld, discoRead: %ld majorPageFaults: %u, minorPagesFautls: %u\n";
	char mensaje[200];
	memset(mensaje, 0, sizeof(mensaje));
	for (size_t i = 0; i < cantidad; i++) {
		if (pids[i] == 0) {
			break;
		}
		Memory *mem = proc_read_mem(pids[i], NULL);
		IO *io = proc_read_io(pids[i], NULL);
		Procinfo *proc = proc_cpu(pids[i], NULL);
		if (mem && io && proc) {
			double uso =  proc_cpu_procentaje(proc);
			snprintf(mensaje, sizeof(mensaje), file, pids[i], uso, mem->totalProgramSize, io->write_bytes, io->read_bytes, proc->minflt, proc->majflt);
			u_escribir_log(mensaje, nombreArchivo);
		} else {
			continue;
		}
		free(mem);
		free(io);
		free(proc);
	}
}
// https://linux.die.net/man/2/getrusage
// https://www.redhat.com/archives/axp-list/2001-January/msg00355.html
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat
// https://scanlibs.com/tips-tricks-techniques-nodejs-development/
