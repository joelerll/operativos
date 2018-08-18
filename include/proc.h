#include <stdio.h>
typedef struct procTDA{
	char *nombre;
  // int user; // normal processes executing in user mode
  // int nice; // niced processes executing in user mode
  // int systemProc; // processes executing in kernel mode
  // int idle; // twiddling thumbs
  // int iowait; // waiting for I/O to complete
  // int irq; // servicing interrupts
  // int softirq; // servicing softirqs
  // int steal; // involuntary wait
  // int guest; // running a normal guest
  // int guest_nice; // running a niced guest
  int PrevIdle;
  int PrevNonIdle;
  int PrevTotal;
  int Idle;
  int NonIdle;
  int Total;
  double porcentajeUso;
}ProcFile;

ProcFile **proc_listaProcesos (char *archivo);
int proc_obtenerProcesosMenosUsado (char *archivo);
